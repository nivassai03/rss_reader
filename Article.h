#pragma once
#include <vector>
#include <string>
#include <wx/filename.h>

class Article
{
public:
	Article() = default;
	Article(const std::string &title, const std::string &description, const std::string &url, const std::string &imgUrl);
	Article(const Article &rhs);
	Article &operator=(const Article &rhs);
	void SetGUID(const std::string &guid);
	void SetImageNameAndPath(const std::string &imageName, const std::string &imagePath);
	void SetCategoryAndSource(const std::string &category, const std::string &source);
	void SetImageNameAndPath(const std::string &imageDirPath);
	void SetImageToDefaultImage();
	std::string GetTitle() const;
	std::string GetDescription() const;
	std::string GetImageUrl() const;
	std::string GetArticleUrl() const;
	std::pair<std::string, std::string> GetImageNameAndPath() const;

private:
	std::string m_title;
	std::string m_description;
	std::string m_url;
	std::string m_imgUrl;
	std::string m_guid;
	std::string m_imgName;
	std::string m_imgPath;
	std::string m_category;
	std::string m_source;
};

void FetchAndAddArticles(const std::vector<Article> &articles);
