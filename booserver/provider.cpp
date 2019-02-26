#include "provider.h"
#include "date.h"
#include "connection.h"
#include <string>
#include <sstream>
#include <list>
#include <algorithm>
#include <functional>

//          POCO structures for DAL
//******************************************
struct profile {
  int profile_id;
  std::string title;
  int doctors;
  int skip;
};

struct sdoctor {
  std::string name;
  std::string study;
  int doctor_id;
};

struct TypicalDay {
  std::string title;
  DateTime from;
  DateTime till;
  int parity;
  int wday;
  int wdaynum;
  
  TypicalDay(const char *t): title(t) {}
};

// Timetable paginator type
typedef std::vector<std::vector<profile>> Pages;

const std::string SET_CODEPAGE = 
  "set names 'utf8' collate 'utf8_general_ci'";
  
const std::string SELECT_PAGES =
  "select p.profile_id, max(p.title) title, count(*)"
  " from profile p"
  " join doctor d on d.profile_id = p.profile_id"
  " where (p.active > 0) and (d.active > 0)"
  " group by p.profile_id"
  " having count(*) > 0"
  " order by p.nline asc";


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

std::string getSelectDoctorsQuery(const profile& prof) {
  std::ostringstream result;
  result
    << "select name, study, doctor_id from doctor"
    << " where (profile_id = " << prof.profile_id
    << ") and (active > 0)"
    << " order by name limit " << prof.skip
    << ", " << prof.doctors;
  return result.str();
}

class ProviderImpl {
  MySqlConnection connection;
  int linesCount;
  int headerLines;
  
  std::string server;
  std::string database;
  std::string user;
  std::string password;
  
  Pages pages;
  
  bool loadProfiles(std::list<profile> &profiles);
public:
  ProviderImpl(int linesCount, int headerLines);

  bool init(const std::string &server,
    const std::string &database,
    const std::string &user,
    const std::string &password);

  bool update(void);
  Profiles getPage(int page);
  int getPagesCount(void) const;
};


//           Provider
// *******************************************
Provider::Provider(int linesCount, int headerLines) {
  impl = new ProviderImpl(linesCount, headerLines);
}

Provider::~Provider() {
  delete static_cast<ProviderImpl*>(impl);
}

bool Provider::update(void) {
  return static_cast<ProviderImpl*>(impl)->update();
}

Profiles Provider::getPage(int page) {
  return static_cast<ProviderImpl*>(impl)->getPage(page);
}

int Provider::getPagesCount(void) const {
  return static_cast<ProviderImpl*>(impl)->getPagesCount();
}

bool Provider::init(const std::string &server,
  const std::string &database,
  const std::string &user,
  const std::string &password)
{
  return static_cast<ProviderImpl*>(impl)->init(server, database, user, password);
}


//                   Utils
// *******************************************
// Check a date against a typed day rule.
// Returns true if the date conforms to the rule
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

// Priming list of doctors for the profile
void fillDoctors(MySqlConnection &conn, std::vector<sdoctor> &docs, const profile &prof) {
  std::shared_ptr<MYSQL_RES> res = conn.query(getSelectDoctorsQuery(prof));
  if (!res) return;
  while (MYSQL_ROW row = mysql_fetch_row(res.get())) {
    docs.push_back({
      std::string(row[0]),
      std::string(row[1]),
      getInt(row, 2)
    });
  }
}

// Get typed day rules for the doctor's timetable
void fillRules (MySqlConnection &conn, std::vector<TypicalDay> &days, const sdoctor &doc) {
  std::string query = getSelectRulesQuery(doc.doctor_id);
  std::shared_ptr<MYSQL_RES> res = conn.query(query);
  if (!res) return;
  
  while (MYSQL_ROW row = mysql_fetch_row(res.get())) {
    TypicalDay day(row[0] ? row[0] : "noname");
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
}


//           ProviderImpl
// *******************************************
ProviderImpl::ProviderImpl(int linesCount, int headerLines):
  connection(),
  linesCount(linesCount),
  headerLines(headerLines) {}

bool ProviderImpl::init(const std::string &server,
  const std::string &database,
  const std::string &user,
  const std::string &password)
{
  this->server = server;
  this->database = database;
  this->user = user;
  this->password = password;
  return connection.init(server, database, user, password);
}

Profiles ProviderImpl::getPage(int page) {
  Profiles result;
  if (!pages.empty()) {
    const std::vector<profile> &ps = pages[page % pages.size()];
    
    // Get beginning of the week
    DateTime wstart;
    wstart.setNow();
    wstart.discardTime();
    wstart.incDay(-wstart.getWeekDay());

    MySqlConnection conn;
    if (conn.init(server, database, user, password)) {
      // For each profile on the page
      for (const profile &prof : ps) {
        result.emplace_back(prof.title);
        std::vector<sdoctor> docs;
        fillDoctors(conn, docs, prof);
        
        // For each doctor in the profile
        for (auto doc : docs) {
          timetable::Doctor doctor(doc.name, doc.study);
          std::vector<TypicalDay> days;
          fillRules(conn, days, doc);

          // For each day of the current week
          DateTime date(wstart);
          for (int i = 0; i < 7; ++i) {
            std::vector<TypicalDay>::const_iterator day = std::find_if(days.begin(),
              days.end(),
              std::bind(dayFits, date, std::placeholders::_1));

            if (day != days.end()) {
              doctor[(timetable::Weekdays)i].title = day->title;
            } else {
              doctor[(timetable::Weekdays)i].title = "-";
            }
            date.incDay(1);
          }
          
          result.back().addDoctor(std::move(doctor));
        }
      }
    }
  }
  return result;
}

int ProviderImpl::getPagesCount(void) const {
  return pages.size();
}

bool ProviderImpl::loadProfiles(std::list<profile> &profiles) {
  std::shared_ptr<MYSQL_RES> res = connection.query(SELECT_PAGES);
  if (!res) return false;

  while (MYSQL_ROW row = mysql_fetch_row(res.get())) {
    profiles.push_back({
      getInt(row, 0),
      std::string(row[1]),
      getInt(row, 2),
      0
    });
  }

  return true;
}

bool ProviderImpl::update(void) {
  std::list<profile> profiles;
  pages.clear();
  
  bool result = false;
  
  if (loadProfiles(profiles)) {
    std::list<profile>::iterator it = profiles.begin();
    
    // Split profiles with too many doctors
    while (it != profiles.end()) {
      if (it->doctors > linesCount - headerLines) {
        it->doctors -= (linesCount - headerLines);
        it->skip += linesCount - headerLines;
        profiles.insert(it, profile { it->profile_id, it->title, linesCount - headerLines, 0 });
      } else {
        ++it;
      }
    }
    
    // Split whole list on pages
    int lines = 0;
    pages.push_back(std::vector<profile>());
    for (const auto& p : profiles) {
      if ((lines + p.doctors + headerLines) <= linesCount)  {
        lines += p.doctors + headerLines;
      } else {
        lines = p.doctors + headerLines;
        pages.push_back(std::vector<profile>());
      }
      pages.back().push_back(p);
    }
    
    result = true;
  }
  
  return result;
}
