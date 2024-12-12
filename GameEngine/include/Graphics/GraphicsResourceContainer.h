#pragma once
#include <wrl/client.h>
#include <string>
#include <d3d11.h>
#include <unordered_map>
#include <map>
#include <imgui.h>
#include "ModelBuffer.h"
#include "ModelInfo.h"
#include "BufferData.h"
#include "ObjectPool.h"
#include "BufferEnum.h"
#include "Texture.h"
#include "UIEnum.h"

#ifdef _DEBUG
#include <iostream>
#define DEBUG_LOG(message) std::cout<<message<<std::endl
#endif // DEBUG

#define BASIC_SIZE 10

using Microsoft::WRL::ComPtr;

///�׷��Ƚ� ���ҽ��� ������ �����̳�***
///���ҽ��� ����, ������ �Ѵ�.
class GraphicsResourceContainer
{
public:
	GraphicsResourceContainer();
	~GraphicsResourceContainer();

	//Constant Buffer -> ���� �ÿ� ���������� ���ȴ�.
private:
	//[�𵨸�, struct{VertexBuffer, IndexCount}], ���ؽ� ���ۿ� �ε��� ���۴� �ϳ��� �����ᵵ �ȴ�.
	std::unordered_map<std::string, ComPtr<ID3D11Buffer>> mVertexBuffers;

	//[�𵨸�, struct{IndexBuffer, IndexCount}], ���ؽ� ���ۿ� �ε��� ���۴� �ϳ��� �����ᵵ �ȴ�.
	std::unordered_map<std::string, ComPtr<ID3D11Buffer>> mIndexBuffers;
	std::unordered_map<std::string, unsigned int> mNumIndex;

	//[�𵨸�, node*], ��� ����� ���� �ʿ��ϴ�
	std::unordered_map<std::string, Model*> mModels;

	//[�ִϸ��̼� �̸�, Animation*]
	std::unordered_map<std::string, Animation*> mModelAnimations;

	//[�ؽ��� �̸�, ID3D11ShaderResourceView]
	std::unordered_map<std::string, ComPtr<ID3D11ShaderResourceView>> mTextures;

	//[Font �̸�, ImFont*] ��Ʈ ����
	std::unordered_map<std::string, ImFont*> mFont;

	//[�𵨸�, ���̸� �迭]
	std::unordered_map<std::string, std::vector<std::string>> mModelBoneName;

public:
	//���ҽ� ���� �߰��Ѵ�.
	void Add_VertexBuffer(std::string _modelName, ComPtr<ID3D11Buffer>_vertexBuffers);
	void Add_IndexBuffer(std::string _modelName, ComPtr<ID3D11Buffer>_indexBuffers);
	void Add_NumIndex(std::string _modelName, unsigned int _numIndex);
	void Add_ModelInfo(std::string _modelName, Model* _rootNode);
	void Add_BoneName(std::string _modelName, std::string _boneName);
	void Add_Animation(std::string _animName, Animation* _animation);
	void Add_Textures(std::string _textureName, ComPtr<ID3D11ShaderResourceView> _shaderResourceView);
	void Add_Font(std::string _fontName, ImFont* _font);

	//Ư�� ���ҽ��� �����.
	void Erase_VertexBuffer(std::string _modelName);
	void Erase_IndexBuffer(std::string _modelName);
	void Erase_NumIndex(std::string _modelName);
	void Erase_ModelInfo(std::string _modelName);
	void Erase_BoneNameVec(std::string _modelName);
	void Erase_Animation(std::string _animName);
	void Erase_Textures(std::string _textureName);
	void Erase_Font(std::string _fontName);

	//Ư�� ���ҽ��� ������ ��������.
	ComPtr<ID3D11Buffer> Get_VertexBuffer(std::string _modelName);
	ComPtr<ID3D11Buffer> Get_IndexBuffer(std::string _modelName);
	unsigned int Get_NumIndex(std::string _modelName);
	Model* Get_ModelInfo(std::string _modelName);
	int Get_ModelBoneIndex(std::string _modelName, std::string boneName);
	Animation* Get_Animation(std::string _animName);
	ComPtr<ID3D11ShaderResourceView> Get_Textures(std::string _textureName);
	ImFont* Get_Font(std::string _fontName);
	Matrix Get_TargetBoneMatrix(std::string _modelName, std::string _boneName);
	Matrix Get_TargetBoneAboveMatrix(std::string _modelName, std::string _boneName);
	Matrix Get_TargetBoneAboveMatrix(std::string _modelName, int _index);

private:
	Matrix SearchBoneMatrix(std::string _modelName, std::string _boneName);
	Matrix SearchBoneAboveMatrix(std::string _modelName, std::string _boneName);
	inline Matrix SearchBoneAboveMatrix(std::string _modelName, int _index);
private:
	template<typename Resource, typename Key>
	bool CheckResource(const Key& _key, const Resource& _resource);
};


template<typename Resource, typename Key>
bool GraphicsResourceContainer::CheckResource(const Key& _key, const Resource& _resource)
{
	const auto& iter = _resource.find(_key);
	//�̹� �ִ� ���� ���
	if (iter != _resource.end())
	{
		return true;
	}
	//���� �������� ���� ���
	else
	{
		return false;
	}
}
