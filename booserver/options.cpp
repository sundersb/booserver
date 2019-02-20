#include "options.h"
#include <fstream>
#include <algorithm>
#include <map>

const int WIDTH = 640;
const int HEIGHT = 480;
const int FPS = 10;

const int LINES_COUNT = 18;
const int HEADER_LINES = 3;
const int DEFAULT_PAGE_DURATION = 30;
const int DEFAULT_PORT = 8554;

const std::string DEFAULT_FONT_FILENAME = "SourceSerifPro-Regular.otf";

const std::string DEFAULT_TITLE_NAME = "NAME";
const std::string DEFAULT_TITLE_STUDY = "ROOM";
const std::string DEFAULT_TITLE_NOT_ACCESSIBLE = "no reception";
const std::string DEFAULT_TITLE_MONDAY = "MON";
const std::string DEFAULT_TITLE_TUESDAY = "TUE";
const std::string DEFAULT_TITLE_WEDNESDAY = "WED";
const std::string DEFAULT_TITLE_THURSDAY = "THU";
const std::string DEFAULT_TITLE_FRYDAY = "FRY";
const std::string DEFAULT_TITLE_SATURDAY = "SAT";
const std::string DEFAULT_TITLE_SUNDAY = "SUN";
const std::string DEFAULT_TITLE_TESTING = "TESTING";

const image::RGB COLOR_CLEAR  = { 0x00, 0x00, 0x00 };
const image::RGB COLOR_TITLE  = { 0x49, 0xba, 0x5a };
const image::RGB COLOR_WHITE  = { 0xff, 0xff, 0xff };
const image::RGB COLOR_ORANGE = { 0xf3, 0x8e, 0x42 };
const image::RGB COLOR_PINK   = { 0xfd, 0xe3, 0xf8 };
const image::RGB COLOR_BLACK  = { 0x00, 0x00, 0x00 };
const image::RGB COLOR_GOLD   = { 0xff, 0xd7, 0x00 };
const image::RGB COLOR_ATTENTION   = { 0xff, 0x10, 0x10 };

const std::string IP = "127.0.0.1";
const std::string DEFAULT_MOUNT_POINT = "/live";

const std::string SQL_SERVER = "localhost";
const std::string SQL_DATABASE = "booserver";
const std::string SQL_USER = "booserver";
const std::string SQL_PASSWORD = "booserver";

enum PDataType { TYPE_INT, TYPE_BOOL, TYPE_STRING, TYPE_COLOR };
struct PMap {
  void *data;
  PDataType dtype;
};

Options::Options():
  width(WIDTH),
  height(HEIGHT),
  fps(FPS),
  lines(LINES_COUNT),
  duration(DEFAULT_PAGE_DURATION),
  port(DEFAULT_PORT),
  testing(false),
  ip(IP),
  mount_point(DEFAULT_MOUNT_POINT),
  font(DEFAULT_FONT_FILENAME),
  sql_host(SQL_SERVER),
  sql_database(SQL_DATABASE),
  sql_user(SQL_USER),
  sql_password(SQL_PASSWORD),
  title_name(DEFAULT_TITLE_NAME),
  title_study(DEFAULT_TITLE_STUDY),
  title_no_time(DEFAULT_TITLE_NOT_ACCESSIBLE),
  title_monday(DEFAULT_TITLE_MONDAY),
  title_tuesday(DEFAULT_TITLE_TUESDAY),
  title_wednesday(DEFAULT_TITLE_WEDNESDAY),
  title_thursday(DEFAULT_TITLE_THURSDAY),
  title_fryday(DEFAULT_TITLE_FRYDAY),
  title_saturday(DEFAULT_TITLE_SATURDAY),
  title_sunday(DEFAULT_TITLE_SUNDAY),
  title_testing(DEFAULT_TITLE_TESTING),
  clear_color(COLOR_CLEAR),
  title_face(COLOR_TITLE),
  header_back(COLOR_ORANGE),
  header_face(COLOR_WHITE),
  line_odd(COLOR_PINK),
  line_even(COLOR_WHITE),
  line_face(COLOR_BLACK),
  counter_face(COLOR_GOLD)
{}

void setInt(PMap &pmap, const std::string &value) {
  int *data = static_cast<int*>(pmap.data);
  *data = std::stoi(value);
}

void setBool(PMap &pmap, const std::string &value) {
  bool *data = static_cast<bool*>(pmap.data);
  
  if (std::all_of(value.begin(), value.end(), [] (char c) { return c >= '0' && c <= '9'; })) {
    // Number
    *data = std::stoi(value) != 0;
  } else {
    // false/true/on/off
    *data = !std::any_of(value.begin(), value.end(), [] (char c) { return c == 'f' || c == 'F'; });
  }
}

