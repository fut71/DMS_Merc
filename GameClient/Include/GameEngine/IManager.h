#pragma once
#include "pch.h"

/// <summary>
/// �Ŵ��� Ŭ�������� �������̽� Ŭ�����Դϴ�.
/// ��ӹ��� Ŭ�������� ��ƼƼ���� ������Ʈ �������� �ʿ��� ���
/// </summary>
class IManager
{
public:
	IManager() = default;
	virtual ~IManager() = default;

	virtual bool Initialize() = 0;
	virtual void FixedUpdate(float _fixedDTime) = 0;
	virtual void Update(float _dTime) = 0;
	virtual void LateUpdate(float _dTime) = 0;
	virtual void Finalize() = 0;

};