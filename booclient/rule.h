#pragma once

#include "date.h"
#include <string>

class Rule {
  bool dirty;
  int id;
  int doctor_id;
  int nline;
  std::string name;
  std::string title;
  DateTime from;
  DateTime till;
  int parity;
  int wday;
  int wdaynum;
public:
  Rule(int id, int doctor_id, int nline, const std::string &name);
  Rule(Rule &&rhs);
  Rule(const Rule &rhs);
  ~Rule();

  bool isDirty(void) const;
  void wash(void);
  int getId(void) const;
  void setId(int value);
  int getDoctor(void) const;
  void setDoctor(int value);
  int getNumber(void) const;
  void setNumber(int value);
  const std::string& getName(void) const;
  void setName(const std::string &value);
  const std::string& getTitle(void) const;
  void setTitle(const std::string &value);
  const DateTime& getFrom(void) const;
  void setFrom(const DateTime &value);
  const DateTime& getTill(void) const;
  void setTill(const DateTime &value);

  /**
   * @brief         Monthday's parity
   * May be only:
   * 0 - irrelevant
   * 1 - odd
   * 2 - even
   */
  int getParity(void) const;
  void setParity(int value);

  /**
   * @brief         Week day (0-7)
   */
  int getWeekday(void) const;
  void setWeekday(int value);

  /**
   * @brief         Weekday's occurence in the month (0 - 4)
   */
  int getWeek(void) const;
  void setWeek(int value);

  std::string showPeriod(void) const;

  /**
   * @brief         Check rule conformity to the given date
   * @param date    Date to check against
   * Returns true if the rule matches the date
   */
  bool fits(const DateTime &date) const;
};
