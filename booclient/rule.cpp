#include "rule.h"

#include <sstream>
#include <iomanip>

Rule::Rule(int id, int doctor_id, int nline, const std::string &name):
  dirty(false),
  id(id),
  doctor_id(doctor_id),
  nline(nline),
  name(name),
  title(),
  from(),
  till(),
  parity(0),
  wday(0),
  wdaynum(0)
{}

Rule::Rule(Rule &&rhs):
  dirty(rhs.dirty),
  id(rhs.id),
  doctor_id(rhs.doctor_id),
  nline(rhs.nline),
  name(std::move(rhs.name)),
  title(std::move(rhs.title)),
  from(rhs.from),
  till(rhs.till),
  parity(rhs.parity),
  wday(rhs.wday),
  wdaynum(rhs.wdaynum)
{
  rhs.dirty = false;
}

Rule::Rule(const Rule &rhs):
  dirty(rhs.dirty),
  id(rhs.id),
  doctor_id(rhs.doctor_id),
  nline(rhs.nline),
  name(rhs.name),
  title(rhs.title),
  from(rhs.from),
  till(rhs.till),
  parity(rhs.parity),
  wday(rhs.wday),
  wdaynum(rhs.wdaynum)
{}

Rule::~Rule(){}

bool Rule::isDirty(void) const { return dirty; }
void Rule::wash(void) { dirty = false; }
int Rule::getId(void) const { return id; }
void Rule::setId(int value) {
  if (value != id) {
    id = value;
    dirty = true;
  }
}

int Rule::getDoctor(void) const { return doctor_id; }
void Rule::setDoctor(int value) {
  if (value != doctor_id) {
    doctor_id = value;
    dirty = true;
  }
}

int Rule::getNumber(void) const { return nline; }
void Rule::setNumber(int value) {
  if (value != nline) {
    nline = value;
    dirty = true;
  }
}

const std::string& Rule::getName(void) const { return name; }
void Rule::setName(const std::string &value) {
  if (value != name) {
    name = value;
    dirty = true;
  }
}

const std::string& Rule::getTitle(void) const { return title; }
void Rule::setTitle(const std::string &value) {
  if (value != title) {
    title = value;
    dirty = true;
  }
}

const DateTime& Rule::getFrom(void) const { return from; }
void Rule::setFrom(const DateTime &value) {
  if (value != from) {
    from = value;
    dirty = true;
  }
}

const DateTime& Rule::getTill(void) const { return till; }
void Rule::setTill(const DateTime &value) {
  if (value != till) {
    till = value;
    dirty = true;
  }
}

int Rule::getParity(void) const { return parity; }
void Rule::setParity(int value) {
  if (value < 0) value = 0;
  else
    if (value > 2)
      value = (value % 2) ? 1 : 2;

  if (value != parity) {
    parity = value;
    dirty = true;
  }
}

int Rule::getWeekday(void) const { return wday; }
void Rule::setWeekday(int value) {
  if (value < 0) value = 0;
  else if (value > 7) {
    value %= 7;
    if (!value) value = 7;
  }

  if (value != wday) {
    wday = value;
    dirty = true;
  }
}

int Rule::getWeek(void) const { return wdaynum; }
void Rule::setWeek(int value) {
  if (value < 0) value = 0;
  else if (value > 5) {
    value %= 5;
    if (!value) value = 5;
  }

  if (value != wdaynum) {
    wdaynum = value;
    dirty = true;
  }
}

std::string Rule::showPeriod(void) const {
  std::ostringstream period;

  struct tm t;
  if (from.isValid()) {
    from.asTime(&t);
    period << t.tm_mday << '.'
      << std::setw(2) << std::setfill('0') << (t.tm_mon + 1) << '.'
      << (t.tm_year + 1900);
  }
  period << " - ";

  if (till.isValid()) {
    till.asTime(&t);
    period << t.tm_mday << '.'
      << std::setw(2) << std::setfill('0') << (t.tm_mon + 1) << '.'
      << (t.tm_year + 1900);
  }

  return period.str();
}

bool Rule::fits(const DateTime &date) const {
  // Date limits
  if (from.isValid() && date < from) return false;
  if (till.isValid() && date > till) return false;

  struct tm time;
  date.asTime(&time);

  // Parity
  if (parity > 0 && (time.tm_mday % 2) != (parity % 2)) return false;

  // Weekday
  if (wday > 0) {
    if (date.getWeekDay() != (wday - 1)) return false;
    if (wdaynum > 0 && (time.tm_mday / 7) != (wdaynum - 1)) return false;
  }

  // All checks done: the rule matches the date
  return true;
}
