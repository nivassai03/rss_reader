#include "Utils.h"
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