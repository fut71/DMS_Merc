#pragma once
#include<directxTK/SimpleMath.h>
#include "LightEnum.h"
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Matrix;

#define MAX_LIGHTS 3

/// ����
struct Light
{
	float strength;                 // 12BYTE ���� ���� - ���� 1, ���� �߱����� �ʴ� ��ü�� 0
	float dummy;					// 4  //���� ���߿� �� �� ����.
	float fallOffStart;				// 4  //���� ������ ��
	float fallOffEnd;               // 4  //���� ������ ��
	Vector3 direction;				// 12 //���� : ���� ���ͷ� �����ϼ���.
	UINT lightType;					// 4 //���� ��Ⱑ ��ο����� �����ϴ� ��.
	Vector3 position;				// 12 //���� ��ġ
	float spotPower;                // 4  //���� ������
	Vector3 lightColor;				// 12 //���� �� ����
	UINT type;					// 4 //����

	Matrix viewProj;				//�׸��� �������� �ʿ�
	Matrix invProj;
	Light()
	{
		strength = 0.0f;
		dummy = 0.0f;
		fallOffStart = 0.0f;
		fallOffEnd = 0.0f;
		direction = Vector3(0.0f, 0.0f, 0.0f);
		lightType = 0;
		position = Vector3(0.0f, 0.0f, 0.0f);
		spotPower = 0.0f;
		lightColor = Vector3(1.0f, 1.0f, 1.0f);
		type = 0;
		viewProj = Matrix();
		invProj = Matrix();
	};

	Light& operator=(Light& _other)
	{
		if (this == &_other)
		{
			return *this;
		}
		strength = _other.strength;
		dummy = _other.dummy;
		fallOffStart = _other.fallOffStart;
		fallOffEnd = _other.fallOffEnd;
		direction = _other.direction;
		lightType = _other.lightType;
		position = _other.position;
		spotPower = _other.spotPower;
		lightColor = _other.lightColor;
		type = _other.type;
		viewProj = _other.viewProj;
		invProj = _other.invProj;
		return *this;
	}
};