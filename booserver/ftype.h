#pragma once
#include "text_bitmap.h"

#include <string>
#include <memory>

namespace image {

/**
 * @class FreeType
 * @author Sunders
 * @date 31.10.2018
 * @file ftype.h
 * @brief Wrapper around libfreetype
 */
class FreeType {
  void *face;
  FreeType(FreeType&) = delete;
  void operator =(FreeType&) = delete;
public:
  FreeType();

  /**
   * @brief                   Load and init font
   * @param fontFilename      TTF filename
   * @param fontSize          In pt
   * @param dpi               Dots per inch
   */
  bool init (const std::string &fontFilename, int fontSize, int dpi = 72);

  ~FreeType();

  /**
   * @brief           Get text converted to pixels
   * @param text      UTF-8 text to draw
   *
   * Output format is GREY8
   */
  std::shared_ptr<TextBitmap> getTextImage(const std::string &text);
};

}
