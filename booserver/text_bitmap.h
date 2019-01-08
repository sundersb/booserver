#pragma once

namespace image {

/**
 * @class TextBitmap
 * @author Sunders
 * @date 31.10.2018
 * @file text_bitmap.h
 * @brief Helper for exchanging CREY8 text bitmaps
 */
class TextBitmap {
  int width;
  int height;
  int baseline;
  unsigned char *pixels;

  TextBitmap() = delete;
  TextBitmap(TextBitmap&) = delete;
  void operator=(TextBitmap&) = delete;
public:
  /**
   * @brief           Ctor
   * @param w         Image width
   * @param h         Image height
   * @param baseline  Text baseline height
   * @memo            This constructor fills buffer with zeros
   */
  TextBitmap(int w, int h, int baseline);

  ~TextBitmap();

  /**
   * @brief Get image data
   */
  const unsigned char* getPixels(void) { return pixels; }

  /**
   * @brief Get image width in pixels
   */
  int getWidth(void) const { return width; }

  /**
   * @brief Get image height in pixels
   */
  int getHeight(void) const { return height; }

  /**
   * @brief Text baseline
   * Distance from this bitmap's top to the text line (in pixels)
   */
  int getBaseline(void) const { return baseline; }
};

}
