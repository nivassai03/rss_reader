#pragma once
#include "Article.h"
#include "RssSourceCtrl.h"
class RssParser
{
public:
	RssParser();
	RssParser(std::string &&, const std::string &);
	std::vector<Article> FetchArticlesData(const std::string &);
	std::unordered_map<std::string, std::unordered_map<std::string, std::vector<Article>>> FetchArticles(const std::unordered_map<std::string, std::vector<RssSource>> &);
	std::unordered_map<std::string, std::unordered_map<std::string, std::vector<Article>>> FetchArticles(const std::vector<RssSource> &sources);
	void DownloadAllArticleImages(const std::unordered_map<std::string, std::unordered_map<std::string, std::vector<Article>>> &);
	void FetchArticleImage(const Article &);
	void SetSourceAndImgDIr(const std::string &, const std::string &);

private:
	std::string m_source;
	std::string m_imageDirPath;
	std::string InitImageDir(const std::string &);
};