void setString(PMap &pmap, const std::string &value) {
  std::string *data = static_cast<std::string*>(pmap.data);
  *data = value;
}

void setColor(PMap &pmap, const std::string &value) {
  int idata = std::stoi(value, nullptr, 16);
//  std::cout << "0x" << std::hex << idata;

  image::RGB *data = static_cast<image::RGB*>(pmap.data);
  data->r = (idata >> 16) & 0xff;
  data->g = (idata >> 8) & 0xff;
  data->b = idata & 0xff;
}

void setValue(std::map<std::string, PMap> &mpx, const std::string &key, const std::string &value) {
  std::map<std::string, PMap>::iterator it = mpx.find(key);
  if (it == mpx.end()) return;

  PMap &pmap = it->second;
  switch (pmap.dtype) {
    case TYPE_INT:
      setInt(pmap, value);
      break;
    case TYPE_BOOL:
      setBool(pmap, value);
      break;
    case TYPE_STRING:
      setString(pmap, value);
      break;
    case TYPE_COLOR:
      setColor(pmap, value);
      break;
  }
}

bool load_file(const std::string &fileName, std::map<std::string, PMap> &mpx) {
  std::ifstream file(fileName.c_str(), std::ifstream::in | std::ifstream::binary);
  if (!file.good()) return false;

  std::string line;
  while (std::getline(file, line)) {
    size_t from = line.find_first_not_of(" \t");
    if (from == std::string::npos || line[from] == '#') continue;

    size_t equ = line.find('=');
    if (equ == from || equ == std::string::npos) continue;
    
    size_t pos = equ - 1;
    while (std::isspace(line[pos])) --pos;
    std::string key(line, from, pos - from +1);
    
    from = equ + 1;
    while (std::isspace(line[from])) ++from;
    pos = line.find_last_not_of(" \t\r\n");
    if (pos == from) continue;
    std::string value(line, from, pos - from + 1);
    
    setValue(mpx, key, value);
  }
  
  return true;
}

bool Options::load(const std::string &fileName) {
  std::string color_file;

  std::map<std::string, PMap> mpx;
  
  mpx["interface"] = { &ip, TYPE_STRING };
  mpx["mount_point"] = { &mount_point, TYPE_STRING };
  mpx["width"] = { &width, TYPE_INT };
  mpx["height"] = { &height, TYPE_INT };
  mpx["fps"] = { &fps, TYPE_INT };
  mpx["lines"] = { &lines, TYPE_INT };
  mpx["page_duration"] = { &duration, TYPE_INT };
  mpx["port"] = { &port, TYPE_INT };
  mpx["testing"] = { &testing, TYPE_BOOL };
  mpx["fontfile"] = { &font, TYPE_STRING };
  mpx["host"] = { &sql_host, TYPE_STRING };
  mpx["database"] = { &sql_database, TYPE_STRING };
  mpx["user"] = { &sql_user, TYPE_STRING };
  mpx["password"] = { &sql_password, TYPE_STRING };
  mpx["title_name"] = { &title_name, TYPE_STRING };
  mpx["title_study"] = { &title_study, TYPE_STRING };
  mpx["title_no_time"] = { &title_no_time, TYPE_STRING };
  mpx["title_monday"] = { &title_monday, TYPE_STRING };
  mpx["title_tuesday"] = { &title_tuesday, TYPE_STRING };
  mpx["title_wednesday"] = { &title_wednesday, TYPE_STRING };
  mpx["title_thursday"] = { &title_thursday, TYPE_STRING };
  mpx["title_fryday"] = { &title_fryday, TYPE_STRING };
  mpx["title_saturday"] = { &title_saturday, TYPE_STRING };
  mpx["title_sunday"] = { &title_sunday, TYPE_STRING };
  mpx["title_testing"] = { &title_testing, TYPE_STRING };
  mpx["color_clear"] = { &clear_color, TYPE_COLOR };
  mpx["color_title_face"] = { &title_face, TYPE_COLOR };
  mpx["color_header_back"] = { &header_back, TYPE_COLOR };
  mpx["color_header_face"] = { &header_face, TYPE_COLOR };
  mpx["color_line_odd"] = { &line_odd, TYPE_COLOR };
  mpx["color_line_even"] = { &line_even, TYPE_COLOR };
  mpx["color_line_face"] = { &line_face, TYPE_COLOR };
  mpx["color_counter_face"] = { &counter_face, TYPE_COLOR };
  
  mpx["color_file"] = { &color_file, TYPE_STRING };

  if (!load_file(fileName, mpx)) return false;
  
  if (!color_file.empty() && !load_file(color_file, mpx)) return false;
  
  if (testing) counter_face = COLOR_ATTENTION;
  
  return true;
}
