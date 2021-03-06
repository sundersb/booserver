#pragma once

#include <mysql.h>
#include <string>
#include <memory>

/**
 * @class MySqlConnection
 * @author Sunders
 * @date 08.11.2018
 * @file connection.h
 * @brief Quick and dirty MYSQL wrapper
 */
class MySqlConnection {
  MYSQL *conn;
  
  MySqlConnection(MySqlConnection&) = delete;
  void operator= (MySqlConnection&) = delete;
public:
  MySqlConnection();
  ~MySqlConnection();
  
  /**
   * @brief           Initialize
   * @param host      Server name or IP-address
   * @param db        Database name
   * @param user      User
   * @param pass      Password
   * Returns true on success
   */
  bool init(const std::string &host, const std::string &db, const std::string &user, const std::string &pass);
  
  /**
   * @brief           Execute query returning dataset
   * @param sql       SQL command to execute
   * Returns NULL on any error
   */
  std::shared_ptr<MYSQL_RES> query(const std::string &sql);
};

/**
 * @brief             Helper to extract integer value from mysql field
 * @param row         Dataset row
 * @param fieldNum    Field index
 * @return            Integer value of the field or -1 in case of NULL
 */
inline int getInt(MYSQL_ROW row, int fieldNum) {
  if (row[fieldNum])
    return std::stoi(row[fieldNum]);
  else
    return -1;
}

inline std::string getString(const char *value) {
  if (value)
    return std::string(value);
  else
    return "???";
}
