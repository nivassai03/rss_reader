#include "SourceInstallDialog.h"
#include <wx/busyinfo.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/log.h>
#include <wx/stattext.h>
#include <wx/bmpbuttn.h>
#include <wx/artprov.h>
#include "RssParser.h"

SourceInstallDialog::SourceInstallDialog(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &pos, const wxSize &size, ArticleDB &articles)
    : wxDialog(parent, id, title, pos, size),
      m_sourceCtrl(RssSourceCtrl()), m_articles(articles)
{
    // Main Panel
    m_panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize);

    // close button
    wxButton *closeButton = new wxButton(m_panel, wxID_ANY, "Close");
    closeButton->SetBitmap(wxArtProvider::GetBitmapBundle(wxART_CLOSE, wxART_BUTTON));
    closeButton->Bind(wxEVT_BUTTON, &SourceInstallDialog::OnClose, this);

    // main sizer
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    m_tabPanel = new wxNotebook(m_panel, wxID_ANY, wxDefaultPosition, wxDefaultSize);

    // Installed Panel
    wxBoxSizer *inSizer = new wxBoxSizer(wxVERTICAL);
    installedPanel = new wxPanel(m_tabPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    m_installedList = new wxTreeListCtrl(installedPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTL_MULTIPLE | wxTL_CHECKBOX | wxTL_NO_HEADER);
    inSizer->Add(m_installedList, 1, wxEXPAND);
    installedPanel->SetSizerAndFit(inSizer);

    wxButton *inbtnUnselectAll = new wxButton(installedPanel, static_cast<int>(SourceInstallID::UNSELECT_ALL_INS), "Unselect All");
    wxButton *inbtnSelectAll = new wxButton(installedPanel, static_cast<int>(SourceInstallID::SELECT_ALL_INS), "Select All");
    wxButton *btnUninstall = new wxButton(installedPanel, wxID_ANY, "Uninstall");

    // Create a sizer for the buttons on the left
    wxBoxSizer *inBtnSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *inlftSizer = new wxBoxSizer(wxHORIZONTAL);
    inlftSizer->Add(inbtnUnselectAll, 0, wxALIGN_LEFT | wxALL, 10);
    inlftSizer->Add(inbtnSelectAll, 0, wxALIGN_LEFT | wxTOP | wxBOTTOM | wxRIGHT, 10);
    inBtnSizer->Add(inlftSizer, 1, wxEXPAND);
    inBtnSizer->AddSpacer(this->GetSize().GetWidth() - 3 * (btnUninstall->GetSize().GetWidth()) - 100);
    inBtnSizer->Add(btnUninstall, 0, wxALL, 10);
    inSizer->Add(inBtnSizer, 0);
    installedPanel->SetSizerAndFit(inSizer);

    // Available Panel
    wxBoxSizer *avSizer = new wxBoxSizer(wxVERTICAL);
    availablePanel = new wxPanel(m_tabPanel, wxID_ANY, wxDefaultPosition, wxDefaultSize);
    m_availableList = new wxTreeListCtrl(availablePanel, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTL_MULTIPLE | wxTL_CHECKBOX | wxTL_NO_HEADER);
    avSizer->Add(m_availableList, 1, wxEXPAND);

    // remove,select all , unselectall buttons
    wxButton *btnUnselectAll = new wxButton(availablePanel, static_cast<int>(SourceInstallID::UNSELECT_ALL_AV), "Unselect All");
    wxButton *btnSelectAll = new wxButton(availablePanel, static_cast<int>(SourceInstallID::SELECT_ALL_AV), "Select All");
    wxButton *btnInstall = new wxButton(availablePanel, wxID_ANY, "Install");

    // Create a sizer for the buttons on the left
    wxBoxSizer *avBtnSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *lftSizer = new wxBoxSizer(wxHORIZONTAL);
    lftSizer->Add(btnUnselectAll, 0, wxALIGN_LEFT | wxALL, 10);
    lftSizer->Add(btnSelectAll, 0, wxALIGN_LEFT | wxTOP | wxBOTTOM | wxRIGHT, 10);
    avBtnSizer->Add(lftSizer, 1, wxEXPAND);
    avBtnSizer->AddSpacer(this->GetSize().GetWidth() - 3 * (btnInstall->GetSize().GetWidth()) - 100);
    avBtnSizer->Add(btnInstall, 0, wxALL, 10);
    avSizer->Add(avBtnSizer, 0);
    availablePanel->SetSizerAndFit(avSizer);

    sizer->Add(m_tabPanel, 1, wxEXPAND | wxALL, 20);
    sizer->Add(closeButton, 0, wxALIGN_RIGHT | wxALL, 10);
    m_panel->SetSizerAndFit(sizer);

    m_tabPanel->AddPage(installedPanel, "Installed");
    m_tabPanel->AddPage(availablePanel, "Available");
    makeAvailableList();
    makeInstalledList();
    m_availableList->Bind(wxEVT_TREELIST_ITEM_CHECKED, &SourceInstallDialog::selectAllChildSources, this);
    m_installedList->Bind(wxEVT_TREELIST_ITEM_CHECKED, &SourceInstallDialog::selectAllChildSources, this);
    inbtnUnselectAll->Bind(wxEVT_BUTTON, &SourceInstallDialog::OnUnSelectAll, this, static_cast<int>(SourceInstallID::UNSELECT_ALL_INS));
    btnUnselectAll->Bind(wxEVT_BUTTON, &SourceInstallDialog::OnUnSelectAll, this, static_cast<int>(SourceInstallID::UNSELECT_ALL_AV));
    inbtnSelectAll->Bind(wxEVT_BUTTON, &SourceInstallDialog::OnSelectAll, this, static_cast<int>(SourceInstallID::SELECT_ALL_INS));
    btnSelectAll->Bind(wxEVT_BUTTON, &SourceInstallDialog::OnSelectAll, this, static_cast<int>(SourceInstallID::SELECT_ALL_AV));
    btnInstall->Bind(wxEVT_BUTTON, &SourceInstallDialog::OnInstall, this);
    btnUninstall->Bind(wxEVT_BUTTON, &SourceInstallDialog::OnUninstall, this);
}

