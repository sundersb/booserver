#pragma once

#include <vector>
#include "doctor.h"


namespace timetable {

/**
 * @class Profile
 * @author ivanhoe
 * @date 03/11/18
 * @file profile.h
 * @brief   Doctors' profile in the timetable
 */
class Profile {
  std::string title;
  std::vector<Doctor> doctors;

  Profile(const Profile& rhs);
  Profile& operator=(const Profile& rhs);
public:
  Profile(Profile &&) = default;
  explicit Profile(const std::string &title);
  ~Profile();

  /**
   * @brief         Get profile title
   */
  const std::string& getTitle(void) const;

  /**
   * @brief         Add a doctor to the profile's collection
   */
  void addDoctor(Doctor &&doctor);

  /**
   * @brief         Get profile doctors
   */
  const std::vector<Doctor>& getDoctors(void) const;
};

}

typedef std::vector<timetable::Profile> Profiles;

