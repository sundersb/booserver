#include "text_bitmap.h"
#include <cstring>

namespace image {

TextBitmap::TextBitmap(int w, int h, int baseline):
  width(w),
  height(h),
  baseline(baseline)
{
  int size = width * height;
  pixels = new unsigned char [size];
  memset(pixels, 0, size);
}

TextBitmap::~TextBitmap() {
  delete [] pixels;
}

}
