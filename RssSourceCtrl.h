#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <nlohmann/json.hpp>

class RssSource
{
public:
    RssSource() = default;
    RssSource(std::string category, std::string name, std::string feedurl);
    std::string GenerateImageDir();
    std::string getCategory() const;
    std::string getName() const;
    std::string getUrl() const;
    std::string getImgDir() const;

private:
    std::string m_category;
    std::string m_name;
    std::string m_imgDir;
    std::string m_feedUrl;
};

class RssSourceCtrl
{
public:
    RssSourceCtrl();
    RssSourceCtrl(std::string fileName, std::string filePath);
    std::unordered_map<std::string, std::vector<RssSource>> fetchInstalledSources();
    std::unordered_map<std::string, std::vector<RssSource>> fetchAvailableSources();
    std::vector<RssSource> installSources(const std::vector<std::pair<std::string, std::string>> &sources);
    void uninstallSources(const std::vector<std::pair<std::string, std::string>> &sources);

private:
    std::string getDefaultSourceFilePath();
    void saveSourceFile(const nlohmann::json &sources);
    std::string m_sourceFileName;
    std::string m_sourceFilePath;
};