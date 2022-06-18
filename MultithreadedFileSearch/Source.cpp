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

	std::mutex mtx;

	const int m_ThreadLim = 8;

	bool m_IsFound;

	std::string GetFileName(const std::string& filePath) {
		for (int i = filePath.size() - 1; i >= 0; i--) {
			if (filePath[i] == '\\') {
				std::string fileName = "";
				for (int j = i + 1; j < filePath.size(); j++) {
					fileName += filePath[j];
				}
				return fileName;
			}
		}
	}

	void SearchFile(const std::string& currentPath, const std::string& fileName) {
		if (m_IsFound)
			return;

		for (const auto& file : fs::directory_iterator(currentPath)) {
			if (fs::is_directory(file)) {
				SearchFile(file.path().string(), fileName);
			}
			else {
				if (GetFileName(file.path().string()) == fileName) {
					std::cout << file.path().string() << std::endl;
					m_IsFound = true;
				}
			}
		}
	}

	void ClearFutures() {
		for (int j = 0; j < m_Futures.size(); j++) {
			if (m_Futures[j].wait_for(std::chrono::milliseconds(0)) == std::future_status::ready) {
				m_Futures.erase(m_Futures.begin() + j);
			}
		}
	}

public:
	SearchDog(const std::string& rootDir)
		:m_RootDir(rootDir), m_IsFound(false)
	{
		for (const auto& file : fs::directory_iterator(m_RootDir)) {
			m_SubDirs.push_back(file.path().string());
		}
	}

	void Search(const std::string& fileName) {
		for (int i = 0; i < m_SubDirs.size() && !m_IsFound; i++) {
			if (!fs::is_directory(m_SubDirs[i])) {
				if (GetFileName(m_SubDirs[i]) == fileName) {
					std::cout << m_SubDirs[i] << std::endl;
					m_IsFound = true;
					break;
				}
			}
			while (true) {
				if (m_Futures.size() < m_ThreadLim) {
					m_Futures.push_back(std::async(std::launch::async, &SearchDog::SearchFile, this, m_SubDirs[i], fileName));
					break;
				}

				ClearFutures();
			}
		}

		while (m_Futures.size() > 0)
			ClearFutures();

		if (!m_IsFound) {
			std::cout << "File not found!" << std::endl;
		}
	}
};

int main() {
	SearchDog doggy("D:\\");

	doggy.Search("cube.obj");
}