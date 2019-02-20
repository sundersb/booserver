#pragma once

namespace image {
  class Builder;
}

/**
 * @class MulticastStreamer
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
class MulticastStreamer {
  void *impl;

  MulticastStreamer() = delete;
  MulticastStreamer(const MulticastStreamer&) = delete;
  MulticastStreamer(MulticastStreamer&&) = delete;
  void operator=(const MulticastStreamer&) = delete;
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
  MulticastStreamer(image::Builder *builder, const char *ip, const char *port, const char *mount_point);
  ~MulticastStreamer();

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
