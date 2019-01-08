#pragma once

#include <string>

/**
 * @class Doctor
 * @author Sunders
 * @date 20.12.2018
 * @file doctor.h
 * @brief             Timetable doctor model
 */
class Doctor {
  bool dirty;
  int id;
  int profile;
  std::string name;
  std::string fullname;
  std::string study;
  bool active;
  std::string duty;
  std::string institute;
  std::string certificate;
  std::string category;
  
  Doctor() = delete;
  
public:
  /**
   * @brief         Ctor
   * @param id      Doctor's ID
   * @param name    Doctor's title
   */
  Doctor(int id, const std::string &name);
  
  /**
   * @brief         Move ctor
   */
  Doctor(Doctor &&rhs);
  
  /**
   * @brief         Copy ctor
   */
  Doctor(const Doctor &rhs);
  ~Doctor();
  
  /**
   * @brief         Doctor has been modified flag
   */
  bool isDirty(void) const;
  
  /**
   * @brief         Clear the dirty flag
   */
  void wash(void);
  
  /**
   * @brief         Get doctor's id
   */
  int getId(void) const;
  
  /**
   * @brief         Set doctor's id
   * @param value
   */
  void setId(int value);
  
  /**
   * @brief         Get doctor's department id
   */
  int getProfile(void) const;
  
  /**
   * @brief         Set doctor's department id
   */
  void setProfile(int value);
  
  /**
   * @brief         Get timetable record title for the doctor
   */
  const std::string& getName(void) const;
  
  /**
   * @brief         Set timetable title for thte doctor
   */
  void setName(const std::string &value);
  
  /**
   * @brief         Get doctor's full name
   */
  const std::string& getFullName(void) const;
  
  /**
   * @brief         Set doctor's full name
   */
  void setFullName(const std::string &value);
  
  /**
   * @brief         Get doctor's study
   */
  const std::string& getStudy(void) const;
  
  /**
   * @brief         Set doctor's study
   */
  void setStudy(const std::string &value);
  
  /**
   * @brief         Is the doctor active (not deleted)?
   */
  bool isActive(void) const;
  
  /**
   * @brief         Set doctor's timetable activity status
   * @param value   True if the timetable is active, false - deleted
   */
  void setActive(bool value);
  
  /**
   * @brief         Get doctor's job position
   */
  const std::string& getDuty(void) const;
  
  /**
   * @brief         Set doctor's job position
   */
  void setDuty(const std::string &value);
  
  /**
   * @brief         Get doctor's institute
   */
  const std::string& getInstitute(void) const;
  
  /**
   * @brief         Set doctor's institute
   */
  void setInstitute(const std::string &value);
  
  /**
   * @brief         Get doctor's certificate data
   */
  const std::string& getCertificate(void) const;
  
  /**
   * @brief         Set doctor's certificate data
   */
  void setCertificate(const std::string &value);
  
  /**
   * @brief         Get doctor's category
   */
  const std::string& getCategory(void) const;
  
  /**
   * @brief         Set doctor's category
   */
  void setCategory(const std::string &value);
};
