#pragma once
//���� ������
#include "directxTk/SimpleMath.h"
#include "Light.h"
#include "Material.h"
using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Vector4;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Quaternion;



struct CommonConstantBufferData
{
	Matrix view;
	Matrix proj;
	Matrix viewProj;

	Matrix invView;			//���� �����
	Matrix invProj;			//���������� �����
	Matrix invViewProj;		//Proj -> World

	Vector3 eyeWorld;
	float strengthIBL;

	Light light[MAX_LIGHTS];

	UINT lightNum;
	Vector3 dummy;
};

struct VSConstantBufferData
{
	Matrix world;		   //���庯ȯ
	Matrix invWorld;	   //�� ����ȯ
};
struct VSEdgeConstantBufferData
{
	Matrix  EdgeScaleMatrix;
};

struct PSConstantBufferData
{
	float maxLights = MAX_LIGHTS;			//12, Light�� �� ����
	Vector3 dummy;					//4
	MaterialProperties material;
};

struct PsShadowConstantBufferData
{
	Vector3 lightDirection;
	Vector3 lightColor;
	float ambientStrength;
	float dummy;
};

struct PSEdgeConstantBufferData
{
	Vector3 color;
	float dummy;
};


#define MAX_BONES 30
struct VSBoneConstantBufferData
{
	Matrix bone[MAX_BONES];
};

struct VSTargetBoneConstantBufferData
{
	Matrix targrtBoneMatrix;
};

struct VSShadowConstantBufferData
{
	Matrix ShadowView;
	Matrix ShadowProjection;
};

#define MAX_INSTANCE 100
struct VSInstantConstantBufferData
{
	Matrix world[MAX_INSTANCE];		   //���庯ȯ
	Matrix invWorld[MAX_INSTANCE];	   //�� ����ȯ
};

struct VSWaterConstantBufferData
{
	float time;     //deltatime;
	float speed;    //moveSpeed
	Vector2 dummy;
};

struct VSParticalConstantBufferData
{
	Vector4 RGBA;
	Matrix world;
};

#define MAX_PARTICLE 1024
struct CSParticleData
{
	Vector3 position = { 0,0,0 };			//ó�� ������
	float playTime = 0;						//���� �ð�
	
	Vector3 color = { 1,1,1 };				//����
	float lifeTime = 0;						//�� �ð�
	
	Vector3 direction = { 0,0,0 };			//����
	float velocity = 0;						//�ӵ�
	
	Vector3 Rotation = { 0,0,0 };			//�ʱ� ȸ����
	float opacity = 1;						//�ʱ� ����

	Vector3 Scale = { 1,1,1 };				//�ʱ� ������
	float deltaOpacity = 0;					//��Ÿ����

	Vector3 deltaRotation = { 0,0,0 };		//��Ÿȸ����
	float SclRandomFactor = 0;				//�������� ���� ����

	Vector3 deltaScale = { 0,0,0 };			//��Ÿ������

	Vector3 PosRandomFactor = {0, 0, 0};	//���������κ����� ��������
	Vector3 RotRandomFactor = {0, 0, 0};	//ȸ���� ��������
	Vector3 dirRandomFactor = { 0, 0, 0 };	//������ ��������
	
	Vector3 deltaColor = { 0,0,0 };			//�÷��� ���ϴ� ��
	float gravity = 0;						//�߷� ���

	Vector3 colorRandomFactor = { 0, 0, 0 };	//�÷��� ���� ����
	float dummy;
};

struct ParticleVertexInput
{
	Matrix ScalexTrans;
	Matrix Rot;
	Vector3 color;
	float Opacity;
};