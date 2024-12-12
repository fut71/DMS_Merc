#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include "LayerEnum.h"
#include "FBXLoader.h"
#include "GraphicsResourceContainer.h"
#include "GeometryGenerator.h"	//�ڽ����鶧 �ʿ���.
#include "RendererHelper.h"

///���ҽ��� ����, ����, ������ ���þ��ϴ� Ŭ����
///FBXLoader, GraphicsResourceContainer�� ����ϰ� �����δ�.
class GraphicsResourceManager
{
public:
	GraphicsResourceManager(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pDeviceContext);
	~GraphicsResourceManager();

private:
	ComPtr<ID3D11Device> mpDevice; 
	ComPtr<ID3D11DeviceContext> mpDeviceContext;

	FBXLoader* mpFBXLoader;
	GraphicsResourceContainer* mpGraphicsResourceContainer;
public:
	void Initialize();
	void Update();
	void Finalize();

public:
	void AddModel(std::string _basePath, std::string _fileName);
	void AddModel(MeshData _meshData, std::string _meshName);
	void AddAnimation(std::string _basePath, std::string _fileName);
	void Add3DTexture(std::string _basePath, std::string _fileName);
	void Add2DTexture(std::string _basePath, std::string _fileName);
	void AddDDSTexture(std::string _basePath, std::string _fileName);
	void Add_Font(std::string fontName, ImFont* _font);

	//�����̳ʿ��� �ʿ��� ���ҽ��� ������ �´�.
	ComPtr<ID3D11Buffer> Get_VertexBuffer(std::string _modelName);
	ComPtr<ID3D11Buffer> Get_IndexBuffer(std::string _modelName);
	unsigned int Get_NumIndex(std::string _modelName);
	Model* Get_ModelInfo(std::string _modelName);
	Animation* Get_Animation(std::string _modelName);
	ComPtr<ID3D11ShaderResourceView> Get_Textures(std::string _modelName);
	ImFont* Get_Font(std::string _modelName);
	Matrix Get_TargetBoneMatrix(std::string _modelName, std::string _boneName);
	Matrix Get_TargetBoneAboveMatrix(std::string _modelName, std::string _boneName);
	Matrix Get_TargetBoneAboveMatrix(std::string _modelName, int _index);
	int Get_TargetModelBoneIndex(std::string _modelName, std::string _boneName);

	//�����̳ʿ��� Ư�� ���ҽ��� �����.
	void Erase_VertexBuffer(std::string _modelName);
	void Erase_IndexBuffer(std::string _modelName);
	void Erase_NumIndex(std::string _modelName);
	void Erase_ModelInfo(std::string _modelName);
	void Erase_Animation(std::string _animName);
	void Erase_Textures(std::string _modelName);
	void Erase_Font(std::string _modelName);



private:



};