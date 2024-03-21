#pragma once
#include "Article.h"
#include "RssSourceCtrl.h"
class RssParser
{
public:
	RssParser();
	RssParser(std::string &&source, const std::string &imgDir);
	std::vector<Article> FetchArticles(const std::string &);
	void FetchAllArticlesFromSources(const std::unordered_map<std::string, std::vector<RssSource>> &);
	void FetchArticlesFromSource(const RssSource &);
	void FetchArticleImage(Article &);
	void SetSourceAndImgDIr(const std::string &source, const std::string &imgDir);

private:
	std::string m_source;
	std::string m_imageDirPath;
	std::string InitImageDir(const std::string &);
};