#pragma once
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/event.h>
#include <wx/stattext.h>
#include "ArticleWebView.h"
#include "Article.h"

wxDECLARE_EVENT(OPEN_ARTICLE_IN_WEBVIEW, wxCommandEvent);

struct MousePositionContent
{
	bool isMouseOnTitle;
	bool isMouseOnDescription;
	bool isMouseOnContent;

public:
	MousePositionContent();
};

struct MousePositionFeedItem
{
	bool isMouseOnContentPanel;
	bool isMouseOnContentTitle;
	bool isMouseOnContentDescription;
	bool isMouseOnFeedItem;
	bool isMouseOnBitmap;

public:
	MousePositionFeedItem();
};

class ContentPanel : public wxPanel
{
private:
	wxStaticText *m_titleText;
	wxStaticText *m_descriptionText;
	MousePositionContent mousePos;

public:
	ContentPanel(wxWindow *parent, wxWindowID id, wxString title, wxString description);
};

enum class ContentID
{
	TITLE = wxID_HIGHEST + 10,
	DESCRIPTION,
	CONTENT_PANEL,
};
class FeedItem : public wxPanel
{

private:
	wxString title;
	wxString description;
	wxString guid;
	wxBitmap thumbnail;
	wxBitmap defaultThumbnail;
	ContentPanel *m_contentPanel;
	MousePositionFeedItem mousePos;
	Article m_article;

public:
	FeedItem(wxWindow *parent, const Article &article);
	void OnClick(wxMouseEvent &);
	void OnMouseEnter(wxMouseEvent &);
	void OnMouseLeave(wxMouseEvent &);
};

enum class FeedItemID
{
	BITMAP = wxID_HIGHEST + 20,
	FEED_ITEM,
};
