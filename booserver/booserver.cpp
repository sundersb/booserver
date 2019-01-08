#include <iostream>
#include "image_builder.h"
#include "multicast_streamer.h"
#include "provider.h"
#include "options.h"

// Enabling multicast on interface:
//ifconfig enp27s0 multicast
//route -n add -net 224.0.0.0 netmask 240.0.0.0 dev enp27s0
//tcpdump -i enp27s0 ip multicast

// booserver.exe --gst-root=C:\Utils\gstreamer\1.0\x86 --gst-plugin-path=C:\Utils\gstreamer\1.0\x86\lib\gstreamer-1.0

const int HEADER_LINES = 3;

const int VERSION_MAJOR = 1;
const int VERSION_MINOR = 0;
const int VERSION_RELEASE = 1;

int main(int argc, char **argv) {
  std::cout << "Booserver v."
    << VERSION_MAJOR << '.'
    << VERSION_MINOR << '.'
    << VERSION_RELEASE << std::endl;
    
  Options options;
  options.load(DEFAULT_OPTIONS_FILENAME);

  Provider provider(options.getLines() - 1, HEADER_LINES);
  provider.init(options.getSqlHost(),
    options.getSqlDatabase(),
    options.getSqlUser(),
    options.getSqlPassword());

  image::Builder imageBuilder(options, provider);
  MulticastStreamer streamer(&imageBuilder, options.getIP().c_str());

  if (streamer.init(argc, argv)) {
    std::cout << "Starting" << std::endl;
    streamer.run();
  } else {
    std::cout << "Failed to init streamer!" << std::endl;
  }

	return 0;
}
