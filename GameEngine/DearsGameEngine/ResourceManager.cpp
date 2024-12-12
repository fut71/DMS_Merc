#include "ResourceManager.h"
#include "FileManager.h"
#include "SoundManager.h"

ResourceManager::ResourceManager(std::shared_ptr<DearsGraphicsEngine> _pGraphicsEngine, SoundManager* _pSoundManager)
{
	mpGraphicsEngine = _pGraphicsEngine;
	mpSoundManager = _pSoundManager;
	mpFileManager = nullptr;
	mIsAlreadyFontLoaded = false;
}

bool ResourceManager::Initialize()
{
	mpFileManager = new FileManager();
	mpFileManager->Initialize();
	return true;
}

void ResourceManager::Finalize()
{
	RemoveFilesInDirTree("Animation");
	RemoveFilesInDirTree("Csv");
	RemoveFilesInDirTree("Dummy");
	RemoveFilesInDirTree("Fbx");
// 	RemoveFilesInDirTree("Font");
	RemoveFilesInDirTree("Graphics");
	RemoveFilesInDirTree("Sound");
	RemoveFilesInDirTree("Texture");

	mpFileManager->Finalize();
	delete mpFileManager;
}

void ResourceManager::AddResource(const std::string& _fileName)
{
	std::string basePath = mpFileManager->GetBasePath(_fileName);

	AddFunctionType selectedFunction = FunctionType(basePath);

	(mpGraphicsEngine.get()->*selectedFunction)(basePath, _fileName);
}

void ResourceManager::AddFilesInDir(const std::string& _dirName)
{
	std::list<std::string>& inDirFileList = mpFileManager->GetDirData(_dirName)->pInDirFiles;

	if (inDirFileList.empty())
	{
		DLOG(LOG_INFO, "File is Empty.");
		return;
	}

	// ���丮 �� ù ��° ���� �̸��� ��θ� ������
	std::string fileName = inDirFileList.front();
	std::string basePath = mpFileManager->GetBasePath(fileName);

	// basePath�� ���� ������ �Լ� ������ ����
	AddFunctionType selectedFunction = FunctionType(basePath);

	// ���õ� �Լ��� ������ ���� ���丮�� ��� ���� ó��
	if (selectedFunction)
	{
		for (const auto& file : inDirFileList)
		{
			(mpGraphicsEngine.get()->*selectedFunction)(basePath, file);
		}
	}
}

void ResourceManager::AddFilesInDir2D(const std::string& _dirName)
{
	std::list<std::string>& inDirFileList = mpFileManager->GetDirData(_dirName)->pInDirFiles;

	if (inDirFileList.empty())
	{
		DLOG(LOG_INFO, "File is Empty.");
		return;
	}

	// ���丮 �� ù ��° ���� �̸��� ��θ� ������
	std::string fileName = inDirFileList.front();
	std::string basePath = mpFileManager->GetBasePath(fileName);


	// ���õ� �Լ��� ������ ���� ���丮�� ��� ���� ó��
	for (const auto& file : inDirFileList)
	{
		mpGraphicsEngine->Add2DTexture(basePath, file);
	}
}

void ResourceManager::AddFilesInDir3D(const std::string& _dirName)
{
	std::list<std::string>& inDirFileList = mpFileManager->GetDirData(_dirName)->pInDirFiles;

	if (inDirFileList.empty())
	{
		DLOG(LOG_INFO, "File is Empty.");
		return;
	}

	// ���丮 �� ù ��° ���� �̸��� ��θ� ������
	std::string fileName = inDirFileList.front();
	std::string basePath = mpFileManager->GetBasePath(fileName);


	// ���õ� �Լ��� ������ ���� ���丮�� ��� ���� ó��
	for (const auto& file : inDirFileList)
	{
		mpGraphicsEngine->Add3DTexture(basePath, file);
	}
}

void ResourceManager::AddFilesInDirModel(const std::string& _dirName)
{
	std::list<std::string>& inDirFileList = mpFileManager->GetDirData(_dirName)->pInDirFiles;

	if (inDirFileList.empty())
	{
		DLOG(LOG_INFO, "File is Empty.");
		return;
	}

	// ���丮 �� ù ��° ���� �̸��� ��θ� ������
	std::string fileName = inDirFileList.front();
	std::string basePath = mpFileManager->GetBasePath(fileName);


	// ���õ� �Լ��� ������ ���� ���丮�� ��� ���� ó��
	for (const auto& file : inDirFileList)
	{
		mpGraphicsEngine->AddModel(basePath, file);
	}
}

