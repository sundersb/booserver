///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 17 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/toolbar.h>
#include <wx/checkbox.h>
#include <wx/choice.h>
#include <wx/bmpbuttn.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/listbox.h>
#include <wx/panel.h>
#include <wx/statbmp.h>
#include <wx/listctrl.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/notebook.h>
#include <wx/splitter.h>
#include <wx/statusbr.h>
#include <wx/frame.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class frmMain
///////////////////////////////////////////////////////////////////////////////
class frmMain : public wxFrame
{
	private:

	protected:
		wxMenuBar* m_menuBar;
		wxMenu* mnuFile;
		wxMenu* mnuTools;
		wxToolBar* tbrMain;
		wxToolBarToolBase* tDeptAdd;
		wxToolBarToolBase* tDeptRemove;
		wxToolBarToolBase* tDeptRename;
		wxToolBarToolBase* tDoctorAdd;
		wxToolBarToolBase* tRuleAdd;
		wxToolBarToolBase* tRuleRemove;
		wxToolBarToolBase* tExit;
		wxPanel* pnlMain;
		wxSplitterWindow* splMain;
		wxPanel* pnlLeft;
		wxCheckBox* cbxOnlyActive;
		wxChoice* chProfile;
		wxBitmapButton* btnDeptAdd;
		wxBitmapButton* btnDeptRemove;
		wxBitmapButton* btnDeptRename;
		wxBitmapButton* btnDoctorAdd;
		wxListBox* lbxDoctors;
		wxPanel* pnlRight;
		wxNotebook* nbkDoctor;
		wxPanel* pnlRules;
		wxStaticBitmap* bmpWeek;
		wxBitmapButton* btnRuleAdd;
		wxBitmapButton* btnRuleRemove;
		wxBitmapButton* btnRuleUp;
		wxBitmapButton* btnRuleDown;
//		wxGrid* sgRules;
		wxListCtrl* lRules;
		wxPanel* pnlRule;
		wxStaticText* lblRuleName;
		wxTextCtrl* edRuleName;
		wxStaticText* lblRuleTitle;
		wxTextCtrl* edRuleTitle;
		wxCheckBox* cbxFrom;
		wxDatePickerCtrl* dpFrom;
		wxCheckBox* cbxTill;
		wxDatePickerCtrl* dpTill;
		wxStaticText* lblParity;
		wxChoice* chParity;
		wxStaticText* lblWeekDay;
		wxChoice* chWeekDay;
		wxStaticText* lblWeek;
		wxChoice* chWeek;
		wxPanel* pnlDoctor;
		wxStaticText* lblTitle;
		wxTextCtrl* edTitle;
		wxStaticText* lblName;
		wxTextCtrl* edName;
		wxStaticText* lblStudy;
		wxTextCtrl* edStudy;
		wxStaticText* lblPosition;
		wxTextCtrl* edPosition;
		wxStaticText* lblInstitute;
		wxTextCtrl* edInstitute;
		wxStaticText* lblCertificate;
		wxTextCtrl* edCertificate;
		wxStaticText* lblCategory;
		wxTextCtrl* edCategory;
		wxCheckBox* cbxActive;
		wxStatusBar* stbrMain;

		// Virtual event handlers, overide them in your derived class
		virtual void OnCloseFrame( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnExitClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDeptAdd( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDeptRemove( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDeptRename( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDoctorAdd( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRuleAdd( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRuleRemove( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnlyActiveClicked( wxCommandEvent& event ) { event.Skip(); }
		virtual void ProfileSelected( wxCommandEvent& event ) { event.Skip(); }
		virtual void DoctorSelected( wxCommandEvent& event ) { event.Skip(); }
		virtual void RuleMove( wxCommandEvent& event ) { event.Skip(); }
    virtual void RuleItemSelected( wxListEvent& event ) { event.Skip(); }
		virtual void OnFromChecked( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTillChecked( wxCommandEvent& event ) { event.Skip(); }
		virtual void WeekdayChanged( wxCommandEvent& event ) { event.Skip(); }


	public:

		frmMain( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Booserver Client"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 850,600 ), long style = wxCLOSE_BOX|wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~frmMain();

		void splMainOnIdle( wxIdleEvent& )
		{
			splMain->SetSashPosition (260);
			splMain->Disconnect( wxEVT_IDLE, wxIdleEventHandler( frmMain::splMainOnIdle ), NULL, this );
		}

};

