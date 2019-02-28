#include "doctor.h"

namespace timetable {

Doctor::Doctor(const std::string &name, const std::string &study):
  name(name),
  study(study)
{}

Doctor::Doctor(Doctor &&rhs):
  name(std::move(rhs.name)),
  study(std::move(rhs.study)),
  table(std::move(rhs.table))
{
}

Doctor::~Doctor() {
}

const std::string& Doctor::getName(void) const { return name; }
const std::string& Doctor::getStudy(void) const { return study; }

const ReceiptTime& Doctor::operator[] (Weekdays day) const {
  return table[static_cast<int>(day)];
}

ReceiptTime& Doctor::operator[] (Weekdays day) {
  return table[static_cast<int>(day)];
}

}
