#pragma once
#include <wx/panel.h>
#include <wx/webview.h>

class ArticleWebView : public wxPanel
{
public:
	ArticleWebView(wxWindow *parent, wxWindowID id, const wxPoint &pos, const wxSize &size);
	void loadUrl(const wxString &url);
	void displayWelcomeMessage();
	void loadArticle(wxCommandEvent &);

private:
	wxWebView *m_webview;
	wxString m_url;
};
