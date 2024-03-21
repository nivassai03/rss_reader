#pragma once
#include <wx/treectrl.h>
#include "RssSourceCtrl.h"

class SourceTree : public wxPanel
{
public:
    SourceTree(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size, const std::unordered_map<std::string, std::vector<RssSource>> &sources);
    void updateSources(const std::unordered_map<std::string, std::vector<RssSource>> &sources);
    void reconstructSourceTree();
    void expandTree();
    RssSource getRssSource(wxTreeItemId id);

private:
    std::string m_sourceFile;
    std::unordered_map<std::string, std::vector<RssSource>> m_sources;
    wxTreeCtrl *m_treeCtrl;
    void ConstructSourceTree();
    void OnTreeSelChanging(wxTreeEvent &);
    bool IsRoot(wxTreeItemId id);
    void OnSourceSelected(wxTreeEvent &);
    void UnselectOnClickInEmptyArea(wxMouseEvent &);
};