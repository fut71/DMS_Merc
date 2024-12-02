#pragma once
#include "Component.h"
#include <string.h>

/// <summary>
/// �ش� ��ƼƼ�� �ε����������� �����ϱ� ���� ������Ʈ
/// </summary>
class IndicatorComponent :
	public Component
{
public:
	IndicatorComponent(std::shared_ptr<Entity> _owner, const std::string& _original = "", const std::string& _changed = "")
		: Component(_owner), mOriginalColor(_original), mChangedColor(_changed)
	{
	}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<IndicatorComponent>(*this);
	}

public:
	// �ε��������� ���� �ؽ�ó ������ �����صд�.
	std::string mOriginalColor;
	std::string mChangedColor;
};

