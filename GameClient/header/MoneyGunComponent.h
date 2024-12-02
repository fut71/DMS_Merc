#pragma once
#include "Component.h"

/// <summary>
/// �� 
/// </summary>
struct MoneyGunComponent
	: public Component
{
public:
	MoneyGunComponent(std::shared_ptr<Entity> _owner, uint8_t _maxLevel, uint8_t _grade)
		: Component(_owner), m_maxLevel(_maxLevel), m_grade(_grade)
		, m_currentLevel(1), m_throwAmount(1), m_throwCoolTime(1)
	{
	}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<MoneyGunComponent>(*this);
	}

public:
	uint8_t m_maxLevel = 2;			// ��� ������ �Ӵϰ��� �ִ� �ܰ�
	uint8_t m_currentLevel = 0;		// ���� ���� ���� �Ӵϰ��� �ܰ�
	uint8_t m_grade;			// �Ӵϰ��� ��ȭ �ܰ�
	uint8_t m_throwAmount;		// ��ô 1ȸ�� �������� �ݾ�
	float m_throwCoolTime;		// ��ô �� ���� ��ô���� ��� �ð�

};

