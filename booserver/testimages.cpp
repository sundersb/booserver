#include <iostream>
#include <iomanip>
#include <cstring>
#include <sstream>
#include "canvas.h"
#include "png_files.h"
#include "ftype.h"
#include "timetable_screen_builder.h"
#include "provider.h"
#include "options.h"

const int ITERATIONS = 3;
const int HEADER_LINES = 3;
const int PAGE_0 = 0;
const int PAGE_1 = 1;

void printColor(const std::string &title, const image::RGB &color) {
  std::cout << "\t" << title << ": " << "0x";
  std::cout << std::setw(2) << (int) color.r;
  std::cout << std::setw(2) << (int) color.g;
  std::cout << std::setw(2) << (int) color.b;
  std::cout << std::endl;
}

int main(int argc, char **argv) {
  Options options;

  if (!options.load(DEFAULT_OPTIONS_FILENAME)) {
    std::cout << "Couldn't load options from "
      << DEFAULT_OPTIONS_FILENAME
      << ", using defaults" << std::endl;
  } else {
    std::cout << "Options loaded:" << std::endl;
    std::cout << "\tWidth: " << options.getWidth() << std::endl;
    std::cout << "\tHeight: " << options.getHeight() << std::endl;
    std::cout << "\tFps: " << options.getFps() << std::endl;
    std::cout << "\tLines: " << options.getLines() << std::endl;
    std::cout << "\tDuration: " << options.getDuration() << std::endl;
    std::cout << "\tIP: " << options.getIP() << std::endl;
    std::cout << "\tFont: " << options.getFontFile() << std::endl;
    std::cout << "\tSQL: " << options.getSqlUser()
      << "@" << options.getSqlHost()
      << '/' << options.getSqlDatabase() << std::endl;

    std::cout << "Colors:" << std::endl;
    std::cout << std::setfill('0') << std::hex;

    printColor ("Title face", options.getTitleFace());
    printColor ("Header back", options.getHeaderBack());
    printColor ("Header face", options.getHeaderFace());
    printColor ("Odd line", options.getLineOdd());
    printColor ("Even line", options.getLineEven());
    printColor ("Line face", options.getLineFace());
    printColor ("Counter face", options.getCounterFace());

    std::cout << std::dec;
  }

  Provider provider(options.getLines() - 1, HEADER_LINES);
  provider.init(options.getSqlHost(),
    options.getSqlDatabase(),
    options.getSqlUser(),
    options.getSqlPassword());

  TimetableScreenBuilder builder(options);
  if (!builder.init()) {
    std::cout << "Failed to initialize image builder!" << std::endl;
    return 1;
  }

  provider.update();
  int count = provider.getPagesCount();

  Profiles page0 = provider.getPage(PAGE_0);
  builder.build(page0, PAGE_0 + 1, count);

  unsigned char* buffer = new unsigned char [builder.getSize()];
  memcpy(buffer, builder.getPixels(), builder.getSize());

  std::cout << "Blending page " << PAGE_0 << " onto " << PAGE_1
    << " in " << ITERATIONS << " iterations:" << std::endl;

  for (int i = 0; i <= ITERATIONS; ++i) {
    std::ostringstream fn;
    fn << "image" << i << ".png";

    Profiles page1 = provider.getPage(PAGE_1);
    builder.build(page1, PAGE_1 + 1, count);

    int factor = ((ITERATIONS - i) * 255 / ITERATIONS) % 256;
    builder.getCanvas().blendImage(buffer, factor);

    if (png::toPNG(fn.str(), builder.getCanvas())) {
      std::cout << "\t" << fn.str() << std::endl;
    } else {
      std::cout << "\t" << fn.str() << "fail!" << std::endl;
    }
  }

  delete [] buffer;

  return 0;
}
