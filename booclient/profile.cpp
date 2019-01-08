#include "profile.h"


Profile::Profile(int id, const std::string& title):
 dirty(false),
 id(id),
 title(title),
 nline(0),
 active(true) {}

Profile::Profile(Profile &&rhs) {
  id = rhs.id;
  title = std::move(rhs.title);
  dirty = rhs.dirty;
  nline = rhs.nline;
  active = rhs.active;
  rhs.dirty = false;
}

Profile::Profile(const Profile &rhs) {
  id = rhs.id;
  title = rhs.title;
  dirty = rhs.dirty;
  nline = rhs.nline;
  active = rhs.active;
}

Profile::~Profile() {}

bool Profile::isDirty(void) const { return dirty; }
void Profile::wash(void) { dirty = false; }

int Profile::getId(void) const { return id; }

void Profile::setId(int value) {
  if (id != value) {
    id = value;
    dirty = true;
  }
}

const std::string& Profile::getTitle(void) const { return title; }

void Profile::setTitle(const std::string &value) {
  if (value != title) {
    title = value;
    dirty = true;
  }
}

int Profile::getNumber(void) const { return nline; }
void Profile::setNumber(int value) {
  if (value != nline) {
    nline = value;
    dirty = true;
  }
}

bool Profile::isActive(void) const { return active; }
void Profile::setActive(bool value) {
  if (value != active) {
    active = value;
    dirty = true;
  }
}
