#pragma once

#include <wx/wx.h>
#include <string>

class MainApp : public wxApp {
  wxLocale *locale;

  std::string server;
  std::string database;
  std::string user;

  int langCode;

  void selectLanguage(int code);
  void loadSetup(void);
public:
  virtual bool OnInit();

  ~MainApp();
};

// declare global static function wxGetApp()
DECLARE_APP(MainApp)
