#include "canvas.h"
#include <cstring>
#include <utility>
#include <assert.h>

namespace image {

const int BPP = 3;

inline unsigned char blend (int oldValue, int newValue, int factor) {
  int result = newValue - oldValue;
  return (unsigned char) (oldValue + result * factor / 256);
}

Canvas::Canvas(int width, int height):
  width(width),
  height(height),
  size(width * height * BPP),
  stride(width * BPP),
  pixels(nullptr)
{
}

bool Canvas::init(void) {
  pixels = new unsigned char [size];
  return pixels != nullptr;
}

Canvas::~Canvas() {
  delete [] pixels;
}

int Canvas::getWidth(void) const { return width; }
int Canvas::getHeight(void) const { return height; }
int Canvas::getSize(void) const { return size; }
unsigned char *Canvas::getPixels(void) { return pixels; }
unsigned char *Canvas::getPixels(void) const { return pixels; }

void Canvas::clear(const RGB &color) {
  if (color.r | color.g | color.b) {
    // Colored fill
    
    unsigned char* ptr = pixels;

    // First row - fill with the color
    for (int j = 0; j < width; ++j) {
      ptr[0] = color.r;
      ptr[1] = color.g;
      ptr[2] = color.b;
      ptr += BPP;
    }

    // Other rows - copy the first one
    for (int i = 1; i < height; ++i) {
      memcpy(ptr, pixels, stride);
      ptr += stride;
    }
  } else {
    // Black fill
    memset(pixels, 0, size);
  }
}

void Canvas::vertical(const RGB &color, int x, int y0, int y1) {
  if (x < 0 || x >= width) return;

  if (y0 > y1) std::swap(y0, y1);
  if (y0 < 0) y0 = 0;
  if (y1 >= height) y1 = height - 1;

  y1 -= y0;
  unsigned char* ptr = pixels + x * BPP + y0 * stride;
  for (int i = y1; i >= 0; --i) {
    ptr[0] = color.r;
    ptr[1] = color.g;
    ptr[2] = color.b;
    ptr += stride;
  }
}

void Canvas::horizontal(const RGB &color, int y, int x0, int x1) {
  if (y < 0 || y >= height) return;

  if (x0 > x1) std::swap(x0, x1);
  if (x0 < 0) x0 = 0;
  if (x1 >= width) x1 = width - 1;

  unsigned char* ptr = pixels + x0 * BPP + y * stride;
  x1 -= x0;
  for (int i = x1; i >= 0; --i) {
    ptr[0] = color.r;
    ptr[1] = color.g;
    ptr[2] = color.b;
    ptr += BPP;
  }
}

void Canvas::box(const RGB &color, const Rect &rect) {
  horizontal(color, rect.y0, rect.x0, rect.x1);
  horizontal(color, rect.y1, rect.x0, rect.x1);
  vertical(color, rect.x0, rect.y0, rect.y1);
  vertical(color, rect.x1, rect.y0, rect.y1);
}

void Canvas::fillBox(const RGB &color, const Rect &rect) {
  Rect clip(rect);
  if (clip.x0 > clip.x1) std::swap(clip.x0, clip.x1);
  if (clip.y0 > clip.y1) std::swap(clip.y0, clip.y1);

  // Clipping
  if (clip.x0 < 0) clip.x0 = 0;
  if (clip.x1 >= width) clip.x1 = width - 1;
  if (clip.y0 < 0) clip.y0 = 0;
  if (clip.y1 >= height) clip.y1 = height - 1;

  // Is the box inside the image?
  if (clip.x0 >= width
    || clip.x1 < 0
    || clip.y0 >= height
    || clip.y1 < 0)
      return;

  int s0 = (clip.x1 - clip.x0 + 1) * BPP;
  unsigned char* src = pixels + clip.y0 * stride + clip.x0 * BPP;
  unsigned char* ptr = src;

  // First row - fill with the color
  for (int j = clip.x0; j <= clip.x1; ++j) {
    ptr[0] = color.r;
    ptr[1] = color.g;
    ptr[2] = color.b;
    ptr += BPP;
  }
  ptr = src + stride;

  // Other rows - copy the first one
  for (int i = clip.y0 + 1; i <= clip.y1; ++i) {
    memcpy(ptr, src, s0);
    ptr += stride;
  }
}

void Canvas::textOut(const std::string &text, FreeType &font, const RGB &color, const Rect &rect, bool centerH) {
  Rect clip(rect);

  if (clip.x0 > clip.x1) std::swap(clip.x0, clip.x1);
  if (clip.y0 > clip.y1) std::swap(clip.y0, clip.y1);

  // No out-of-screen position so far
  assert(clip.x0 >= 0
    && clip.x1 < width
    && clip.y0 >= 0
    && clip.y1 < height);

  // Get text bitmap
  std::shared_ptr<TextBitmap> tbmp = font.getTextImage(text);

  // Position to start (center text vertically in the rect)
  unsigned char *ptr = pixels
    + clip.x0 * BPP
    + stride * (clip.y0 + (clip.y1 - clip.y0 - tbmp->getBaseline()) / 2);

  int w = clip.x1 - clip.x0;

  int skip = 0;
  if (centerH) {
    if (w > tbmp->getWidth()) {
      // Move starting position to the right
      ptr += (w - tbmp->getWidth()) / 2 * BPP;
      w = tbmp->getWidth();
    } else {
      // Cut out text outside the rect
      skip = (tbmp->getWidth() - w) / 2;
    }
  } else {
    if (w > tbmp->getWidth() ) w = tbmp->getWidth();
  }

  int dstride = stride - w * BPP;

  // Cut out text below/above the clipping area
  int from = 0;
  int till = tbmp->getHeight();

  if (till > clip.y1 - clip.y0) {
    till = clip.y1 - clip.y0;
    from = (till + 1) / 2;
    ptr += stride * from;
  }

  for (int i = from; i < till; ++i) {
    const unsigned char* src = tbmp->getPixels() + i * tbmp->getWidth() + skip;
    for (int j = 0; j < w; ++j) {
      int value = src[0];
      switch (value) {
        case 0: break;

        case 0xff:
          ptr[0] = color.r;
          ptr[1] = color.g;
          ptr[2] = color.b;
          break;

        default:
          ptr[0] = blend(ptr[0], color.r, value);
          ptr[1] = blend(ptr[1], color.g, value);
          ptr[2] = blend(ptr[2], color.b, value);
      }
      ptr += 3;
      ++src;
    }
    ptr += dstride;
  }
}

void Canvas::blendImage(const unsigned char *src, unsigned factor) {
  blend(pixels, src, size, factor);
}

void Canvas::copy(const unsigned char *src) {
  memmove(pixels, src, size);
}

void blend(unsigned char *dest, const unsigned char *src, int size, unsigned factor) {
  if (!factor) return;

  if (factor == 0xff) {
    memmove(dest, src, size);
    return;
  }

  for (int i = 0; i < size; ++i) {
    int result = (int)(*src) - (int)(*dest);
    *dest += result * factor / 256;
    ++dest;
    ++src;
  }
}

}