void ResourceManager::AddFilesInDirAni(const std::string& _dirName)
{
	std::list<std::string>& inDirFileList = mpFileManager->GetDirData(_dirName)->pInDirFiles;

	if (inDirFileList.empty())
	{
		DLOG(LOG_INFO, "File is Empty.");
		return;
	}

	// ���丮 �� ù ��° ���� �̸��� ��θ� ������
	std::string fileName = inDirFileList.front();
	std::string basePath = mpFileManager->GetBasePath(fileName);


	// ���õ� �Լ��� ������ ���� ���丮�� ��� ���� ó��
	for (const auto& file : inDirFileList)
	{
		mpGraphicsEngine->AddAnimation(basePath, file);
	}
}

void ResourceManager::AddFilesInDirDDS(const std::string& _dirName)
{
	std::list<std::string>& inDirFileList = mpFileManager->GetDirData(_dirName)->pInDirFiles;

	if (inDirFileList.empty())
	{
		DLOG(LOG_INFO, "File is Empty.");
		return;
	}

	// ���丮 �� ù ��° ���� �̸��� ��θ� ������
	std::string fileName = inDirFileList.front();
	std::string basePath = mpFileManager->GetBasePath(fileName);


	// ���õ� �Լ��� ������ ���� ���丮�� ��� ���� ó��
	for (const auto& file : inDirFileList)
	{
		mpGraphicsEngine->AddDDSTexture(basePath, file);
	}
}

void ResourceManager::AddFilesInDirBGMSound(const std::string& _dirName)
{
	std::list<std::string>& inDirFileList = mpFileManager->GetDirData(_dirName)->pInDirFiles;

	if (inDirFileList.empty())
	{
		DLOG(LOG_INFO, "File is Empty.");
		return;
	}

	// ���丮 �� ù ��° ���� �̸��� ��θ� ������
	std::string fileName = inDirFileList.front();
	std::string basePath = mpFileManager->GetBasePath(fileName);


	// ���õ� �Լ��� ������ ���� ���丮�� ��� ���� ó��
	for (const auto& file : inDirFileList)
	{
		std::string soundName = std::filesystem::path(file).stem().string();
		mpSoundManager->LoadBGM(basePath + file, soundName);
	}
}

void ResourceManager::AddFilesInDirSFXSound(const std::string& _dirName)
{
	std::list<std::string>& inDirFileList = mpFileManager->GetDirData(_dirName)->pInDirFiles;

	if (inDirFileList.empty())
	{
		DLOG(LOG_INFO, "File is Empty.");
		return;
	}

	// ���丮 �� ù ��° ���� �̸��� ��θ� ������
	std::string fileName = inDirFileList.front();
	std::string basePath = mpFileManager->GetBasePath(fileName);


	// ���õ� �Լ��� ������ ���� ���丮�� ��� ���� ó��
	for (const auto& file : inDirFileList)
	{
		std::string soundName = std::filesystem::path(file).stem().string();
		mpSoundManager->LoadSFX(basePath + file, soundName);
	}
}

void ResourceManager::AddFilesInDirTree(const std::string& _dirName)
{
	// DirData ����ü���� ���� ����Ʈ�� ���� ���丮 ������ ������
	auto pDirData = mpFileManager->GetDirData(_dirName);
	std::list<std::string>& inDirFileList = pDirData->pInDirFiles;
	auto* subDirs = &pDirData->subDirectories;

	// ������ �ִ� ���
	if (!inDirFileList.empty())
	{
		// ���丮 �� ù ��° ���� �̸��� ��θ� ������
		std::string fileName = inDirFileList.front();
		std::string basePath = mpFileManager->GetBasePath(fileName);

		// basePath�� ���� ������ �Լ� ������ ����
		AddFunctionType selectedFunction = FunctionType(basePath);

		// ���õ� �Լ��� ������ ���� ���丮�� ��� ���� ó��
		if (selectedFunction)
		{
			for (const auto& file : inDirFileList)
			{
				(mpGraphicsEngine.get()->*selectedFunction)(basePath, file);
			}
		}
	}

	// ���� ���丮�� ��������� ó��
	for (auto& subDir : *subDirs)
	{
		AddFilesInDirTree(subDir->dirName); // ��� ȣ��� ���� ���丮 ó��
	}
}

void ResourceManager::AddFont(const std::string& _fileName, float _size, bool _isKorean)
{
	std::string basePath = mpFileManager->GetBasePath(_fileName);

	mpGraphicsEngine->AddFont(basePath, _fileName, _size, _isKorean);
}

