#pragma once

namespace image {
  class Builder;
}

class Options;

/**
 * @class Streamer
 * @author Sunders
 * @date 22.02.2019
 * @file streamer.h
 * @brief Base interface and factory class for multimedia streamer
 */
class Streamer {
  Streamer(const Streamer&) = delete;
  Streamer(Streamer&&) = delete;
  void operator=(const Streamer&) = delete;
  
protected:
  void *impl;

public:
  Streamer(): impl(nullptr) {};

  /**
   * @brief   Factory function. Creates needed subclass of Streamer accordina to options
   * @param builder       Dynamic image builder
   * @param options       Application options
   */
  static Streamer* getStreamer(image::Builder *builder, const Options &options);
  virtual ~Streamer();

  /**
   * @brief                     Initialize
   * @param argc          Commandline parameters count for GStreamer
   * @param argv          Commandline arguments for GStreamer
   */
  virtual bool init(int argc, char **argv) = 0;

  /**
   * @brief                     Run server
   *    Blocks program execution
   */
  virtual void run(void) = 0;

  /**
   * @brief                     Stop server
   */
  virtual void quit(void) = 0;
};
