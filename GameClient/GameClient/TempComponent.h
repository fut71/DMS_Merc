#pragma once
#include "Component.h"

/// <summary>
/// �ӽ� ������Ʈ Ŭ����
/// </summary>
struct TempComponent
	: public Component
{
public:
	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<TempComponent>(*this);
	}


public:
	bool m_onGround;

};