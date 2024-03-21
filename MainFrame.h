#pragma once

#include <wx/frame.h>
#include <wx/treectrl.h>
#include "FeedList.h"
#include "ArticleWebView.h"
#include "SourceTree.h"
#include "RssParser.h"
#include <wx/panel.h>
#include <wx/progdlg.h>

class MainFrame : public wxFrame
{
private:
	SourceTree *m_sourceTree;
	FeedList *m_newsList;
	ArticleWebView *m_articleWebView;
	RssSourceCtrl m_sourceCtrl;
	void OnSourceSelection(wxTreeEvent &);
	void FetchAllArticlesFromSources(const std::unordered_map<std::string, std::vector<RssSource>> &);
	void InitDefaultData();
	void RedirectOpenArticleEvent(wxCommandEvent &);
	std::string getSourceFilePath();
	void setupToolBar();
	void OnInstallSources(wxCommandEvent &);

public:
	MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size);
	void InitAllArticles();
};

enum class MenuID
{
	INSTALL_SOURCES = wxID_HIGHEST + 30,
};