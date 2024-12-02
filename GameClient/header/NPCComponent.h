#pragma once
#include "Entity.h"
#include "Component.h"

/// <summary>
/// NPC ������Ʈ
/// </summary>
struct NPCComponent
	: public Component
{
public:
	NPCComponent(std::shared_ptr<Entity> _owner, std::string _npcName = std::string())
		: Component(_owner), mNpcName(_npcName), mIsPlayerInteracted(false), mElapsTime(0.0f)
	{
	}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<NPCComponent>(*this);
	}

public:
	std::string mNpcName;
	bool mIsPlayerInteracted; // �÷��̾�� ��ȣ�ۿ��� �ߴ��� ���� : �ؽ�Ʈ �Ϸ� ���γ� Ư�� �ൿ�� Ʈ���Ÿ� ����� �����̴�.
	float mElapsTime; // �󸶳� �ð��� ����ߴ��� �ʿ��� �� ���� ����
};