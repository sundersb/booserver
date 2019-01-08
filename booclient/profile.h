#pragma once

#include <string>

/**
 * @class Profile
 * @author Sunders
 * @date 20.12.2018
 * @file profile.h
 * @brief             Timetable profile model
 *                    Single model contains several doctors time datas
 */
class Profile {
  bool dirty;
  int id;
  std::string title;
  int nline;
  bool active;
  
public:
  Profile(int id, const std::string& title);
  Profile(Profile &&rhs);
  Profile(const Profile &rhs);
  ~Profile();
  
  /**
   * @brief       The record has been modified and must be saved
   */
  bool isDirty(void) const;
  
  /**
   * @brief       Clear modification flag
   */
  void wash(void);
  
  /**
   * @brief       Get profile's id
   */
  int getId(void) const;
  
  /**
   * @brief       Set pdofile's id
   */
  void setId(int value);
  
  /**
   * @brief       Get profile's title
   */
  const std::string& getTitle(void) const;
  
  /**
   * @brief       Set title of the profile
   */
  void setTitle(const std::string &value);

  /**
   * @brief       Get ordinal number of this profile in profiles list
   */
  int getNumber(void) const;
  
  /**
   * @brief       Set ordinal number of the profile
   */
  void setNumber(int value);
  
  /**
   * @brief       The profile is active (not deleted) flag
   */
  bool isActive(void) const;
  
  /**
   * @brief       Delete/undelete profile
   */
  void setActive(bool value);
};
