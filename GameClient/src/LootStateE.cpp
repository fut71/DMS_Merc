#include "LootStateE.h"

void LootStateE::Initialize()
{
}

void LootStateE::Enter(std::shared_ptr<Entity> _entity)
{
	// �� ������ �ش� (����)�ִϸ��̼� ��� �ð� �ʱ�ȭ �� ����
	auto& animationComp = _entity->GetComponent<AnimationComponent>();
	animationComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@Clapping.fbx");

	// ���� ���¿��� �����ϵ��� ��ġ�� �����Ѵ�.
	mpPhysicsManager->SetFixPosition(_entity, true);

	///������
	//std::cout << "Enter Enemy Loot State" << std::endl;
}

void LootStateE::Update(float _dTime, std::shared_ptr<Entity> _entity)
{
	/// 0.Ÿ���� ������ ������.
	if (!_entity->GetComponent<EnemyComponent>().mpTarget)
	{
		return;
	}

	/// 1. ��ƼƼ�� ��ġ�� ����
	mpPhysicsManager->UpdatePosition(_entity, _entity->GetComponent<Transform>().m_localPosition);

	/// 2. ������ �����ݿ� ���� �߰��ϰ�, MoneyComponent ��ƼƼ�� ������ �� Ÿ���� nullptr�� ����
	auto& animationComp = _entity->GetComponent<AnimationComponent>();
	auto& enemyComp = _entity->GetComponent<EnemyComponent>();

	float mAnimationCoolTime = animationComp.mpTargetAnimation->mDuration / animationComp.mpTargetAnimation->mTicksPerSecond;

	if (mAnimationCoolTime - animationComp.mAnimationPlaytime < 0.05f)
	{
		if (enemyComp.mpTarget->HasComponent<MoneyComponent>())
		{
			if (!enemyComp.mpTarget->GetComponent<ProjectileComponent>().m_isTriggered
				/*|| _entity != enemyComp.mpTarget->GetComponent<MoneyComponent>().m_pTarget*/)
			{
				return;
			}
			// ������ �����ݿ� ���� �߰��Ѵ�.
			enemyComp.mMoney += enemyComp.mpTarget->GetComponent<MoneyComponent>().m_amount;
			std::shared_ptr<Entity>& pTarget = enemyComp.mpTarget;

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
			mpEntityManager->RemoveEntity(enemyComp.mpTarget->GetUID());
			mpSoundManager->PlaySFX("Snd_sfx_CharaLoot");
		}
	}
}

void LootStateE::Exit(std::shared_ptr<Entity> _entity)
{
	mpPhysicsManager->SetFixPosition(_entity, false);
	_entity->GetComponent<EnemyComponent>().mpTarget = nullptr;

}
