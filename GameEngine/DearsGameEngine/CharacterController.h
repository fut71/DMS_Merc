#pragma once
#include "Component.h"

namespace physx
{
	class PxCapsuleController;
}

struct CharacterController
	: public Component
{
public:
	CharacterController(std::shared_ptr<Entity> _owner, float _contactOffset = 0.1f, float _stepOffset = 0.5f
		, float _slopeLimit = 0.f, float _density = 10.f)
		: Component(_owner)
		, m_contactOffset(_contactOffset)
		, m_stepOffset(_stepOffset)
		, m_slopeLimit(_slopeLimit)
		, m_density(_density)
		, m_pController(nullptr)
	{}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<CharacterController>(*this);
	}

public:
	float m_contactOffset;   // ���� ������
	float m_stepOffset;      // ��� ������ ������
	float m_slopeLimit;      // �ִ� ���� ����
	float m_density;         // �е�

	physx::PxCapsuleController* m_pController;
};