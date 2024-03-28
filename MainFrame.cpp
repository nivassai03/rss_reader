#include "MainFrame.h"
#include <wx/sizer.h>
#include <wx/toolbar.h>
#include <wx/menu.h>
#include <wx/artprov.h>
#include "SourceInstallDialog.h"
#include <wx/notebook.h>
#include <wx/aui/auibook.h>

MainFrame::MainFrame(const wxString &title, const wxPoint &pos, const wxSize &size) : wxFrame(nullptr, wxID_ANY, title, pos, size), m_articles(ArticleDB())
{
    SetupToolBar();
    SetupMenuBar();
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);

    m_tabpanel = new wxAuiNotebook(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    RssReaderCtrl *newreader = new RssReaderCtrl(m_tabpanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_articles);
    m_tabpanel->AddPage(newreader, "Welcome");
    sizer->Add(m_tabpanel, 1, wxEXPAND | wxALL, 10);
    Bind(SOURCE_SELECTED_IN_SOURCETREE, &MainFrame::ChangeTabNameOnSourceSelection, this, wxID_ANY);
    this->SetSizerAndFit(sizer);
}

void MainFrame::InitArticles()
{
    m_articles.InitInstalledSourceArticlesData();
}

void MainFrame::OnInstallSources(wxCommandEvent &evt)
{
    SourceInstallDialog installSource(this, wxID_ANY, "Install Sources", wxDefaultPosition, wxSize(800, 600), m_articles);
    if (installSource.ShowModal() == wxID_CANCEL)
    {
        installSource.Close();
    }
    RefreshAllPagesSourceTrees();
}

void MainFrame::RefreshAllPagesSourceTrees()
{
    int pageCount = m_tabpanel->GetPageCount();
    for (int i = 0; i < pageCount; i++)
    {
        auto page = m_tabpanel->GetPage(i);
        RssReaderCtrl *readerPage = dynamic_cast<RssReaderCtrl *>(page);
        readerPage->RefreshSourceTree();
    }
}
void MainFrame::SetupToolBar()
{
    wxToolBar *toolBar = new wxToolBar(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTB_FLAT | wxTB_HORZ_TEXT);
    wxBitmap plusBitmap = wxArtProvider::GetBitmap(wxART_ADD_BOOKMARK);
    toolBar->AddTool(wxID_ANY, "Install Sources", plusBitmap, "Add a new source");
    toolBar->AddSeparator();
    toolBar->Bind(wxEVT_TOOL, &MainFrame::OnInstallSources, this, wxID_ANY);
    toolBar->Realize();
    SetToolBar(toolBar);
}

void MainFrame::SetupMenuBar()
{
    wxMenuBar *menuBar = new wxMenuBar;

    wxMenu *fileMenu = new wxMenu;

    fileMenu->Append(wxID_NEW, "&New Tab\tCtrl+n");
    fileMenu->Append(wxID_CLOSE, "&Close Tab\tCtrl+w");
    fileMenu->Append(wxID_EXIT, "&Quit \tCtrl+q");
    fileMenu->AppendSeparator();

    menuBar->Bind(wxEVT_MENU, &MainFrame::OnNewPage, this, wxID_NEW);
    menuBar->Bind(wxEVT_MENU, &MainFrame::OnClosePage, this, wxID_CLOSE);
    menuBar->Bind(wxEVT_MENU, &MainFrame::OnQuit, this, wxID_EXIT);

    menuBar->Append(fileMenu, "&File");
    SetMenuBar(menuBar);
}

void MainFrame::OnNewPage(wxCommandEvent &)
{
    RssReaderCtrl *newReader = new RssReaderCtrl(m_tabpanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_articles);
    m_tabpanel->AddPage(newReader, "Welcome");
    m_tabpanel->ChangeSelection(m_tabpanel->GetPageCount() - 1);
}
void MainFrame::OnClosePage(wxCommandEvent &)
{
    int currentPageIndex = m_tabpanel->GetPageIndex(m_tabpanel->GetCurrentPage());
    m_tabpanel->DeletePage(currentPageIndex);
}

void MainFrame::OnQuit(wxCommandEvent &)
{
    Close(true);
}

void MainFrame::ChangeTabNameOnSourceSelection(wxCommandEvent &evt)
{

    int currentPageIndex = m_tabpanel->GetPageIndex(m_tabpanel->GetCurrentPage());
    m_tabpanel->SetPageText(currentPageIndex, evt.GetString());
}