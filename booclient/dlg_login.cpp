#include "dlg_login.h"
#include <wx/msgdlg.h>

///////////////////////////////////////////////////////////////////////////

DialogLogin::DialogLogin (wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style) {
  this->SetSizeHints( wxDefaultSize, wxDefaultSize );

  wxBoxSizer* bsVertical;
  bsVertical = new wxBoxSizer( wxVERTICAL );

  wxFlexGridSizer* gsMain;
  gsMain = new wxFlexGridSizer( 0, 2, 0, 0 );
  gsMain->AddGrowableCol( 1 );
  gsMain->SetFlexibleDirection( wxBOTH );
  gsMain->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

  lblUser = new wxStaticText( this, wxID_ANY, _("User"), wxDefaultPosition, wxDefaultSize, 0 );
  lblUser->Wrap( -1 );
  gsMain->Add( lblUser, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );

  edUser = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
  gsMain->Add( edUser, 0, wxALL, 2 );

  lblPassword = new wxStaticText( this, wxID_ANY, _("Password"), wxDefaultPosition, wxDefaultSize, 0 );
  lblPassword->Wrap( -1 );
  gsMain->Add( lblPassword, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );

  edPassword = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD );
  gsMain->Add( edPassword, 0, wxALL, 2 );
  bsVertical->Add( gsMain, 1, wxEXPAND, 0 );

  wxBoxSizer* bsButtons;
  bsButtons = new wxBoxSizer( wxHORIZONTAL );

  btnOk = new wxButton( this, wxID_ANY, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
  btnOk->SetDefault();
  bsButtons->Add( btnOk, 0, wxALL, 2 );

  btnCancel = new wxButton( this, wxID_ANY, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
  bsButtons->Add(btnCancel, 0, wxALL, 2);
  bsVertical->Add(bsButtons, 0, wxALIGN_RIGHT, 2);

  this->SetSizer( bsVertical );
  this->Layout();
  bsVertical->Fit( this );

  this->Centre( wxBOTH );

  // Connect Events
  btnOk->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogLogin::OnLogin ), NULL, this );
  btnCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogLogin::OnCancel ), NULL, this );

  SetEscapeId(btnCancel->GetId());

  if (edPassword->CanAcceptFocus()) edPassword->SetFocus();
}

DialogLogin::~DialogLogin() {
  // Disconnect Events
  btnOk->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogLogin::OnLogin ), NULL, this);
  btnCancel->Disconnect(wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogLogin::OnCancel ), NULL, this);
}

std::string DialogLogin::getUser(void) const {
  return std::string(edUser->GetValue().mb_str(wxConvUTF8));
}

std::string DialogLogin::getPassword(void) const {
  return std::string(edPassword->GetValue().mb_str(wxConvUTF8));
}

void DialogLogin::OnLogin(wxCommandEvent& event) {
  wxString user = edUser->GetValue();
  user.Trim(false).Trim(true);

  wxString pass = edPassword->GetValue();
  pass.Trim(false).Trim(true);

  if (user.IsEmpty()) {
    wxMessageBox(_("You should specify username"), _("No User"), wxOK | wxICON_EXCLAMATION | wxCENTRE, this);
    if (edUser->CanAcceptFocus()) edUser->SetFocus();
    return;
  } else if (pass.IsEmpty()) {
    wxMessageBox(_("You should specify password"), _("Password Empty"), wxOK | wxICON_EXCLAMATION | wxCENTRE, this);
    if (edPassword->CanAcceptFocus()) edPassword->SetFocus();
    return;
  }
  EndModal(wxID_OK);
}

void DialogLogin::OnCancel(wxCommandEvent& event) {
  EndModal(wxID_CANCEL);
}

void DialogLogin::setUser(const std::string &value) {
  edUser->SetValue(wxString(value.c_str(), wxConvUTF8));
}

void DialogLogin::setPassword(const std::string &value) {
  edPassword->SetValue(wxString(value.c_str(), wxConvUTF8));
  edPassword->SelectAll();
}
