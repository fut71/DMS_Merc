#pragma once
#include <filesystem>
#include <string>
#include <iostream>
#include <unordered_set>
#include <list>
#include <unordered_map>

/// <summary>
/// ���������� Ʈ�������� ����ϵ��� �ٲ��.
/// 2024.9.15.
/// </summary>
class FileManager
{
public:
	FileManager();
	~FileManager();

private:

	struct DirData
	{
		std::string dirName;					// ���丮 �̸�(= ���� �̸�)
		std::list<std::string> pInDirFiles;		// ���丮 �� ���� ����Ʈ
		std::list<DirData*> subDirectories;		// ���� ���丮
	};

public:
	/// <summary> �ʱ�ȭ : ���ҽ��� ��Ʈ �Լ��� ���ϰ�, ���丮 �� ���� ������ Ʈ���� �����Ѵ�. </summary>
	void Initialize();
	void Finalize();

public:
	/// <summary> ��Ʈ DirData�� �����´�. </summary>
	/// <returns>DirData*</returns>
	DirData* GetRootDirData();

	/// <summary> ���丮 �̸��� ���ؼ� DirData ������ �����´�. </summary>
	/// <param name="_dirName">���丮 �̸�</param> <returns>DirData*</returns>
	DirData* GetDirData(const std::string& _dirName);

	/// <summary> ���� �̸��� ���ؼ� DirData ������ �����´�. </summary>
	/// <param name="_fileName">���� �̸�</param> <returns>DirData*</returns>
	DirData* GetDirDataFromFile(const std::string& _fileName);

	/// <summary> ���� �̸��� ���ؼ� DirData �̸� �����´�. </summary>
	/// <param name="_fileName">���� �̸�</param> <returns>std::string</returns>
	std::string GetDirName(const std::string& _fileName);

	/// <summary> �ش� ������ BasePath�� �����´�. </summary>
	/// <param name="_fileName">���� �̸�</param> <returns>std::string</returns>
	std::string GetBasePath(const std::string& _fileName);

	/// <summary> ��� ������ ��ü ��θ� ���� ����Ʈ�� �����´�. </summary>
	/// <returns> std::list&lt;std::string&gt; </returns>
	std::list<std::string> GetAllFilePathList();

	/// <summary> ���� ���Ͽ� �����ϴ� ���丮�� �̸����� ����Ѵ�. </summary>
	/// <param name="_dirData">���丮 �������� ����ü</param> <returns>std::list&lt;std::string&gt;</returns>
	std::list<std::string> GetOnlySubDirNameList(DirData* _dirData);

	/// <summary> ���� ���丮�� �����ϴ� ������ �̸����� ����Ѵ�. </summary>
	/// <param name="_dirData">���丮 �������� ����ü</param> <param name="_isOnlyNowDir">true : �ش� ���丮�� ���Ե� ���� �̸��鸸 ������, false : �ش� ���丮���� ��� �������� ���� �̸����� ������</param>
	/// <returns>std::list&lt;std::string&gt;</returns>
	std::list<std::string> GetAllFilesInDirList(DirData* _dirData, bool _isOnlyNowDir = true);

	/// <summary> ���� ������ ����Ѵ�. </summary>
	/// <param name="mIsOnlyDir">true : ���丮�� ���, false : ���丮�� ���� ��ü ���</param>
	void PrintAll(bool mIsOnlyDir);

	/// <summary> ������ ��ü ��θ� ����Ѵ�. </summary>
	/// <param name="_fileName">���� �̸�</param> <returns>std::string</returns>
	std::string GetFullFilePath(const std::string& _fileName);

private:
	DirData* mRootDirData = nullptr;			// �ֻ��� ���丮
	std::filesystem::path mResourceBaseDir;		// ���ҽ� ��ġ

private:
	// ������ ��ü���� ���� �� ���丮 ������ Ʈ���� �����Ѵ�.
	void SaveFolderStructure(const std::filesystem::path& _directory, DirData* _currentDir);
	// ���� ó��
	std::string ProcessFilePath(const std::filesystem::path& _filePath);

	/// ����ȭ�� ���Ͽ� ����Լ��� ���� ��� �����ߴ�. (����Ʈ�� �ߺ� ���� ����)
	// ��ü ��θ� ã�� ����Լ�
	void SearchAllFilePaths(DirData* _currentDir, const std::string& _parentPath, std::list<std::string>& _filePathList);
	// ���丮�� �̸� ���� DirData* ������ ã�� ����Լ�
	DirData* SearchDirName(DirData* _currentDir, const std::string& _DirName);
	// ������ �̸� ���� DirData* ������ ã�� ����Լ�
	DirData* SearchDirDataFromFile(DirData* _currentDir, const std::string& _fileName);
	// Ư�� ���� ���� �����ϴ� ��� ������ ã�� ����Լ�
	void SearchAllFilesInDir(DirData* _dirData, std::list<std::string>& _fileNameList);

	// BasePath�� ��θ� �̾Ƴ���. ���� �̸��� �Է��ϸ� �˾Ƽ� \\���� �����..
	std::string SearchBasePath(DirData* currentDir, const std::string& fileName, const std::string& parentPath);

	// ���丮 ��ü ���� ���
	void PrintDirStructure(DirData* _currentDir, int _depth = 0);
	// ���丮�� �� �ȿ� ���Ե� ���� ��θ� ���
	void PrintDirNFileStructure(DirData* _currentDir, int _depth = 0);
};

// 2024.6.4.
// ���� �ý����� ���丮 ������ Ʈ�� ������ �ſ� �����ϴ�.
// �� ������ ���� �ý����� ���丮 ������ Ȱ���Ͽ� Ʈ�� ������ ���������� ����ϴ� ���� �����ϴ�.
// ���� unordered_map�� ����ؼ� ��Ī��ų ������ �״��� ����. �׳� ���͸��� ã�� �Լ��� �̿��� �̸��� ��Ī��Ű�� �� ũ�� ���� ����

// �ڷᱸ���� �׳� ����Ʈ�� �� ���� : ������ ������ �츮�� ���ƺ��� ��κ� 100�� ���϶�� �Ǵ��ߴ�.

// 2024.9.15.
// <filesystem>�� ������ ����ϰ� Ʈ���� ������ ¥��, �ʹݿ� ��� ������ Ʈ�� ������ �����ϰԲ� �ٲ��.
// �ʹݿ� �� ���̺귯������ �����ͼ� �� Ʈ�� ������ �����ϰ�, ������ ��� ���� ����� Ʈ�������� ���� Ž���Ѵ�.
// ��� ������ Ž������ �ʰ� ������ �˻��� �� �ְ� �Ǿ���.