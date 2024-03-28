#include "RssReaderCtrl.h"
#include <wx/laywin.h>
#include <wx/sashwin.h>
#include <wx/toolbar.h>
#include <wx/artprov.h>
#include <wx/busyinfo.h>
#include <wx/menu.h>
#include <wx/log.h>
#include <wx/sizer.h>
#include <wx/splitter.h>
#include <wx/utils.h>
#include <thread>
#include <wx/stdpaths.h>
#include <wx/msgdlg.h>
#include <chrono>
#include <wx/msgdlg.h>
#include "RssParser.h"
#include "SourceInstallDialog.h"
#include <wx/event.h>
#include "FeedItem.h"
#include "RssSourceCtrl.h"

wxDEFINE_EVENT(SOURCE_SELECTED_IN_SOURCETREE, wxCommandEvent);

RssReaderCtrl::RssReaderCtrl(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, const ArticleDB &articles) : wxPanel(parent, id, pos, size), m_sourceCtrl(RssSourceCtrl()), m_articles(articles)
{

    wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
    wxSplitterWindow *mainSplitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    m_sourceTree = new SourceTree(this, wxID_ANY, wxDefaultPosition, wxSize(300, 600), m_sourceCtrl.fetchInstalledSources());
    m_newsList = new FeedList(mainSplitter, wxID_ANY, wxDefaultPosition, wxSize(600, 600));
    m_articleWebView = new ArticleWebView(mainSplitter, wxID_ANY, wxDefaultPosition, wxSize(500, 600));
    mainSplitter->SplitVertically(m_newsList, m_articleWebView, 0);

    sizer->Add(m_sourceTree, 0, wxEXPAND);
    sizer->Add(mainSplitter, 1, wxEXPAND);
    this->SetSizerAndFit(sizer);
    Bind(OPEN_ARTICLE_IN_WEBVIEW, &RssReaderCtrl::RedirectOpenArticleEvent, this);
    Bind(wxEVT_TREE_ITEM_ACTIVATED, &RssReaderCtrl::OnSourceSelection, this);
}

void RssReaderCtrl::RedirectOpenArticleEvent(wxCommandEvent &evt)
{
    m_articleWebView->GetEventHandler()->ProcessEvent(evt);
}

void RssReaderCtrl::OnSourceSelection(wxTreeEvent &evt)
{
    wxTreeItemId id = evt.GetItem();
    RssSource source = m_sourceTree->getRssSource(id);
    m_newsList->LoadArticles(m_articles.GetArticles(source.getCategory(), source.getName()));
    wxCommandEvent sourceSelected(SOURCE_SELECTED_IN_SOURCETREE, evt.GetId());
    sourceSelected.SetString(source.getName());
    ProcessWindowEvent(sourceSelected);
}
std::string RssReaderCtrl::getSourceFilePath()
{
    wxFileName rootDir = wxFileName::DirName(wxStandardPaths::Get().GetUserDataDir());
    wxString sourceFilePath = rootDir.GetFullPath() + "sources.json";
    return sourceFilePath.ToStdString();
}

void RssReaderCtrl::RefreshSourceTree()
{
    m_sourceTree->updateSources(m_sourceCtrl.fetchInstalledSources());
    m_sourceTree->reconstructSourceTree();
    m_sourceTree->expandTree();
}
