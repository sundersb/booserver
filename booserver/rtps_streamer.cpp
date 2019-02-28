#include "rtps_streamer.h"

#include <gst/gst.h>
#include <gst/app/gstappsrc.h>
#include <gst/video/video.h>
#include <gst/rtsp-server/rtsp-server.h>
#include <cstring>
#include <iostream>

// H264 encoding
//C:\Utils\gstreamer\1.0\x86\bin>gst-launch-1.0 -v udpsrc port=5000 caps = "application/x-rtp, media=(string)video, clock-rate=(int)90000, encoding-name=(string)H264, payload=(int)96" ! rtph264depay ! decodebin ! videoconvert ! autovideosink
//const char* GSTREAMER_PIPELINE =
//  "appsrc name=appsource "
//  "! video/x-raw "
//  "! videoconvert "
//  "! queue "
//  "! x264enc tune=zerolatency "
//  "! video/x-h264, stream-format=byte-stream "
//  "! rtph264pay "
//  "! udpsink host=127.0.0.1 port=5000";
  
//const char* GSTREAMER_PIPELINE = "( appsrc name=appsource ! h264parse ! x264enc tune=zerolatency bitrate=500 speed-preset=superfast ! queue ! rtph264pay name=pay0 pt=96 )";
const char* GSTREAMER_PIPELINE =
  "( appsrc name=appsource "
  "! video/x-raw "
  "! videoconvert "
  "! video/x-raw,format=I420"
//  "! queue "
  "! x264enc tune=zerolatency "
//  "! video/x-h264, stream-format=byte-stream "
  "! h264parse"
  "! rtph264pay name=pay0 pt=96 )";
// This one works!

const char *MOUNT_POINT = "/live";
 
GST_DEBUG_CATEGORY (appsrc_pipeline_debug);
#define GST_CAT_DEFAULT appsrc_pipeline_debug

class RtpsStreamerImplementation {
  image::Builder *builder;
  GMainLoop *loop;
  GstRTSPServer *server;
  int fps;
public:
  RtpsStreamerImplementation(image::Builder *builder);
  ~RtpsStreamerImplementation();
  
  bool init(int argc, char **argv);
  void run(void);
  
  const unsigned char *getNextImage(const GstClockTime &timestamp, unsigned &size) {
    return builder->get(timestamp, size);
  }
  
  int getWidth(void) const { return builder->getWidth(); }
  int getHeight(void) const { return builder->getHeight(); }
  int getFps(void) const { return fps; }
};

RtpsStreamer::RtpsStreamer(image::Builder *builder) {
  impl = new RtpsStreamerImplementation(builder);
}

RtpsStreamer::~RtpsStreamer() {
  delete static_cast<RtpsStreamerImplementation*>(impl);
}

bool RtpsStreamer::init(int argc, char **argv) {
  if (impl)
    return static_cast<RtpsStreamerImplementation*>(impl)->init(argc, argv);
  else
    return false;
}

void RtpsStreamer::run(void) {
  if (impl) static_cast<RtpsStreamerImplementation*>(impl)->run();
}

bool initGstream(int argc, char **argv) {
  static bool inited = false;
  
  if (!inited) {
    gst_init (&argc, &argv);

    GST_DEBUG_CATEGORY_INIT (appsrc_pipeline_debug,
      "timetable-pipeline",
      0, 
      "Timetable translation"); 

    inited = true;
  }
  return inited;
}

RtpsStreamerImplementation::RtpsStreamerImplementation(image::Builder *builder):
  builder(builder),
  loop(nullptr),
  server(nullptr)
{
  fps = builder->getFps();
}

RtpsStreamerImplementation::~RtpsStreamerImplementation() {
  if (loop) g_main_loop_unref (loop);
  builder = nullptr;
}

