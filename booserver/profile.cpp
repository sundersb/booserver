#include "profile.h"

namespace timetable {

Profile::Profile(const std::string &title):
  title(title)
{}

Profile::~Profile() {
}

void Profile::addDoctor(Doctor &&doctor) {
  doctors.push_back(std::move(doctor));
}

const std::string& Profile::getTitle(void) const {
  return title;
}

const std::vector<Doctor>& Profile::getDoctors(void) const {
  return doctors;
}

}
