#pragma once
#include "canvas.h"
#include "text_bitmap.h"
#include <string>

namespace png {  
  /**
   * @brief                       Save image as PNG file
   * @param fileName      Filename to save the image in
   * @param canvas        Image to save
   */
  bool toPNG (const std::string& fileName, image::Canvas &canvas);
  
  /**
   * @brief                       Load PNG file and draw it on canvas
   * @param fileName      PNG filename
   * @param canvas        Canvas to draw image on
   */
  bool drawPNG (const std::string& fileName, image::Canvas &canvas);
}
