#include "FeedItem.h"
#include "Utils.h"
#include <wx/stattext.h>
#include <wx/wrapsizer.h>
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/statbmp.h>
#include <wx/stdpaths.h>
#include <iostream>
#include <wx/image.h>
#include <wx/gbsizer.h>

wxDEFINE_EVENT(OPEN_ARTICLE_IN_WEBVIEW, wxCommandEvent);

MousePositionContent::MousePositionContent() : isMouseOnTitle(false), isMouseOnDescription(false), isMouseOnContent(false)
{
}

ContentPanel::ContentPanel(wxWindow *parent, wxWindowID id, wxString title, wxString description) : wxPanel(parent, id)
{

	wxBoxSizer *contentSizer = new wxBoxSizer(wxVERTICAL);
	m_titleText = new wxStaticText(this, wxID_ANY, title, wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE | wxELLIPSIZE_END);
	m_descriptionText = new wxStaticText(this, wxID_ANY, description, wxDefaultPosition, wxDefaultSize, wxST_NO_AUTORESIZE | wxELLIPSIZE_END);

	wxFont titleFont = m_titleText->GetFont();
	titleFont.MakeBold();
	titleFont.SetPointSize(13);
	m_titleText->SetFont(titleFont);

	// description
	wxFont description_font = m_descriptionText->GetFont();
	description_font.SetPointSize(11);
	m_descriptionText->SetFont(description_font);

	contentSizer->Add(m_titleText, 1, wxEXPAND | wxTOP, 2);
	contentSizer->AddSpacer(3);
	contentSizer->Add(m_descriptionText, 1, wxEXPAND | wxBOTTOM, 2);
	SetSizerAndFit(contentSizer);
	SetAutoLayout(true);
}

MousePositionFeedItem::MousePositionFeedItem() : isMouseOnContentPanel(false), isMouseOnBitmap(false), isMouseOnFeedItem(false), isMouseOnContentTitle(false), isMouseOnContentDescription(false)
{
}
FeedItem::FeedItem(wxWindow *_parent, const Article &article) : wxPanel(_parent, static_cast<int>(FeedItemID::FEED_ITEM))
{

	m_article = article;
	m_contentPanel = new ContentPanel(this, static_cast<int>(ContentID::CONTENT_PANEL), wxString::FromUTF8(article.GetTitle()), wxString::FromUTF8(article.GetDescription()));
	wxBoxSizer *sizer = new wxBoxSizer(wxHORIZONTAL);

	const auto &[imgName, imgPath] = article.GetImageNameAndPath();
	wxBitmap defaultThumbnail = wxBitmap(getDefaultImagePath(), wxBITMAP_TYPE_JPEG);
	wxStaticBitmap *bitmap;
	wxImage img;
	if (img.CanRead(imgPath) && img.LoadFile(imgPath, wxBITMAP_TYPE_ANY) && img.IsOk())
	{
		bitmap = new wxStaticBitmap(this, static_cast<int>(FeedItemID::BITMAP), img, wxDefaultPosition, wxSize(150, 100));
	}
	else
	{

		bitmap = new wxStaticBitmap(this, static_cast<int>(FeedItemID::BITMAP), defaultThumbnail, wxDefaultPosition, wxSize(150, 100));
	}
	sizer->Add(bitmap, 0);
	sizer->AddSpacer(10);
	sizer->Add(m_contentPanel, 1, wxEXPAND);

	SetSizerAndFit(sizer);
	this->SetAutoLayout(true);

	SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));

	// bind events to bitmap
	bitmap->Bind(wxEVT_LEFT_UP, &FeedItem::OnClick, this, static_cast<int>(FeedItemID::BITMAP));
	bitmap->Bind(wxEVT_ENTER_WINDOW, &FeedItem::OnMouseEnter, this, static_cast<int>(FeedItemID::BITMAP));
	bitmap->Bind(wxEVT_LEAVE_WINDOW, &FeedItem::OnMouseLeave, this, static_cast<int>(FeedItemID::BITMAP));

	// bind events to content panel
	m_contentPanel->Bind(wxEVT_LEFT_UP, &FeedItem::OnClick, this, static_cast<int>(ContentID::CONTENT_PANEL));
	m_contentPanel->Bind(wxEVT_ENTER_WINDOW, &FeedItem::OnMouseEnter, this, static_cast<int>(ContentID::CONTENT_PANEL));
	m_contentPanel->Bind(wxEVT_LEAVE_WINDOW, &FeedItem::OnMouseLeave, this, static_cast<int>(ContentID::CONTENT_PANEL));

	// bind events content panel entire children
	wxWindowList ContentPanelChildren = m_contentPanel->GetChildren();
	for (auto child : ContentPanelChildren)
	{
		child->Bind(wxEVT_LEFT_UP, &FeedItem::OnClick, this, child->GetId());
		child->Bind(wxEVT_ENTER_WINDOW, &FeedItem::OnMouseEnter, this, child->GetId());
		child->Bind(wxEVT_LEAVE_WINDOW, &FeedItem::OnMouseLeave, this, child->GetId());
	}

	// bind events to entire control
	Bind(wxEVT_LEFT_UP, &FeedItem::OnClick, this, static_cast<int>(FeedItemID::FEED_ITEM));
	Bind(wxEVT_ENTER_WINDOW, &FeedItem::OnMouseEnter, this, static_cast<int>(FeedItemID::FEED_ITEM));
	Bind(wxEVT_LEAVE_WINDOW, &FeedItem::OnMouseLeave, this, static_cast<int>(FeedItemID::FEED_ITEM));
}
void FeedItem::OnClick(wxMouseEvent &evt)
{
	wxCommandEvent openInWebview(OPEN_ARTICLE_IN_WEBVIEW, evt.GetId());
	openInWebview.SetString(m_article.GetArticleUrl());
	ProcessWindowEvent(openInWebview);
}

