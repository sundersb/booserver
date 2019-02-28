#pragma once

#include "provider.h"
#include "options.h"

namespace image {
  /// guchar
  typedef unsigned char Uchar;

  /**
   * @class Builder
   * @author Sunders
   * @date 25.10.2018
   * @file image_builder.h
   * @brief Bitmap generator
   */

  class Builder {
    void *impl;
    Builder() = delete;
    Builder(const Builder&) = delete;
    Builder(Builder&&) = delete;
    void operator=(const Builder&) = delete;
  public:
  /**
   * @brief           Ctor
   * @param width     Image width in pixels
   * @param height    Image height in pixels
   * @param fps       Sequence FPS
   */
    Builder(const Options &options, Provider &provider);
    ~Builder();
    
    bool init(void);

    /**
     * @brief         Get image frame for current time
     * @param now     Current timestamp
     * @param size    Image size in bytes
     * @returns       Pointer to image's raw pixel data
     */
    const Uchar* getNext(void);

    /**
     * @brief Get picture width in pixels
     */
    int getWidth(void) const;

    /**
     * @brief Get picture height in pixels
     */
    int getHeight(void) const;

    /**
     * @brief Get FPS
     */
    int getFps(void) const;

    /**
     * @brief  Get image size in pixels
     */
    unsigned getSize(void) const;

    /**
     * @brief  Get current frame number
     */
    unsigned getOffset(void) const;

    /**
     * @brief  Reset current stream position
     */
    void reset(void);
  };
}
