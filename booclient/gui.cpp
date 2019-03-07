///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 17 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "gui.h"
#include "buttons.h"

///////////////////////////////////////////////////////////////////////////

frmMain::frmMain (wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style):
  wxFrame(parent, id, title, pos, size, style)
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	m_menuBar = new wxMenuBar( 0 );
	mnuFile = new wxMenu();
	wxMenuItem* menuFileExit;
	menuFileExit = new wxMenuItem( mnuFile, wxID_EXIT, wxString( _("E&xit") ) + wxT('\t') + wxT("Alt+X"), wxEmptyString, wxITEM_NORMAL );
	#ifdef __WXMSW__
	menuFileExit->SetBitmaps( wxBitmap( wxT("icoexit"), wxBITMAP_TYPE_PNG_RESOURCE ) );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	menuFileExit->SetBitmap(wxBITMAP_PNG (icoexit));
	#endif
	mnuFile->Append( menuFileExit );

	m_menuBar->Append( mnuFile, _("&File") );

	mnuTools = new wxMenu();
	wxMenuItem* miDeptAdd;
	miDeptAdd = new wxMenuItem( mnuTools, wxID_ANY, wxString( _("Add department...") ) , _("Add department"), wxITEM_NORMAL );
	#ifdef __WXMSW__
	miDeptAdd->SetBitmaps( wxBitmap( wxT("icogroupadd"), wxBITMAP_TYPE_PNG_RESOURCE ) );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	miDeptAdd->SetBitmap( wxBITMAP_PNG(icogroupadd));
	#endif
	mnuTools->Append( miDeptAdd );

	wxMenuItem* miDeptRemove;
	miDeptRemove = new wxMenuItem( mnuTools, wxID_ANY, wxString( _("Remove department") ) , _("Remove department"), wxITEM_NORMAL );
	#ifdef __WXMSW__
	miDeptRemove->SetBitmaps( wxBitmap( wxT("icogroupremove"), wxBITMAP_TYPE_PNG_RESOURCE ) );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	miDeptRemove->SetBitmap( wxBITMAP_PNG(icogroupremove));
	#endif
	mnuTools->Append( miDeptRemove );

	wxMenuItem* miDeptRename;
	miDeptRename = new wxMenuItem( mnuTools, wxID_ANY, wxString( _("Rename department...") ) , _("Rename department"), wxITEM_NORMAL );
	#ifdef __WXMSW__
	miDeptRename->SetBitmaps( wxBitmap( wxT("icogroupedit"), wxBITMAP_TYPE_PNG_RESOURCE ) );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	miDeptRename->SetBitmap( wxBITMAP_PNG(icogroupedit));
	#endif
	mnuTools->Append( miDeptRename );

	mnuTools->AppendSeparator();

	wxMenuItem* miDoctorAdd;
	miDoctorAdd = new wxMenuItem( mnuTools, wxID_ANY, wxString( _("Add doctor...") ) , _("Add doctor"), wxITEM_NORMAL );
	#ifdef __WXMSW__
	miDoctorAdd->SetBitmaps( wxBitmap( wxT("icodoctoradd"), wxBITMAP_TYPE_PNG_RESOURCE ) );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	miDoctorAdd->SetBitmap( wxBITMAP_PNG(icodoctoradd));
	#endif
	mnuTools->Append( miDoctorAdd );

	mnuTools->AppendSeparator();

	wxMenuItem* miRuleAdd;
	miRuleAdd = new wxMenuItem( mnuTools, wxID_ANY, wxString( _("Add rule...") ) + wxT('\t') + wxT("Ctrl-N"), _("Add rule"), wxITEM_NORMAL );
	#ifdef __WXMSW__
	miRuleAdd->SetBitmaps( wxBitmap( wxT("icoadd"), wxBITMAP_TYPE_PNG_RESOURCE ) );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	miRuleAdd->SetBitmap( wxBITMAP_PNG(icoadd));
	#endif
	mnuTools->Append( miRuleAdd );

	wxMenuItem* miRuleRemove;
	miRuleRemove = new wxMenuItem( mnuTools, wxID_ANY, wxString( _("Remove rule") ) , _("Remove rule"), wxITEM_NORMAL );
	#ifdef __WXMSW__
	miRuleRemove->SetBitmaps( wxBitmap( wxT("icoremove"), wxBITMAP_TYPE_PNG_RESOURCE ) );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	miRuleRemove->SetBitmap( wxBITMAP_PNG(icoremove));
	#endif
	mnuTools->Append( miRuleRemove );

	mnuTools->AppendSeparator();

	wxMenuItem* miHoliday;
	miHoliday = new wxMenuItem( mnuTools, wxID_ANY, wxString( _("Add holiday...") ) , _("Add holiday to all doctors"), wxITEM_NORMAL );
	#ifdef __WXMSW__
	miHoliday->SetBitmaps( wxBitmap( wxT("icoholiday"), wxBITMAP_TYPE_PNG_RESOURCE ) );
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	miHoliday->SetBitmap( wxBITMAP_PNG(icoholiday));
	#endif
	mnuTools->Append( miHoliday );

	m_menuBar->Append( mnuTools, _("Tools") );

	this->SetMenuBar( m_menuBar );

	tbrMain = this->CreateToolBar( wxTB_HORIZONTAL|wxBORDER_DEFAULT, wxID_ANY );
	tDeptAdd = tbrMain->AddTool( wxID_ANY, _("Add department..."), wxBITMAP_PNG(icogroupadd), wxNullBitmap, wxITEM_NORMAL, _("Add department"), _("Add department"), NULL );

	tDeptRemove = tbrMain->AddTool( wxID_ANY, _("Remove department"), wxBITMAP_PNG(icogroupremove), wxNullBitmap, wxITEM_NORMAL, _("Remove department"), _("Remove department"), NULL );

	tDeptRename = tbrMain->AddTool( wxID_ANY, _("Rename department..."), wxBITMAP_PNG(icogroupedit), wxNullBitmap, wxITEM_NORMAL, _("Rename department"), _("Rename department"), NULL );

	tbrMain->AddSeparator();

	tDoctorAdd = tbrMain->AddTool( wxID_ANY, _("Add doctor..."), wxBITMAP_PNG(icodoctoradd), wxNullBitmap, wxITEM_NORMAL, _("Add doctor"), _("Add doctor"), NULL );

	tbrMain->AddSeparator();

	tRuleAdd = tbrMain->AddTool( wxID_ANY, _("Add rule..."), wxBITMAP_PNG(icoadd), wxNullBitmap, wxITEM_NORMAL, _("Add rule"), _("Add rule"), NULL );

	tRuleRemove = tbrMain->AddTool( wxID_ANY, _("Remove rule"), wxBITMAP_PNG(icoremove), wxNullBitmap, wxITEM_NORMAL, _("Remove rule"), _("Remove rule"), NULL );

	tbrMain->AddSeparator();

	tExit = tbrMain->AddTool( wxID_ANY, _("Exit"), wxBITMAP_PNG(icoexit), wxNullBitmap, wxITEM_NORMAL, _("Exit the application"), _("Exit the application"), NULL );

	tbrMain->Realize();

	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );

	pnlMain = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bsMain;
	bsMain = new wxBoxSizer( wxVERTICAL );

	splMain = new wxSplitterWindow( pnlMain, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_3D );
	splMain->SetSashGravity( 0 );
	splMain->Connect( wxEVT_IDLE, wxIdleEventHandler( frmMain::splMainOnIdle ), NULL, this );
	splMain->SetMinimumPaneSize( 200 );

	pnlLeft = new wxPanel( splMain, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bsLeft;
	bsLeft = new wxBoxSizer( wxVERTICAL );

	cbxOnlyActive = new wxCheckBox( pnlLeft, wxID_ANY, _("Only active"), wxDefaultPosition, wxDefaultSize, 0 );
	cbxOnlyActive->SetValue(true);
	cbxOnlyActive->SetToolTip( _("Hide removed profiles and doctors") );

	bsLeft->Add( cbxOnlyActive, 0, wxALL, 2 );

	wxArrayString chProfileChoices;
	chProfile = new wxChoice( pnlLeft, wxID_ANY, wxDefaultPosition, wxDefaultSize, chProfileChoices, 0 );
	chProfile->SetSelection( 0 );
	bsLeft->Add( chProfile, 0, wxALL|wxEXPAND, 5 );

	wxBoxSizer* bsProfileButtons;
	bsProfileButtons = new wxBoxSizer( wxHORIZONTAL );

	btnDeptAdd = new wxBitmapButton( pnlLeft, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	btnDeptAdd->SetBitmap( wxBITMAP_PNG(icogroupadd));
	btnDeptAdd->SetBitmapDisabled( wxNullBitmap );
	btnDeptAdd->SetBitmapPressed( wxNullBitmap );
	btnDeptAdd->SetBitmapFocus( wxNullBitmap );
	btnDeptAdd->SetBitmapCurrent( wxNullBitmap );
	btnDeptAdd->SetToolTip( _("Add department") );

	bsProfileButtons->Add( btnDeptAdd, 0, wxALL, 2 );

	btnDeptRemove = new wxBitmapButton( pnlLeft, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	btnDeptRemove->SetBitmap( wxBITMAP_PNG(icogroupremove));
	btnDeptRemove->SetToolTip( _("Remove department") );

	bsProfileButtons->Add( btnDeptRemove, 0, wxALL, 2 );

	btnDeptRename = new wxBitmapButton( pnlLeft, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	btnDeptRename->SetBitmap( wxBITMAP_PNG(icogroupedit));
	btnDeptRename->SetToolTip( _("Rename department") );

	bsProfileButtons->Add( btnDeptRename, 0, wxALL, 2 );

	btnDeptUp = new wxBitmapButton( pnlLeft, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	btnDeptUp->SetBitmap( wxBITMAP_PNG(icoup));
	btnDeptUp->SetToolTip( _("Move department up") );

	bsProfileButtons->Add( btnDeptUp, 0, wxALL, 2 );

	btnDeptDown = new wxBitmapButton( pnlLeft, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	btnDeptDown->SetBitmap( wxBITMAP_PNG(icodown));
	btnDeptDown->SetToolTip( _("Move department down") );

	bsProfileButtons->Add( btnDeptDown, 0, wxALL, 2 );

	bsProfileButtons->Add( 10, 0, 0, wxEXPAND, 5 );

	btnDoctorAdd = new wxBitmapButton( pnlLeft, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	btnDoctorAdd->SetBitmap( wxBITMAP_PNG(icodoctoradd));
	btnDoctorAdd->SetToolTip( _("Add doctor") );

	bsProfileButtons->Add( btnDoctorAdd, 0, wxALL, 2 );


	bsLeft->Add( bsProfileButtons, 0, wxEXPAND, 5 );

	lbxDoctors = new wxListBox( pnlLeft, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	bsLeft->Add( lbxDoctors, 1, wxALL|wxEXPAND, 5 );


	pnlLeft->SetSizer( bsLeft );
	pnlLeft->Layout();
	bsLeft->Fit( pnlLeft );
	pnlRight = new wxPanel( splMain, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bsRight;
	bsRight = new wxBoxSizer( wxVERTICAL );

	nbkDoctor = new wxNotebook( pnlRight, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	pnlRules = new wxPanel( nbkDoctor, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bsRules;
	bsRules = new wxBoxSizer( wxVERTICAL );

	bmpWeek = new wxStaticBitmap( pnlRules, wxID_ANY, wxArtProvider::GetBitmap( wxART_ADD_BOOKMARK, wxART_TOOLBAR ), wxDefaultPosition, wxSize(560, 70), 0 );
	bsRules->Add( bmpWeek, 0, wxALL, 5 );

	wxBoxSizer* bsRulesGrid;
	bsRulesGrid = new wxBoxSizer( wxHORIZONTAL );

	wxBoxSizer* bsGrid;
	bsGrid = new wxBoxSizer( wxVERTICAL );

	wxBoxSizer* bsRulesButtons;
	bsRulesButtons = new wxBoxSizer( wxHORIZONTAL );

	btnRuleAdd = new wxBitmapButton( pnlRules, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	btnRuleAdd->SetBitmap( wxBITMAP_PNG(icoadd));
	btnRuleAdd->SetToolTip( _("Add rule") );
	btnRuleAdd->SetHelpText( _("Add rule") );

	bsRulesButtons->Add( btnRuleAdd, 0, wxALL, 2 );

	btnRuleRemove = new wxBitmapButton( pnlRules, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	btnRuleRemove->SetBitmap( wxBITMAP_PNG(icoremove));
	btnRuleRemove->SetToolTip( _("Remove rule") );
	btnRuleRemove->SetHelpText( _("Remove rule") );

	bsRulesButtons->Add( btnRuleRemove, 0, wxALL, 2 );


	bsRulesButtons->Add( 10, 0, 0, wxEXPAND, 5 );

	btnRuleUp = new wxBitmapButton( pnlRules, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	btnRuleUp->SetBitmap( wxBITMAP_PNG(icoup));
	btnRuleUp->SetToolTip( _("Move selected rule up") );

	bsRulesButtons->Add( btnRuleUp, 0, wxALL, 2 );

	btnRuleDown = new wxBitmapButton( pnlRules, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, wxBU_AUTODRAW|0 );

	btnRuleDown->SetBitmap( wxBITMAP_PNG(icodown));
	btnRuleDown->SetToolTip( _("Move selected rule down") );

	bsRulesButtons->Add( btnRuleDown, 0, wxALL, 2 );


	bsGrid->Add( bsRulesButtons, 0, wxEXPAND, 2 );

  lRules = new wxListCtrl (pnlRules, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL | wxLC_HRULES | wxLC_VRULES);
  lRules->AppendColumn(_("Name"), wxLIST_FORMAT_LEFT, 110);
  lRules->AppendColumn(_("Title"), wxLIST_FORMAT_CENTER, 90);
  lRules->AppendColumn(_("Period"), wxLIST_FORMAT_CENTER, 140);
	bsGrid->Add( lRules, 1, wxALL|wxEXPAND, 2 );

	bsRulesGrid->Add( bsGrid, 1, wxEXPAND, 5 );

	pnlRule = new wxPanel( pnlRules, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgRule;
	fgRule = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgRule->SetFlexibleDirection( wxBOTH );
	fgRule->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	lblRuleName = new wxStaticText( pnlRule, wxID_ANY, _("Name"), wxDefaultPosition, wxDefaultSize, 0 );
	lblRuleName->Wrap( -1 );
	fgRule->Add( lblRuleName, 0, wxALIGN_BOTTOM|wxALL, 2 );

	edRuleName = new wxTextCtrl( pnlRule, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
 	edRuleName->SetMinSize( wxSize( 150,-1 ) );
	fgRule->Add( edRuleName, 0, wxALL, 2 );

	lblRuleTitle = new wxStaticText( pnlRule, wxID_ANY, _("Title"), wxDefaultPosition, wxDefaultSize, 0 );
	lblRuleTitle->Wrap( -1 );
	fgRule->Add( lblRuleTitle, 0, wxALIGN_BOTTOM|wxALL, 2 );

	edRuleTitle = new wxTextCtrl( pnlRule, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
 	edRuleTitle->SetMinSize( wxSize( 150,-1 ) );
	fgRule->Add( edRuleTitle, 0, wxALL, 2 );

	cbxFrom = new wxCheckBox( pnlRule, wxID_ANY, _("From"), wxDefaultPosition, wxDefaultSize, 0 );
	fgRule->Add( cbxFrom, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_LEFT|wxALL, 2 );

	dpFrom = new wxDatePickerCtrl( pnlRule, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DROPDOWN );
	fgRule->Add( dpFrom, 0, wxALL, 2 );

	cbxTill = new wxCheckBox( pnlRule, wxID_ANY, _("Till"), wxDefaultPosition, wxDefaultSize, 0 );
	fgRule->Add( cbxTill, 0, wxALIGN_CENTER_VERTICAL|wxALIGN_LEFT|wxALL, 2 );

	dpTill = new wxDatePickerCtrl( pnlRule, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DROPDOWN );
	fgRule->Add( dpTill, 0, wxALL, 2 );

	lblParity = new wxStaticText( pnlRule, wxID_ANY, _("Parity"), wxDefaultPosition, wxDefaultSize, 0 );
	lblParity->Wrap( -1 );
	fgRule->Add( lblParity, 0, wxALIGN_BOTTOM|wxALL, 2 );

	wxString chParityChoices[] = { _("None"), _("Odd"), _("Even") };
	int chParityNChoices = sizeof( chParityChoices ) / sizeof( wxString );
	chParity = new wxChoice( pnlRule, wxID_ANY, wxDefaultPosition, wxDefaultSize, chParityNChoices, chParityChoices, 0 );
 	chParity->SetMinSize( wxSize( 150,-1 ) );
	chParity->SetSelection( 0 );
	fgRule->Add( chParity, 0, wxALL, 2 );

	lblWeekDay = new wxStaticText( pnlRule, wxID_ANY, _("Weekday"), wxDefaultPosition, wxDefaultSize, 0 );
	lblWeekDay->Wrap( -1 );
	fgRule->Add( lblWeekDay, 0, wxALIGN_BOTTOM|wxALL, 2 );

	wxString chWeekDayChoices[] = { _("None"), _("Monday"), _("Tuesday"), _("Wednesday"), _("Thursday"), _("Friday"), _("Saturday"), _("Sunday") };
	int chWeekDayNChoices = sizeof( chWeekDayChoices ) / sizeof( wxString );
	chWeekDay = new wxChoice( pnlRule, wxID_ANY, wxDefaultPosition, wxDefaultSize, chWeekDayNChoices, chWeekDayChoices, 0 );
 	chWeekDay->SetMinSize( wxSize( 150,-1 ) );
	chWeekDay->SetSelection( 0 );
	fgRule->Add( chWeekDay, 0, wxALL, 2 );

	lblWeek = new wxStaticText( pnlRule, wxID_ANY, _("of week"), wxDefaultPosition, wxDefaultSize, 0 );
	lblWeek->Wrap( -1 );
	fgRule->Add( lblWeek, 0, wxALIGN_BOTTOM|wxALL, 2 );

	wxString chWeekChoices[] = { _("None"), _("First"), _("Second"), _("Third"), _("Fourth"), _("Fifth") };
	int chWeekNChoices = sizeof( chWeekChoices ) / sizeof( wxString );
	chWeek = new wxChoice( pnlRule, wxID_ANY, wxDefaultPosition, wxDefaultSize, chWeekNChoices, chWeekChoices, 0 );
 	chWeek->SetMinSize( wxSize( 150,-1 ) );
	chWeek->SetSelection( 0 );
	fgRule->Add( chWeek, 0, wxALL, 2 );


	pnlRule->SetSizer( fgRule );
	pnlRule->Layout();
	fgRule->Fit( pnlRule );
	bsRulesGrid->Add( pnlRule, 0, wxEXPAND | wxALL, 2 );


	bsRules->Add( bsRulesGrid, 1, wxEXPAND, 5 );


	pnlRules->SetSizer( bsRules );
	pnlRules->Layout();
	bsRules->Fit( pnlRules );
	nbkDoctor->AddPage( pnlRules, _("Rules"), true );
	pnlDoctor = new wxPanel( nbkDoctor, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxFlexGridSizer* fgDoctor;
	fgDoctor = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgDoctor->AddGrowableCol( 1 );
	fgDoctor->SetFlexibleDirection( wxBOTH );
	fgDoctor->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	lblTitle = new wxStaticText( pnlDoctor, wxID_ANY, _("Title"), wxDefaultPosition, wxDefaultSize, 0 );
	lblTitle->Wrap( -1 );
	fgDoctor->Add( lblTitle, 0, wxALIGN_BOTTOM|wxALL, 2 );

	edTitle = new wxTextCtrl( pnlDoctor, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgDoctor->Add( edTitle, 0, wxALL|wxEXPAND, 2 );

	lblStudy = new wxStaticText( pnlDoctor, wxID_ANY, _("Study"), wxDefaultPosition, wxDefaultSize, 0 );
	lblStudy->Wrap( -1 );
	fgDoctor->Add( lblStudy, 0, wxALIGN_BOTTOM|wxALL, 2 );

	edStudy = new wxTextCtrl( pnlDoctor, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgDoctor->Add( edStudy, 0, wxALL|wxEXPAND, 2 );

	lblName = new wxStaticText( pnlDoctor, wxID_ANY, _("Name"), wxDefaultPosition, wxDefaultSize, 0 );
	lblName->Wrap( -1 );
	fgDoctor->Add( lblName, 0, wxALIGN_BOTTOM|wxALL, 2 );

	edName = new wxTextCtrl( pnlDoctor, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgDoctor->Add( edName, 0, wxALL|wxEXPAND, 2 );

	lblPosition = new wxStaticText( pnlDoctor, wxID_ANY, _("Position"), wxDefaultPosition, wxDefaultSize, 0 );
	lblPosition->Wrap( -1 );
	fgDoctor->Add( lblPosition, 0, wxALIGN_BOTTOM|wxALL, 2 );

	edPosition = new wxTextCtrl( pnlDoctor, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgDoctor->Add( edPosition, 0, wxALL|wxEXPAND, 2 );

	lblInstitute = new wxStaticText( pnlDoctor, wxID_ANY, _("Institute"), wxDefaultPosition, wxDefaultSize, 0 );
	lblInstitute->Wrap( -1 );
	fgDoctor->Add( lblInstitute, 0, wxALIGN_BOTTOM|wxALL, 2 );

	edInstitute = new wxTextCtrl( pnlDoctor, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgDoctor->Add( edInstitute, 0, wxALL|wxEXPAND, 2 );

	lblCertificate = new wxStaticText( pnlDoctor, wxID_ANY, _("Certificate"), wxDefaultPosition, wxDefaultSize, 0 );
	lblCertificate->Wrap( -1 );
	fgDoctor->Add( lblCertificate, 0, wxALIGN_BOTTOM|wxALL, 2 );

	edCertificate = new wxTextCtrl( pnlDoctor, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgDoctor->Add( edCertificate, 0, wxALL|wxEXPAND, 2 );

	lblCategory = new wxStaticText( pnlDoctor, wxID_ANY, _("Category"), wxDefaultPosition, wxDefaultSize, 0 );
	lblCategory->Wrap( -1 );
	fgDoctor->Add( lblCategory, 0, wxALIGN_BOTTOM|wxALL, 2 );

	edCategory = new wxTextCtrl( pnlDoctor, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	fgDoctor->Add( edCategory, 0, wxALL|wxEXPAND, 2 );

	cbxActive = new wxCheckBox( pnlDoctor, wxID_ANY, _("Active"), wxDefaultPosition, wxDefaultSize, 0 );
	fgDoctor->Add( cbxActive, 0, wxALIGN_BOTTOM|wxALL, 5 );


	pnlDoctor->SetSizer( fgDoctor );
	pnlDoctor->Layout();
	fgDoctor->Fit( pnlDoctor );
	nbkDoctor->AddPage( pnlDoctor, _("Doctor"), false );

	bsRight->Add( nbkDoctor, 1, wxEXPAND | wxALL, 5 );


	pnlRight->SetSizer( bsRight );
	pnlRight->Layout();
	bsRight->Fit( pnlRight );
	splMain->SplitVertically( pnlLeft, pnlRight, 260 );
	bsMain->Add( splMain, 1, wxEXPAND, 5 );


	pnlMain->SetSizer( bsMain );
	pnlMain->Layout();
	bsMain->Fit( pnlMain );
	mainSizer->Add( pnlMain, 1, wxEXPAND | wxALL, 0 );


	this->SetSizer( mainSizer );
	this->Layout();
	stbrMain = this->CreateStatusBar( 1, wxSTB_SIZEGRIP, wxID_ANY );

	this->Centre( wxBOTH );

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( frmMain::OnCloseFrame ) );
	mnuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMain::OnExitClick ), this, menuFileExit->GetId());
	mnuTools->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMain::OnDeptAdd ), this, miDeptAdd->GetId());
	mnuTools->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMain::OnDeptRemove ), this, miDeptRemove->GetId());
	mnuTools->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMain::OnDeptRename ), this, miDeptRename->GetId());
	mnuTools->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMain::OnDoctorAdd ), this, miDoctorAdd->GetId());
	mnuTools->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMain::OnRuleAdd ), this, miRuleAdd->GetId());
	mnuTools->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMain::OnRuleRemove ), this, miRuleRemove->GetId());
  mnuTools->Bind(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler( frmMain::OnAddHoliday ), this, miHoliday->GetId());
	this->Connect( tDeptAdd->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMain::OnDeptAdd ) );
	this->Connect( tDeptRemove->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMain::OnDeptRemove ) );
	this->Connect( tDeptRename->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMain::OnDeptRename ) );
	this->Connect( tDoctorAdd->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMain::OnDoctorAdd ) );
	this->Connect( tRuleAdd->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMain::OnRuleAdd ) );
	this->Connect( tRuleRemove->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMain::OnRuleRemove ) );
	this->Connect( tExit->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMain::OnExitClick ) );
	cbxOnlyActive->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( frmMain::OnlyActiveClicked ), NULL, this );
	chProfile->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( frmMain::ProfileSelected ), NULL, this );
	btnDeptAdd->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( frmMain::OnDeptAdd ), NULL, this );
	btnDeptRemove->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( frmMain::OnDeptRemove ), NULL, this );
  btnDeptUp->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( frmMain::DeptMove ), NULL, this );
	btnDeptDown->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( frmMain::DeptMove ), NULL, this );
	btnDeptRename->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( frmMain::OnDeptRename ), NULL, this );
	btnDoctorAdd->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( frmMain::OnDoctorAdd ), NULL, this );
	lbxDoctors->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( frmMain::DoctorSelected ), NULL, this );
	btnRuleAdd->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( frmMain::OnRuleAdd ), NULL, this );
	btnRuleRemove->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( frmMain::OnRuleRemove ), NULL, this );
	btnRuleUp->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( frmMain::RuleMove ), NULL, this );
	btnRuleDown->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( frmMain::RuleMove ), NULL, this );
	lRules->Connect( wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler( frmMain::RuleItemSelected ), NULL, this );
	cbxFrom->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( frmMain::OnFromChecked ), NULL, this );
	cbxTill->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( frmMain::OnTillChecked ), NULL, this );
	chWeekDay->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( frmMain::WeekdayChanged ), NULL, this );
}

