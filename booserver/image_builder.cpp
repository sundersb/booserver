#include "image_builder.h"
#include "timetable_screen_builder.h"
#include <cstring>
#include <assert.h>
#include <iostream>

namespace image {
  const int BPP = 3;

  class BuilderImplementation {
    Uchar* pixels;
    Uchar* page_current;
    TimetableScreenBuilder builder;
    Provider &provider;
    int width;
    int height;
    int fps;
    unsigned offset;
    unsigned pixelsSize;
    unsigned screen;
    unsigned screen_duration;
    bool transition;
    int transition_frame;

    BuilderImplementation() = delete;
    BuilderImplementation(BuilderImplementation&) = delete;
    void operator=(BuilderImplementation&) = delete;
  public:
    explicit BuilderImplementation(const Options &options, Provider &provider);

    ~BuilderImplementation();

    const Uchar* getNext(void);

    int getWidth(void) const { return width; }
    int getHeight(void) const { return height; }
    int getFps(void) const { return fps; }
    unsigned getSize(void) const { return pixelsSize; }
    unsigned getOffset(void) const { return offset; }
    void reset(void) { offset = 0; }
  };

  BuilderImplementation::BuilderImplementation(const Options &options, Provider &provider):
    pixels(nullptr),
    page_current(nullptr),
    builder(options),
    provider(provider),
    width(options.getWidth()),
    height(options.getHeight()),
    fps(options.getFps()),
    offset(0),
    pixelsSize(options.getWidth() * options.getHeight() * BPP),
    screen(0),
    screen_duration(options.getDuration() * options.getFps()),
    transition(false),
    transition_frame(0)
  {
    pixels = new Uchar[pixelsSize];
    page_current = new Uchar[pixelsSize];

    provider.update();
    Profiles page = provider.getPage(0);
    builder.build(page, 1, provider.getPagesCount());
    memcpy(pixels, builder.getPixels(), pixelsSize);
  }

  BuilderImplementation::~BuilderImplementation() {
    delete [] pixels;
    delete [] page_current;
  }

  Builder::Builder(const Options &options, Provider &provider) {
    impl = new BuilderImplementation(options, provider);
  }

  Builder::~Builder() {
    if (impl) {
      delete static_cast<BuilderImplementation*>(impl);
    }
  }

  const Uchar* Builder::getNext(void) {
    if (impl)
      return static_cast<BuilderImplementation*>(impl)->getNext();
    else
      return nullptr;
  }

  int Builder::getWidth(void) const {
    if (impl)
      return static_cast<BuilderImplementation*>(impl)->getWidth();
    else
      return 0;
  }

  int Builder::getHeight(void) const {
    if (impl)
      return static_cast<BuilderImplementation*>(impl)->getHeight();
    else
      return 0;
  }

  int Builder::getFps(void) const {
    if (impl)
      return static_cast<BuilderImplementation*>(impl)->getFps();
    else
      return 0;
  }

  unsigned Builder::getSize(void) const {
    if (impl)
      return static_cast<BuilderImplementation*>(impl)->getSize();
    else
      return 0;
  }

  unsigned Builder::getOffset(void) const {
    if (impl)
      return static_cast<BuilderImplementation*>(impl)->getOffset();
    else
      return 0;
  }

  void Builder::reset(void) {
    if (impl) static_cast<BuilderImplementation*>(impl)->reset();
  }

  const Uchar* BuilderImplementation::getNext(void) {
    ++offset;
    if (offset % screen_duration == 0) {
      memcpy(page_current, builder.getPixels(), pixelsSize);

      ++screen;
      provider.update();
      Profiles page = provider.getPage(screen);
      screen %= provider.getPagesCount();
      builder.build(page, screen + 1, provider.getPagesCount());

      transition = true;
      transition_frame = fps / 2;
//      transition_frame = fps;
    }

    if (transition) {
      --transition_frame;
      if (transition_frame > 0) {
        memcpy(pixels, page_current, pixelsSize);
        image::blend(pixels, builder.getPixels(), pixelsSize, 256 - transition_frame * 128 / fps);
//        image::blend(pixels, builder.getPixels(), pixelsSize, 256 - transition_frame * 256 / fps);
      } else {
        memcpy(pixels, builder.getPixels(), pixelsSize);
        transition = false;
      }
    }

    return pixels;
  }
}
