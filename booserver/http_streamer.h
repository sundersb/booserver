#pragma once

#include "streamer.h"

class HttpStreamer: public Streamer {
  HttpStreamer(const HttpStreamer&) = delete;
  HttpStreamer(HttpStreamer&&) = delete;
  void operator=(const HttpStreamer&) = delete;
  
public:
  HttpStreamer(image::Builder *builder, const Options &options);
  ~HttpStreamer();

  /**
   * @brief                     Initialize
   * @param argc          Commandline parameters count for GStreamer
   * @param argv          Commandline arguments for GStreamer
   */
  bool init(int argc, char **argv);

  /**
   * @brief                     Run server
   *    Blocks program execution
   */
  void run(void);

  /**
   * @brief                     Stop server
   */
  void quit(void);
};

