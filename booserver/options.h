#pragma once

#include <string>
#include "rgb.h"

const std::string DEFAULT_OPTIONS_FILENAME = "booserver.conf";

class Options {
  int width;
  int height;
  int fps;
  int lines;
  int duration;
  bool testing;
  std::string ip;
  std::string font;
  std::string sql_host;
  std::string sql_database;
  std::string sql_user;
  std::string sql_password;
  std::string title_name;
  std::string title_study;
  std::string title_no_time;
  std::string title_monday;
  std::string title_tuesday;
  std::string title_wednesday;
  std::string title_thursday;
  std::string title_fryday;
  std::string title_saturday;
  std::string title_sunday;
  std::string title_testing;
  image::RGB title_face;
  image::RGB header_back;
  image::RGB header_face;
  image::RGB line_odd;
  image::RGB line_even;
  image::RGB line_face;
  image::RGB counter_face;
public:
  Options();
  bool load(const std::string &fileName);
  
  int getWidth(void) const { return width; }
  int getHeight(void) const { return height; }
  int getFps(void) const { return fps; }
  int getLines(void) const { return lines; }
  int getDuration(void) const { return duration; }
  bool isTesting(void) const { return testing; }
  
  const std::string& getIP(void) const { return ip; }
  const std::string& getFontFile(void) const { return font; }
  const std::string& getSqlHost(void) const { return sql_host; }
  const std::string& getSqlDatabase(void) const { return sql_database; }
  const std::string& getSqlUser(void) const { return sql_user; }
  const std::string& getSqlPassword(void) const { return sql_password; }

  const std::string& getTitleName(void) const { return title_name; }
  const std::string& getTitleStudy(void) const { return title_study; }
  const std::string& getTitleNoTime(void) const { return title_no_time; }
  const std::string& getTitleMonday(void) const { return title_monday; }
  const std::string& getTitileTuesday(void) const { return title_tuesday; }
  const std::string& getTitleWednesday(void) const { return title_wednesday; }
  const std::string& getTitleThursday(void) const { return title_thursday; }
  const std::string& getTitleFryday(void) const { return title_fryday; }
  const std::string& getTitleSaturday(void) const { return title_saturday; }
  const std::string& getTitleSunday(void) const { return title_sunday; }
  const std::string& getTitleTesting(void) const { return title_testing; }
  
  const image::RGB& getTitleFace(void) const { return title_face; }
  const image::RGB& getHeaderBack(void) const { return header_back; }
  const image::RGB& getHeaderFace(void) const { return header_face; }
  const image::RGB& getLineOdd(void) const { return line_odd; }
  const image::RGB& getLineEven(void) const { return line_even; }
  const image::RGB& getLineFace(void) const { return line_face; }
  const image::RGB& getCounterFace(void) const { return counter_face; }
};
