#include "http_streamer.h"
#include "options.h"
#include "image_builder.h"

#include <assert.h>
#include <string>
#include <sstream>
#include <iostream>
#include <cstring>

#include <gst/gst.h>
#include <gst/app/gstappsrc.h>
#include <gst/video/video.h>

const int MAX_FILES = 10;
const int TARGET_DURATION = 30;
const std::string HTTP_GST_PIPELINE =
  "appsrc name=appsource "
  "! videoconvert "
  "! video/x-raw,format=I420 "
  "! x264enc tune=zerolatency "
  "! mpegtsmux";

std::string getHttpPipeline(const Options &options) {
  std::ostringstream str;
  
  str << HTTP_GST_PIPELINE;
  str << "! hlssink max-files=" << MAX_FILES
    << " playlist-root=http://" << options.getIP();
  
  if (options.getMountPoint().empty()) {
    str << '/';
  } else {
    str << options.getMountPoint();
    if (options.getMountPoint().back() != '/')
      str << '/';
  }
  
  str << " playlist-location=" << options.getPlaylistLocation()
    << " location=" << options.getSegmentsLocation()
    << " target-duration=" << TARGET_DURATION;
  
  return str.str();
}
  
class HttpImplementation {
  const Options &options;
  image::Builder *builder;
  bool running;
  bool enough;

  GMainLoop *loop;

  guint64 offset;
  GstClockTime duration;
  std::string pipe;
public:
  HttpImplementation(image::Builder *builder, const Options &options);
  ~HttpImplementation();
  
  GMainLoop* getMainLoop(void) { return loop; }
  
  bool init(void);
  void run(void);
  void quit(void);
  
  void feed(GstAppSrc *appsrc, guint size);
  void stop_feeding(GstAppSrc *appsrc);
};


//          HttpStreamer
// ******************************
HttpStreamer::HttpStreamer(image::Builder *builder, const Options &options) {
  impl = new HttpImplementation(builder, options);
}

HttpStreamer::~HttpStreamer() {
  assert(impl);
  delete static_cast<HttpImplementation*>(impl);
}

bool HttpStreamer::init(int argc, char **argv) {
  assert(impl);
  if (!Streamer::init(argc, argv)) return false;
  return static_cast<HttpImplementation*>(impl)->init();
}

void HttpStreamer::run(void) {
  assert(impl);
  static_cast<HttpImplementation*>(impl)->run();
}

void HttpStreamer::quit(void) {
  assert(impl);
  static_cast<HttpImplementation*>(impl)->quit();
}



//       GLIBC callbacks
// ***********************
static gboolean bus_message (GstBus *bus, GstMessage *message, HttpImplementation *app) { 
  GST_DEBUG ("got message %s", gst_message_type_get_name (GST_MESSAGE_TYPE (message)));

  switch (GST_MESSAGE_TYPE (message)) {
    
  case GST_MESSAGE_ERROR: {
    GError *err = NULL;
    gchar *dbg_info = NULL;

    gst_message_parse_error (message, &err, &dbg_info);
    g_printerr ("ERROR from element %s: %s\n", GST_OBJECT_NAME (message->src), err->message);
    g_printerr ("Debugging info: %s\n", (dbg_info) ? dbg_info : "none");
    g_error_free (err);
    g_free (dbg_info);
    g_main_loop_quit (app->getMainLoop());
    break;
  }
  
  case GST_MESSAGE_EOS:
    g_main_loop_quit (app->getMainLoop());
    break;
    
  default:
    break;
  }
  
  return TRUE;
}

// Start feeding frames to the streaming queue
static void need_data (GstAppSrc *appsrc, guint size, HttpImplementation *streamer) {
  streamer->feed(appsrc, size);
}

// Stop feeding queue with frames
static void enough_data(GstAppSrc *appsrc, HttpImplementation *streamer) {
  streamer->stop_feeding(appsrc);
}


//          HttpImplementation
// **************************************
HttpImplementation::HttpImplementation(image::Builder *builder, const Options &options):
  options(options),
  builder(builder),
  running(false),
  enough(false),
//  pipeline(nullptr),
//  appsrc(nullptr),
  loop(nullptr),
//  bus(nullptr),
  offset(0),
  pipe(getHttpPipeline(options))
{
  assert(builder);
  duration = TRILLION / builder->getFps();
}

HttpImplementation::~HttpImplementation() {
  quit();
}

bool HttpImplementation::init(void) {
  loop = g_main_loop_new (NULL, TRUE);

  GstElement *pipeline = gst_parse_launch(pipe.c_str(), NULL);
  if (!pipeline) return false;
  
  GstBus *bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
  if (!bus) return false;
  
  gst_bus_add_watch (bus, (GstBusFunc)bus_message, this);
  GstElement *appsrc = gst_bin_get_by_name (GST_BIN(pipeline), "appsource");

  if (!appsrc) return false;
  if (!GST_IS_APP_SRC(appsrc)) return false;

  g_signal_connect (appsrc, "need-data", G_CALLBACK(need_data), this);
  g_signal_connect (appsrc, "enough-data", G_CALLBACK(enough_data), this);

  GstVideoInfo info;
  gst_video_info_set_format(&info, GST_VIDEO_FORMAT_RGB, builder->getWidth(), builder->getHeight());
  info.fps_n = builder->getFps();
  info.fps_d = 1;
  info.par_n = 1;
  info.par_d = 1;

  GstCaps *caps = gst_video_info_to_caps(&info);
  gst_app_src_set_caps(GST_APP_SRC(appsrc), caps);
  
  // Added:
  g_object_set (appsrc, "format", GST_FORMAT_TIME, NULL);
  gst_app_src_set_stream_type(GST_APP_SRC(appsrc), GST_APP_STREAM_TYPE_STREAM);

  gst_object_unref (appsrc);
  gst_object_unref (bus);
  gst_object_unref (pipeline);

  return true;
}

void HttpImplementation::run(void) {
  assert(loop);
  std::string res_name = options.getPlaylistLocation();
  
  
  std::cout << "Playlist is ready at http://"
    << options.getIP()
    << ':'
    << options.getPort();
    
  if (options.getMountPoint().empty()) {
    std::cout << '/';
  } else {
    std::cout << options.getMountPoint();
    if (options.getMountPoint().back() != '/')
      std::cout << '/';
  }

  size_t pos = options.getPlaylistLocation().find_last_of('/');
  
  if (pos == std::string::npos)
    std::cout << options.getPlaylistLocation();
  else
    std::cout << options.getPlaylistLocation().substr(pos + 1);
    
  std::cout << std::endl;

  running = true;

  g_main_loop_run (loop);
}

void HttpImplementation::quit(void) {
  if (!running) {
    g_main_loop_quit(loop);
    running = false;
  }
}

void HttpImplementation::feed (GstAppSrc *appsrc, guint size) {
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

void HttpImplementation::stop_feeding (GstAppSrc *appsrc) {
  // Signal to stop feeding
  enough = true;
}
