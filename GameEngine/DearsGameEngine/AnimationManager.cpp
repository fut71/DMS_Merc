#include "AnimationManager.h"

AnimationManager::AnimationManager(entt::registry& _registry, std::shared_ptr<DearsGraphicsEngine> _graphicsEngine)
	: m_registry(_registry), mpGraphicsEngine(_graphicsEngine)
{
}

AnimationManager::~AnimationManager()
{
}

void AnimationManager::AnimationUpdate(std::shared_ptr<Entity> _entity, float _dTime)
{
	//��ƼƼ�� �𵨹��۸� �ҷ��´�.
	auto& modelBuffer = _entity->GetComponent<MeshRenderer>().m_pModel;
	//��ƼƼ�� animationComponent�� �ҷ��´�.
	if (_entity->HasComponent<AnimationComponent>())
	{
		auto& animationComp = _entity->GetComponent<AnimationComponent>();

		// ���࿡ Animation�� ���� ���¸� ����
		//���� �����ΰ� ����������..? �׾�����? ������ ������ �Ǿ�����?
		if (animationComp.mPaused == true)
		{
			return;
		}

		animationComp.mAnimationCoolTime = GetCoolTime(_entity);



		// �ִϸ��̼� �÷���Ÿ�ӿ� ��� �ð��� �����ش�.
		animationComp.mAnimationPlaytime += _dTime * animationComp.mPlaySpeed;
		//	std::cout << _dTime << std::endl;
			//���⼭ ����ó���� ���ִ� ������. ���� ������ true�� ��� �ִϸ��̼��� ���������Ű�� �װ� �ƴҰ��� �ѹ��� ����ǵ���..!
		if (animationComp.mIsLoop)
		{
			///��¼�� ��¼��
			if (animationComp.mAnimationPlaytime * modelBuffer->mpTargetAnimation->mTicksPerSecond >= modelBuffer->mpTargetAnimation->mDuration)
			{
				animationComp.mAnimationPlaytime = 0.0f;
			}

		}

		// ���� �������� ���� �ִϸ��̼��� �����Ѵٸ�, ���� �ִϸ��̼ǿ��� ����ð��� �����ش�.
		if (animationComp.mpNextTargetAnimation)
		{
			animationComp.mNextAnimationPlayTime += _dTime * animationComp.mPlaySpeed;
		}
		modelBuffer->mAnimationPlaytime = animationComp.mAnimationPlaytime;
		modelBuffer->mNextAnimationPlaytime = animationComp.mNextAnimationPlayTime;
		modelBuffer->mpTargetAnimation = animationComp.mpTargetAnimation;
		modelBuffer->mpNextTargetAnimation = animationComp.mpNextTargetAnimation;



		// 	// �� ���ۿ� �ִ� �÷���Ÿ�Ӱ� �ִϸ��̼� �÷���Ÿ���� �ð��� ��ġ�����ش�.
		// 	modelBuffer->mAnimationPlaytime = animationComp.mAnimationPlaytime;
		// 
		// 
		// 	// ������ �� �ִϸ��̼��� �ִٸ�
		// 	if (animationComp.mpNextTargetAnimation != "")
		// 	{
		// 		Animation* tempAnimation = mpGraphicsEngine->Get_Animation(animationComp.mpNextTargetAnimation);
		// 
		// 		// Ÿ�� �ִϸ��̼ǰ� �ؽ�Ʈ Ÿ�� �ִϸ��̼��� ���� ��� ������ ���� ����
		// 		if (animationComp.mpTargetAnimation != animationComp.mpNextTargetAnimation)
		// 		{
		// 			// ���� �ؽ�Ʈ �ִϸ��̼ǰ� ��ȯ�Ϸ��� �ִϸ��̼��� �ٸ� ��� && ���� ���� �ִϸ��̼��� ����
		// 			if (modelBuffer->mpNextTargetAnimation != tempAnimation && (!modelBuffer->mpNextTargetAnimation))
		// 			{
		// 				modelBuffer->mpNextTargetAnimation = tempAnimation;
		// 				animationComp.isInterpolation = true;
		// 			}
		// 		}
		// 		else
		// 		{
		// 			// �ؽ�Ʈ Ÿ�� �ִϸ��̼ǰ� Ÿ�� �ִϸ��̼��� �����Ƿ� ������ �����ϰ� �ٷ� ���� ó��
		// 			animationComp.mpNextTargetAnimation = "";
		// 			animationComp.isInterpolation = false;
		// 		}
		// 	}
		// 
		// 	// ��Ÿ�ӵ� �־��ش�.
		// 	animationComp.mAnimationCoolTime = GetCoolTime(_entity);
		// 
		// 	// �ִϸ��̼��� ���� ������ ���
		// 	if (animationComp.mIsLoop == true)
		// 	{
		// 		SettingLoopAnimation(_entity);
		// 	}
		// 	else
		// 	{
		// 		// ������ �� �ִϸ��̼��� ���ٸ�
		// 		if (_entity->GetComponent<AnimationComponent>().mpNextTargetAnimation == "")
		// 		{
		// 			animationComp.isInterpolation = false;
		// 			_entity->GetComponent<AnimationComponent>().mAnimationPlaytime = 0.0f;
		// 		}
		// 	}
	}

}