void ResourceManager::FontSetFinish()
{
	if (mIsAlreadyFontLoaded == false)
	{
		mpGraphicsEngine->FontSetFinish();
		mIsAlreadyFontLoaded = true;
	}
	else
	{
		std::cout << "�̹� ��Ʈ ������ ȣ���߽��ϴ�. �߰� ������ ���õ˴ϴ�." << std::endl;
	}
}

std::vector<std::vector<int>> ResourceManager::ParseMapCSV(const std::string& _fileName)
{
	std::vector<std::vector<int>> mMap = {};
	std::string filepath = mpFileManager->GetFullFilePath(_fileName);
	std::ifstream file(filepath);
	std::string line;

	// ���� ���� ���� �� �� ���� ��ȯ
	if (!file.is_open())
	{
		DLOG(LOG_INFO, "Failed to open file: " + filepath);
		return mMap;
	}

	int columnCount = 0;

	// ù ��° ��(���)�� �о���� -> Į�� ���� ī��Ʈ
	if (std::getline(file, line))
	{
		std::stringstream ss(line);
		std::string column;

		// ��ǥ�� ���е� �� �ʵ带 ���� Į�� ���� ī��Ʈ
		while (std::getline(ss, column, ','))
		{
			columnCount++;
		}

		// ù ��° ���� �����ͷ� ��ȯ�ؼ� mMap�� �߰�
		std::stringstream ssFirstRow(line);
		std::string cell;
		std::vector<int> row;  // ���⼭ string ��� T ���
		while (std::getline(ssFirstRow, cell, ','))
		{
			/*for (char c : line) { // BOM�� �� �� �ִ� �ڵ�
				std::cout << "Character: '" << c << "', ASCII: " << static_cast<int>(c) << std::endl;
			}*/

			// ó�� 3���ڰ� BOM�̶�� �ؼ� Ư���� ���� ����. �����غ��� �ߴµ� �� �Ǿ try catch�� �ߴ�.
			try
			{
				row.push_back(std::stoi(cell));  // ���ڿ��� ������ ��ȯ
			}
			catch (const std::invalid_argument& e)
			{
				DLOG(LOG_INFO, "Error: Failed to convert '" + cell + "' to integer. Skipping this value.");
			}
		}
		mMap.push_back(row);
	}

	// �� ��° �ٺ��� �ٽ� �о��ش�.
	while (std::getline(file, line))
	{
		std::stringstream ss(line);
		std::string cell;
		std::vector<int> row;
		while (std::getline(ss, cell, ','))
		{
			row.push_back(std::stoi(cell));  // ���ڿ��� ������ ��ȯ
		}

		// �� ���� �ùٸ� Į�� ������ ������ Ȯ��
		if (row.size() == columnCount)
		{  // ��� Į���� �ִ��� Ȯ��
			mMap.emplace_back(row);
		}
		else
		{
			DLOG(LOG_INFO, "Incomplete row found and skipped.");
		}
	}
	return mMap;
}

FileManager* ResourceManager::GetFileManager()
{
	return mpFileManager;
}

Model* ResourceManager::Get_ModelInfo(std::string _modelName)
{
	return mpGraphicsEngine->Get_ModelInfo(_modelName);
}

Animation* ResourceManager::Get_Animation(std::string _animeName)
{
	return mpGraphicsEngine->Get_Animation(_animeName);
}

ComPtr<ID3D11ShaderResourceView> ResourceManager::Get_Textures(std::string _textureName)
{
	return mpGraphicsEngine->Get_Textures(_textureName);
}

ImFont* ResourceManager::Get_Font(std::string _fontName)
{
	return mpGraphicsEngine->Get_Font(_fontName);
}

int ResourceManager::Get_TargetModelBoneIndex(std::string _meshName, std::string _boneName)
{
	return mpGraphicsEngine->Get_TargetModelBoneIndex(_meshName, _boneName);
}

Matrix ResourceManager::GetTargetBoneMatrix(std::string _targetModelName, std::string _targetBoneName)
{
	return mpGraphicsEngine->GetTargetBoneMatrix(_targetModelName, _targetBoneName);
}

Matrix ResourceManager::GetTargetBoneAboveMatrix(std::string _targetModelName, std::string _targetBoneName, float _scale /*= 1.f*/)
{
	return mpGraphicsEngine->GetTargetBoneAboveMatrix(_targetModelName, _targetBoneName, _scale);
}

AABB ResourceManager::Get_AABB(std::string __targetModelName)
{
	return mpGraphicsEngine->Get_AABB(__targetModelName);
}

