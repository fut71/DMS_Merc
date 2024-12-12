#pragma once
#include "pch.h"

/// <summary>
/// �� ������ ������ ���� �ý��� Ŭ�������� �������̽��Դϴ�.
/// 2024.06.12 _Doyo;
/// </summary>
class System
{
public:
	System(entt::registry& _registry) :m_registry(_registry) {}
	virtual ~System() {}
	
	virtual bool Initialize() = 0;
	virtual void FixedUpdate(float _dTime) = 0;
	virtual void Update(float _dTime) = 0;
	virtual void LateUpdate(float _dTime) = 0;
	virtual void Finalize() = 0;

protected:
	entt::registry& m_registry;
};

