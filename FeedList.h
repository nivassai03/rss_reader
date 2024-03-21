#pragma once
#include <wx/scrolwin.h>
#include <wx/filename.h>
#include "Article.h"

class FeedList : public wxScrolledWindow
{

private:
	wxFileName appDir;
	wxFileName imgDir;

public:
	FeedList(wxWindow *_parent, wxWindowID _id, const wxPoint &_pos, const wxSize &_size);
	void AddArticle(const Article &article);
	void LoadArticles(const std::vector<Article> &articles);
};