void SourceInstallDialog::makeInstalledList()
{

    m_installedList->AppendColumn("Sources");
    auto sources = m_sourceCtrl.fetchInstalledSources();
    for (const auto &it : sources)
    {
        wxTreeListItem categoryItem = m_installedList->AppendItem(m_installedList->GetRootItem(), it.first);
        for (const auto &source : it.second)
        {

            wxTreeListItem sourceItem = m_installedList->AppendItem(categoryItem, source.getName());
        }
    }
}

void SourceInstallDialog::makeAvailableList()
{
    m_availableList->AppendColumn("Sources");
    auto sources = m_sourceCtrl.fetchAvailableSources();
    for (const auto &it : sources)
    {
        wxTreeListItem categoryItem = m_availableList->AppendItem(m_availableList->GetRootItem(), it.first);
        for (const auto &source : it.second)
        {

            wxTreeListItem sourceItem = m_availableList->AppendItem(categoryItem, source.getName());
        }
    }
}
void SourceInstallDialog::selectAllChildSources(wxTreeListEvent &evt)
{
    wxTreeListItem currentItem = evt.GetItem();
    if (evt.GetId() == m_availableList->GetId())
    {
        if (m_availableList->GetItemParent(currentItem) == m_availableList->GetRootItem())
        {
            m_availableList->CheckItemRecursively(currentItem, m_availableList->GetCheckedState(currentItem));
        }
    }
    else
    {

        if (m_installedList->GetItemParent(currentItem) == m_installedList->GetRootItem())
        {
            m_installedList->CheckItemRecursively(currentItem, m_installedList->GetCheckedState(currentItem));
        }
    }
}

void SourceInstallDialog::OnClose(wxCommandEvent &evt)
{
    this->Close();
}

