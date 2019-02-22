#include "streamer.h"
#include "options.h"
#include "rtsp_streamer.h"
#include <assert.h>
#include <string>
#include <cctype>
#include <algorithm>

const std::string METHOD_HTTP = "http";
const std::string METHOD_RTSP = "rtsp";

Streamer* Streamer::getStreamer(image::Builder *builder, const Options &options) {
  assert(builder);
  std::string method(options.getMethod());
  std::transform(method.begin(), method.end(), method.begin(), tolower);
  
  Streamer *result = nullptr;
  if (method == METHOD_HTTP) {
    // HTTP
  } else {
    // RTSP
    std::string port = std::to_string(options.getPort());
    result = new RtspStreamer(builder, options.getIP().c_str(), port, options.getMountPoint().c_str());
  }
  
  return result;
}

Streamer::~Streamer() {}
