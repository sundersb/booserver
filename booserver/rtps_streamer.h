#pragma once
#include "image_builder.h"

/**
 * @class RtpsStreamer
 * @author Sunders
 * @date 26.10.2018
 * @file rtp_streamer.h
 * @brief Stream images sequence by RTPS in H264
 */
class RtpsStreamer {
  void *impl;
public:
  /**
   * @brief Ctor
   * @param builder       Image builder to obtain frames for streaming
   */
  RtpsStreamer(image::Builder *builder);
  ~RtpsStreamer();
  
  /**
   * @brief Initialize the streamer
   */
  bool init(int argc, char **argv);
  
  /**
   * @brief Run RTP server
   */
  void run(void);
};