void SourceInstallDialog::OnSelectAll(wxCommandEvent &evt)
{
    if (evt.GetId() == static_cast<int>(SourceInstallID::SELECT_ALL_AV))
    {
        m_availableList->CheckItemRecursively(m_availableList->GetRootItem(), wxCHK_CHECKED);
        for (wxTreeListItem item = m_availableList->GetFirstItem(); item.IsOk(); item = m_availableList->GetNextItem(item))
        {
            m_availableList->Expand(item);
        }
    }
    else
    {
        m_installedList->CheckItemRecursively(m_installedList->GetRootItem(), wxCHK_CHECKED);
        for (wxTreeListItem item = m_installedList->GetFirstItem(); item.IsOk(); item = m_installedList->GetNextItem(item))
        {
            m_installedList->Expand(item);
        }
    }
}

void SourceInstallDialog::OnUnSelectAll(wxCommandEvent &evt)
{

    if (evt.GetId() == static_cast<int>(SourceInstallID::UNSELECT_ALL_AV))
    {
        m_availableList->CheckItemRecursively(m_availableList->GetRootItem(), wxCHK_UNCHECKED);
        for (wxTreeListItem item = m_availableList->GetFirstItem(); item.IsOk(); item = m_availableList->GetNextItem(item))
        {
            m_availableList->Expand(item);
        }
    }
    else
    {
        m_installedList->CheckItemRecursively(m_installedList->GetRootItem(), wxCHK_UNCHECKED);
        for (wxTreeListItem item = m_installedList->GetFirstItem(); item.IsOk(); item = m_installedList->GetNextItem(item))
        {
            m_installedList->Expand(item);
        }
    }
}
void SourceInstallDialog::OnInstall(wxCommandEvent &evt)
{
    std::vector<std::pair<std::string, std::string>> sources;
    for (wxTreeListItem item = m_availableList->GetFirstItem(); item.IsOk(); item = m_availableList->GetNextItem(item))
    {
        bool isHeading = m_availableList->GetItemParent(item) == m_availableList->GetRootItem() ? true : false;
        bool isChecked = m_availableList->GetCheckedState(item) == wxCHK_CHECKED ? true : false;
        if (isChecked && !isHeading)
        {
            wxString source = m_availableList->GetItemText(item);
            wxString category = m_availableList->GetItemText(m_availableList->GetItemParent(item));
            sources.push_back({source.ToStdString(), category.ToStdString()});
        }
    }
    auto installedSources = m_sourceCtrl.installSources(sources);
    wxWindowDisabler disableAll;
    wxBusyInfo info{
        wxBusyInfoFlags()
            .Parent(nullptr)
            .Icon(wxArtProvider::GetBitmapBundle(wxART_INFORMATION, wxART_OTHER, wxSize(128, 128)))
            .Title("<b>Please wait!</b>")
            .Text("Please wait downloading articles from new sources...")
            .Foreground(*wxWHITE)
            .Background(*wxBLACK)
            .Transparency(4 * wxALPHA_OPAQUE / 5)};
    wxYield();
    m_articles.UpdateWithNewArticles(installedSources);
    m_availableList->DeleteAllItems();
    m_installedList->DeleteAllItems();
    makeAvailableList();
    makeInstalledList();
}

void SourceInstallDialog::OnUninstall(wxCommandEvent &evt)
{
    std::vector<std::pair<std::string, std::string>> sources;
    for (wxTreeListItem item = m_installedList->GetFirstItem(); item.IsOk(); item = m_installedList->GetNextItem(item))
    {
        bool isHeading = m_installedList->GetItemParent(item) == m_installedList->GetRootItem() ? true : false;
        bool isChecked = m_installedList->GetCheckedState(item) == wxCHK_CHECKED ? true : false;
        if (isChecked && !isHeading)
        {
            wxString source = m_installedList->GetItemText(item);
            wxString category = m_installedList->GetItemText(m_installedList->GetItemParent(item));
            sources.push_back({source.ToStdString(), category.ToStdString()});
        }
    }
    m_sourceCtrl.uninstallSources(sources);
    m_availableList->DeleteAllItems();
    m_installedList->DeleteAllItems();
    makeAvailableList();
    makeInstalledList();
}
