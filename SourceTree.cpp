#include "SourceTree.h"
#include <wx/log.h>
#include <unordered_map>
#include <vector>
#include <wx/sizer.h>

SourceTree::SourceTree(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, const std::unordered_map<std::string, std::vector<RssSource>> &sources) : wxPanel(parent, id, pos, size), m_sourceFile("sources.json"), m_sources(sources)
{
    m_treeCtrl = new wxTreeCtrl(this, wxID_ANY, pos, size, wxTR_HAS_BUTTONS | wxTR_HIDE_ROOT);
    wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
    ConstructSourceTree();
    m_treeCtrl->ExpandAll();
    sizer->Add(m_treeCtrl, 1, wxEXPAND);
    SetSizerAndFit(sizer);

    Bind(wxEVT_TREE_SEL_CHANGING, &SourceTree::OnTreeSelChanging, this);
    m_treeCtrl->Bind(wxEVT_LEFT_DOWN, &SourceTree::UnselectOnClickInEmptyArea, this);
    m_treeCtrl->Bind(wxEVT_TREE_ITEM_ACTIVATED, &SourceTree::OnSourceSelected, this);
}

void SourceTree::ConstructSourceTree()
{
    wxTreeItemId rootId = m_treeCtrl->AddRoot("root");
    for (auto it : m_sources)
    {
        wxFont root_font = this->GetFont();
        wxFont child_font = this->GetFont();
        root_font.SetPointSize(16);
        root_font.MakeBold();
        child_font.SetPointSize(12);
        wxTreeItemId id = m_treeCtrl->AppendItem(rootId, it.first);
        m_treeCtrl->SetItemFont(id, root_font);
        for (auto source : it.second)
        {
            wxTreeItemId child_id = m_treeCtrl->AppendItem(id, source.getName());
            m_treeCtrl->SetItemFont(child_id, child_font);
        }
    }
}

bool SourceTree::IsRoot(wxTreeItemId id)
{
    wxTreeItemId root_id = m_treeCtrl->GetRootItem();
    if (m_treeCtrl->GetItemParent(id) == root_id)
    {
        return true;
    }
    return false;
}

void SourceTree::OnTreeSelChanging(wxTreeEvent &evt)
{
    wxTreeItemId id = evt.GetItem();
    if (IsRoot(id))
    {
        evt.Veto();
    }
    else
    {
        evt.Skip();
    }
}
void SourceTree::OnSourceSelected(wxTreeEvent &evt)
{
    evt.Skip();
}

void SourceTree::UnselectOnClickInEmptyArea(wxMouseEvent &evt)
{
    wxPoint pos = evt.GetPosition();
    wxPoint clientPos = ScreenToClient(pos);
    wxTreeItemId id = m_treeCtrl->HitTest(clientPos);
    if (!id.IsOk())
    {
        m_treeCtrl->UnselectAll();
    }
    evt.Skip();
}
RssSource SourceTree::getRssSource(wxTreeItemId id)
{
    std::string categoryName = m_treeCtrl->GetItemText(m_treeCtrl->GetItemParent(id)).ToStdString();
    std::string sourceName = m_treeCtrl->GetItemText(id).ToStdString();
    for (auto source : m_sources[categoryName])
    {
        if (source.getName() == sourceName)
        {
            return source;
        }
    }
    return RssSource("empty", "empty", "empty");
}
void SourceTree::updateSources(const std::unordered_map<std::string, std::vector<RssSource>> &sources)
{
    m_sources = sources;
}

void SourceTree::reconstructSourceTree()
{
    m_treeCtrl->DeleteAllItems();
    ConstructSourceTree();
}

void SourceTree::expandTree()
{
    m_treeCtrl->ExpandAll();
}