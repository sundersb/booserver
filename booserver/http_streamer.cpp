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

const int MAX_FILES = 5;
const int TARGET_DURATION = 30;
const std::string HTTP_GST_PIPELINE =
  "appsrc name=appsource is-live=true "
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
  GTimer *timer;
  bool running;
  bool enough;

  GMainLoop *loop;
  GstPipeline *pipeline;

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
  timer(nullptr),
  running(false),
  enough(false),
  loop(nullptr),
  pipeline(nullptr),
  offset(0),
  pipe(getHttpPipeline(options))
{
  assert(builder);
  duration = TRILLION / builder->getFps();
}

HttpImplementation::~HttpImplementation() {
  quit();
  if (pipeline) gst_object_unref(pipeline);
}

bool HttpImplementation::init(void) {
  loop = g_main_loop_new (NULL, TRUE);

  pipeline = GST_PIPELINE(gst_parse_launch(pipe.c_str(), NULL));
  if (!pipeline) return false;
  
//  GstBus *bus = gst_pipeline_get_bus (GST_PIPELINE (pipeline));
//  if (!bus) return false;
//  
//  gst_bus_add_watch (bus, (GstBusFunc)bus_message, this);
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
  g_object_set (G_OBJECT(appsrc), "format", GST_FORMAT_TIME, NULL);
  gst_app_src_set_stream_type(GST_APP_SRC(appsrc), GST_APP_STREAM_TYPE_STREAM);

  gst_object_unref (appsrc);
  timer = g_timer_new();
//  gst_object_unref (bus);

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

  g_timer_start(timer);
  gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PLAYING);
  g_main_loop_run (loop);
}

void HttpImplementation::quit(void) {
  if (!running) {
    assert(pipeline);
    gst_element_set_state(GST_ELEMENT(pipeline), GST_STATE_PAUSED);
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
    
    if (offset % (options.getFps() * TARGET_DURATION) == 1) {
      // Prevent TS-segments from being pushed too fast
      while (g_timer_elapsed(timer, NULL) < TARGET_DURATION) {
        g_usleep(1000 * 200);
      }
      g_timer_start(timer);
    }
  }
}

void HttpImplementation::stop_feeding (GstAppSrc *appsrc) {
  // Signal to stop feeding
  enough = true;
}
