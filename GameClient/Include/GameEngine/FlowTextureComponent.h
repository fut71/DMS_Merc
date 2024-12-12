#pragma once
#include "Component.h"
#include "Graphics/BufferData.h"

/// <summary>
/// FlowTextureComponent(float �ð�, float �ӵ�)
/// </summary>
struct FlowTextureComponent
	:public Component
{
public:
	FlowTextureComponent(std::shared_ptr<Entity> _owner, float _time = 0.f, float _speed = 0.1f)
		: Component(_owner), m_time(_time), m_speed(_speed)
	{}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<FlowTextureComponent>(*this);
	}

public:
	float m_time;
	float m_speed;
	VSWaterConstantBufferData m_VSWCSD;
};