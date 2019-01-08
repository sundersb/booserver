#pragma once

#include <string>
#include "profile.h"

/**
 * @class Provider
 * @author ivanhoe
 * @date 03/11/18
 * @file provider.h
 * @brief             Class to load timetable pages. Just mock so far
 */
class Provider {
  void *impl;

public:
  /**
   * @brief           Ctor
   * @param linesCount  How many lines fit to the screen height
   * @param headerLines Lines amount for each timetable's header
   * 
   * Timetable for a doctor profile has a header which includes profile's title
   * (centered on two lines) and timetable captions (i.e. weekdays). So headerLines
   * generally mustn't be changed unless you changed some code in TimetableScreenBuilder
   */
  Provider(int linesCount, int headerLines);
  ~Provider();
  
  /**
   * @brief           Connect to server
   * @param server    SQL server hostname or IP
   * @param database  Database name
   * @param user      Database user name
   * @param password  Database user's password
   * @return          True on successfull connection
   */
  bool init(const std::string &server,
    const std::string &database,
    const std::string &user,
    const std::string &password);
  
  /**
   * @brief           Update timetable information from the database
   */
  bool update(void);
  
  /**
   * @brief           Get a screenfull of information
   * @param page      Timetable page to acquire
   */
  Profiles getPage(int page);

  /**
   * @brief           Get timetable pages (i.e. screens) count
   */
  int getPagesCount(void) const;
};

void est(void);
