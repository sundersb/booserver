#pragma once

#include <wx/bitmap.h>
#include <vector>

#include "rule.h"
#include "date.h"

/**
 * @class Chart
 * @author ivanhoe
 * @date 03/01/19
 * @file chart.h
 * @brief Rules tester
 */
class Chart {
  wxBitmap week;
  wxBitmap bitmap;
  DateTime monday;

  Chart(Chart&) = delete;
  void operator= (Chart&) = delete;

  void update(void);
public:
  Chart();
  ~Chart();

  /**
   * @brief       Go to the previous week
   */
  void goBack(void);

  /**
   * @brief       Go to the next week
   */
  void goForth(void);

  /**
   * @brief       Get empty week's bitmap
   * Returns wxBitmap with currently selected week with no rules applied
   */
  const wxBitmap& get(void) const;

  /**
   * @brief       Get bitmap for the week marked by the rules
   * @param rules List of timetable rules for a doctor
   *
   * Returns wxBitmap depicting current week with relevant rules applied
   * to each weekday
   */
  const wxBitmap& get(std::vector<Rule*> &rules);
};
