#include "connection.h"
#include <assert.h>

const std::string SET_CODEPAGE = 
  "set names 'utf8' collate 'utf8_general_ci'";

MySqlConnection::MySqlConnection(): conn(nullptr) {}

MySqlConnection::~MySqlConnection() {
  if (conn) mysql_close(conn);
}

bool MySqlConnection::init(const std::string &host,
  const std::string &db,
  const std::string &user,
  const std::string &pass)
{
  conn = mysql_init(nullptr);
  if (!conn) return false;
  
  if (!mysql_real_connect(conn,
    host.c_str(),
    user.c_str(),
    pass.c_str(),
    db.c_str(), 0, 0, 0)) return false;
    
  return mysql_query(conn, SET_CODEPAGE.c_str()) == 0;
}

std::shared_ptr<MYSQL_RES> MySqlConnection::query(const std::string &sql) {
  assert(conn);
  if (mysql_query(conn, sql.c_str())) return nullptr;
  
  std::shared_ptr<MYSQL_RES> result(mysql_store_result(conn), mysql_free_result);
  return result;
}

bool MySqlConnection::exec(const std::string &sql) {
  assert(conn);
  return mysql_query(conn, sql.c_str()) == 0;
}

unsigned MySqlConnection::getLastId(void) {
  assert(conn);
  return mysql_insert_id(conn);
}
