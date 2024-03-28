#pragma once
#include <wx/dialog.h>
#include "ArticleDB.h"
#include <wx/notebook.h>
#include <wx/treelist.h>
#include "RssSourceCtrl.h"
#include <wx/treectrl.h>
#include <wx/panel.h>

class SourceInstallDialog : public wxDialog
{
public:
    SourceInstallDialog(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, ArticleDB &aritcles);

private:
    RssSourceCtrl m_sourceCtrl;
    wxPanel *m_panel;
    wxNotebook *m_tabPanel;
    wxPanel *installedPanel;
    wxPanel *availablePanel;
    wxTreeListCtrl *m_availableList;
    wxTreeListCtrl *m_installedList;
    ArticleDB &m_articles;
    void makeInstalledList();
    void OnClose(wxCommandEvent &);
    void makeAvailableList();
    void selectAllChildSources(wxTreeListEvent &);
    void OnSelectAll(wxCommandEvent &);
    void OnUnSelectAll(wxCommandEvent &);
    void OnInstall(wxCommandEvent &);
    void OnUninstall(wxCommandEvent &);
};

enum class SourceInstallID
{
    UNSELECT_ALL_INS = wxID_HIGHEST + 50,
    SELECT_ALL_INS,
    UNSELECT_ALL_AV,
    SELECT_ALL_AV,
};