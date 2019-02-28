#include "doctor.h"

Doctor::Doctor(int id, const std::string &name):
 dirty(false),
 id(id),
 profile(0),
 name(name),
 active(true) {
}

Doctor::Doctor(Doctor &&rhs):
  dirty(rhs.dirty),
  id(rhs.id),
  profile(rhs.profile),
  name(std::move(rhs.name)),
  fullname(std::move(rhs.fullname)),
  study(std::move(rhs.study)),
  active(rhs.active),
  duty(std::move(rhs.duty)),
  institute(std::move(rhs.institute)),
  certificate(std::move(rhs.certificate)),
  category(std::move(rhs.category))
{
  rhs.dirty = false;
}

Doctor::Doctor(const Doctor &rhs):
  dirty (rhs.dirty),
  id (rhs.id),
  profile (rhs.profile),
  name (rhs.name),
  fullname (rhs.fullname),
  study (rhs.study),
  active (rhs.active),
  duty (rhs.duty),
  institute (rhs.institute),
  certificate (rhs.certificate),
  category (rhs.category)
{
}

Doctor::~Doctor() {}

bool Doctor::isDirty(void) const { return dirty; }
void Doctor::wash(void) { dirty = false; }
int Doctor::getId(void) const { return id; }

void Doctor::setId(int value) {
  if (value != id) {
    id = value;
    dirty = true;
  }
}

int Doctor::getProfile(void) const { return profile; }

void Doctor::setProfile(int value) {
  if (value != profile) {
    profile = value;
    dirty = true;
  }
}

const std::string& Doctor::getName(void) const { return name; }

void Doctor::setName(const std::string &value) {
  if (value != name) {
    name = value;
    dirty = true;
  }
}

const std::string& Doctor::getFullName(void) const { return fullname; }

void Doctor::setFullName(const std::string &value) {
  if (value != fullname) {
    fullname = value;
    dirty = true;
  }
}

const std::string& Doctor::getStudy(void) const { return study; }

void Doctor::setStudy(const std::string &value) {
  if (value != study) {
    study = value;
    dirty = true;
  }
}

bool Doctor::isActive(void) const { return active; }

void Doctor::setActive(bool value) {
  if (active != value) {
    active = value;
    dirty = true;
  }
}

const std::string& Doctor::getDuty(void) const { return duty; }

void Doctor::setDuty(const std::string &value) {
  if (value != duty) {
    duty = value;
    dirty = true;
  }
}

const std::string& Doctor::getInstitute(void) const { return institute; }

void Doctor::setInstitute(const std::string &value) {
  if (value != institute) {
    institute = value;
    dirty = true;
  }
}

const std::string& Doctor::getCertificate(void) const { return certificate; }

void Doctor::setCertificate(const std::string &value) {
  if (value != certificate) {
    certificate = value;
    dirty = true;
  }
}

const std::string& Doctor::getCategory(void) const { return category; }

void Doctor::setCategory(const std::string &value) {
  if (value != category) {
    category = value;
    dirty = true;
  }
}

