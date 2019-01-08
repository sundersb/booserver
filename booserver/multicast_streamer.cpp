#include "multicast_streamer.h"
#include "image_builder.h"

// https://github.com/GStreamer/gst-rtsp-server/blob/master/examples/test-multicast.c

#include <gst/gst.h>
#include <gst/video/video.h>
#include <gst/app/gstappsrc.h>
#include <gst/rtsp-server/rtsp-server.h>
#include <assert.h>
#include <cstring>
#include <iostream>

GST_DEBUG_CATEGORY (appsrc_pipeline_debug);
#define GST_CAT_DEFAULT appsrc_pipeline_debug

const GstClockTime TRILLION = 1000000000;

// Gstreamer pipe for dynamic frames acquisition, H264 compressing
// and making ready to be served away
const char *GSTREAMER_PIPELINE =
  "( appsrc name=appsource "
  "! videoconvert "
  "! video/x-raw,format=I420 "
  "! x264enc tune=zerolatency "
  "! rtph264pay name=pay0 pt=96 )";

const char *MOUNT_POINT = "/live";
const char *PORT = "8554";

const char *MULTICAST_IP_FIRST = "227.5.0.0";
const char *MULTICAST_IP_LAST = "227.5.0.10";

const guint16 PORT_MIN = 5000;
const guint16 PORT_MAX = 5010;
const guint8 TTL = 16;

/**
 * @class MCImplementation
 * @author ivanhoe
 * @date 28/10/18
 * @file multicast_streamer.cpp
 * @brief     Multicast RTSP implementation
 */
class MCImplementation {
  GMainLoop *loop;          // glibc main loop
  image::Builder *builder;  // frames generator
  guint64 offset;           // last frame ordinal number
  const char *ip;           // IP for the server to listen on
  GstClockTime duration;    // single frame duration (in nanoseconds)
  bool enough;              // flag to stop frames feeding
  bool running;             // flag to show run() is on the way
public:
  MCImplementation (image::Builder *builder, const char *ip);

  bool init(int argc, char **argv);
  void run(void);

  // Configure appsrc when media is about to be used
  void configure (GstRTSPMedia *media);

  void reset(void);

  // Stuff frames in the appsrc queue until it is full
  void feed (GstAppSrc *appsrc, guint size);

  // Callback to stop feeding
  void stop_feeding (GstAppSrc *appsrc);

  // Quit main loop while run() is working
  void quit(void);
};


//          MulticastStreamer
// ***********************
MulticastStreamer::MulticastStreamer(image::Builder *builder, const char *ip) {
  impl = new MCImplementation(builder, ip);
}

MulticastStreamer::~MulticastStreamer() {
  assert(impl);
  delete static_cast<MCImplementation*>(impl);
}

bool MulticastStreamer::init(int argc, char **argv) {
  assert(impl);
  return static_cast<MCImplementation*>(impl)->init(argc, argv);
}

void MulticastStreamer::run(void) {
  assert(impl);
  static_cast<MCImplementation*>(impl)->run();
}

void MulticastStreamer::quit(void) {
  assert(impl);
  static_cast<MCImplementation*>(impl)->quit();
}


//       GLIBC callbacks
// ***********************
// Free pool from stall sessions
static gboolean timeout (GstRTSPServer * server) {
  GstRTSPSessionPool *pool;

  pool = gst_rtsp_server_get_session_pool (server);
  gst_rtsp_session_pool_cleanup (pool);
  g_object_unref (pool);

  return TRUE;
}

// Configure streaming pipe when it is about to start working
static void media_configured (GstRTSPMediaFactory * factory, GstRTSPMedia * media, MCImplementation *streamer) {
  streamer->configure(media);
}

// Reset timestamp when all clients have been disconnected
static void media_unprepared (GstRTSPMedia * media, MCImplementation *streamer) {
  // Without this trick all following clients would lack old frames
  streamer->reset();
}

// Start feeding frames to the streaming queue
static void need_data (GstAppSrc *appsrc, guint size, MCImplementation *streamer) {
  streamer->feed(appsrc, size);
}

// Stop feeding queue with frames
static void enough_data(GstAppSrc *appsrc, MCImplementation *streamer) {
  streamer->stop_feeding(appsrc);
}

// Initialize GStreamer
bool initGstream(int argc, char **argv) {
  static bool inited = false;

  if (!inited) {
    if (gst_init_check(&argc, &argv, NULL) == FALSE)
      return false;

    GST_DEBUG_CATEGORY_INIT (appsrc_pipeline_debug,
      "booserver-pipeline",
      0,
      "Booserver timetable translation");

    inited = true;
  }
  return inited;
}


//         MCImplementation
// ***********************
MCImplementation::MCImplementation (image::Builder *builder, const char *ip):
  loop(NULL),
  builder(builder),
  offset(0),
  ip(ip),
  enough(false),
  running(false)
{
  assert(builder);
  duration = TRILLION / builder->getFps();
}


