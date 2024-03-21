#include "ArticleWebView.h"
#include <wx/sizer.h>
#include <wx/filename.h>
#include <wx/wfstream.h>
#include <wx/stdpaths.h>
#include <wx/file.h>
#include "FeedItem.h"

ArticleWebView::ArticleWebView(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size) : wxPanel(parent, id, pos, size)
{
	if (wxWebView::IsBackendAvailable(wxWebViewBackendEdge))
	{
		m_webview = wxWebView::New(this, id, "", pos, size, wxWebViewBackendEdge);
	}
	else
	{
		m_webview = wxWebView::New(this, id, "", pos, size, wxWebViewBackendDefault);
	}
	m_webview->EnableAccessToDevTools(false);
	m_webview->SetEditable(false);
	displayWelcomeMessage();
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	sizer->Add(m_webview, 1, wxEXPAND);
	Bind(OPEN_ARTICLE_IN_WEBVIEW, &ArticleWebView::loadArticle, this, id);
	SetSizerAndFit(sizer);
}

void ArticleWebView::displayWelcomeMessage()
{
	wxFileName appDir = wxFileName::DirName(wxStandardPaths::Get().GetUserDataDir());
	wxString welcomeFile = appDir.GetFullPath() + "welcome.html";
	wxFile welcomeHtmlFile(welcomeFile);
	wxString welcomeHtml = "";
	if (!welcomeHtmlFile.ReadAll(&welcomeHtml))
	{
		welcomeHtml = "";
	}
	m_webview->SetPage(welcomeHtml, welcomeFile);
}

void ArticleWebView::loadUrl(const wxString &url)
{
	m_url = url;
	m_webview->LoadURL(m_url);
}

void ArticleWebView::loadArticle(wxCommandEvent &evt)
{
	wxString url = evt.GetString();
	loadUrl(url);
}