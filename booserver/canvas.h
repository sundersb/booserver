#pragma once

#include <string>
#include "rgb.h"
#include "ftype.h"

namespace image {
  typedef struct Rect {
    int x0;
    int y0;
    int x1;
    int y1;
  } Rect;

  /**
   * @class Canvas
   * @author Sunders
   * @date 31.10.2018
   * @file canvas.h
   * @brief Memory canvas to draw on
   * RGB24, no pixel-rows alignment
   */
  class Canvas {
    int width;
    int height;
    int size;
    int stride;
    unsigned char *pixels;

    Canvas() = delete;
    Canvas(Canvas&) = delete;
    Canvas(Canvas&&) = delete;
    void operator=(Canvas&) = delete;

  public:
    /**
     * @brief                 Ctor
     * @param width     Image width in pixels
     * @param height    Image height in pixels
     */
    Canvas(int width, int height);

    ~Canvas();

    /**
     * @brief           Get image width in pixels
     */
    int getWidth(void) const;

    /**
     * @brief           Get image height in pixels
     */
    int getHeight(void) const;

    /**
     * @brief           Get buffer size
     */
    int getSize(void) const;

    /**
     * @brief           Get image data
     * RGB24, no pixel-rows alignment
     */
    unsigned char *getPixels(void);
    unsigned char *getPixels(void) const;

    /**
     * @brief           Fill canvas with black color
     */
    void clear(void);

    /**
     * @brief               Draw vertical line
     * @param color     Line color
     * @param x         Vertical position
     * @param y0        Starting row
     * @param y1        Ending row
     */
    void vertical(const RGB &color, int x, int y0, int y1);

    /**
     * @brief               Draw horizontal line
     * @param color     Line color
     * @param y         Offset from the topmost pixel row
     * @param x0        X position of the line start
     * @param x1        X position of the line end
     */
    void horizontal(const RGB &color, int y, int x0, int x1);

    /**
     * @brief               Draw a box filled with the given color
     * @param color     Fill color
     * @param rect      Coordinates of the boxe's top-left and bottom-right corners
     */
    void fillBox(const RGB &color, const Rect &rect);

    /**
     * @brief               Print a text on the canvas
     * @param text      UTF-8 encoded text to print
     * @param font      Font provider
     * @param color     Text color
     * @param rect      Bounding box of the text on the image
     * @param centerH   Center text horizontally within the box
     *
     *   Text is always centered vertically. Pixels which do not fit into the box are cut away
     */
    void textOut(const std::string &text, FreeType &font, const RGB &color, const Rect &rect, bool centerH = true);

    /**
     * @brief               Blend some other picture upon canvas
     * @param src       Data of the image to blend upon canvas
     * @param factor    Blending factor
     *
     * Source image MUST have the same size as this one - no checks are performed
     * Blending factor 0 means no action. Factor's value of 255 will draw source image
     * above this one without blending
     */
    void blendImage(const unsigned char *src, unsigned factor);
  };

  /**
   * @brief             Blend one image upon another
   * @param dest        Destination image
   * @param src         Source image
   * @param size        Image size
   * @param factor      Blend factor: 0 - only source, 0xff - only destination
   */
  void blend(unsigned char *dest, const unsigned char *src, int size, unsigned factor);
}
