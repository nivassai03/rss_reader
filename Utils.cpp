#include "Utils.h"
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/string.h>
#include <sstream>

std::string titleToSlug(const std::string &title, const char del = '-', const int len = 50)
{
	std::stringstream ss;
	int cnt{};
	for (auto it : title)
	{
		if (std::isalnum(static_cast<unsigned char>(it)))
		{
			ss.put(std::tolower(static_cast<unsigned char>(it)));
		}
		else if (std::isspace(static_cast<unsigned char>(it)))
		{
			ss.put(del);
		}
		if (cnt == len)
		{
			return ss.str();
		}
	}
	return ss.str();
}

std::string GenerateImageDir(const std::string &m_category, const std::string &m_name)
{
	std::stringstream ss;
	ss << '.';
	for (char c : m_name)
	{
		if (c != ' ')
		{
			ss.put(std::tolower(static_cast<unsigned char>(c)));
		}
		else
		{
			ss << "_";
		}
	}
	ss << "_";
	for (char c : m_category)
	{
		if (c != ' ')
		{
			ss.put(std::tolower(static_cast<unsigned char>(c)));
		}
		else
		{
			ss << "_";
		}
	}
	return ss.str();
}

std::string getDefaultImageName()
{
	return "default_thumbnail.jpg";
}
std::string getDefaultImagePath()
{
	wxFileName appDir = wxFileName::DirName(wxStandardPaths::Get().GetUserDataDir());
	wxString defaultImageName = getDefaultImageName();
	wxString defaultImagePath = appDir.GetFullPath() + defaultImageName;
	return defaultImagePath.ToStdString();
}