void ResourceManager::RemoveResource(const std::string& _fileName)
{
	std::string basePath = mpFileManager->GetBasePath(_fileName);

	RemoveFuctionType selectedFunction = EraseFunctionType(basePath);

	(mpGraphicsEngine.get()->*selectedFunction)(_fileName);
}

void ResourceManager::RemoveFilesInDir(const std::string& _dirName)
{
	std::list<std::string>& inDirFileList = mpFileManager->GetDirData(_dirName)->pInDirFiles;

	if (inDirFileList.empty())
	{
		std::cout << "�� �����Դϴ�." << std::endl;
		return;
	}

	// ���丮 �� ù ��° ���� �̸��� ��θ� ������
	std::string fileName = inDirFileList.front();
	std::string basePath = mpFileManager->GetBasePath(fileName);

	// basePath�� ���� ������ �Լ� ������ ����
	RemoveFuctionType selectedFunction = EraseFunctionType(basePath);

	// ���õ� �Լ��� ������ ���� ���丮�� ��� ���� ó��
	if (selectedFunction)
	{
		for (const auto& file : inDirFileList)
		{
			(mpGraphicsEngine.get()->*selectedFunction)(file);
		}
	}
}

void ResourceManager::RemoveFilesInDirTree(const std::string& _dirName)
{
	// DirData ����ü���� ���� ����Ʈ�� ���� ���丮 ������ ������
	auto pDirData = mpFileManager->GetDirData(_dirName);
	std::list<std::string>& inDirFileList = pDirData->pInDirFiles;
	auto* subDirs = &pDirData->subDirectories;

	// ������ �ִ� ���
	if (!inDirFileList.empty())
	{
		// ���丮 �� ù ��° ���� �̸��� ��θ� ������
		std::string fileName = inDirFileList.front();
		std::string basePath = mpFileManager->GetBasePath(fileName);

		// basePath�� ���� ������ �Լ� ������ ����
		RemoveFuctionType selectedFunction = EraseFunctionType(basePath);

		// ���õ� �Լ��� ������ ���� ���丮�� ��� ���� ó��
		if (selectedFunction)
		{
			for (const auto& file : inDirFileList)
			{
				(mpGraphicsEngine.get()->*selectedFunction)(file);
			}
		}
	}

	// ���� ���丮�� ��������� ó��
	for (auto& subDir : *subDirs)
	{
		RemoveFilesInDirTree(subDir->dirName); // ��� ȣ��� ���� ���丮 ó��
	}
}

ResourceManager::AddFunctionType ResourceManager::FunctionType(const std::string& _basePath)
{
	// �Լ� ������ ���� �� �ʱ�ȭ
	AddFunctionType selectedFunction = nullptr;

	// basePath�� ���� ������ �Լ� ������ ����
	if (_basePath.find("\\Texture2D\\") != std::string::npos)
	{
		selectedFunction = &DearsGraphicsEngine::Add2DTexture;
	}
	else if (_basePath.find("\\Texture3D\\") != std::string::npos)
	{
		selectedFunction = &DearsGraphicsEngine::Add3DTexture;
	}
	else if (_basePath.find("\\Fbx\\") != std::string::npos)
	{
		selectedFunction = &DearsGraphicsEngine::AddModel;
	}
	else if (_basePath.find("\\Animation\\") != std::string::npos)
	{
		selectedFunction = &DearsGraphicsEngine::AddAnimation;
	}
	else if (_basePath.find("\\CubeMap\\") != std::string::npos)
	{
		selectedFunction = &DearsGraphicsEngine::AddDDSTexture;
	}

	return selectedFunction;
}

ResourceManager::RemoveFuctionType ResourceManager::EraseFunctionType(const std::string& _basePath)
{
	// �Լ� ������ ���� �� �ʱ�ȭ
	RemoveFuctionType selectedFunction = nullptr;

	// basePath�� ���� ������ �Լ� ������ ����
	if (_basePath.find("\\Texture\\") != std::string::npos)
	{
		selectedFunction = &DearsGraphicsEngine::Erase_Textures;
	}
	else if (_basePath.find("\\Fbx\\") != std::string::npos)
	{
		selectedFunction = &DearsGraphicsEngine::Erase_ModelInfo;
	}
	else if (_basePath.find("\\Animation\\") != std::string::npos)
	{
		selectedFunction = &DearsGraphicsEngine::Erase_Animation;
	}
	else if (_basePath.find("\\Font\\") != std::string::npos)
	{
		selectedFunction = &DearsGraphicsEngine::Erase_Font;
	}

	return selectedFunction;
}