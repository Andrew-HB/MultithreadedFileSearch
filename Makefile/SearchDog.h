#pragma once
#include <iostream>
#include <string>
#include <experimental/filesystem>
#include <future>
#include <mutex>

namespace fs = std::experimental::filesystem;

class SearchDog {
private:
	std::string m_RootDir;
	std::vector<std::string> m_SubDirs;
	std::vector<std::future<void>> m_Futures;

	const int m_ThreadLim = 8;
	bool m_IsFound;

	std::string GetFileName(const std::string& filePath); 
	void SearchFile(const std::string& currentPath, const std::string& fileName);
	void ClearFutures();

public:
	SearchDog(const std::string& rootDir);
	void Search(const std::string& fileName);
};