extern "C" {

typedef struct _StreamContext {
  GTimer *timer;
  GstClockTime timestamp;
  RtpsStreamerImplementation *streamer;
  guint64 offset;
  guint sourceid;
  GstElement *appsrc;
  
  ~_StreamContext() { gst_object_unref (appsrc); }
} StreamContext;


static gboolean read_data (StreamContext *ctx) {
  GstFlowReturn ret;
  gdouble ms;

  ms = g_timer_elapsed(ctx->timer, NULL);
  if (ms > 1.0 / ctx->streamer->getFps()) {
    GstClockTime duration = ms * 1000000000;
    GstBuffer *buffer;
    gboolean ok = TRUE;
    
    unsigned size = 0;

    ctx->timestamp += duration;
    const unsigned char *src = ctx->streamer->getNextImage(ctx->timestamp, size);
    
    if (src && size) {
      guchar *b = (guchar*)g_malloc(size);
      memmove(b, src, size);
      buffer = gst_buffer_new_wrapped(b, size);

      GST_BUFFER_PTS (buffer) = ctx->timestamp;
      GST_BUFFER_DURATION (buffer) = duration;
      
      // Added:
      GST_BUFFER_OFFSET(buffer) = ctx->offset++;
      GST_BUFFER_OFFSET_END(buffer) = ctx->offset;

      GST_DEBUG ("feed buffer");
      g_signal_emit_by_name (ctx->appsrc, "push-buffer", buffer, &ret);
      gst_buffer_unref (buffer);

      if (ret != GST_FLOW_OK) {
        GST_DEBUG ("some error");
        ok = FALSE;
      }
    } else {
      GST_DEBUG ("failed to poll image data");
      ok = FALSE;
    }

    g_timer_start(ctx->timer);
    
    return ok;
  }

  // g_signal_emit_by_name (app->appsrc, "end-of-stream", &ret);
  return TRUE;
} 


static void start_feed (GstElement *pipeline, guint size, StreamContext *ctx) {
  if (ctx->sourceid == 0) {
    GST_DEBUG ("start feeding");
    ctx->sourceid = g_idle_add ((GSourceFunc) read_data, ctx);
  }
}

static void stop_feed (GstElement *pipeline, StreamContext *ctx) {
  if (ctx->sourceid != 0) {
    GST_DEBUG ("stop feeding");
    g_source_remove (ctx->sourceid);
    ctx->sourceid = 0;
  }
}

void delContext(StreamContext *data) {
  delete data;
}

static void media_configure (GstRTSPMediaFactory * factory, GstRTSPMedia * media, RtpsStreamerImplementation *streamer) {
  GstElement *element;
  StreamContext *ctx;
  
  std::cout << "media_configure()" << std::endl;

  /* get the element used for providing the streams of the media */
  element = gst_rtsp_media_get_element (media);

  /* get our appsrc, we named it 'mysrc' with the name property */

  //ctx = g_new0 (StreamContext, 1);
  ctx = new StreamContext();
  ctx->timestamp = 0;
  ctx->streamer = streamer;
  ctx->timer = g_timer_new();
  ctx->appsrc = gst_bin_get_by_name_recurse_up (GST_BIN (element), "appsource");
  
  GstVideoInfo info;
  gst_video_info_set_format(&info, GST_VIDEO_FORMAT_RGB, streamer->getWidth(), streamer->getHeight());
  GstCaps *caps = gst_video_info_to_caps(&info);
  gst_app_src_set_caps(GST_APP_SRC(ctx->appsrc), caps);
  g_object_set (ctx->appsrc, "format", GST_FORMAT_TIME, NULL);

//  /* this instructs appsrc that we will be dealing with timed buffer */
//  gst_util_set_object_arg (G_OBJECT (appsrc), "format", "time");
//  /* configure the caps of the video */
//  g_object_set (G_OBJECT (appsrc), "caps",
//      gst_caps_new_simple ("video/x-raw",
//          "format", G_TYPE_STRING, "BGR",
//          "width", G_TYPE_INT, streamer->getWidth(),
//          "height", G_TYPE_INT, streamer->getHeight(),
//          "framerate", GST_TYPE_FRACTION, streamer->getFps(), 1, NULL), NULL);

  /* make sure their data is freed when the media is gone */
//  g_object_set_data_full (G_OBJECT (media), "my-extra-data", ctx, (GDestroyNotify) g_free);
  g_object_set_data_full (G_OBJECT (media), "my-extra-data", ctx, (GDestroyNotify) delContext);

  /* install the callback that will be called when a buffer is needed */
  //g_signal_connect (appsrc, "need-data", (GCallback) need_data, ctx);
  
  g_signal_connect (ctx->appsrc, "need-data", G_CALLBACK (start_feed), ctx);
  g_signal_connect (ctx->appsrc, "enough-data", G_CALLBACK (stop_feed), ctx);
  
//  gst_object_unref (appsrc);
  gst_object_unref (element);
}

} // extern "C"

bool RtpsStreamerImplementation::init(int argc, char **argv) {
  if (!initGstream(argc, argv)) return false;
  GstRTSPMountPoints *mounts;
  GstRTSPMediaFactory *factory;

  loop = g_main_loop_new (NULL, FALSE);

  server = gst_rtsp_server_new ();

  /* get the mount points for this server, every server has a default object
   * that be used to map uri mount points to media factories */
  mounts = gst_rtsp_server_get_mount_points (server);

  /* make a media factory for a test stream. The default media factory can use
   * gst-launch syntax to create pipelines.
   * any launch line works as long as it contains elements named pay%d. Each
   * element with pay%d names will be a stream */
  factory = gst_rtsp_media_factory_new ();
  gst_rtsp_media_factory_set_launch (factory, GSTREAMER_PIPELINE);

  /* notify when our media is ready, This is called whenever someone asks for
   * the media and a new pipeline with our appsrc is created */
  g_signal_connect (factory, "media-configure", (GCallback) media_configure, this);

  /* attach the test factory to the /test url */
  gst_rtsp_mount_points_add_factory (mounts, MOUNT_POINT, factory);

  /* don't need the ref to the mounts anymore */
  g_object_unref (mounts);

  /* attach the server to the default maincontext */
  gst_rtsp_server_attach (server, NULL);

  return true;
}

void RtpsStreamerImplementation::run(void) {
  std::cout << "stream ready at rtsp://127.0.0.1:8554" << MOUNT_POINT << std::endl;
  g_main_loop_run (loop);
  GST_DEBUG ("stopping"); 
}

