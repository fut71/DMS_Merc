#pragma once
#include "Component.h"
#include "Graphics/BufferData.h"

/// <summary>
/// �𵨵��� ������ �����Ͱ� �ִ� ������Ʈ�Դϴ�.
/// </summary>
struct TargetBoneComponent
	: public Component
{
public:
	TargetBoneComponent(std::shared_ptr<Entity> _owner)
		: Component(_owner), mTargetBoneData(), mTarGetBoneConstantBufferData(nullptr), mTargetBoneAboveMatrix(Matrix())
	{}
	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<TargetBoneComponent>(*this);
	}


public:
	// 	std::string m_file;
	Matrix mTargetBoneAboveMatrix;
	int mTargetBoneIndex;			//����� ��� Ÿ�ٺ��� �ε����� �˰� �־���Ѵ�. -> �Ȱ��� ����� �ι� ���ص� �ȴ�.
	VSBoneConstantBufferData* mTarGetBoneConstantBufferData;
	VSTargetBoneConstantBufferData mTargetBoneData;

	std::weak_ptr<Entity> myPlayer;
};