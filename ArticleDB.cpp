#include "ArticleDB.h"
#include "RssParser.h"
#include "RssSourceCtrl.h"

ArticleDB::ArticleDB() : m_parser(RssParser()), m_sourceCtrl(RssSourceCtrl()), m_articles(std::move(m_parser.FetchArticles(m_sourceCtrl.fetchInstalledSources()))) {}
ArticleDB::ArticleDB(std::string &&source, const std::string &imgDir) : m_parser(RssParser(std::move(source), imgDir)), m_sourceCtrl(RssSourceCtrl()) {}

void ArticleDB::InitInstalledSourceArticlesData()
{
    m_articles = std::move(m_parser.FetchArticles(m_sourceCtrl.fetchInstalledSources()));
}

std::vector<Article> ArticleDB::GetArticles(const std::string &category, const std::string &source) const
{
    auto sourceArticles = m_articles.at(category);
    return sourceArticles.at(source);
}

void ArticleDB::UpdateWithNewArticles(const std::vector<RssSource> &sources)
{
    auto newArticles = m_parser.FetchArticles(sources);
    for (const auto &[category, sourceArticles] : newArticles)
    {
        for (const auto &[source, articleList] : sourceArticles)
        {
            m_articles[category][source] = articleList;
        }
    }
}