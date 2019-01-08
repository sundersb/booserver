#pragma once

#include <vector>
#include <string>
#include "profile.h"
#include "doctor.h"
#include "rule.h"

namespace data {
  class Repository {
    void *impl;

    Repository(Repository&) = delete;
    void operator= (Repository&) = delete;
  public:
    Repository();
    ~Repository();

    bool init(const std::string &host, const std::string &db, const std::string &user, const std::string &pass);

    std::vector<Profile*> loadProfiles(bool activeOnly = true);
    bool saveProfile(Profile *value);
    bool saveProfiles(std::vector<Profile> &value);
    int getLastProfileNumber(void);

    std::vector<Doctor*> loadDoctors(const Profile* profile, bool activeOnly = true);
    bool saveDoctor(Doctor *value);
    bool saveDoctors(std::vector<Doctor> &value);

    std::vector<Rule*> loadRules(const Doctor *doctor);
    bool saveRule(Rule *value);
    bool saveRules(std::vector<Rule> &value);
    bool deleteRule(Rule *value);
  };
};
