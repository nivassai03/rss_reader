#include "RssParser.h"
#include <cpr/cpr.h>
#include "pugixml.hpp"
#include <iostream>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/image.h>

RssParser::RssParser() : m_source(""), m_imageDirPath("") {}

RssParser::RssParser(std::string &&source, const std::string &imgDirName) : m_source(std::move(source))
{
	m_imageDirPath = InitImageDir(imgDirName);
}

std::string RssParser::InitImageDir(const std::string &imgDirName)
{

	wxFileName rootImageDir = wxFileName::DirName(wxFileName(wxStandardPaths::Get().GetUserDataDir(), ".thumbnails").GetFullPath());
	wxString sourceName = imgDirName;
	wxFileName currentSourceImageDir = wxFileName::DirName(wxFileName(rootImageDir.GetFullPath(), sourceName).GetFullPath());
	if (!wxDirExists(currentSourceImageDir.GetFullPath()))
	{
		currentSourceImageDir.Mkdir(wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);
	}
	return currentSourceImageDir.GetFullPath().ToStdString();
}

void RssParser::FetchArticleImage(Article &article)
{
	auto [imageName, imagePath] = article.GetImageNameAndPath();
	if (!wxFileExists(imagePath))
	{
		std::ofstream of(imagePath, std::ios::binary);
		cpr::Response response = cpr::Download(of, cpr::Url(article.GetImageUrl()));
	}
	wxImage img;
	if (img.CanRead(imagePath) && img.LoadFile(imagePath, wxBITMAP_TYPE_ANY) && img.IsOk())
	{
		img.Rescale(150, 100, wxIMAGE_QUALITY_BOX_AVERAGE);
		img.SaveFile(imagePath);
	}
	else
	{
		wxRemoveFile(imagePath);
		article.SetImageToDefaultImage();
	}
}

std::vector<Article> RssParser::FetchArticles(const std::string &feedUrl)
{

	cpr::Response response = cpr::Get(cpr::Url{feedUrl});
	std::vector<Article> articles;
	pugi::xml_document doc;
	pugi::xml_parse_result parse_result = doc.load_string(response.text.c_str());
	if (parse_result)
	{
		pugi::xml_node items = doc.child("rss").child("channel");
		for (pugi::xml_node item = items.child("item"); item; item = item.next_sibling("item"))
		{

			std::string title = item.child("title").child_value();
			std::string description = item.child("description").child_value();
			std::string link = item.child("link").child_value();
			std::string imageUrl = item.child("media:content").attribute("url").value();
			std::string guid = item.child("guid").child_value();
			Article article(title, description, link, imageUrl);
			article.SetGUID(guid);
			article.SetImageNameAndPath(m_imageDirPath);
			FetchArticleImage(article);
			articles.push_back(article);
		}
	}
	return articles;
}
void RssParser::SetSourceAndImgDIr(const std::string &source, const std::string &imgDir)
{
	m_source = source;
	m_imageDirPath = InitImageDir(imgDir);
}

void RssParser::FetchArticlesFromSource(const RssSource &source)
{
	m_source = source.getName();
	m_imageDirPath = InitImageDir(source.getImgDir());
	FetchArticles(source.getUrl());
}

void RssParser::FetchAllArticlesFromSources(const std::unordered_map<std::string, std::vector<RssSource>> &sources)
{
	std::vector<std::thread> threads;
	for (const auto &it : sources)
	{
		for (const auto &source : it.second)
		{
			threads.push_back(std::thread([=, this]
										  {
				// RssParser parser;
				// parser.SetSourceAndImgDIr(source.getName(),source.getImgDir());
				// std::vector<Article> articles = parser.FetchArticles(source.getUrl());
				FetchArticlesFromSource(source); }));
		}
		for (auto &thread : threads)
		{
			thread.join();
		}
		threads.clear();
	}
}