#include "LootState.h"
#include "Scene.h"
#include "WorldManager.h"
#include "Rigidbody.h"

void LootState::Initialize()
{
}

void LootState::Enter(std::shared_ptr<Entity> _entity)
{
	// �� ������ �ش� (����)�ִϸ��̼� ��� �ð� �ʱ�ȭ �� ����
	auto& animationComp = _entity->GetComponent<AnimationComponent>();
	animationComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@Clapping.fbx");

	// ���� ���¿��� �����ϵ��� ��ġ�� �����Ѵ�.
	mpPhysicsManager->SetFixPosition(_entity, true);

	///������
	//std::cout << "Enter Loot State" << std::endl;
}

void LootState::Update(float _dTime, std::shared_ptr<Entity> _entity)
{
	/// 0.Ÿ���� ������ ������.
	if (!_entity->GetComponent<PlayerComponent>().mpTarget)
	{
		return;
	}

	/// 1. ��ƼƼ�� ��ġ�� ����
	mpPhysicsManager->UpdatePosition(_entity, _entity->GetComponent<Transform>().m_localPosition);

	/// 2. �÷��̾��� �����ݿ� ���� �߰��ϰ�, MoneyComponent ��ƼƼ�� ������ �� Ÿ���� nullptr�� ����
	auto& animationComp = _entity->GetComponent<AnimationComponent>();
	auto& playerComp = _entity->GetComponent<PlayerComponent>();

	float mAnimationCoolTime = animationComp.mpTargetAnimation->mDuration / animationComp.mpTargetAnimation->mTicksPerSecond;

	if (mAnimationCoolTime - animationComp.mAnimationPlaytime < 0.05f)
	{
		if (playerComp.mpTarget->HasComponent<MoneyComponent>())
		{
			if (!playerComp.mpTarget->GetComponent<ProjectileComponent>().m_isTriggered
				/*|| _entity != playerComp.mpTarget->GetComponent<MoneyComponent>().m_pTarget*/)
			{
				return;
			}
			// �÷��̾��� �����ݿ� ���� �߰��Ѵ�.
			playerComp.mMoney += playerComp.mpTarget->GetComponent<MoneyComponent>().m_amount;
			std::shared_ptr<Entity>& pTarget = playerComp.mpTarget;

			/// moneyVec���� �ش� ��ƼƼ�� �����.
			auto& moneyVec = *mpFSMHelper->GetMoneyVec(); // ���� ���� ���
			for (auto it = moneyVec.begin(); it != moneyVec.end(); )
			{
				if (it->first == pTarget) // Ư�� Ÿ�ٰ� ��ġ�ϴ� ��Ҹ� ã��
				{
					it = moneyVec.erase(it); // �ش� ��� ���� ��, ���� ��ҷ� �̵�
				}
				else
				{
					++it; // ��ġ���� ������ ���� ��ҷ� �̵�
				}
			}

			// �浹�� moneyComponent ��ƼƼ�� �����ϰ�, Ÿ���� nullptr�� �����Ѵ�.
			mpEntityManager->RemoveEntity(playerComp.mpTarget->GetUID());
			auto uid = _entity->GetUID();
			auto uid2 = playerComp.mpTarget->GetUID();
			mpSoundManager->PlaySFX("Snd_sfx_CharaLoot");
		}
	}
}

void LootState::Exit(std::shared_ptr<Entity> _entity)
{
	mpPhysicsManager->SetFixPosition(_entity, false);
	_entity->GetComponent<PlayerComponent>().mpTarget = nullptr;
}
