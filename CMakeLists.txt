cmake_minimum_required(VERSION 3.24)

project(rss_reader)

include(FetchContent)
include(ExternalProject)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


message("Fetching wxWidgets....")
set(wxBUILD_SHARED OFF)
set(wxUSE_WEBVIEW_EDGE ON)
FetchContent_Declare(
	wxWidgets
	GIT_REPOSITORY https://github.com/wxWidgets/wxWidgets.git
	GIT_SHALLOW ON
)
FetchContent_MakeAvailable(wxWidgets)

message("Fetching cpr....")
set(BUILD_SHARED_LIBS OFF)

FetchContent_Declare(
	cpr 
	GIT_REPOSITORY https://github.com/libcpr/cpr.git
	GIT_TAG 3b15fa82ea74739b574d705fea44959b58142eb8
)
FetchContent_MakeAvailable(cpr)

message("Fetching pugixml....")
FetchContent_Declare(
	pugixml
	GIT_REPOSITORY https://github.com/zeux/pugixml.git
	GIT_TAG db78afc2b7d8f043b4bc6b185635d949ea2ed2a8
)

FetchContent_MakeAvailable(pugixml)

message("Fetching nlohmann_json....")
FetchContent_Declare(
	nlohmann_json
	GIT_REPOSITORY https://github.com/nlohmann/json.git
	GIT_TAG master
)
FetchContent_MakeAvailable(nlohmann_json)

set(SRCS main.cpp MainFrame.h MainFrame.cpp RssReaderCtrl.h RssReaderCtrl.cpp ArticleDB.h ArticleDB.cpp FeedItem.h FeedItem.cpp FeedList.h FeedList.cpp Utils.h Utils.cpp ArticleWebView.h ArticleWebView.cpp SourceTree.h SourceTree.cpp Article.h Article.cpp RssSourceCtrl.h RssSourceCtrl.cpp RssParser.h RssParser.cpp SourceInstallDialog.h SourceInstallDialog.cpp)

if (APPLE)
	add_executable(rssreader MACOSX_BUNDLE ${SRCS})
	set_target_properties(rssreader PROPERTIES MACOSX_BUNDLE_INFO_PLIST ${CMAKE_CURRENT_SOURCE_DIR}/Info.plist)
else()
	add_executable(rssreader WIN32 ${SRCS} rssreader.exe.manifest)
endif()

target_link_libraries(rssreader PRIVATE wx::core wx::base wx::net wx::webview wx::aui cpr::cpr pugixml::pugixml nlohmann_json::nlohmann_json)
