#pragma once
#include <wx/frame.h>
#include "ArticleDB.h"
#include "RssReaderCtrl.h"
#include <wx/aui/auibook.h>

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);
    void InitArticles();

private:
    ArticleDB m_articles;
    wxAuiNotebook *m_tabpanel;
    void SetupMenuBar();
    void SetupToolBar();
    void OnInstallSources(wxCommandEvent &);
    void OnNewPage(wxCommandEvent &);
    void OnClosePage(wxCommandEvent &);
    void OnQuit(wxCommandEvent &);
    void ChangeTabNameOnSourceSelection(wxCommandEvent &);
    void RefreshAllPagesSourceTrees();
};
