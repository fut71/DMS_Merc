#pragma once
#include "IManager.h"
#include "Graphics/DearsGraphicsEngine.h"
#include "FileManager.h"

// �Ͼ�� �׷��Ƚ� ���� ���ҽ��Ŵ����� �����ϰ� �׽�Ʈ �غ� ��

class SoundManager;
class FileManager;

/// <summary>
/// ���� ������ ���ҽ����� �����ϴ� Ŭ�����Դϴ�.
/// 2024.9.16. [Riun] �Ŵ����� �����մϴ�.
/// ������ ���� �ڼ��� ���� �ڵ�� ���͵� ���� : https://www.notion.so/Resource-Manager-Game-Engine-1033e506dcbd804d8457eb51c46cafb8?pvs=4
/// </summary>
class ResourceManager
	: public IManager
{
public:
	ResourceManager(std::shared_ptr<DearsGraphicsEngine> _pGraphicsEngine, SoundManager* _pSoundManager);
	~ResourceManager() = default;

	virtual bool Initialize() override;/* { return true; }*/
	virtual void FixedUpdate(float _fixedDTime) override{}
	virtual void Update(float _dTime) override{}
	virtual void LateUpdate(float _dTime) override{}
	virtual void Finalize() override;

	// �׷��Ƚ� ������ �ش�Ǵ� ���ҽ� �߰�-------------------------------
	/// <summary> 
	/// �ش�Ǵ� ���� 1���� �߰��Ѵ�. 
	/// </summary>
	/// <param name="_fileName">���� �̸�</param>
	void AddResource(const std::string& _fileName);

	/// <summary> 
	/// Ư�� ���丮�� ���ϵ��� �߰��Ѵ�. 
	/// </summary>
	/// <param name="_fileName">���丮 �̸�</param>
	void AddFilesInDir(const std::string& _dirName);

	// ������ �����Ƽ� ���ϰڴ�. �׳� �ӽ÷� �߰��Ѵ�.
	void AddFilesInDir2D(const std::string& _dirName);
	void AddFilesInDir3D(const std::string& _dirName);
	void AddFilesInDirModel(const std::string& _dirName);
	void AddFilesInDirAni(const std::string& _dirName);
	void AddFilesInDirDDS(const std::string& _dirName);
	void AddFilesInDirBGMSound(const std::string& _dirName);
	void AddFilesInDirSFXSound(const std::string& _dirName);

	/// <summary> 
	/// Ư�� ���丮�� ��� ���� ���ϵ��� ���� �߰��Ѵ�. 
	/// </summary>
	/// <param name="_fileName">���丮 �̸�</param>
	void AddFilesInDirTree(const std::string& _dirName);

	/// <summary> 
	/// ��Ʈ�� ������ �߰��Ѵ�. 
	/// </summary>
	/// <param name="_fileName">���� �̸�</param> 
	/// <param name="_size">������</param> 
	/// <param name="_isKorean">true : �ѱ���, false : ���</param>
	void AddFont(const std::string& _fileName, float _size, bool _isKorean);

	/// <summary> ��Ʈ�� ��� �߰������� ���� �ϷḦ ȣ���� </summary>
	void FontSetFinish();

	// �׷��Ƚ� �������� ���ҽ��� �����´�. ------------------------------
	
	/// <summary> 
	/// ���� ������ ������ �´�. 
	/// </summary>
	/// <param name="_modelName">�� �̸�</param> 
	/// <returns>Model*</returns>
	Model* Get_ModelInfo(std::string _modelName);

	/// <summary> 
	/// �ִϸ��̼� ������ �����´�. 
	/// </summary>
	/// <param name="_animeName">�ִϸ��̼� �̸�</param> 
	/// <returns>Animation*</returns>
	Animation* Get_Animation(std::string _animeName);

	/// <summary> 
	/// �ؽ�ó�� ������ �´�. 
	/// </summary>
	/// <param name="_textureName">�ؽ�ó �̸�</param> 
	/// <returns> ComPtr&lt;ID3D11ShaderResourceView&gt;</returns>
	ComPtr<ID3D11ShaderResourceView> Get_Textures(std::string _textureName);

	/// <summary> 
	/// ��Ʈ�� ������ �´�. 
	/// </summary>
	/// <param name="_fontName">��Ʈ �̸�</param> <returns>ImFont*</returns>
	ImFont* Get_Font(std::string _fontName);

	/// <summary> 
	/// ���� Ư�� ���� �����´� -> ��� ������ �� ����Ѵ�. 
	/// </summary>
	/// <param name="_modelName">�� �̸�</param> 
	/// <param name="_boneName">�� �̸�</param> 
	/// <returns>���� �ε��� </returns>
	int Get_TargetModelBoneIndex(std::string _meshName, std::string _boneName);

	// ������ ������ ���� �Լ� -> ���� offsetMatrix�� ������ Matrix�� �޾ƿ´�.
	Matrix GetTargetBoneMatrix(std::string _targetModelName, std::string _targetBoneName);

	/// <summary>
	/// ���� ���� ��ġ�� �޾ƿ��� �Լ� -> ���� offsetMatrix�� �������� ���� ���� ��ġ���� �޾ƿ´� 
	/// </summary>
	/// <param name="_targetModel">�� �̸�</param> 
	/// <param name="_targetBoneName">�� �̸�</param> 
	/// <param name="_scale">������ = 1.0f</param>
	/// <returns>���� ���� ��ġ ��Ʈ����</returns>
	Matrix GetTargetBoneAboveMatrix(std::string _targetModel, std::string _targetBoneName, float _scale = 1.0f);

	/// <summary> 
	/// �浹 üũ�� ���� AABB�� �������� �Լ� 
	/// </summary>
	/// <param name="__targetModel">�� �̸�</param> 
	/// <returns>AABB</returns>
	AABB Get_AABB(std::string __targetModel);

	// �׷��Ƚ� �������� ���ҽ��� �����Ѵ�.(��Ʈ ���� ������) --------------
	/// <summary>
	///  �ش�Ǵ� ���� 1���� �����Ѵ�. 
	/// </summary>
	/// <param name="_fileName">���� �̸�</param>
	void RemoveResource(const std::string& _fileName);

	/// <summary>
	///  Ư�� ���丮�� ���ϵ��� �����Ѵ�. 
	/// </summary>
	/// <param name="_fileName">���丮 �̸�</param>
	void RemoveFilesInDir(const std::string& _dirName);

	/// <summary>
	///  Ư�� ���丮�� ��� ���� ���ϵ��� ���� �����Ѵ�. 
	/// </summary>
	/// <param name="_fileName">���丮 �̸�</param>
	void RemoveFilesInDirTree(const std::string& _dirName);
	
	// Csv �ļ� -------------------------------------------------------
	/// <summary> 
	/// ù ���� �����ϰ� �д� string ���� ���Ե� cvs ������ ���� ��� ���� �ļ� 
	/// </summary>
	/// <typeparam name="T">���̺� Info</typeparam> 
	/// <param name="_fileName">���� �̸�</param> 
	/// <returns>std::vector<T></returns>
	template <typename T>
	std::vector<T> ParseCSV(const std::string& _fileName);

	// �������� �� ��� ó���ϴ� �� �ٸ��� ������ ���ø����� ������ �ʾҴ�.
	/// <summary> 
	/// ù ���� �����ϰ� �д� ���ڰ�(����)�� ���Ե� cvs ������ ���� ��� ���� �ļ� 
	/// </summary>
	/// <param name="_fileName">���� �̸�</param> 
	/// <returns>std::vector<std::vector<int>></returns>
	std::vector<std::vector<int>> ParseMapCSV(const std::string& _fileName);

	// ���� �Ŵ��� -----------------------------------------------------
	/// <summary>
	/// ���� �Ŵ����� �����´�. 
	/// </summary>
	FileManager* GetFileManager();
private:
	// ��� �Լ� �����͸� ����ϱ� ���� using ����
	using AddFunctionType = void(DearsGraphicsEngine::*)(std::string, std::string);	
	using RemoveFuctionType = void(DearsGraphicsEngine::*)(std::string);

	std::shared_ptr<DearsGraphicsEngine> mpGraphicsEngine;	// �׷��Ƚ� ���� ��ü
	FileManager* mpFileManager;								// ���� �Ŵ���
	SoundManager* mpSoundManager;

	AddFunctionType FunctionType(const std::string& _basePath);			// �߰��Լ�
	RemoveFuctionType EraseFunctionType(const std::string& _basePath);	// �����Լ�
	bool mIsAlreadyFontLoaded; // �̹� ��Ʈ�� �߰��ƴ��� Ȯ���ؼ�, �߰� �ϷḦ ���� �÷���
};

template <typename T>
std::vector<T> ResourceManager::ParseCSV(const std::string& _fileName)
{
	std::vector<T> units = {};
	std::string filepath = mpFileManager->GetFullFilePath(_fileName);
	std::ifstream file(filepath);
	std::string line;

	// ���� ���� ���� �� �� ���� ��ȯ
	if (!file.is_open())
	{
		std::cerr << "Failed to open file: " << filepath << std::endl;
		return units;
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
	}

	// �� ��° �ٺ��� �о��ش�.
	while (std::getline(file, line))
	{
		std::stringstream ss(line);
		std::string cell;
		std::vector<std::string> row;
		while (std::getline(ss, cell, ','))
		{
			row.push_back(cell);
		}
		if (row.size() >= columnCount)
		{  // ��� Į���� �ִ��� Ȯ��
			units.emplace_back(row);
		}
		else
		{
			std::cerr << "Incomplete row found and skipped." << std::endl;
		}
	}
	return units;
}