void AnimationManager::SetInitialAnimation(std::shared_ptr<Entity> _entity)
{
	auto& modelBuffer = _entity->GetComponent<MeshRenderer>().m_pModel;
	auto& animationComp = _entity->GetComponent<AnimationComponent>();
	if (animationComp.mpTargetAnimation)
	{
		modelBuffer->mpTargetAnimation = animationComp.mpTargetAnimation;
	}
	//modelBuffer->mpTargetAnimation = mpGraphicsEngine->Get_Animation(_entity->GetComponent<AnimationComponent>().mpTargetAnimation);
}

void AnimationManager::InterPolationAnimation(std::shared_ptr<Entity> _entity)
{
	// 	auto& modelBuffer = _entity->GetComponent<MeshRenderer>().m_pModel;
	// 
	// 	// ���� �ִϸ��̼��� ������ �ִϸ��̼����� ��ü
	// 	modelBuffer->mAnimationPlaytime = modelBuffer->mNextAnimationPlaytime;
	// 	modelBuffer->mpTargetAnimation = modelBuffer->mpNextTargetAnimation;
	// 	_entity->GetComponent<AnimationComponent>().mpTargetAnimation = _entity->GetComponent<AnimationComponent>().mpNextTargetAnimation;
	// 
	// 	// ���� ���� �ʱ�ȭ
	// 	modelBuffer->mNextAnimationPlaytime = 0;
	// 	modelBuffer->mpNextTargetAnimation = nullptr;
	// 	_entity->GetComponent<AnimationComponent>().mpNextTargetAnimation = "";
	auto& animationComp = _entity->GetComponent<AnimationComponent>();
	animationComp.mAnimationPlaytime = animationComp.mNextAnimationPlayTime;
	animationComp.mpTargetAnimation = animationComp.mpNextTargetAnimation;
	animationComp.mNextAnimationPlayTime = 0;
	animationComp.mpNextTargetAnimation = nullptr;


}

void AnimationManager::SettingLoopAnimation(std::shared_ptr<Entity> _entity)
{
	// 	auto& animationComp = _entity->GetComponent<AnimationComponent>();
	// 
	// 	if (animationComp.mAnimationPlaytime > animationComp.mAnimationCoolTime)
	// 	{
	// 		if (_entity->GetComponent<AnimationComponent>().mpNextTargetAnimation == "")
	// 		{
	// 			_entity->GetComponent<AnimationComponent>().mAnimationPlaytime = 0.0f;
	// 		}
	// 		else
	// 		{
	// 			animationComp.isInterpolation = false;
	// 			mpGraphicsEngine->Get_Animation(_entity->GetComponent<AnimationComponent>().mpTargetAnimation); // �����Ǵ� �ִϸ��̼��� �ݺ����� �ʰ� �����ϱ� ���� �ڵ��. 
	// 			animationComp.mAnimationPlaytime = 0.0f;
	// 		}
	// 	}
}

float AnimationManager::GetCoolTime(std::shared_ptr<Entity> _entity)
{
	// 	auto& modelBuffer = _entity->GetComponent<MeshRenderer>().m_pModel;
	// 	auto& animationComp = _entity->GetComponent<AnimationComponent>();
	// 	Animation* animation = mpGraphicsEngine->Get_Animation(animationComp.mpTargetAnimation);
	// 
	// 	// ���� ���� �ִϸ��̼��� ����
	// 	if (animationComp.mpNextTargetAnimation != "" && animationComp.isInterpolation == true)
	// 	{
	// 		Animation* nextAnimation = mpGraphicsEngine->Get_Animation(animationComp.mpNextTargetAnimation);
	// 
	// 		double targetDuration = animation->mDuration / animation->mTicksPerSecond;
	// 		double nextTargetDuration = nextAnimation->mDuration / nextAnimation->mTicksPerSecond;
	// 		double interpolationTime = modelBuffer->mNextAnimationPlaytime;
	// 		float playtime = modelBuffer->mAnimationPlaytime;
	// 
	// 		//int a = 1; // ������
	// 
	// 		if (playtime >= targetDuration)
	// 		{
	// 			// ù ��° �ִϸ��̼��� ������ ���� �ð����� �� ��° �ִϸ��̼��� �ð��� ���
	// 			double remainingTime = playtime - targetDuration;
	// 			//std::cout << remainingTime << "/" << nextTargetDuration << std::endl;
	// 
	// 			if (remainingTime < nextTargetDuration)
	// 			{
	// 				return static_cast<float>(remainingTime);
	// 			}
	// 			else
	// 			{
	// 				// �� ��° �ִϸ��̼��� ����� ���
	// 				return static_cast<float>(nextTargetDuration);
	// 			}
	// 		}
	// 		else
	// 		{
	// 			// ���� ���̹Ƿ�, ��ü �ð��� �ջ��Ͽ� ��ȯ
	// 			//std::cout << playtime << "/" << static_cast<float>(targetDuration + nextTargetDuration) << std::endl;
	// 			return static_cast<float>(targetDuration + nextTargetDuration - playtime);
	// 		}
	// 
	// 	}
	// 	else
	// 	{
	// 		//std::cout << modelBuffer->mAnimationPlaytime << "/" << static_cast<float>(animation->mDuration / animation->mTicksPerSecond + (animationComp.mHoldTimeAtEnd / animationComp.mPlaySpeed)) << std::endl;
	// 		return static_cast<float>(animation->mDuration / animation->mTicksPerSecond + (animationComp.mHoldTimeAtEnd / animationComp.mPlaySpeed));
	// 	}
	return 0;
}
