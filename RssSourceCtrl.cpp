#include "RssSourceCtrl.h"
#include <sstream>
#include <fstream>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/log.h>

RssSource::RssSource(std::string category, std::string name, std::string feedurl) : m_category(category), m_name(name), m_feedUrl(feedurl), m_imgDir(std::move(GenerateImageDir()))
{
}

std::string RssSource::GenerateImageDir()
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

std::string RssSource::getCategory() const
{
    return m_category;
}
std::string RssSource::getName() const
{
    return m_name;
}
std::string RssSource::getUrl() const
{
    return m_feedUrl;
}
std::string RssSource::getImgDir() const
{
    return m_imgDir;
}

RssSourceCtrl::RssSourceCtrl() : m_sourceFileName("sources.json"), m_sourceFilePath(getDefaultSourceFilePath()) {}
RssSourceCtrl::RssSourceCtrl(std::string fileName, std::string filePath) : m_sourceFileName(fileName), m_sourceFilePath(filePath) {}

std::unordered_map<std::string, std::vector<RssSource>> RssSourceCtrl::fetchInstalledSources()
{

    std::ifstream sourceFile(m_sourceFilePath);
    nlohmann::json j;
    sourceFile >> j;
    std::unordered_map<std::string, std::vector<RssSource>> sources;
    for (const auto &[key, value] : j["Installed"].items())
    {
        if (!value.empty())
        {
            if (sources.find(key) == sources.end())
            {
                sources[key] = std::vector<RssSource>{};
            }
            for (const auto &item : value)
            {
                sources[key].emplace_back(key, item["source"], item["url"]);
            }
        }
    }
    return sources;
}

std::unordered_map<std::string, std::vector<RssSource>> RssSourceCtrl::fetchAvailableSources()
{

    std::ifstream sourceFile(m_sourceFilePath);
    nlohmann::json j;
    sourceFile >> j;
    std::unordered_map<std::string, std::vector<RssSource>> sources;
    for (const auto &[key, value] : j["Available"].items())
    {
        if (!value.empty())
        {
            if (sources.find(key) == sources.end())
            {
                sources[key] = std::vector<RssSource>{};
            }
            for (const auto &item : value)
            {
                sources[key].emplace_back(key, item["source"], item["url"]);
            }
        }
    }
    return sources;
}

std::string RssSourceCtrl::getDefaultSourceFilePath()
{
    wxFileName rootDir = wxFileName::DirName(wxStandardPaths::Get().GetUserDataDir());
    wxString sourceFilePath = rootDir.GetFullPath() + "sources.json";
    return sourceFilePath.ToStdString();
}

void RssSourceCtrl::installSources(const std::vector<std::pair<std::string, std::string>> &sources)
{
    // wxMessageOutputDebug dbg;
    // dbg.Printf("Installing Following sources...\n");
    // for (const auto &source : sources)
    // {
    //     dbg.Printf("Category:%s , Source:%s\n", source.first, source.second);
    // }
    std::ifstream sourceFile(m_sourceFilePath);
    nlohmann::json j;
    sourceFile >> j;
    sourceFile.close();
    for (const auto &source_pair : sources)
    {
        auto &availableList = j["Available"][source_pair.second];
        auto &installedList = j["Installed"][source_pair.second];
        for (auto it = availableList.begin(); it != availableList.end(); ++it)
        {
            if ((*it)["source"] == source_pair.first)
            {
                installedList.push_back(*it);
                availableList.erase(it);
                break;
            }
        }
    }
    saveSourceFile(j);
}

void RssSourceCtrl::uninstallSources(const std::vector<std::pair<std::string, std::string>> &sources)
{

    // wxMessageOutputDebug dbg;
    // dbg.Printf("Uninstalling Following sources...\n");
    // for (const auto &source : sources)
    // {
    //     dbg.Printf("Category:%s , Source:%s\n", source.first, source.second);
    // }
    std::ifstream sourceFile(m_sourceFilePath);
    nlohmann::json j;
    sourceFile >> j;
    sourceFile.close();
    for (const auto &source_pair : sources)
    {

        auto &installedList = j["Installed"][source_pair.second];
        auto &avialbleList = j["Available"][source_pair.second];
        for (auto it = installedList.begin(); it != installedList.end(); ++it)
        {
            if ((*it)["source"] == source_pair.first)
            {
                // dbg.Printf("Uninstalling source:%s....\n", source_pair.first);
                avialbleList.push_back(*it);
                installedList.erase(it);
                break;
            }
        }
    }
    saveSourceFile(j);
}

void RssSourceCtrl::saveSourceFile(const nlohmann::json &sources)
{
    std::ofstream outFile(m_sourceFilePath);
    outFile << sources.dump(4);
    outFile.close();
}