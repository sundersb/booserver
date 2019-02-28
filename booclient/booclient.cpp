#include "booclient.h"
#include "main.h"
#include "dlg_login.h"

#include <wx/stdpaths.h>
#include <wx/xml/xml.h>
#include <stdexcept>

const std::string SERVER = "localhost";
const std::string DATABASE = "booserver";
const std::string USER = "booserver";

const wxString SETUP_FILENAME = "booclient.xml";

// initialize the application
IMPLEMENT_APP(MainApp);

const wxString& getBaseDirectory(void) {
  static wxString path;

  if (path.IsEmpty()) {
    wxFileName app(wxStandardPaths::Get().GetExecutablePath());
    path = app.GetPath(true);
  }

  return path;
}

bool MainApp::OnInit() {
  locale = nullptr;

  loadSetup();
  selectLanguage(langCode);

  std::shared_ptr<data::Repository> repo(new data::Repository());

  DialogLogin dlg(nullptr);
  dlg.setUser(user);

  while (dlg.ShowModal() == wxID_OK) {
    if (repo->init(server, database, dlg.getUser(), dlg.getPassword())) {
      wxInitAllImageHandlers();
      MainFrame *frame = new MainFrame(nullptr);

      frame->setRepository(repo);
      SetTopWindow(frame);
      frame->SetIcon(wxICON(icon));
      frame->Show();
      frame->load();
      return true;
    } else {
      wxMessageBox(_("Invalid login data or the database is offline!"), _("Error"), wxICON_ERROR | wxOK, nullptr);
    }
  }
  return false;
}

MainApp::~MainApp() {
  delete locale;
}

void MainApp::selectLanguage(int code) {
  delete locale;
  locale = new wxLocale(code);
  // TODO: Use /usr/local/share/booclient as XML location
  locale->AddCatalogLookupPathPrefix(getBaseDirectory());
  locale->AddCatalog(wxT("booclient"));
}

std::string getNodeValue(const wxXmlNode *node) {
  wxString value = node->GetNodeContent();
  value.Trim(false).Trim(true);
  return std::string(value.mb_str(wxConvUTF8));
}

void MainApp::loadSetup(void) {
  server = SERVER;
  database = DATABASE;
  user = USER;
  langCode = wxLANGUAGE_DEFAULT;

  wxXmlDocument xml;
  if (!xml.Load(getBaseDirectory() + SETUP_FILENAME)) return;
  if (!xml.GetRoot()->GetName().IsSameAs(wxT("setup"))) return;

  wxXmlNode *node = xml.GetRoot()->GetChildren();
  while (node) {
    const wxString &name = node->GetName();

    if (name.IsSameAs(wxT("server"))) {
      server = getNodeValue(node);
    } else if (name.IsSameAs(wxT("database"))) {
      database = getNodeValue(node);
    } else if (name.IsSameAs(wxT("user"))) {
      user = getNodeValue(node);
    } else if (name.IsSameAs(wxT("locale"))) {
      wxString value = node->GetNodeContent();
      value.Trim(false).Trim(true);
      long lc;
      if (value.ToLong(&lc)) langCode = lc;
    }
    node = node->GetNext();
  }
}
