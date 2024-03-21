#include "Article.h"
#include "Utils.h"
#include <wx/stdpaths.h>

Article::Article(const std::string &title, const std::string &description, const std::string &url, const std::string &imgUrl) : m_title(title), m_description(description), m_url(url), m_imgUrl{imgUrl}
{
}

Article::Article(const Article &rhs)
{
	m_title = rhs.m_title;
	m_description = rhs.m_description;
	m_url = rhs.m_url;
	m_imgUrl = rhs.m_imgUrl;
	m_guid = rhs.m_guid;
	m_imgName = rhs.m_imgName;
	m_imgPath = rhs.m_imgPath;
}

Article &Article::operator=(const Article &rhs)
{

	m_title = rhs.m_title;
	m_description = rhs.m_description;
	m_url = rhs.m_url;
	m_imgUrl = rhs.m_imgUrl;
	m_guid = rhs.m_guid;
	m_imgName = rhs.m_imgName;
	m_imgPath = rhs.m_imgPath;
	return *this;
}

void Article::SetGUID(const std::string &guid)
{
	m_guid = guid;
}

void Article::SetImageNameAndPath(const std::string &imageName, const std::string &imagePath)
{
	m_imgName = imageName;
	m_imgPath = imagePath;
}

void Article::SetCategoryAndSource(const std::string &category, const std::string &source)
{
	m_category = category;
	m_source = source;
}

std::string Article::GetTitle() const
{
	return m_title;
}

std::string Article::GetImageUrl() const
{
	return m_imgUrl;
}

std::string Article::GetDescription() const
{
	return m_description;
}

std::string Article::GetArticleUrl() const
{
	return m_url;
}

std::pair<std::string, std::string> Article::GetImageNameAndPath() const
{
	return {m_imgName, m_imgPath};
}
void Article::SetImageNameAndPath(const std::string &imageDirPath)
{
	std::string imageName = titleToSlug(m_title, '-', 50) + ".jpg";
	std::string imagePath = imageDirPath + imageName;
	SetImageNameAndPath(imageName, imagePath);
}

void Article::SetImageToDefaultImage()
{
	wxFileName appDir = wxFileName::DirName(wxStandardPaths::Get().GetUserDataDir());
	wxString defaultImageName = "default_thumbnail.jpg";
	wxString defaultImagePath = appDir.GetFullPath() + defaultImageName;
	m_imgName = defaultImageName;
	m_imgPath = defaultImagePath;
}
