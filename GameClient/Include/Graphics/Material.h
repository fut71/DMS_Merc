#pragma once
#include "directxTk/SimpleMath.h"
using DirectX::SimpleMath::Vector3;

/// ����
struct MaterialProperties
{
	Vector3 ambient = Vector3(0.1f);		// 12byte
	float shininess = 0.01f;					// 4byte	/����
	Vector3 diffuse = Vector3(0.5f);		// 12byte	
	float dummy1 = 0.0f;					// 4byte	/���� ������
	Vector3 specular = Vector3(0.5f);		// 12byte
	float dummy2 = 0.0f;					// 4byte	/���� ������
	//Vector3 fresnel = Vector3(0.5f, 0.5f, 0.5f);			 //�⺻������ ���� �����Ѱ� �������
	//Vector3 fresnel = Vector3(1.0f, 0.71f, 0.29f);		 //gold
	Vector3 fresnel = Vector3(0.02, 0.02, 0.02);		 //water
	float dummy3; //16����Ʈ
};

///9.9.2024 DJ 01:17
///diffuse�� specular�� vector3�� �ϴ� ������ RGB���� ���� �� ������ ���� �ٷ�� �����̴�.


///Schlick �ٻ簪(approximation) : 9.17 "����Ÿ�ӷ�����"
//_frenel�� ������ ��������
//Water   :(0.02, 0.02, 0.02) 
//Glass   :(0.08, 0.08, 0.08) 
//Plastic :(0.05, 0.05, 0.05) 
//Gold    :( 1.0, 0.71, 0.29) 
//Silver  :(0.95, 0.93, 0.88) 
//Copper  :(0.95, 0.64, 0.54)