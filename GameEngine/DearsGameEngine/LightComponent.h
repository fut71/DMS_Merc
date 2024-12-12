#pragma once
#include "Component.h"
#include "Graphics/BufferData.h"

/// <summary>
/// �߱�ü ������Ʈ�� ���Ǵ� ������Ʈ�Դϴ�.
/// </summary>
struct LightComponent 
	: public Component
{
public:
	LightComponent(std::shared_ptr<Entity> _owner)
		: Component(_owner), m_commonConstData()/*, m_isShadow(false), m_targetCamera(nullptr)*/
	{}
	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<LightComponent>(*this);
	}

	//bool m_isShadow;	//�׸��ڸ� �׸��� �� ���

public:
	CommonConstantBufferData m_commonConstData;		//#sung �ȿ� ���������� �� ������Ʈ ���� �����͸� ����� �ξ���. -> �� �ȿ�  Light�� ���� �迭�� �����Ѵ�.

public:
	CommonConstantBufferData* GetCommonConstantBufferData() 
	{
		return &m_commonConstData;
	}


};