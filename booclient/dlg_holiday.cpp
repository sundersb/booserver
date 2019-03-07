///////////////////////////////////////////////////////////////////////////

#include "dlg_holiday.h"

///////////////////////////////////////////////////////////////////////////

DialogHoliday::DialogHoliday( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style ) {
  this->SetSizeHints( wxDefaultSize, wxDefaultSize );

  wxBoxSizer* mainSizer;
  mainSizer = new wxBoxSizer( wxVERTICAL );

  wxFlexGridSizer* fgSizer1;
  fgSizer1 = new wxFlexGridSizer( 0, 2, 0, 0 );
  fgSizer1->AddGrowableCol( 1 );
  fgSizer1->SetFlexibleDirection( wxBOTH );
  fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

  lblName = new wxStaticText( this, wxID_ANY, _("Name"), wxDefaultPosition, wxDefaultSize, 0 );
  lblName->Wrap( -1 );
  fgSizer1->Add( lblName, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );

  edName = new wxTextCtrl( this, wxID_ANY, _("Some holiday"), wxDefaultPosition, wxDefaultSize, 0 );
  fgSizer1->Add( edName, 0, wxALL|wxEXPAND, 2 );

  lblTitle = new wxStaticText( this, wxID_ANY, _("Title"), wxDefaultPosition, wxDefaultSize, 0 );
  lblTitle->Wrap( -1 );
  fgSizer1->Add( lblTitle, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );

  edTitle = new wxTextCtrl( this, wxID_ANY, _("holiday"), wxDefaultPosition, wxDefaultSize, 0 );
  fgSizer1->Add( edTitle, 0, wxALL|wxEXPAND, 2 );

  lblFrom = new wxStaticText( this, wxID_ANY, _("From"), wxDefaultPosition, wxDefaultSize, 0 );
  lblFrom->Wrap( -1 );
  fgSizer1->Add( lblFrom, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );

  dtpFrom = new wxDatePickerCtrl( this, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DROPDOWN );
  fgSizer1->Add( dtpFrom, 0, wxALL, 2 );

  lblTill = new wxStaticText( this, wxID_ANY, _("Till"), wxDefaultPosition, wxDefaultSize, 0 );
  lblTill->Wrap( -1 );
  fgSizer1->Add( lblTill, 0, wxALIGN_CENTER_VERTICAL|wxALL, 2 );

  dtpTill = new wxDatePickerCtrl( this, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DROPDOWN );
  fgSizer1->Add( dtpTill, 0, wxALL, 2 );

  mainSizer->Add( fgSizer1, 1, wxEXPAND, 5 );

  sdbButtons = new wxStdDialogButtonSizer();
  sdbButtonsOK = new wxButton( this, wxID_OK );
  sdbButtons->AddButton( sdbButtonsOK );
  sdbButtonsCancel = new wxButton( this, wxID_CANCEL );
  sdbButtons->AddButton( sdbButtonsCancel );
  sdbButtons->Realize();

  mainSizer->Add( sdbButtons, 0, wxEXPAND, 2 );


  this->SetSizer( mainSizer );
  this->Layout();
  mainSizer->Fit( this );

  this->Centre( wxBOTH );

  // Connect Events
  sdbButtonsCancel->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogHoliday::OnCancelClicked ), NULL, this );
  sdbButtonsOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogHoliday::OnOkClicked ), NULL, this );
}

DialogHoliday::~DialogHoliday() {
  // Disconnect Events
  sdbButtonsCancel->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogHoliday::OnCancelClicked ), NULL, this );
  sdbButtonsOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DialogHoliday::OnOkClicked ), NULL, this );

}

void DialogHoliday::OnCancelClicked (wxCommandEvent& event) {
  EndModal(wxID_CANCEL);
}

void DialogHoliday::OnOkClicked (wxCommandEvent& event) {
  EndModal(wxID_OK);
}

std::string DialogHoliday::getName(void) const {
  return std::string(edName->GetValue().mb_str(wxConvUTF8));
}

std::string DialogHoliday::getTitle(void) const {
  return std::string(edTitle->GetValue().mb_str(wxConvUTF8));
}

DateTime DialogHoliday::getFrom(void) const {
  DateTime value(dtpFrom->GetValue().MakeFromUTC().GetTicks());
  value.discardTime();
  return value;
}

DateTime DialogHoliday::getTill(void) const {
  DateTime value(dtpTill->GetValue().MakeFromUTC().GetTicks());
  value.discardTime();
  return value;
}
