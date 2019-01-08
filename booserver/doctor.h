#pragma once
#include <string>
#include <array>

namespace timetable {
  
enum Weekdays : int {
  Monday,
  Tuesday,
  Wednesday,
  Thursday,
  Friday,
  Saturday,
  Sunday
};

struct ReceiptTime {
  std::string title;
  bool blocked;
  
  ReceiptTime(): title(), blocked(false) {}
};

/**
 * @class Doctor
 * @author ivanhoe
 * @date 03/11/18
 * @file doctor.h
 * @brief Doctor model for the timetable
 */
class Doctor {
  std::string name;
  std::string study;
  std::array<ReceiptTime, 7> table;

  Doctor(Doctor& rhs) = delete;
  void operator=(Doctor& rhs) = delete;
public:
  Doctor(const std::string &name, const std::string &study);
  Doctor(Doctor &&rhs);
  ~Doctor();
  
  /**
   * @brief         Get doctor's name
   */
  const std::string& getName(void) const;
  
  /**
   * @brief         Get study number
   */
  const std::string& getStudy(void) const;
  
  /**
   * @brief         Access receipt time for the given weekday
   */
  const ReceiptTime& operator[] (Weekdays day) const;
  ReceiptTime& operator[] (Weekdays day);
};

}