bool MCImplementation::init(int argc, char **argv) {
  // Init GStreamer
  if (!initGstream(argc, argv)) return false;

  // Create server and main loop
  loop = g_main_loop_new (NULL, FALSE);

  GstRTSPServer *server = gst_rtsp_server_new ();
  gst_rtsp_server_set_address (server, ip);
  gst_rtsp_server_set_service (server, PORT);

  GstRTSPMountPoints *mounts = gst_rtsp_server_get_mount_points (server);

  // Create media factory to bring media on request
  GstRTSPMediaFactory *factory = gst_rtsp_media_factory_new ();
  gst_rtsp_media_factory_set_launch (factory, GSTREAMER_PIPELINE);
  gst_rtsp_media_factory_set_shared (factory, TRUE);      // This one allows multicast

  GstRTSPAddressPool *pool = gst_rtsp_address_pool_new ();
  gst_rtsp_address_pool_add_range (pool,
    MULTICAST_IP_FIRST,
    MULTICAST_IP_LAST,
    PORT_MIN,
    PORT_MAX,
    TTL);
  gst_rtsp_media_factory_set_address_pool (factory, pool);

  // Callback when media is constructed and configured
  g_signal_connect (factory, "media-configure", (GCallback) media_configured, this);

  // Limit available protocols to multicasting only. Somehow prevents from running properly...
//  gst_rtsp_media_factory_set_protocols (factory, GST_RTSP_LOWER_TRANS_UDP_MCAST);
  // TODO: HOWTO make this really multicast??

  g_object_unref (pool);

  // Set mount point for the factory
  gst_rtsp_mount_points_add_factory (mounts, MOUNT_POINT, factory);
  g_object_unref (mounts);

  if (gst_rtsp_server_attach (server, NULL) == 0) return false;

  // Make main loop periodically clear sessions pool
  g_timeout_add_seconds (2, (GSourceFunc) timeout, server);

  return true;
}


void MCImplementation::run(void) {
  assert(loop);
  std::cout << "stream ready at rtsp://"
    << ip
    << ':'
    << PORT
    << MOUNT_POINT
    << std::endl;

  running = true;

  g_main_loop_run (loop);
}


void MCImplementation::configure (GstRTSPMedia *media) {
  GstElement *element = gst_rtsp_media_get_element (media);
  GstElement *appsrc = gst_bin_get_by_name_recurse_up (GST_BIN (element), "appsource");

  // Setup appsrc's caps: color format, picture size, fps
  GstVideoInfo info;
  gst_video_info_set_format(&info, GST_VIDEO_FORMAT_RGB, builder->getWidth(), builder->getHeight());
  info.fps_n = builder->getFps();
  info.fps_d = 1;
  info.par_n = 1;
  info.par_d = 1;

  GstCaps *caps = gst_video_info_to_caps(&info);
  gst_app_src_set_caps(GST_APP_SRC(appsrc), caps);

  // The appsrc should be time driven
  g_object_set (G_OBJECT(appsrc), "format", GST_FORMAT_TIME, NULL);

  // Set live video - no seek allowed
  gst_app_src_set_stream_type(GST_APP_SRC(appsrc), GST_APP_STREAM_TYPE_STREAM);

  // Set queue size to five frames
//  gst_app_src_set_max_bytes(GST_APP_SRC(appsrc), builder->getSize() * 5);

  // Set appsrc callbacks to feed
  g_signal_connect (appsrc, "need-data", (GCallback) need_data,  this);
  g_signal_connect (appsrc, "enough-data", (GCallback) enough_data,  this);

  gst_object_unref (appsrc);
  gst_object_unref (element);

  // Allow multicast
  gst_rtsp_media_set_shared(media, TRUE);

  // Turning this to false helped to solve the error when
  // no new client connections succeed after a period of all-clients-disconnected:
  gst_rtsp_media_set_reusable(media, FALSE);

  // These two don't solve the issue:
//  gst_rtsp_media_set_eos_shutdown(media, TRUE);
//  gst_rtsp_media_set_stop_on_disconnect(media, FALSE);

  // Reset *this when no media is in the job
  g_signal_connect (media, "unprepared", (GCallback) media_unprepared, this);
}

void MCImplementation::feed (GstAppSrc *appsrc, guint size) {
  assert(appsrc);
  enough = false;

  // Push frame-buffers to the appsrc queue untill it's full
  while (!enough) {
    GstClockTime timestamp = duration * offset;

    const image::Uchar *pixels = builder->getNext();
    unsigned bufsize = builder->getSize();

    assert (pixels);
    assert(bufsize);

    // Make a GstBuffer wrapped around copy of the current frame
    guchar *b = (guchar*)g_malloc(bufsize);
    memmove(b, pixels, bufsize);

    GstBuffer *buffer = gst_buffer_new_wrapped(b, bufsize);

    // Set buffers time mark, duration and position
    GST_BUFFER_PTS (buffer) = timestamp;
    GST_BUFFER_DURATION (buffer) = duration;
    GST_BUFFER_OFFSET(buffer) = offset;
    GST_BUFFER_OFFSET_END(buffer) = ++offset;

    // Feed the appsrc
    GstFlowReturn ret;
    g_signal_emit_by_name (appsrc, "push-buffer", buffer, &ret);

    gst_buffer_unref (buffer);
  }
}

void MCImplementation::stop_feeding (GstAppSrc *appsrc) {
  // Signal to stop feeding
  enough = true;
}

void MCImplementation::quit(void) {
  assert(loop);
  if (running) g_main_loop_quit (loop);
}

void MCImplementation::reset(void) {
  offset = 0;
  builder->reset();
}
