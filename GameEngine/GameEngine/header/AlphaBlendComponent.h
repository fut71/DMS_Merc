#pragma once
#include "Component.h"
#include "Graphics/BufferData.h"

/// <summary>
/// ���� ���� �����ϱ� ���� ������Ʈ
/// <para >
/// AlphaBlendComponent(float ���İ�);
/// </para>
/// </summary>
struct AlphaBlendComponent
	: public Component
{
public:
	AlphaBlendComponent(std::shared_ptr<Entity> _owner, float _alpha = 1.f)
		: Component(_owner), m_alpha(_alpha)
	{
	}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<AlphaBlendComponent>(*this);
	}

public:
	float m_alpha = 1.f;
};