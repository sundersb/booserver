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
#include "batch_config.h"

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

int makeBlend(const Options &options, Provider &provider) {
  std::cout << "\nBlend mode\n";
  
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

int makeBatch(Options &options, Provider &provider) {
  std::cout << "\nBatch mode\n\n";
  BatchConfig config = BatchConfig::get(".");
  
  provider.update();
  Profiles page = provider.getPage(PAGE_0);
  
  for (const BatchFile& image : config.getImages()) {
    std::cout << "Image: " << image.getTitle() << std::endl;
    options.setBackgroundImageFile(image.getFilename());
    for (const BatchFile& color : config.getColors()) {
      if (options.load(color.getFilename())) {
        std::ostringstream fn;
        fn << image.getTitle() << '_' << color.getTitle() << ".png";
        std:: cout << '\t' << fn.str() << "...";
        
        TimetableScreenBuilder builder(options);
        if (!builder.init()) {
          std::cout << " failed to initialize image builder!" << std::endl;
          return 1;
        }
        
        builder.build(page, PAGE_0 + 1, provider.getPagesCount());
        
        if (png::toPNG(fn.str(), builder.getCanvas())) {
          std::cout << " ok" << std::endl;
        } else {
          std::cout << " failed to save PNG" << std::endl;
          return 1;
        }
      } else {
        std::cout << "Failed to load " << color.getFilename() << std::endl;
        return 1;
      }
    }
  }
  
  return 0;
}

int makeHelp() {
  std::cout << "Testimages: Tool for testing color schemes for Booserver\n";
  std::cout << "Usage:\n";
  std::cout << "testimages [batch|help]\n";
  std::cout << "\tbatch\tMake all possible combinations of images and color"
            << " schemes found in current directory. Color files should match"
            << " 'color_*.conf' pattern and images '*_2160.png'.\n";
  std::cout << "\thelp\tShow this help and exit.\n";
  std::cout << "\tWith no arguments the tool will generate "
            << (ITERATIONS + 1) << " images with different stages of blending"
            << " two adjacent screens\n\n";
  return 0;
}

int main(int argc, char **argv) {
  std::string mode;
  if (argc > 1)
    mode = argv[1];
    
  if (mode == "help")
    return makeHelp();
  
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

  if (mode == "batch")
    return makeBatch(options, provider);
  else
    return makeBlend(options, provider);

  return 0;
}
