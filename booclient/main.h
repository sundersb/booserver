/*********************************************************************
 * Name:      	main.h
 * Purpose:   	Declares simple wxWidgets application with GUI
 * 				created using wxFormBuilder.
 * Author:
 * Created:
 * Copyright:
 * License:   	wxWidgets license (www.wxwidgets.org)
 *
 * Notes:		Note that all GUI creation code is declared in
 * 				gui.h source file which is generated by wxFormBuilder.
 *********************************************************************/

#pragma once

#include <wx/wx.h>

#include <memory>
#include <vector>

#include "gui.h"
#include "repository.h"
#include "chart.h"

class MainFrame : public frmMain {
  // Models' repository
  std::shared_ptr<data::Repository> repo;

  // Currently selected doctors' profile
  Profile *m_profile;

  // Current doctor
  Doctor *m_doctor;

  Rule *m_rule;

  std::vector<Rule*> rules;

  Chart chart;

public:
  MainFrame(wxWindow *parent);
  virtual ~MainFrame();

  /**
   * @brief         Get models' repository
   */
  std::shared_ptr<data::Repository> getRepository(void);

  /**
   * @brief         Set repository for the form
   * @param value
   */
  void setRepository(std::shared_ptr<data::Repository> value);

  /**
   * @brief         Load profiles list etc. on form startup
   */
  bool load(void);

protected:
  // protected event handlers
  virtual void OnCloseFrame( wxCloseEvent& event );
  virtual void OnExitClick( wxCommandEvent& event );

  virtual void ProfileSelected( wxCommandEvent& event );
  virtual void DoctorSelected( wxCommandEvent& event );
  virtual void RuleItemSelected(wxListEvent& event);
  virtual void WeekdayChanged( wxCommandEvent& event );
  virtual void OnDeptAdd( wxCommandEvent& event );
  virtual void OnDeptRemove( wxCommandEvent& event );
  virtual void OnDeptRename( wxCommandEvent& event );
  virtual void DeptMove( wxCommandEvent& event );
  virtual void OnDoctorAdd( wxCommandEvent& event );
  virtual void OnRuleAdd( wxCommandEvent& event );
  virtual void OnRuleRemove( wxCommandEvent& event );
  virtual void OnAddHoliday( wxCommandEvent& event );
  virtual void OnlyActiveClicked( wxCommandEvent& event );
  virtual void RuleMove( wxCommandEvent& event );
  virtual void OnFromChecked( wxCommandEvent& event );
  virtual void OnTillChecked( wxCommandEvent& event );

private:
  bool loadProfiles(void);
  bool loadDoctors(const Profile *profile);
  bool loadDoctor(Doctor *doctor);
  bool loadRules(const Doctor *doctor);

  bool saveDoctor(void);
  bool loadRule(Rule *rule);
  bool saveRule(void);

  void updateImage(void);
};
