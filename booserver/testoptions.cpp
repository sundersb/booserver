#include <iostream>
#include "options.h"

int main(int argc, char **argv) {
  Options options;
  
  if (!options.load(DEFAULT_OPTIONS_FILENAME)) {
    std::cout << "Couldn't load options from "
      << DEFAULT_OPTIONS_FILENAME
      << ", using defaults" << std::endl;
  } else {
    std::cout << "Width: " << options.getWidth() << std::endl;
    std::cout << "Height: " << options.getHeight() << std::endl;
    std::cout << "Fps: " << options.getFps() << std::endl;
    std::cout << "Lines: " << options.getLines() << std::endl;
    std::cout << "Duration: " << options.getDuration() << std::endl;
    std::cout << "IP: " << options.getIP() << std::endl;
    std::cout << "Font: " << options.getFontFile() << std::endl;
    std::cout << "SQL: " << options.getSqlUser()
      << "@" << options.getSqlHost()
      << '/' << options.getSqlDatabase() << std::endl;
  }
  
	return 0;
}
