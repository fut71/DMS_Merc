#pragma once
#include "Component.h"
#include "Graphics/ModelInfo.h"
struct AnimationComponent
	: public Component
{
public:
	AnimationComponent(std::shared_ptr<Entity> _owner)
		: Component(_owner)
	{}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<AnimationComponent>(*this);
	}

public:
	// ���� ��� ���� �ִϸ��̼��� Ÿ�Ӷ��ο��� ����� �ð�
	float mAnimationPlaytime = 0.0f;

	float mNextAnimationPlayTime = 0.0f;

	// ���� ��� ���� �ִϸ��̼��� ��ü�ð�
	float mAnimationCoolTime = 0.0f;

	// ���� ��� ���� �ִϸ��̼� 
	Animation* mpTargetAnimation = nullptr;

	// ������ ����� �ִϸ��̼� (��ȯ�� ��쿡 ���)
	Animation* mpNextTargetAnimation = nullptr;

	// �ִϸ��̼��� �����ϴ��� ����
	bool mIsLoop = false;

	// �ִϸ��̼� ��� �ӵ� (1.0 = �⺻ �ӵ�, 0.5 = ���� �ӵ�)
	float mPlaySpeed = 1.f;

	// �ִϸ��̼��� �����ϴ� �÷��� (�����ϸ� ���� �����ӿ��� ����)
	bool mPaused = false;

	void SetTargetAnimation(Animation* _targetAnimaion)
	{
		mpTargetAnimation = _targetAnimaion;
	}
	
	void SetNextTargetAnimation(Animation* _nextTargetAnimaion)
	{
		mpNextTargetAnimation = _nextTargetAnimaion;
	}

	//std::vector<std::shared_ptr<Entity>> myEntityVec;
};
//
//void AnimationComponent::SetTargetAnimation(Animation* _targetAnimaion)
//{
//	mpTargetAnimation = _targetAnimaion;
//}
//
//void AnimationComponent::SetNextTargetAnimation(Animation* _nextTargetAnimaion)
//{
//	mpNextTargetAnimation = _nextTargetAnimaion;
//}
