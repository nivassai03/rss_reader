#pragma once
#include <wx/frame.h>
#include <wx/treectrl.h>
#include "FeedList.h"
#include "ArticleWebView.h"
#include "SourceTree.h"
#include "RssParser.h"
#include "ArticleDB.h"
#include <wx/panel.h>
#include <wx/progdlg.h>

wxDECLARE_EVENT(SOURCE_SELECTED_IN_SOURCETREE, wxCommandEvent);

class RssReaderCtrl : public wxPanel
{
private:
    SourceTree *m_sourceTree;
    FeedList *m_newsList;
    ArticleWebView *m_articleWebView;
    RssSourceCtrl m_sourceCtrl;
    const ArticleDB &m_articles;

    void OnSourceSelection(wxTreeEvent &);
    void RedirectOpenArticleEvent(wxCommandEvent &);
    std::string getSourceFilePath();

public:
    RssReaderCtrl(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, const ArticleDB &articles);
    void RefreshSourceTree();
};

enum class MenuID
{
    INSTALL_SOURCES = wxID_HIGHEST + 30,
};