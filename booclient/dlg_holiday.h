///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct 26 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

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
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/dialog.h>

#include "date.h"

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class DialogHoliday
///////////////////////////////////////////////////////////////////////////////
class DialogHoliday : public wxDialog {
private:

protected:
  wxStaticText* lblName;
  wxTextCtrl* edName;
  wxStaticText* lblTitle;
  wxTextCtrl* edTitle;
  wxStaticText* lblFrom;
  wxDatePickerCtrl* dtpFrom;
  wxStaticText* lblTill;
  wxDatePickerCtrl* dtpTill;
  wxStdDialogButtonSizer* sdbButtons;
  wxButton* sdbButtonsOK;
  wxButton* sdbButtonsCancel;

  void OnCancelClicked (wxCommandEvent& event);
  void OnOkClicked (wxCommandEvent& event);

public:

  DialogHoliday( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Add Holiday"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );
  ~DialogHoliday();

  std::string getName(void) const;
  std::string getTitle(void) const;
  
  DateTime getFrom(void) const;
  DateTime getTill(void) const;
};

