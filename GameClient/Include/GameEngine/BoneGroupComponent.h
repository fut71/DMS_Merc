#pragma once
#include "Component.h"
#include "Graphics/BufferData.h"

/// <summary>
/// �𵨵��� ������ �����Ͱ� �ִ� ������Ʈ�Դϴ�.
/// </summary>
struct BoneGroupComponent
	: public Component
{
public:
	BoneGroupComponent(std::shared_ptr<Entity> _owner)
		: Component(_owner), m_boneData()
	{}
	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<BoneGroupComponent>(*this);
	}


public:
// 	std::string m_file;
	VSBoneConstantBufferData m_boneData;
};