frmMain::~frmMain()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( frmMain::OnCloseFrame ) );
	this->Disconnect( tDeptAdd->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMain::OnDeptAdd ) );
	this->Disconnect( tDeptRemove->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMain::OnDeptRemove ) );
	this->Disconnect( tDeptRename->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMain::OnDeptRename ) );
	this->Disconnect( tDoctorAdd->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMain::OnDoctorAdd ) );
	this->Disconnect( tRuleAdd->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMain::OnRuleAdd ) );
	this->Disconnect( tRuleRemove->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMain::OnRuleRemove ) );
	this->Disconnect( tExit->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( frmMain::OnExitClick ) );
	cbxOnlyActive->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( frmMain::OnlyActiveClicked ), NULL, this );
	chProfile->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( frmMain::ProfileSelected ), NULL, this );
	btnDeptAdd->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( frmMain::OnDeptAdd ), NULL, this );
	btnDeptRemove->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( frmMain::OnDeptRemove ), NULL, this );
	btnDeptRename->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( frmMain::OnDeptRename ), NULL, this );
  btnDeptUp->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( frmMain::DeptMove ), NULL, this );
	btnDeptDown->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( frmMain::DeptMove ), NULL, this );
	btnDoctorAdd->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( frmMain::OnDoctorAdd ), NULL, this );
	lbxDoctors->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( frmMain::DoctorSelected ), NULL, this );
	btnRuleAdd->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( frmMain::OnRuleAdd ), NULL, this );
	btnRuleRemove->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( frmMain::OnRuleRemove ), NULL, this );
	btnRuleUp->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( frmMain::RuleMove ), NULL, this );
	btnRuleDown->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( frmMain::RuleMove ), NULL, this );
	cbxFrom->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( frmMain::OnFromChecked ), NULL, this );
	cbxTill->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( frmMain::OnTillChecked ), NULL, this );
	chWeekDay->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( frmMain::WeekdayChanged ), NULL, this );
}
