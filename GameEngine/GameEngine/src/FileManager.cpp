#include "FileManager.h"

FileManager::FileManager()
{
}

FileManager::~FileManager()
{
}

void FileManager::Initialize()
{
	// ���� ��� ������
	std::filesystem::path mCurrentPath = std::filesystem::current_path();

	// ���ҽ� ��ġ�� �̵��ϱ�
	std::filesystem::path targetDir = mCurrentPath.parent_path().parent_path() / "Resources";	///������
	mResourceBaseDir = std::filesystem::relative(targetDir, mCurrentPath);						// �����

	// ���丮 ������ ����ϴ� �Լ�
	//printFolderStructure(m_resourceBaseDir, 0);
	// ��Ʈ ���丮 ����ü ����
	mRootDirData = new DirData{ mResourceBaseDir.string(), {}, {} };

	// ���丮 �� ���� ���� ����
	SaveFolderStructure(mResourceBaseDir, mRootDirData);
}

void FileManager::Finalize()
{
	// Ʈ�� ������ Ž���ϸ� �������� �Ҵ�� �޸� ����
	std::list<DirData*> toDeleteList; // �����ؾ��� ���丮 ��� ���� ����Ʈ ����
	toDeleteList.push_back(mRootDirData); // mRootDirData�� ����
	while (!toDeleteList.empty())
	{
		DirData* current = toDeleteList.front(); // ���� ����� ù ��° ���丮�� �����´�.
		toDeleteList.pop_front(); // ��� ������ ���丮 ����

		for (auto subDir : current->subDirectories)
		{
			toDeleteList.push_back(subDir);  // ���� ���丮�� ���� ��Ͽ� �߰�
		}
		delete current;  // ���� ���丮 ����
	}

}

FileManager::DirData* FileManager::GetRootDirData()
{
	return mRootDirData;
}

FileManager::DirData* FileManager::GetDirData(const std::string& _dirName)
{
	return SearchDirName(mRootDirData, _dirName);
}

FileManager::DirData* FileManager::GetDirDataFromFile(const std::string& _fileName)
{
	return SearchDirDataFromFile(mRootDirData, _fileName);
}

std::string FileManager::GetDirName(const std::string& _fileName)
{
	return SearchDirDataFromFile(mRootDirData, _fileName)->dirName;
}

std::string FileManager::SearchBasePath(DirData* _currentDir, const std::string& _fileName, const std::string& _parentPath)
{
	// ���� ���丮 ��θ� parentPath�� ����
	std::string currentPath = mResourceBaseDir.string();
	if (currentPath == _parentPath)
	{
		currentPath = currentPath + "\\";
	}
	else
	{
		currentPath = _parentPath + _currentDir->dirName + "\\";
	}

	// ���� ���丮���� ������ �˻�
	for (const auto& file : _currentDir->pInDirFiles)
	{
		if (file == _fileName)
		{
			return currentPath; // ���� ��� ��ȯ
		}
	}

	// ���� ���丮���� ��������� ������ Ž��
	for (auto& subDir : _currentDir->subDirectories)
	{
		std::string result = SearchBasePath(subDir, _fileName, currentPath);
		if (!result.empty())
		{
			return result; // ���� ���丮���� ������ ã���� ��ü basedPath ��ȯ
		}
	}
	return std::string(); // ������ ã�� ���� ��� �� ���ڿ� ��ȯ
}

std::string FileManager::GetBasePath(const std::string& _fileName)
{
	return SearchBasePath(mRootDirData, _fileName, mResourceBaseDir.string());
}

void FileManager::PrintDirStructure(DirData* _currentDir, int _depth /*= 0*/)
{
	for (int i = 0; i < _depth; i++)
	{
		std::cout << "    "; // �鿩����
	}
	std::cout << "[Dir] " << _currentDir->dirName << std::endl;

	for (auto& subDir : _currentDir->subDirectories)
	{
		PrintDirStructure(subDir, _depth + 1);
	}
}

void FileManager::PrintDirNFileStructure(DirData* _currentDir, int _depth /*= 0*/)
{
	for (int i = 0; i < _depth; i++)
	{
		std::cout << "    "; // �鿩����
	}
	std::cout << "[Dir] " << _currentDir->dirName << std::endl;

	for (const auto& file : _currentDir->pInDirFiles)
	{
		for (int i = 0; i < _depth; i++)
		{
			std::cout << "    " << file << std::endl; // ���� ��½� �� ��� �鿩����
		}
	}

	for (auto& subDir : _currentDir->subDirectories)
	{
		PrintDirNFileStructure(subDir, _depth + 1);
	}
}

void FileManager::SaveFolderStructure(const std::filesystem::path& _directory, DirData* _currentDir)
{
	for (const auto& entry : std::filesystem::directory_iterator(_directory))
	{
		if (entry.is_directory())
		{
			// ���ο� ���� ���丮�� �����ϰ�, ���� ���丮�� ���� ��Ͽ� �߰�
			DirData* newDir = new DirData{ entry.path().filename().string(), {}, {} };
			_currentDir->subDirectories.push_back(newDir);

			// ���� ���丮 ��� ȣ��
			SaveFolderStructure(entry.path(), newDir);
		}
		else if (entry.is_regular_file()) // ������ ���
		{
			// ������ ���� ���丮�� ���� ��Ͽ� �߰�
			_currentDir->pInDirFiles.push_back(entry.path().filename().string());
		}
	}
}

