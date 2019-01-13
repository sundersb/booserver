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

const std::string DEFAULT_FONT_FILENAME = "SourceSerifPro-Regular.otf";

const image::RGB COLOR_TITLE  = { 0x49, 0xba, 0x5a };
const image::RGB COLOR_WHITE  = { 0xff, 0xff, 0xff };
const image::RGB COLOR_ORANGE = { 0xf3, 0x8e, 0x42 };
const image::RGB COLOR_PINK   = { 0xfd, 0xe3, 0xf8 };
const image::RGB COLOR_BLACK  = { 0x00, 0x00, 0x00 };
const image::RGB COLOR_GOLD   = { 0xff, 0xd7, 0x00 };

const std::string IP = "127.0.0.1";

const std::string SQL_SERVER = "localhost";
const std::string SQL_DATABASE = "booserver";
const std::string SQL_USER = "booserver";
const std::string SQL_PASSWORD = "booserver";

enum PDataType { TYPE_INT, TYPE_STRING, TYPE_COLOR };
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
  ip(IP),
  font(DEFAULT_FONT_FILENAME),
  sql_host(SQL_SERVER),
  sql_database(SQL_DATABASE),
  sql_user(SQL_USER),
  sql_password(SQL_PASSWORD),
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
//  std::cout << " - 0x" << std::hex << std::setfill('0') << std::setw(2) << (int)data->r
//    << ", 0x" << std::hex << std::setfill('0') << std::setw(2) << (int)data->g
//    << ", 0x" << std::hex << std::setfill('0') << std::setw(2) << (int)data->b
//    << std::setbase(10) << std::setfill(' ') << std::endl;
}

void setValue(std::map<std::string, PMap> &mpx, const std::string &key, const std::string &value) {
  std::map<std::string, PMap>::iterator it = mpx.find(key);
  if (it == mpx.end()) return;

  PMap &pmap = it->second;
  switch (pmap.dtype) {
    case TYPE_INT:
      setInt(pmap, value);
      break;
    case TYPE_STRING:
      setString(pmap, value);
      break;
    case TYPE_COLOR:
      setColor(pmap, value);
      break;
  }
}

bool Options::load(const std::string &fileName) {
  std::ifstream file(fileName.c_str(), std::ifstream::in | std::ifstream::binary);
  if (!file.good()) return false;

  std::map<std::string, PMap> mpx;
  mpx["interface"] = { &ip, TYPE_STRING };
  mpx["width"] = { &width, TYPE_INT };
  mpx["height"] = { &height, TYPE_INT };
  mpx["fps"] = { &fps, TYPE_INT };
  mpx["lines"] = { &lines, TYPE_INT };
  mpx["page_duration"] = { &duration, TYPE_INT };
  mpx["fontfile"] = { &font, TYPE_STRING };
  mpx["host"] = { &sql_host, TYPE_STRING };
  mpx["database"] = { &sql_database, TYPE_STRING };
  mpx["user"] = { &sql_user, TYPE_STRING };
  mpx["password"] = { &sql_password, TYPE_STRING };
  mpx["color_title_face"] = { &title_face, TYPE_COLOR };
  mpx["color_header_back"] = { &header_back, TYPE_COLOR };
  mpx["color_header_face"] = { &header_face, TYPE_COLOR };
  mpx["color_line_odd"] = { &line_odd, TYPE_COLOR };
  mpx["color_line_even"] = { &line_even, TYPE_COLOR };
  mpx["color_line_face"] = { &line_face, TYPE_COLOR };
  mpx["color_counter_face"] = { &counter_face, TYPE_COLOR };

  std::string line;
  while (std::getline(file, line)) {
    // TODO: cleanup?
    size_t from = line.find_first_not_of(' ');
    if (from == std::string::npos || line[from] == '#') continue;

    size_t till = line.find_last_not_of(' ');

    std::string::const_iterator begin = line.begin() + from;
    std::string::const_iterator end = till == std::string::npos ? line.end() : line.begin() + till + 1;

    std::string::const_iterator pos = std::find(begin, end, '=');
    if (pos == end) continue;

    std::string key (begin, pos);
    std::string value (pos + 1, end);
    setValue(mpx, key, value);
  }
  return true;
}
