#pragma once
#include "Component.h"

struct ProjectileComponent
	: public Component
{
public:
	ProjectileComponent(std::shared_ptr<Entity> _owner, float _lifeTime = -1, float _height = -1
		, const Vector3& _targetPosition = Vector3(), float _speed = -1)
		: Component(_owner), m_lifeTime(_lifeTime), m_height(_height), m_targetPosition(_targetPosition)
		, m_speed(_speed)
	{}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<ProjectileComponent>(*this);
	}

public:
	uint8_t m_damage;
	float m_lifeTime;
	float m_height;
	float m_speed;
	bool m_isThrown = false;
	bool m_isTriggered = false;	// ĳ���� �� ��ֹ� ��� �浹 ���� ��� true�� ��ȯ
	Vector3 m_targetPosition;
	Vector3 m_control1 = Vector3();		// ������
	Vector3 m_control2 = Vector3();		// ������
};