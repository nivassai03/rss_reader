#include <wx/app.h>
#include <wx/splash.h>
#include "MainFrame.h"
#include <wx/stdpaths.h>
#include <wx/busyinfo.h>
#include <wx/artprov.h>

class MyApp : public wxApp
{

private:
	wxBitmap m_splashImg;
	bool OnInit() override;
	void InitApplicationData();
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
	MyApp::SetAppName("rss_reader");
	wxInitAllImageHandlers();
	InitApplicationData();
	MainFrame *frame = new MainFrame("rss_reader", wxDefaultPosition, wxSize(800, 600));
	wxFileName rootDir = wxFileName::DirName(wxStandardPaths::Get().GetUserDataDir()).GetFullPath();
	wxString splash_path = rootDir.GetFullPath() + "splash.jpg";
	wxBitmap bitmap(splash_path, wxBITMAP_TYPE_PNG);
	wxSplashScreen *splash = new wxSplashScreen(m_splashImg,
												wxSPLASH_NO_TIMEOUT,
												0, NULL, wxID_ANY, wxDefaultPosition, wxDefaultSize,
												wxBORDER_SIMPLE | wxSTAY_ON_TOP);
	wxYield();
	frame->InitAllArticles();
	splash->Close();
	splash->Destroy();
	frame->Show();
	return true;
}
void MyApp::InitApplicationData()
{

	wxString assetsDir = wxFileName::DirName(wxFileName(wxFileName::GetCwd(), "assets").GetFullPath()).GetFullPath();
	wxString default_thumbnail_path = assetsDir + "default_thumbnail.jpg";
	wxString welcome_msg_path = assetsDir + "welcome.html";
	wxString default_sourcefile_path = assetsDir + "sources.json";
	wxString splash_image_path = assetsDir + "splash.jpg";
	wxFileName rootDir = wxFileName::DirName(wxStandardPaths::Get().GetUserDataDir()).GetFullPath();
	wxString rootDirPath = rootDir.GetFullPath();
	if (!wxDirExists(rootDirPath))
	{
		rootDir.Mkdir(wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
	}
	wxString dest_thumbnail_path = rootDirPath + "default_thumbnail.jpg";
	wxString dest_welcome_msg_path = rootDirPath + "welcome.html";
	wxString dest_sourcefile_path = rootDirPath + "sources.json";
	wxString dest_splash_path = rootDirPath + "splash.jpg";

	if (!wxFileExists(dest_thumbnail_path))
		wxCopyFile(default_thumbnail_path, dest_thumbnail_path);
	if (!wxFileExists(dest_welcome_msg_path))
		wxCopyFile(welcome_msg_path, dest_welcome_msg_path);
	if (!wxFileExists(dest_sourcefile_path))
		wxCopyFile(default_sourcefile_path, dest_sourcefile_path);
	if (!wxFileExists(dest_splash_path))
		wxCopyFile(splash_image_path, dest_splash_path);
	m_splashImg.LoadFile(dest_splash_path, wxBITMAP_TYPE_JPEG);
}