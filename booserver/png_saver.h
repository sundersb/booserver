#pragma once
#include "canvas.h"
#include <string>

namespace png {
  /**
   * @brief                       Save image as PNG file
   * @param fileName      Filename to save the image in
   * @param canvas        Image to save
   */
  bool toPNG (const std::string& fileName, image::Canvas &canvas);
}
