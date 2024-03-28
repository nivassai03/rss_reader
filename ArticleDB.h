#pragma once
#include "Article.h"
#include <unordered_map>
#include "RssParser.h"
#include <string>
#include <vector>

class ArticleDB
{
public:
    ArticleDB();
    ArticleDB(std::string &&source, const std::string &imgDir);
    void InitInstalledSourceArticlesData();
    std::vector<Article> GetArticles(const std::string &, const std::string &) const;
    void UpdateWithNewArticles(const std::vector<RssSource> &sources);

private:
    RssSourceCtrl m_sourceCtrl;
    RssParser m_parser;
    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<Article>>> m_articles;
};
