#pragma once
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include"ModelInfo.h"

///FBX�ε带 ������ Ŭ����.
///���� �Լ��� templateȭ ��Ű��.
using DirectX::SimpleMath::Matrix;

class FBXLoadHelper
{
public:
	/// <summary>
	/// assimp ���� �����͸� �����Ѵ�.
	/// </summary>
	/// <param name="_pScene"></param>
	/// <returns></returns>
	Model* CopyModeldata(const aiScene* _pScene);

	/// <summary>
	/// ���ؽ��� �����Ѵ�.
	/// </summary>
	/// <param name="_aiMesh"></param>
	/// <param name="_mesh"></param>
	void Copy_Vertex(const aiMesh* _aiMesh, Mesh* _mesh);

	/// <summary>
	/// �ε����� �����Ѵ�.
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="_aiMesh"></param>
	/// <param name="_mesh"></param>
	template <typename T>
	void Copy_Index(const aiMesh* _aiMesh, T* _mesh);

	/// <summary>
	/// �ؽ��ĸ� �����Ѵ�.
	/// </summary>
	/// <param name="_aimesh"></param>
	/// <param name="_pScene"></param>
	/// <param name="_texturename"></param>
	void Copy_Texture(const aiMesh* _aimesh, const aiScene* _pScene, std::string*& _texturename);

	/// <summary>
	/// ���� �����Ѵ�.
	/// </summary>
	/// <param name="_aiMesh"></param>
	/// <param name="_bone"></param>
	/// <param name="_targetNode"></param>
	void Copy_Bone(const aiMesh* _aiMesh, Bone*& _bone, Node* _targetNode);

	/// <summary>
	/// ����� ������ �����ϰ� �����Ѵ�.
	/// </summary>
	/// <typeparam name="Node"></typeparam>
	/// <param name="_ainode"></param>
	/// <param name="_node"></param>
	template<typename Node>
	void ProcessNode(const aiNode* _ainode, Node* _node);

	/// <summary>
	/// ���ؽ��� ����ġ�� �޴� ���� �����Ѵ�.
	/// </summary>
	/// <param name="_mesh"></param>
	void SetVertexesBoneInfo(Mesh& _mesh);

	/// <summary>
	/// aiMatrix�� ��Ʈ������ SimpleMath::Matrix�� �����Ѵ�.
	/// </summary>
	/// <param name="_aiMatrix"></param>
	/// <param name="_matrix"></param>
	void Copy_Matrix(const aiMatrix4x4& _aiMatrix, Matrix& _matrix);

	/// <summary>
	/// �ִϸ��̼� ������ ������ ��ȯ�Ѵ�.
	/// </summary>
	/// <param name="_pScene"></param>
	/// <returns></returns>
	Animation* CopyAnimation(const aiScene* _pScene);

	/// <summary>
	/// NOdeAnimation�� ������ �����Ѵ�. CopyAnimation�Լ����� ����Ѵ�.
	/// </summary>
	/// <param name="_aiNodeAnim"></param>
	/// <param name="_nodeAnim"></param>
	void Copy_NodeAnimationInfo(const aiNodeAnim* _aiNodeAnim, NodeAnimation& _nodeAnim);

	/// <summary>
	/// �޽��� AABB�� �����Ѵ�.
	/// </summary>
	/// <param name="_aiNodeAnim"></param>
	/// <param name="_AABB"></param>
	void Copy_AABB(const aiMesh* _aiNodeAnim, Mesh* _AABB);
private:
	unsigned int GetNumTextureCoords(const aiMesh* mesh);
	Node* GetTargetNode(std::string& _name, Node* _node);

};



template <typename T>
void FBXLoadHelper::Copy_Index(const aiMesh* _aiMesh, T* _mesh)
{
	_mesh->mNumIndices = _aiMesh->mNumFaces * 3;	// ��� ���� �ﰢ��
	_mesh->mIndices = new unsigned int[_mesh->mNumIndices];

	unsigned int index = 0;
	for (unsigned int i = 0; i < _aiMesh->mNumFaces; i++)
	{
		const aiFace& face = _aiMesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			if (index < _mesh->mNumIndices)		// ���� ������ ����
			{
				_mesh->mIndices[index++] = face.mIndices[j];
			}
		}
	}
}

template<typename Node>
void FBXLoadHelper::ProcessNode(const aiNode* _ainode, Node* _node)
{
	_node->mNodeName = _ainode->mName.C_Str();
	Copy_Matrix(_ainode->mTransformation, _node->mTransformation);
	_node->mNumNextNode = _ainode->mNumChildren;
	_node->mNumMeshes = _ainode->mNumMeshes;
	_node->mMeshes = new unsigned int[_node->mNumMeshes];
	for (int i = 0; i < _node->mNumMeshes; i++)
	{
		_node->mMeshes[i] = _ainode->mMeshes[i];
	}
	if (_node->mNumNextNode)
	{
		_node->mNextNode = new Node[_node->mNumNextNode];

		for (int i = 0; i < _node->mNumNextNode; i++)
		{
			ProcessNode(_ainode->mChildren[i], &_node->mNextNode[i]);
		}
	}
}