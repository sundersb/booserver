#pragma once
#include <string>
#include "streamer.h"
/**
 * @class RtspStreamer
 * @author ivanhoe
 * @date 28/10/18
 * @file multicast_streamer.h
 * @brief     RTSP multicast video streaming
 *
 *  Check with:
 *  gst-launch-1.0 -vv rtspsrc location="rtsp://127.0.0.1:8554/live" ! \
 *    "application/x-rtp,media=video,encoding-name=H264,payload=96" ! \
 *    rtph264depay ! decodebin ! autovideosink
 */
class RtspStreamer: public Streamer {
  RtspStreamer() = delete;
  RtspStreamer(const RtspStreamer&) = delete;
  RtspStreamer(RtspStreamer&&) = delete;
  void operator=(const RtspStreamer&) = delete;
public:
  /**
   * @brief   Ctor
   * @param builder       Dynamic image builder
   * @param ip              IP address to listen for connections on
   * @param port            Port to listen on
   * @param mount_point     Mount point to associate with the media
   *
   *  Builder has frame width, height and fps set
   */
  RtspStreamer(image::Builder *builder, const std::string &ip, const std::string &port, const std::string &mount_point);
  ~RtspStreamer();

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