void FeedItem::OnMouseEnter(wxMouseEvent &evt)
{
	int eventID = evt.GetId();
	switch (eventID)
	{
	case static_cast<int>(FeedItemID::BITMAP):
		mousePos.isMouseOnBitmap = true;
		break;
	case static_cast<int>(ContentID::CONTENT_PANEL):
		mousePos.isMouseOnContentPanel = true;
		break;
	case static_cast<int>(ContentID::TITLE):
		mousePos.isMouseOnContentTitle = true;
		break;
	case static_cast<int>(ContentID::DESCRIPTION):
		mousePos.isMouseOnContentDescription = true;
		break;
	case static_cast<int>(FeedItemID::FEED_ITEM):
		mousePos.isMouseOnFeedItem = true;
		break;
	}
	if (mousePos.isMouseOnContentDescription || mousePos.isMouseOnContentTitle || mousePos.isMouseOnContentPanel || mousePos.isMouseOnBitmap || mousePos.isMouseOnFeedItem)
	{
		SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_HIGHLIGHT));
	}
	Refresh();
}

void FeedItem::OnMouseLeave(wxMouseEvent &evt)
{
	int eventID = evt.GetId();
	switch (eventID)
	{
	case static_cast<int>(FeedItemID::BITMAP):
		mousePos.isMouseOnBitmap = false;
		break;
	case static_cast<int>(ContentID::CONTENT_PANEL):
		mousePos.isMouseOnContentPanel = false;
		break;
	case static_cast<int>(ContentID::TITLE):
		mousePos.isMouseOnContentTitle = false;
		break;

	case static_cast<int>(ContentID::DESCRIPTION):
		mousePos.isMouseOnContentDescription = false;
		break;
	case static_cast<int>(FeedItemID::FEED_ITEM):
		mousePos.isMouseOnFeedItem = false;
		break;
	}
	if (mousePos.isMouseOnContentDescription == false && mousePos.isMouseOnContentPanel == false && mousePos.isMouseOnContentTitle == false && mousePos.isMouseOnBitmap == false && mousePos.isMouseOnFeedItem == false)
	{
		SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
	}
	Refresh();
}
