#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/button.h>
#include <wx/dialog.h>

#include <string>

///////////////////////////////////////////////////////////////////////////////
/// Class DialogLogin
///////////////////////////////////////////////////////////////////////////////
class DialogLogin : public wxDialog {
protected:
  wxStaticText* lblUser;
  wxTextCtrl* edUser;
  wxStaticText* lblPassword;
  wxTextCtrl* edPassword;
  wxButton* btnOk;
  wxButton* btnCancel;

  // Virtual event handlers, overide them in your derived class
  void OnLogin(wxCommandEvent& event);
  void OnCancel(wxCommandEvent& event);

public:
  DialogLogin (wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Log In"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE);
  ~DialogLogin();

  std::string getUser(void) const;
  void setUser(const std::string &value);
  std::string getPassword(void) const;
  void setPassword(const std::string &value);
};

