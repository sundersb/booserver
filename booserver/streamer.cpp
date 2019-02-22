#include "streamer.h"
#include "options.h"
#include "rtsp_streamer.h"
#include "http_streamer.h"
#include <assert.h>
#include <string>
#include <cctype>
#include <algorithm>
#include <gst/gst.h>

GST_DEBUG_CATEGORY (appsrc_pipeline_debug);
#define GST_CAT_DEFAULT appsrc_pipeline_debug

const std::string METHOD_HTTP = "http";
const std::string METHOD_RTSP = "rtsp";

Streamer* Streamer::getStreamer(image::Builder *builder, const Options &options) {
  assert(builder);
  std::string method(options.getMethod());
  std::transform(method.begin(), method.end(), method.begin(), tolower);
  
  Streamer *result = nullptr;
  if (method == METHOD_HTTP) {
    // HTTP
    result = new HttpStreamer(builder, options);
  } else {
    // RTSP
    result = new RtspStreamer(builder, options);
  }
  
  return result;
}

Streamer::~Streamer() {}


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

bool Streamer::init(int argc, char **argv) {
  return initGstream(argc, argv);
}

