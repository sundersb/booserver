#include <iostream>
#include <array>
#include <algorithm>
#include <functional>
#include <mysql.h>
#include <memory>
#include <sstream>
#include <assert.h>
#include "date.h"

struct TypicalDay {
  std::string title;
  DateTime from;
  DateTime till;
  int parity;
  int wday;
  int wdaynum;
  
  TypicalDay(const char *t): title(t) {}
};

const int DOCTOR_ID = 119;

const std::array<std::string, 7> WEEKDAYS = {
  "Mon",
  "Tue",
  "Wed",
  "Thr",
  "Fri",
  "Sat",
  "Sun"
};

std::string getSelectRulesQuery(int doctor_id) {
  std::ostringstream result;
  result
    << "select title, dfrom, dtill, parity, wday, wdaynum"
    << " from rule"
    << " where doctor_id = "
    << doctor_id
    << " order by nline";
  return result.str();
}

const std::string SET_CODEPAGE = 
  "set names 'utf8' collate 'utf8_general_ci'";

class MySqlConnection {
  MYSQL *conn;
public:
  MySqlConnection();
  ~MySqlConnection();
  bool init(const std::string &host, const std::string &db, const std::string &user, const std::string &pass);
  MYSQL* get(void) { return conn; }
  std::shared_ptr<MYSQL_RES> query(const std::string &sql);
};

MySqlConnection::MySqlConnection(): conn(nullptr) {}

MySqlConnection::~MySqlConnection() {
  if (conn) mysql_close(conn);
}

bool MySqlConnection::init(const std::string &host,
  const std::string &db,
  const std::string &user,
  const std::string &pass)
{
  conn = mysql_init(nullptr);
  if (!conn) return false;
  
  if (!mysql_real_connect(conn,
    host.c_str(),
    user.c_str(),
    pass.c_str(),
    db.c_str(), 0, 0, 0)) return false;
    
  return mysql_query(conn, SET_CODEPAGE.c_str()) == 0;
}

std::shared_ptr<MYSQL_RES> MySqlConnection::query(const std::string &sql) {
  assert(conn);
  if (mysql_query(conn, sql.c_str())) return nullptr;
  
  std::shared_ptr<MYSQL_RES> result(mysql_store_result(conn), mysql_free_result);
  return result;
}

inline int getInt(MYSQL_ROW row, int fieldNum) {
  if (row[fieldNum]) return std::stoi(row[fieldNum]);
  else return -1;
}

bool loadRules(std::vector<TypicalDay> &days) {
  MySqlConnection conn;
  if (!conn.init("localhost", "booserver", "booserver", "booserver")) return false;
  
  auto res = conn.query(getSelectRulesQuery(DOCTOR_ID));
  if (!res) return false;

  while (MYSQL_ROW row = mysql_fetch_row(res.get())) {
    TypicalDay day(row[0]);
    if (row[1]) day.from = DateTime(std::string(row[1]));
    if (row[2]) day.till = DateTime(std::string(row[2]));
    day.parity = getInt(row, 3);
    if (day.parity >= 0) day.parity %= 2;

    day.wday = getInt(row, 4);
    if (day.wday > 0) day.wday = (day.wday - 1) % 7;

    day.wdaynum = getInt(row, 5);
    if (day.wdaynum > 0) --day.wdaynum;
    
    days.push_back(day);
  }
  
  return true;
}

void describeRules(const std::vector<TypicalDay> &days) {
  for (const TypicalDay &day : days) {
    std::cout << day.title << ", ";
    
    if (day.from.isValid()) {
      if (day.till.isValid()) {
        std::cout << day.from << " - " << day.till << ", ";
      } else {
        std::cout << "from " << day.from << ", ";
      }
    } else {
      if (day.till.isValid()) {
        std::cout << "till " << day.till << ", ";
      }
    }
    
    if (day.parity >= 0) {
      if (day.parity)
        std::cout << "odd, ";
      else
        std::cout << "even, ";
    }
    
    if (day.wday >= 0) {
      std::cout << WEEKDAYS[day.wday];
    }
    
    if (day.wdaynum >= 0) {
      std::cout << " (" << day.wdaynum + 1 << " in the month)";
    }
    std:: cout << std::endl;
  }
}

bool dayFits(const DateTime &date, const TypicalDay &day) {
  if (day.from.isValid() && date < day.from) return false;
  if (day.till.isValid() && date > day.till) return false;
  
  struct tm time;
  date.asTime(&time);
  if (day.parity >= 0 && (time.tm_mday % 2) != day.parity) return false;
  if (day.wday >= 0) {
    if (date.getWeekDay() != day.wday) return false;
    if (day.wdaynum >= 0 && (time.tm_mday / 7) != day.wdaynum) return false;
  }
  return true;
}

int main(int argc, char **argv) {
  DateTime wstart;
  wstart.setNow();
  std::cout << "Now " << wstart << std::endl;

  wstart.discardTime();
  std::cout << "Today " << wstart << std::endl;
  
  wstart.incDay(-wstart.getWeekDay());
  std::cout << "Week start " << wstart << std::endl;

  std::vector<TypicalDay> days;
  if (!loadRules(days)) {
    std::cout << "Error loading rules!" << std::endl;
    return 1;
  }

  describeRules(days);

  DateTime date(wstart);
  for (int i = 0; i < 7; ++i) {
    std::cout << WEEKDAYS[i] << ", " << date << ": ";
    
    std::vector<TypicalDay>::const_iterator day = std::find_if(days.begin(),
      days.end(),
      std::bind(dayFits, date, std::placeholders::_1));
    
    if (day != days.end()) {
      std::cout << day->title;
    } else {
      std::cout << "not rule!";
    }
    
    std::cout << std::endl;
    date.incDay(1);
  }
  std::cout << "Total of " << days.size() << " rules" << std::endl;
	return 0;
}