std::list<std::string> FileManager::GetAllFilePathList()
{
	std::list<std::string> filePathList;

	// Ʈ�� ������ ��ȸ�ϸ鼭 ��� ���� ��θ� ������
	SearchAllFilePaths(mRootDirData, mResourceBaseDir.string() + "\\", filePathList);

	return filePathList;
}

std::list<std::string> FileManager::GetOnlySubDirNameList(DirData* _dirData)
{
	std::list<std::string> dirNameList;

	for (auto& e : _dirData->subDirectories)
	{
		dirNameList.push_back(e->dirName);
	}
	return dirNameList;
}

std::list<std::string> FileManager::GetAllFilesInDirList(DirData* _dirData, bool _isOnlySubDir)
{
	std::list<std::string> allFilesInDirList;

	for (const auto& file : _dirData->pInDirFiles)
	{
		allFilesInDirList.push_back(file);
	}

	if (_isOnlySubDir == false)
	{
		SearchAllFilesInDir(_dirData, allFilesInDirList);
	}

	return allFilesInDirList;
}

void FileManager::PrintAll(bool mIsOnlyDir)
{
	if (mRootDirData)
	{
		if (mIsOnlyDir == true)
		{
			PrintDirStructure(mRootDirData); // ��Ʈ ���丮���� ��� ����
		}
		else
		{
			PrintDirNFileStructure(mRootDirData);
		}
	}
}

std::string FileManager::GetFullFilePath(const std::string& _fileName)
{
	// ��Ʈ ���丮���� �˻��� ����
	std::string dirPath = SearchBasePath(mRootDirData, _fileName, mResourceBaseDir.string());
	if (!dirPath.empty())
	{
		return dirPath + "//" + _fileName;  // ��ü ��ο� ���� �̸��� ���ļ� ��ȯ
	}
	return "";  // ������ ã�� ���� ��� �� ���ڿ� ��ȯ
}

std::string FileManager::ProcessFilePath(const std::filesystem::path& _filePath)
{
	std::cout << "File Path: " << _filePath << std::endl;
	return _filePath.string();
}

void FileManager::SearchAllFilePaths(DirData* _currentDir, const std::string& _parentPath, std::list<std::string>& _filePathList)
{
	// ���� ���丮 ��θ� ����
	std::string currentPath = _parentPath + _currentDir->dirName + "\\";

	// ���� ���丮 ���� ���� ��θ� ����Ʈ�� �߰�
	for (const auto& file : _currentDir->pInDirFiles)
	{
		_filePathList.push_back(currentPath + file);
	}

	// ���� �ٷ��丮���� ��������� ���� ��θ� �߰�
	for (auto& subDir : _currentDir->subDirectories)
	{
		SearchAllFilePaths(subDir, currentPath, _filePathList);
	}
}

FileManager::DirData* FileManager::SearchDirName(DirData* _currentDir, const std::string& _DirName)
{
	// ���� ���丮 �̸��� ã�� ���丮 �̸��� ���ٸ� ��ȯ
	if (_currentDir->dirName == _DirName)
	{
		return _currentDir;
	}

	// ���� ���丮�� ��ȸ�ϸ� ��������� Ž��
	for (auto& subDir : _currentDir->subDirectories)
	{
		// ���� ���丮���� ã�� ����� ����
		DirData* foundDir = SearchDirName(subDir, _DirName);

		// ã�Ҵٸ� �� ����� ��ȯ
		if (foundDir != nullptr)
		{
			return foundDir;
		}
	}

	// ã�� ���ϸ� nullptr ��ȯ
	return nullptr;
}

FileManager::DirData* FileManager::SearchDirDataFromFile(DirData* _currentDir, const std::string& _fileName)
{
	// ���� ������ ���� ���丮�� �ִ��� Ȯ���Ѵ�.
	for (auto& file : _currentDir->pInDirFiles)
	{
		if (file == _fileName)
		{
			return _currentDir;
		}
	}

	// ���� ���丮�� ��ȸ�ϸ� ��������� Ž��
	for (auto& subDir : _currentDir->subDirectories)
	{
		// ���� ���丮���� ã�� ����� ����
		DirData* foundDir = SearchDirName(subDir, _fileName);

		// ã�Ҵٸ� �� ����� ��ȯ
		if (foundDir != nullptr)
		{
			return foundDir;
		}
	}

	// ã�� ���ϸ� nullptr ��ȯ
	return nullptr;
}

void FileManager::SearchAllFilesInDir(DirData* _dirData, std::list<std::string>& _fileNameList)
{
	// ���� ���丮�� ��������� Ž���Ͽ� ������ �߰�
	for (auto& subDir : _dirData->subDirectories)
	{
		// ���� ���丮�� ������ ����Ʈ�� �߰�
		for (const auto& file : subDir->pInDirFiles)
		{
			_fileNameList.push_back(file);
		}

		// ��������� ���� ���丮 Ž��
		SearchAllFilesInDir(subDir, _fileNameList);
	}
}
