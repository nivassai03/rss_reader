#include "MainFrame.h"
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

MainFrame::MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size) : wxFrame(nullptr, wxID_ANY, title, pos, size), m_sourceCtrl(RssSourceCtrl())
{

	setupToolBar();
	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);
	wxSplitterWindow *mainSplitter = new wxSplitterWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
	m_sourceTree = new SourceTree(this, wxID_ANY, wxDefaultPosition, wxSize(300, 600), m_sourceCtrl.fetchInstalledSources());
	m_newsList = new FeedList(mainSplitter, wxID_ANY, wxDefaultPosition, wxSize(600, 600));
	m_articleWebView = new ArticleWebView(mainSplitter, wxID_ANY, wxDefaultPosition, wxSize(500, 600));

	// sizer->Add(m_firstPanel, 0, wxEXPAND | wxALL, 10);
	sizer->Add(m_sourceTree, 0, wxEXPAND | wxALL, 10);
	mainSplitter->SplitVertically(m_newsList, m_articleWebView);
	sizer->Add(mainSplitter, 1, wxEXPAND | wxALL, 10);

	this->SetSizerAndFit(sizer);
	Bind(OPEN_ARTICLE_IN_WEBVIEW, &MainFrame::RedirectOpenArticleEvent, this);
	Bind(wxEVT_TREE_ITEM_ACTIVATED, &MainFrame::OnSourceSelection, this);
}

void MainFrame::RedirectOpenArticleEvent(wxCommandEvent &evt)
{
	m_articleWebView->GetEventHandler()->ProcessEvent(evt);
}
void MainFrame::InitDefaultData()
{

	wxString assetsDir = wxFileName::DirName(wxFileName(wxFileName::GetCwd(), "assets").GetFullPath()).GetFullPath();
	wxString default_thumbnail_path = assetsDir + "default_thumbnail.jpg";
	wxString welcome_msg_path = assetsDir + "welcome.html";
	wxString default_sourcefile_path = assetsDir + "sources.json";
	wxFileName rootDir = wxFileName::DirName(wxStandardPaths::Get().GetUserDataDir()).GetFullPath();
	wxString rootDirPath = rootDir.GetFullPath();
	if (!wxDirExists(rootDirPath))
	{
		rootDir.Mkdir(wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
	}
	wxString dest_thumbnail_path = rootDirPath + "default_thumbnail.jpg";
	wxString dest_welcome_msg_path = rootDirPath + "welcome.html";
	wxString dest_sourcefile_path = rootDirPath + "sources.json";

	wxCopyFile(default_thumbnail_path, dest_thumbnail_path);
	wxCopyFile(welcome_msg_path, dest_welcome_msg_path);
	wxCopyFile(default_sourcefile_path, dest_sourcefile_path);
}

void MainFrame::OnSourceSelection(wxTreeEvent &evt)
{
	wxTreeItemId id = evt.GetItem();
	RssSource source = m_sourceTree->getRssSource(id);
	RssParser parser;
	parser.SetSourceAndImgDIr(source.getName(), source.getImgDir());
	std::vector<Article> articles = parser.FetchArticles(source.getUrl());
	m_newsList->LoadArticles(articles);
}

void MainFrame::FetchAllArticlesFromSources(const std::unordered_map<std::string, std::vector<RssSource>> &sources)
{
	std::vector<std::thread> threads;
	for (const auto &it : sources)
	{
		for (const auto &source : it.second)
		{
			threads.push_back(std::thread([=, this]
										  {
				wxMessageOutputDebug dbg;	
				dbg.Printf("Dowloading from source: %s",source.getName().c_str());
				auto start = std::chrono::system_clock::now();
				RssParser parser;
				parser.SetSourceAndImgDIr(source.getName(),source.getImgDir());
				std::vector<Article> articles = parser.FetchArticles(source.getUrl());
				auto end = std::chrono::system_clock::now();
				dbg.Printf("time for source: %s is :%s",source.getName().c_str(),std::to_string(std::chrono::floor<std::chrono::seconds>(end-start).count())); }));
		}
	}
	for (auto &thread : threads)
	{
		thread.join();
	}
}
void MainFrame::InitAllArticles()
{
	// FetchAllArticlesFromSources(m_sourceCtrl.fetchInstalledSources());
	RssParser parser;
	parser.FetchAllArticlesFromSources(m_sourceCtrl.fetchInstalledSources());
}

std::string MainFrame::getSourceFilePath()
{
	wxFileName rootDir = wxFileName::DirName(wxStandardPaths::Get().GetUserDataDir());
	wxString sourceFilePath = rootDir.GetFullPath() + "sources.json";
	return sourceFilePath.ToStdString();
}

void MainFrame::OnInstallSources(wxCommandEvent &evt)
{
	SourceInstallDialog installSource(this, wxID_ANY, "Install Sources", wxDefaultPosition, wxSize(800, 600));
	if (installSource.ShowModal() == wxID_CANCEL)
	{
		installSource.Close();
	}
	m_sourceTree->updateSources(m_sourceCtrl.fetchInstalledSources());
	m_sourceTree->reconstructSourceTree();
	m_sourceTree->expandTree();
}

void MainFrame::setupToolBar()
{
	wxToolBar *toolBar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT | wxTB_HORZ_TEXT);
	wxBitmap plusBitmap = wxArtProvider::GetBitmap(wxART_ADD_BOOKMARK);
	toolBar->AddTool(wxID_ANY, "Install Sources", plusBitmap, "Add a new source");
	toolBar->AddSeparator();
	toolBar->Bind(wxEVT_TOOL, &MainFrame::OnInstallSources, this, wxID_ANY);
	toolBar->Realize();
	SetToolBar(toolBar);
}