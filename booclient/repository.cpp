#include "repository.h"
#include "connection.h"

#include <assert.h>
#include <sstream>
#include <algorithm>

namespace data {
  const std::string SQL_NULL = "null";
  const std::string SELECT_PROFILES_ACTIVE = "select profile_id, title, nline, active from profile where active > 0 order by nline";
  const std::string SELECT_PROFILES_ALL = "select profile_id, title, nline, active from profile order by nline";
  const std::string GET_LAST_PROFILE_NLINE = "select max(nline) from profile";
  const std::string RESERVE_FIRST_RULE = "update rule set nline = nline + 1 where doctor_id in (select doctor_id from doctor where active > 0)";

std::string getSelectDoctorsQuery(int profile, bool activeOnly) {
  std::ostringstream sql;
  sql << "select doctor_id, profile_id, name, study, active, duty, institute, certificate, category, fullname"
    << " from doctor where (profile_id = " << profile << ')';

  if (activeOnly) sql << " and (active > 0)";

  sql << " order by name";
  return sql.str();
}

/**
 * @brief             Escape single qoutes inside a string to avoid SQL-injection
 * @param value       Value to secure
 * @param fieldSize   Target field size. Resulting string will be no longer.
 * @return
 */
std::string secureString(const std::string &value, int fieldSize) {
  if (value.empty()) return SQL_NULL;

  std::ostringstream result;

  std::string::const_iterator from = value.begin();
  std::string::const_iterator end = value.end();
  std::string::const_iterator pos = from;

  do {
    pos = std::find(from, end, '\'');
    result << std::string(from, pos);
    if (pos != end) {
      while (pos != end && *pos == '\'') {
        result << "''";
        ++pos;
      }
      from = pos;
    }
  } while (pos != end);

  std::string res = result.str().substr(0, fieldSize);

  return '\'' + res + '\'';
}

std::string secureInt(int value) {
  if (value) {
    return std::to_string(value);
  } else {
    return SQL_NULL;
  }
}

std::string secureDate(const DateTime &value) {
  if (value.isValid()) {
    std::ostringstream result;
    result << '\'' << value.formatDate() << '\'';
    return result.str();
  } else {
    return SQL_NULL;
  }
}

Repository::Repository() {
  impl = new MySqlConnection();
}

Repository::~Repository() {
  delete static_cast<MySqlConnection*>(impl);
}

bool Repository::init(const std::string &host, const std::string &db, const std::string &user, const std::string &pass) {
  return static_cast<MySqlConnection*>(impl)->init(host, db, user, pass);
}

std::vector<Profile*> Repository::loadProfiles(bool activeOnly) {
  std::vector<Profile*> result;

  const std::string &query = activeOnly ? SELECT_PROFILES_ACTIVE : SELECT_PROFILES_ALL;

  std::shared_ptr<MYSQL_RES> res = static_cast<MySqlConnection*>(impl)->query(query);
  if (res) {
    while (MYSQL_ROW row = mysql_fetch_row(res.get())) {
      result.push_back(new Profile(getInt(row, 0), getString(row, 1)));
      Profile *p = result.back();
      p->setNumber(getInt(row, 2));
      p->setActive(getBool(row, 3));
      p->wash();
    }
  }
  return result;
}

int Repository::getLastProfileNumber(void) {
  int result = 0;

  std::shared_ptr<MYSQL_RES> res = static_cast<MySqlConnection*>(impl)->query(GET_LAST_PROFILE_NLINE);
  if (res) {
    MYSQL_ROW row = mysql_fetch_row(res.get());
    if (row) result = getInt(row, 0);
  }
  return result;
}

bool Repository::saveProfile(Profile *value) {
  if (!value || !value->isDirty()) return true;

  bool result = false;

  std::ostringstream sql;

  if (!value->getId()) {
    // Insert
    sql << "insert into profile (title, nline, active) values ("
      << secureString(value->getTitle(), 100)
      << ", " << value->getNumber()
      << ", " << (value->isActive() ? 1 : 0)
      << ")";
  } else {
    // Update
    sql << "update profile set title = " << secureString(value->getTitle(), 100)
      << ", nline = " << value->getNumber()
      << ", active = " << (value->isActive() ? 1 : 0)
      << " where profile_id = " << value->getId();
  }

  if (static_cast<MySqlConnection*>(impl)->exec(sql.str())) {
    if (!value->getId()) {
      value->setId(static_cast<MySqlConnection*>(impl)->getLastId());
      assert(value->getId());
    }
    value->wash();
    result = true;
  }

  return result;
}

bool Repository::saveProfiles(std::vector<Profile> &value) {
  for (Profile &p : value)
    if (!saveProfile(&p))
      return false;

  return true;
}

std::vector<Doctor*> Repository::loadDoctors(const Profile* profile, bool activeOnly) {
  std::vector<Doctor*> result;

  if (profile && profile->getId()) {
    const std::string query = getSelectDoctorsQuery(profile->getId(), activeOnly);

    std::shared_ptr<MYSQL_RES> res = static_cast<MySqlConnection*>(impl)->query(query);

    if (res) {
      while (MYSQL_ROW row = mysql_fetch_row(res.get())) {
        result.push_back(new Doctor(getInt(row, 0), getString(row, 2)));
        Doctor *doc = result.back();
        doc->setProfile(getInt(row, 1));
        doc->setStudy(getString(row, 3));
        doc->setActive(getBool(row, 4));
        doc->setDuty(getString(row, 5));
        doc->setInstitute(getString(row, 6));
        doc->setCertificate(getString(row, 7));
        doc->setCategory(getString(row, 8));
        doc->setFullName(getString(row, 9));
        doc->wash();
      }
    }
  }

  return result;
}

bool Repository::saveDoctor(Doctor *value) {
  if (!value || !value->isDirty()) return true;

  bool result = false;

  std::ostringstream sql;

  if (!value->getId()) {
    // Insert
    sql << "insert into doctor ("
      << "profile_id, name, fullname, study, active, duty, institute, certificate, category"
      << ") values ("
      << value->getProfile()
      << ", " << secureString(value->getName(), 100)
      << ", " << secureString(value->getFullName(), 100)
      << ", " << secureString(value->getStudy(), 20)
      << ", " << (value->isActive() ? 1 : 0)
      << ", " << secureString(value->getDuty(), 50)
      << ", " << secureString(value->getInstitute(), 50)
      << ", " << secureString(value->getCertificate(), 50)
      << ", " << secureString(value->getCategory(), 50)
      << ")";
  } else {
    // Update
    sql << "update doctor set profile_id = " << value->getProfile()
      << ", name = " << secureString(value->getName(), 100)
      << ", fullname = " << secureString(value->getFullName(), 100)
      << ", study = " << secureString(value->getStudy(), 20)
      << ", active = " << (value->isActive() ? 1 : 0)
      << ", duty = " << secureString(value->getDuty(), 50)
      << ", institute = " << secureString(value->getInstitute(), 50)
      << ", certificate = " << secureString(value->getCertificate(), 50)
      << ", category = " << secureString(value->getCategory(), 50)
      << " where doctor_id = " << value->getId();
  }

  if (static_cast<MySqlConnection*>(impl)->exec(sql.str())) {
    if (!value->getId()) {
      value->setId(static_cast<MySqlConnection*>(impl)->getLastId());
      assert(value->getId());
    }
    value->wash();
    result = true;
  }

  return result;
}

bool Repository::saveDoctors(std::vector<Doctor> &value) {
  for (Doctor &d : value)
    if (!saveDoctor(&d))
      return false;

  return true;
}

std::vector<Rule*> Repository::loadRules(const Doctor *doctor) {
  std::vector<Rule*> result;
  if (doctor && doctor->getId()) {
    std::ostringstream sql;
    sql << "select rule_id, doctor_id, nline, name, title, dfrom, dtill, parity, wday, wdaynum"
      << " from rule where doctor_id = " << doctor->getId()
      << " order by nline";

    std::shared_ptr<MYSQL_RES> res = static_cast<MySqlConnection*>(impl)->query(sql.str());
    if (res) {
      while (MYSQL_ROW row = mysql_fetch_row(res.get())) {
        result.push_back(new Rule(getInt(row, 0), getInt(row, 1), getInt(row, 2), getString(row, 3)));
        Rule *rule = result.back();
        rule->setTitle(getString(row, 4));
        if (row[5]) rule->setFrom(DateTime(std::string(row[5])));
        if (row[6]) rule->setTill(DateTime(std::string(row[6])));
        rule->setParity(getInt(row, 7));
        rule->setWeekday(getInt(row, 8));
        rule->setWeek(getInt(row, 9));
        rule->wash();
      }
    }
  }
  return result;
}

bool Repository::saveRule(Rule *value) {
  if (!value || !value->isDirty()) return true;

  bool result = false;

  std::ostringstream sql;

  if (!value->getId()) {
    // Insert
    sql << "insert into rule (doctor_id, nline, name, title, dfrom, dtill, parity, wday, wdaynum)"
      << " values ("
      << value->getDoctor()
      << ", " << value->getNumber()
      << ", " << secureString(value->getName(), 50)
      << ", " << secureString(value->getTitle(), 50)
      << ", " << secureDate(value->getFrom())
      << ", " << secureDate(value->getTill())
      << ", " << secureInt(value->getParity())
      << ", " << secureInt(value->getWeekday())
      << ", " << secureInt(value->getWeek())
      << ')';
  } else {
    // Update
    sql << "update rule set"
      << " doctor_id = " << value->getDoctor()
      << ", nline = " << value->getNumber()
      << ", name = " << secureString(value->getName(), 50)
      << ", title = " << secureString(value->getTitle(), 50)
      << ", dfrom = " << secureDate(value->getFrom())
      << ", dtill = " << secureDate(value->getTill())
      << ", parity = " << secureInt(value->getParity())
      << ", wday = " << secureInt(value->getWeekday())
      << ", wdaynum = " << secureInt(value->getWeek())
      << " where rule_id = " << value->getId();
  }

  if (static_cast<MySqlConnection*>(impl)->exec(sql.str())) {
    if (!value->getId()) {
      value->setId(static_cast<MySqlConnection*>(impl)->getLastId());
      assert(value->getId());
    }
    value->wash();
    result = true;
  }

  return result;
}

bool Repository::deleteRule(Rule *value) {
  if (!value || !value->getId()) return true;

  bool result = false;
  std::ostringstream sql;
  sql << "delete from rule where rule_id = " << value->getId();

  if (static_cast<MySqlConnection*>(impl)->exec(sql.str())) {
    value->wash();
    result = true;
  }

  return result;
}

bool Repository::saveRules(std::vector<Rule> &value) {
  for (Rule &r : value)
    if (!saveRule(&r))
      return false;

  return true;
}

bool Repository::massLock(const std::string &name, const std::string &title, const DateTime &dfrom, const DateTime &dtill) {
  std::ostringstream sql;
  
  sql << "insert into rule (doctor_id, nline, name, title, dfrom, dtill)"
    << " select doctor_id, 1, " << secureString(name, 50)
    << ", " << secureString(title, 50)
    << ", " << secureDate(dfrom)
    << ", " << secureDate(dtill)
    << " from doctor where active > 0";
  
  // Move all rules for active doctors down by one
  //  and paste new rule for each doctor on the first place
  return static_cast<MySqlConnection*>(impl)->exec(RESERVE_FIRST_RULE)
    && static_cast<MySqlConnection*>(impl)->exec(sql.str());
}

};

