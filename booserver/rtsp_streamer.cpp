#include "rtsp_streamer.h"
#include "image_builder.h"

// https://github.com/GStreamer/gst-rtsp-server/blob/master/examples/test-multicast.c

#include <gst/gst.h>
#include <gst/video/video.h>
#include <gst/app/gstappsrc.h>
#include <gst/rtsp-server/rtsp-server.h>
#include <assert.h>
#include <cstring>
#include <iostream>

// Gstreamer pipe for dynamic frames acquisition, H264 compressing
// and making ready to be served away
const char *GSTREAMER_PIPELINE =
  "( appsrc name=appsource "
  "! videoconvert "
  "! video/x-raw,format=I420 "
  "! x264enc tune=zerolatency "
  "! rtph264pay name=pay0 pt=96 )";

const char *MULTICAST_IP_FIRST = "227.5.0.0";
const char *MULTICAST_IP_LAST = "227.5.0.10";

const guint16 PORT_MIN = 5000;
const guint16 PORT_MAX = 5010;
const guint8 TTL = 16;

/**
 * @class RtspImplementation
 * @author ivanhoe
 * @date 28/10/18
 * @file multicast_streamer.cpp
 * @brief     Multicast RTSP implementation
 */
class RtspImplementation {
  GMainLoop *loop;          // glibc main loop
  image::Builder *builder;  // frames generator
  guint64 offset;           // last frame ordinal number
  std::string ip;           // IP for the server to listen on
  std::string port;
  std::string mount_point;
  GstClockTime duration;    // single frame duration (in nanoseconds)
  bool enough;              // flag to stop frames feeding
  bool running;             // flag to show run() is on the way
public:
  RtspImplementation (image::Builder *builder, const std::string &ip, const std::string &port, const std::string &mount_point);

  bool init(void);
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


//          RtspStreamer
// ***********************
RtspStreamer::RtspStreamer(image::Builder *builder, const Options &options) {
  std::string port = std::to_string(options.getPort());
  impl = new RtspImplementation(builder, options.getIP(), port, options.getMountPoint());
}

RtspStreamer::~RtspStreamer() {
  assert(impl);
  delete static_cast<RtspImplementation*>(impl);
}

bool RtspStreamer::init(int argc, char **argv) {
  assert(impl);
  if (!Streamer::init(argc, argv)) return false;
  return static_cast<RtspImplementation*>(impl)->init();
}

void RtspStreamer::run(void) {
  assert(impl);
  static_cast<RtspImplementation*>(impl)->run();
}

void RtspStreamer::quit(void) {
  assert(impl);
  static_cast<RtspImplementation*>(impl)->quit();
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
static void media_configured (GstRTSPMediaFactory * factory, GstRTSPMedia * media, RtspImplementation *streamer) {
  streamer->configure(media);
}

// Reset timestamp when all clients have been disconnected
static void media_unprepared (GstRTSPMedia * media, RtspImplementation *streamer) {
  // Without this trick all following clients would lack old frames
  streamer->reset();
}

// Start feeding frames to the streaming queue
static void need_data (GstAppSrc *appsrc, guint size, RtspImplementation *streamer) {
  streamer->feed(appsrc, size);
}

// Stop feeding queue with frames
static void enough_data(GstAppSrc *appsrc, RtspImplementation *streamer) {
  streamer->stop_feeding(appsrc);
}

//         RtspImplementation
// ***********************
RtspImplementation::RtspImplementation (image::Builder *builder, const std::string &ip, const std::string &port, const std::string &mount_point):
  loop(NULL),
  builder(builder),
  offset(0),
  ip(ip),
  port(port),
  mount_point(mount_point),
  enough(false),
  running(false)
{
  assert(builder);
  duration = TRILLION / builder->getFps();
}


bool RtspImplementation::init(void) {
  // Create server and main loop
  loop = g_main_loop_new (NULL, FALSE);

  GstRTSPServer *server = gst_rtsp_server_new ();
  gst_rtsp_server_set_address (server, ip.c_str());
  gst_rtsp_server_set_service (server, port.c_str());

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
  gst_rtsp_mount_points_add_factory (mounts, mount_point.c_str(), factory);
  g_object_unref (mounts);

  if (gst_rtsp_server_attach (server, NULL) == 0) return false;

  // Make main loop periodically clear sessions pool
  g_timeout_add_seconds (2, (GSourceFunc) timeout, server);

  return true;
}


void RtspImplementation::run(void) {
  assert(loop);
  std::cout << "stream ready at rtsp://"
    << ip
    << ':'
    << port
    << mount_point
    << std::endl;

  running = true;

  g_main_loop_run (loop);
}


void RtspImplementation::configure (GstRTSPMedia *media) {
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

void RtspImplementation::feed (GstAppSrc *appsrc, guint size) {
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

void RtspImplementation::stop_feeding (GstAppSrc *appsrc) {
  // Signal to stop feeding
  enough = true;
}

void RtspImplementation::quit(void) {
  assert(loop);
  if (running) g_main_loop_quit (loop);
}

void RtspImplementation::reset(void) {
  offset = 0;
  builder->reset();
}
