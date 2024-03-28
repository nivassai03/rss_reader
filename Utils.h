#pragma once
#include <string>

std::string titleToSlug(const std::string &title, const char del, const int len);
std::string GenerateImageDir(const std::string &category, const std::string &source);
std::string getDefaultImageName();
std::string getDefaultImagePath();