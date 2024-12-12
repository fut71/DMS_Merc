#pragma once
#include<directxTK/SimpleMath.h>
#include <string>

using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Vector4;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Quaternion;

#define MAX_BONES_INDICE 8
#define MAX_NUM_TEXCOORD 1
class Vertex
{
public:
	Vertex();
	Vertex(Vector3 a, Vector3 b, Vector2 c)
	{
		mPosition = a;
		mNormal = b;
		mTexcoords[0] = c;
	};
	~Vertex();
public:
	Vector3 mPosition;
	Vector3 mNormal;
	Vector2 mTexcoords[MAX_NUM_TEXCOORD];
	UINT mBoneIndex[MAX_BONES_INDICE] = { UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX };
	float mBoneWeights[MAX_BONES_INDICE] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f };
};

//�޽����� �� ��带 �̷��.
class Node
{
public:
	Node();
	~Node();
public:
	std::string mNodeName;			//��� �̸�
	Matrix mTransformation;			//����� ��ȯ ��� / SimpleMath�� ������ ��ǥ�踦 ����.
	Matrix mWorldTransformation;	//������ǥ

	unsigned int mNumNextNode;
	Node* mNextNode;
	//�� ����� �޽� ��
	unsigned int mNumMeshes;

	//�� ����� �޽�, �� �׸��� Model�� �޽� ��Ͽ� ���� �ε���
	unsigned int* mMeshes;
};

struct VertexWeight
{
	unsigned int mVertexId = UINT_MAX;
	float mWeight = 0.0f;
};

///���� �ַ� ��Ű�� �ִϸ��̼��� �����Ҷ� ���ȴ�.
class Bone
{
public:
	Bone();
	~Bone();

public:
	std::string mBoneName;		//�� �̸�
	Node* mTargetNode;			//�� �̸��� ����� �̸��� ���� ��� Ÿ�� ���� �������ش�. �׷��� �����tranform�� ������ �ü� �ֱ� ����
	unsigned int mNumWeight = 0;
	VertexWeight* mpVertexWeight;
	Matrix mOffsetMatrix;		//�޽��� �������� ������ ��� -> ���� �������� �޽��� �������� ���� ���� �������� ��ȯ�ϴµ� ���
};

class AABB
{
public:
	Vector3 mMin = { FLT_MAX, FLT_MAX, FLT_MAX };
	Vector3 mMax = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
};

class Mesh
{
public:
	Mesh();
	~Mesh();
public:
	std::string mMeshName;				// �޽��� �̸�

	unsigned int  mNumVertices;			// ���ؽ��� ��
	Vertex* mVertices = nullptr;					// Vertex�迭

	unsigned int mNumIndices;			// �ε����� ��
	unsigned int* mIndices = nullptr;				// Index �迭

	AABB mAABB;

	unsigned int mNumBones;
	Bone* mBone;						//�޽��� �ش��ϴ� ��
};


struct VecKey
{
	double mTime;
	Vector3 mValue;
};

struct QuatKey
{
	double mTime;
	Quaternion mValue;
};

class NodeAnimation
{
public:
	NodeAnimation();
	~NodeAnimation();
public:
	unsigned int mNumPosKeys;
	VecKey* mPosKey;			//Positon Key

	unsigned int mNumRotKeys;
	QuatKey* mRotKey;			//Rotation Key

	unsigned int mNumScaKeys;
	VecKey* mScaKey;			//Scale Key

	std::string mName;			//�� �ִϸ��̼��� ������ �޴� ����� �̸�
};

class Animation
{
public:
	Animation();
	~Animation();
public:
	std::string mName;						//�ִϸ��̼��� �̸�
	double mDuration;						//�ִϸ��̼��� ���ӽð�(ƽ)
	double mTicksPerSecond;					//�ʴ� ƽ, ������ ���Ͽ� �������� ���� ��� -> 0

	unsigned int mNumChannels;				//�� �ִϸ��̼� ä�� ��
	NodeAnimation* mChannels;				//��� �ִϸ��̼��� ä��, �� ä���� ���� ��忡 ������ �ش�. ũ��� mNumChannels;

	///���߿� �޽þִϸ��̼��� �߰��Ϸ��� ����� �߰��ϸ�ȴ�.
};

//������ �� ���� �̷��. �ٵ� �ϳ��� ���θ� ������ ���� ���� ���� ����
class Model
{
public:
	Model();
	~Model();
public:
	Node* mRootNode;

	unsigned int mNumMesh;
	Mesh* mMeshData;

	//	std::string mBasePath;

	// 	unsigned int mNumAnimation;
	// 	Animation* mAnimation;
};
