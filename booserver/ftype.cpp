#include "ftype.h"
#include <vector>
#include <numeric>
#include <assert.h>
#include <codecvt>
#include <locale>
#include <iostream>

#ifdef _WIN32
  #include <io.h>
  #define access _access
  #include <ft2build.h>
  #include FT_FREETYPE_H
#else
  #include <unistd.h>
  #include <freetype2/ft2build.h>
  #include <freetype2/freetype/freetype.h>
#endif

// https://www.freetype.org/freetype2/docs/tutorial/step1.html#section-7

namespace image {

// Helper class to contain and draw letter glyph
class Glyph {
  int h;
  int w;
  int ax;
  int left;
  int top;
  int tail;
  unsigned char *pixels;

  void operator==(Glyph&) = delete;
  Glyph(Glyph&) = delete;
public:
  Glyph(Glyph &&rhs);
  explicit Glyph(const FT_GlyphSlot &slot);
  Glyph();
  ~Glyph();

  // Distance from baseline to the glyph's top
  int getTop(void) const { return top; }

  // Length of the glyph's part below the text baseline
  int getTail(void) const { return tail; }

  // Full width of the glyph including actual width and left/right margins
  int getFullWidth(void) const { return ax; }

  /**
   * @brief           Draw glyph on a memory buffered canvas
   * @param dest      Destination address
   * @param x         Letter's horizontal position on the canvas
   * @param baseline  Text baseline
   * @param stride    Width of the canvas in pixels
   * @param size      Canva's buffer size
   */
  void draw(unsigned char* dest, int x, int baseline, int stride, int size) const;

  void draw(unsigned char* dest, int x, int baseline, int stride) const;
};

Glyph::Glyph():
  h(0),
  w(0),
  ax(0),
  left(0),
  top(0),
  tail(),
  pixels(nullptr)
{
}

Glyph::Glyph(Glyph &&rhs):
  h(rhs.h),
  w(rhs.w),
  ax(rhs.ax),
  left(rhs.left),
  top(rhs.top),
  tail(rhs.tail),
  pixels(nullptr)
{
  assert(rhs.pixels);
  std::swap(pixels, rhs.pixels);
  assert(rhs.pixels == nullptr);
}

Glyph::Glyph(const FT_GlyphSlot &slot):
  h(slot->bitmap.rows),
  w(slot->bitmap.width),
  ax(slot->advance.x >> 6),
  left(slot->bitmap_left),
  top(slot->bitmap_top),
  tail(h - top)
{
  int size(w * h);
  pixels = new unsigned char [size];
  memcpy(pixels, slot->bitmap.buffer, size);
}

Glyph::~Glyph() {
  delete [] pixels;
}

void Glyph::draw(unsigned char* dest, int x, int baseline, int stride) const {
  unsigned char *src = pixels;
  dest += x + left + stride * (baseline - top);
  for (int i = 0; i < h; ++i) {
    memcpy(dest, src, w);
    src += w;
    dest += stride;
  }
}

void Glyph::draw(unsigned char* dest, int x, int baseline, int stride, int size) const {
  // Characters horizontal offset on the canvas
  int dx = x + left + stride * (baseline - top);

  // Sometimes freetype's left is below zero
  if (dx < 0) dx = 0;

  unsigned char *src = pixels;
  dest += dx;
  size -= dx;
  dx = w;

  for (int i = 0; i < h; ++i) {
    // Sometimes freetype's metrics allows a character to outrun the
    //  buffer by single pixel:
    if (size <= dx) {
      if (size > 0) memcpy(dest, src, size);
      break;
    }

    memcpy(dest, src, dx);
    src += dx;
    dest += stride;
    size -= stride;
  }
}

FT_Library getLibrary (void) {
  static FT_Library library;
  static bool inited = false;

  if (!inited) {
    inited = FT_Init_FreeType (&library) == 0;
  }

  return library;
}

FreeType::FreeType():
  face(nullptr) {}

FreeType::~FreeType() {
  if (face) FT_Done_Face(static_cast<FT_Face>(face));
}

bool fileExists (const std::string &Filename) {
    return access (Filename.c_str(), 0) == 0;
}

bool FreeType::init(const std::string &fontFilename, int fontSize, int dpi) {
  if (!fileExists(fontFilename)) return false;

  FT_Face lface;
  if (FT_New_Face(getLibrary(),
    fontFilename.c_str(),
    0,
    &lface)) return false;

  face = lface;
  if (FT_Set_Char_Size (lface, fontSize * 64, 0, dpi, 0)) return false;
  return true;
}

std::shared_ptr<TextBitmap> FreeType::getTextImage(const std::string &text) {
  // Convert to UTF-16
  std::wstring value = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(text);

  // Copy and keep glyphs around to detect text width and height
  int len = value.length();
  std::vector<Glyph> gs;
  gs.reserve(len);

  int width = 0;
  int baseline = 0;
  int height = 0;

  FT_Face lface = static_cast<FT_Face>(face);
  FT_GlyphSlot slot = lface->glyph;
  for (int i = 0; i < len; ++i) {
    if (FT_Load_Char(lface, value[i], FT_LOAD_RENDER)) continue;
    Glyph last(slot);

    width += last.getFullWidth();
    if (last.getTop() > baseline)
      baseline = last.getTop();

    // Max length of letter parts below the baseline
    if (last.getTail() > height)
      height = last.getTail();

    gs.push_back(std::move(last));
  }

  // Bitmap height:
  height += baseline;

  // Freetypes artefacts: sometimes left margin is -1 and actual size more by 1
  width += 2;

  std::shared_ptr<TextBitmap> result(new TextBitmap(width, height, baseline));

  // Ugly but keeps TextBitmap secure from client code
  unsigned char *buffer = const_cast<unsigned char*>(result->getPixels());

  int x = 1;
  for (const Glyph &g : gs) {
//    g.draw(buffer, x, baseline, width, size);
    g.draw(buffer, x, baseline, width);
    x += g.getFullWidth();
  }

  return result;
}

} // namespace image
