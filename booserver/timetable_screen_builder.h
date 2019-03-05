#pragma once

#include "canvas.h"
#include "ftype.h"
#include "profile.h"
#include "options.h"
#include <vector>
#include <array>

/**
 * @class TimetableScreenBuilder
 * @author Sunders
 * @date 01.11.2018
 * @file timetable_screen_builder.h
 * @brief Builds screen image of a timetable page
 */
class TimetableScreenBuilder {
  image::Canvas canvas;
  unsigned char* background;
  int line_height;
  int title_width;
  int study_width;
  int day_width;
  bool testing;
  image::RGB color_clear;
  image::RGB title_face;
  image::RGB header_back;
  image::RGB header_face;
  image::RGB line_odd;
  image::RGB line_even;
  image::RGB line_face;
  image::RGB counter_face;
  image::FreeType font_title;
  image::FreeType font_caption;
  image::FreeType font_text;
  std::string font_file;
  std::string title_name;
  std::string title_study;
  std::string title_no_time;
  std::array<std::string, 7> weekdays;
  std::string title_testing;
  std::string bg_image_file;

  TimetableScreenBuilder(TimetableScreenBuilder&) = delete;
  void operator=(TimetableScreenBuilder&) = delete;
  TimetableScreenBuilder() = delete;
public:
  /**
   * @brief               Ctor
   * @param options       Booserver options
   */
  explicit TimetableScreenBuilder(const Options &options);
  ~TimetableScreenBuilder();
  
  bool init(void);

  /**
   * @brief               Make image for the given timetable page
   * @param ps            Timetable page description
   * @param page          Page ordinal number
   * @param pageCount     Total pages count
   */
  void build(const Profiles &ps, int page, int pageCount);
  
  image::Canvas& getCanvas(void);
  
  /**
   * @brief               Get image
   */
  const unsigned char *getPixels(void) const;
  
  /**
   * @brief               Get image
   */
  unsigned char *getPixels(void);

  /**
   * @brief               Get image width in pixels
   */
  int getWidth(void) const;
  
  /**
   * @brief               Get image height in pixels
   */
  int getHeight(void) const;
  
  /**
   * @brief               Get buffer size
   */
  int getSize(void) const;
};

