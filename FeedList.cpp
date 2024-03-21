#include "FeedList.h"
#include "FeedItem.h"
#include <wx/sizer.h>
#include <wx/bitmap.h>
#include <ostream>
#include <cpr/cpr.h>
#include <wx/url.h>
#include "Utils.h"
#include <wx/stream.h>
#include <wx/stdpaths.h>

FeedList::FeedList(wxWindow *_parent, wxWindowID _id, const wxPoint &_pos, const wxSize &_size) : wxScrolledWindow(_parent, _id, _pos, _size, wxVSCROLL)
{

	appDir = wxFileName::DirName(wxStandardPaths::Get().GetUserDataDir());
	imgDir = wxFileName::DirName(wxFileName(wxStandardPaths::Get().GetUserDataDir(), ".thumbnails").GetFullPath());
	SetScrollbar(0, 20, 0, 50);
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	SetSizer(sizer);
	SetVirtualSize(wxSize(_size.x, 4000));
	SetScrollRate(1, 10);
}

void FeedList::AddArticle(const Article &article)
{
	FeedItem *feed_item = new FeedItem(this, article);
	GetSizer()->Add(feed_item, 0, wxEXPAND | wxALL, 5);
	Layout();
}

void FeedList::LoadArticles(const std::vector<Article> &articles)
{
	DestroyChildren();
	for (auto &article : articles)
	{
		AddArticle(article);
	}
}