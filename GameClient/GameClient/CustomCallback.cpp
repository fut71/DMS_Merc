#include "CustomCallback.h"
#include "CustomFilter.h"
#include "PhysicsManager.h"
#include "EventManager.h"
#include "EntityManager.h"
#include "Event.h"
#include "Entity.h"

#include "PlayerComponent.h"
#include "EnemyComponent.h"
#include "Transform.h"
#include "ProjectileComponent.h"
#include "MoneyComponent.h"

CustomCallback::CustomCallback(PhysicsManager* _pPhysicsManager, EventManager* _pEventManager, EntityManager* _pEntityManager)
	:m_pPhysicsManager(_pPhysicsManager), EventListener(_pEventManager), m_pEntityManager(_pEntityManager)
{
}

CustomCallback::~CustomCallback()
{
}

void CustomCallback::onContact(const physx::PxContactPairHeader& _pairHeader, const physx::PxContactPair* _pairs, physx::PxU32 _nbPairs)
{
	for (physx::PxU32 i = 0; i < _nbPairs; i++)
	{
		const physx::PxContactPair& cp = _pairs[i];

		if (cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			physx::PxRigidActor* actor0 = _pairHeader.actors[0]->is<physx::PxRigidActor>();
			physx::PxRigidActor* actor1 = _pairHeader.actors[1]->is<physx::PxRigidActor>();

			std::string name = actor0->getName() + std::string(", ") + actor1->getName();
			DLOG(LOG_INFO, "onContact: " + name);

			if (actor0 && actor1)
			{
				physx::PxShape* shape0;
				physx::PxShape* shape1;

				actor0->getShapes(&shape0, 1);
				actor1->getShapes(&shape1, 1);

				physx::PxFilterData filterData0 = shape0->getSimulationFilterData();
				physx::PxFilterData filterData1 = shape1->getSimulationFilterData();

				if (filterData0.word0 & TYPE_WEAPON || filterData1.word0 & TYPE_WEAPON)
				{
					int a = 0;
				}

				if ((filterData0.word0 & TYPE_GOLD) && (filterData1.word0 & TYPE_GROUND))
				{
					filterData0.word1 |= ATTR_ON_GROUND;
					m_pPhysicsManager->SetFilterData(shape0, filterData0);
					m_pPhysicsManager->SetFilterData(shape1, filterData1);
				}
				if ((filterData1.word0 & TYPE_GOLD) && (filterData0.word0 & TYPE_GROUND))
				{
					filterData1.word1 |= ATTR_ON_GROUND;
					m_pPhysicsManager->SetFilterData(shape0, filterData0);
					m_pPhysicsManager->SetFilterData(shape1, filterData1);
				}

				// 
				if ((filterData0.word0 & TYPE_UNIT) && (filterData0.word1 & ~ATTR_IS_DEAD))
				{
					if (filterData1.word0 & TYPE_WEAPON)
					{
						HandleMeleeWeaponHit(actor0, actor1);
					}
					else if (filterData1.word0 & TYPE_GOLD)
					{
						HandleCoinCollision(actor0, actor1);
					}
				}
				else if ((filterData1.word0 & TYPE_UNIT) && (filterData1.word1 & ~ATTR_IS_DEAD))
				{
					if (filterData0.word0 & TYPE_WEAPON)
					{
						HandleMeleeWeaponHit(actor1, actor0);
					}
					else if (filterData0.word0 & TYPE_GOLD)
					{
						HandleCoinCollision(actor1, actor0);
					}
				}

			}
		}
	}
}

void CustomCallback::onTrigger(physx::PxTriggerPair* _pairs, physx::PxU32 _count)
{
	for (physx::PxU32 i = 0; i < _count; i++)
	{
		const physx::PxTriggerPair& tp = _pairs[i];

		if (tp.status & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			// ���Ϳ��� ù ��° ������ ��������
			physx::PxRigidActor* actor0 = tp.triggerActor->is<physx::PxRigidActor>();
			physx::PxRigidActor* actor1 = tp.otherActor->is<physx::PxRigidActor>();

			std::string name = actor0->getName() + std::string(", ") + actor1->getName();
			DLOG(LOG_INFO, "onTrigger: " + name);

			if (actor0 && actor1)
			{
				physx::PxShape* shape0;
				physx::PxShape* shape1;

				actor0->getShapes(&shape0, 1);
				actor1->getShapes(&shape1, 1);

				physx::PxFilterData filterData0 = shape0->getSimulationFilterData();
				physx::PxFilterData filterData1 = shape1->getSimulationFilterData();

				if (filterData0.word0 & TYPE_UNIT)
				{
					if ((filterData1.word0 & TYPE_PROJECTILE) && (filterData0.word1 & ~ATTR_IS_DEAD))
					{
						HandleProjectileHit(actor0, actor1);
					}
				}
				else if (filterData1.word0 & TYPE_UNIT)
				{
					if ((filterData0.word0 & TYPE_PROJECTILE) && (filterData1.word1 & ~ATTR_IS_DEAD))
					{
						HandleProjectileHit(actor1, actor0);
					}
				}

				if ((filterData0.word0 & TYPE_UNIT) && (filterData0.word1 & ~ATTR_IS_DEAD))
				{
					if (filterData1.word0 & TYPE_WEAPON)
					{
						//m_pEventManager->TriggerEvent(Event("HandleMeleeWeaponHit", actorPair));
						HandleMeleeWeaponHit(actor0, actor1);
					}
					else if (filterData1.word0 & TYPE_GOLD)
					{
						HandleCoinCollision(actor0, actor1);
					}

				}
				else if ((filterData1.word0 & TYPE_UNIT) && (filterData1.word1 & ~ATTR_IS_DEAD))
				{
					if (filterData0.word0 & TYPE_WEAPON)
					{
						HandleMeleeWeaponHit(actor1, actor0);
					}
					else if (filterData0.word0 & TYPE_GOLD)
					{
						HandleCoinCollision(actor1, actor0);
					}

				}

				if (((filterData0.word0 & TYPE_GROUND) || (filterData0.word0 & TYPE_OBSTACLE))
					&& (filterData1.word0 & TYPE_PROJECTILE))
				{
					HandleWallCollision(actor0, actor1);
				}
				else if ((filterData0.word0 & TYPE_PROJECTILE)
					&& ((filterData1.word0 & TYPE_GROUND) || (filterData1.word0 & TYPE_OBSTACLE)))
				{
					HandleWallCollision(actor1, actor0);
				}

			}
		}
	}
}

void CustomCallback::onConstraintBreak(physx::PxConstraintInfo* _constraints, physx::PxU32 _count)
{
	PX_UNUSED((_constraints));
	DLOG(LOG_INFO, "onConstraintBreak");
}

void CustomCallback::onWake(physx::PxActor** _actors, physx::PxU32 _count)
{
	PX_UNUSED((_actors));
	DLOG(LOG_INFO, "onWake");
}

void CustomCallback::onSleep(physx::PxActor** _actors, physx::PxU32 _count)
{
	PX_UNUSED((_actors));
	DLOG(LOG_INFO, "onSleep");
}

void CustomCallback::onAdvance(const physx::PxRigidBody* const* _bodyBuffer, const physx::PxTransform* _poseBuffer, const physx::PxU32 _count)
{
	DLOG(LOG_INFO, "onAdvance");
}

void CustomCallback::HandleProjectileHit(physx::PxRigidActor* _unit, physx::PxRigidActor* _projectile)
{

	/// ����ü�� ĳ���Ϳ� �浹���� ���� �̺�Ʈ ó�� ����
	//auto actorPair = _pEvent.GetDataAs<std::shared_ptr<std::pair<physx::PxRigidActor*, physx::PxRigidActor*>>>().value();
	// ������ ����
	//auto first = actorPair->first;
	auto first = _unit;
	// ����ü�� ����
	//auto second = actorPair->second;
	auto second = _projectile;

	// ������ ��ƼƼ
	auto firstEntity = m_pPhysicsManager->GetEntityFromActor(first);
	// ����ü�� ��ƼƼ
	auto secondEntity = m_pPhysicsManager->GetEntityFromActor(second);

	DLOG(LOG_INFO, "HandleProjectileHit: " + std::to_string(firstEntity->GetUID()) + '/' + std::to_string(secondEntity->GetUID()));
	// ����ü�� ���ݷ�
	auto& damage = secondEntity->GetComponent<ProjectileComponent>().m_damage;

	// ����ü�� ���ݷ¸�ŭ ������ HP�� ���ҽ�Ŵ
	if (firstEntity->HasComponent<PlayerComponent>())
	{
		auto& hp = firstEntity->GetComponent<PlayerComponent>().mHP;
		//std::cout << "hp: " << hp << '/';
		hp -= damage;
		//std::cout << "damage: " << damage << "/current hp: " << hp;
		secondEntity->GetComponent<ProjectileComponent>().m_isTriggered = true;
	}
	else if (firstEntity->HasComponent<EnemyComponent>())
	{
		auto& hp = firstEntity->GetComponent<EnemyComponent>().mHP;
		//std::cout << "hp: " << hp << '/';
		hp -= damage;
		//std::cout << "damage: " << damage << "/current hp: " << hp;
		secondEntity->GetComponent<ProjectileComponent>().m_isTriggered = true;
	}
}

void CustomCallback::HandleMeleeWeaponHit(physx::PxRigidActor* _unit, physx::PxRigidActor* _weapon)
{
	/// �ٰŸ� ���Ⱑ ĳ���Ϳ� �浹���� ���� �̺�Ʈ ó�� ����
	//auto actorPair = _pEvent.GetDataAs<std::shared_ptr<std::pair<physx::PxRigidActor*, physx::PxRigidActor*>>>().value();
	// ���ݹ޴� ������ ����
	//auto first = actorPair->first;
	auto first = _unit;
	// ������ ����
	//auto second = actorPair->second;
	auto second = _weapon;

	// ���ݹ޴� ������ ��ƼƼ
	auto firstEntity = m_pPhysicsManager->GetEntityFromActor(first);
	// ������ ��ƼƼ
	auto secondEntity = m_pPhysicsManager->GetEntityFromActor(second);

	DLOG(LOG_INFO, "HandleMeleeWeaponHit: " + std::to_string(firstEntity->GetUID()) + '/' + std::to_string(secondEntity->GetUID()));

	auto name1 = firstEntity->GetName();
	auto name2 = secondEntity->GetName();
	std::cout << name1 << " / " << name2 << std::endl;

	if (firstEntity->HasComponent<PlayerComponent>())
	{
		// ������ ĳ������ ���ݷ�
		float attackPower = 0.f;
		if (secondEntity->GetComponent<Transform>().m_pParent->m_pOwner->HasComponent<EnemyComponent>())
		{
			attackPower = secondEntity->GetComponent<Transform>().m_pParent->m_pOwner->GetComponent<EnemyComponent>().mAttackPower;
		}

		// ������ ĳ������ attakPower��ŭ HP�� ���ҽ�Ŵ
		auto& hp = firstEntity->GetComponent<PlayerComponent>().mHP;
		hp -= attackPower;
	}
	else if (firstEntity->HasComponent<EnemyComponent>())
	{
		// ������ ĳ������ ���ݷ�
		auto& attackPower = secondEntity->GetComponent<Transform>().m_pParent->m_pOwner->GetComponent<PlayerComponent>().mAttackPower;

		// ������ ĳ������ attakPower��ŭ HP�� ���ҽ�Ŵ
		auto& hp = firstEntity->GetComponent<EnemyComponent>().mHP;
		hp -= attackPower;
	}

}

void CustomCallback::HandleCoinCollision(physx::PxRigidActor* _unit, physx::PxRigidActor* _gold)
{
	/// ������ ĳ���Ϳ� �浹���� ���� �̺�Ʈ ó�� ����
	//auto actorPair = _pEvent.GetDataAs<std::shared_ptr<std::pair<physx::PxRigidActor*, physx::PxRigidActor*>>>().value();
	// ������ ����
	//auto first = actorPair->first;
	auto first = _unit;
	// ������ ����
	//auto second = actorPair->second;
	auto second = _gold;

	// ������ ��ƼƼ
	auto firstEntity = m_pPhysicsManager->GetEntityFromActor(first);
	// ������ ��ƼƼ
	auto secondEntity = m_pPhysicsManager->GetEntityFromActor(second);
	DLOG(LOG_INFO, "HandleCoinCollision: " + std::to_string(firstEntity->GetUID()) + '/' + std::to_string(secondEntity->GetUID()));

	if (!secondEntity->GetComponent<ProjectileComponent>().m_isTriggered)
	{

		if (firstEntity->HasComponent<PlayerComponent>())
		{

			if (firstEntity->GetComponent<PlayerComponent>().mpTarget == secondEntity)
			{
				secondEntity->GetComponent<ProjectileComponent>().m_isTriggered = true;
			}

		}
		if (firstEntity->HasComponent<EnemyComponent>())
		{
			if (firstEntity->GetComponent<EnemyComponent>().mpTarget == secondEntity)
			{
				secondEntity->GetComponent<ProjectileComponent>().m_isTriggered = true;
			}
		}

		secondEntity->GetComponent<MoneyComponent>().m_pTarget = firstEntity;
	}

	// 	// ������ �ݾ�
	// 	auto& amount = secondEntity->GetComponent<MoneyComponent>().m_amount;
	// 
	// 	// ĳ���Ϳ��� �ݾ׸�ŭ �������� �߰�
	// 	firstEntity->GetComponent<PlayerComponent>().mMoney + amount;

}

void CustomCallback::HandleWallCollision(physx::PxRigidActor* _wall, physx::PxRigidActor* _projectile)
{
	/// ����ü�� ���� �浹���� ���� �̺�Ʈ ó�� ����
	// ���� ����
	auto first = _wall;
	// ����ü�� ����
	auto second = _projectile;

	// ���� ��ƼƼ
	auto firstEntity = m_pPhysicsManager->GetEntityFromActor(first);
	// ����ü�� ��ƼƼ
	auto secondEntity = m_pPhysicsManager->GetEntityFromActor(second);

	DLOG(LOG_INFO, "HandleWallCollision: " + std::to_string(firstEntity->GetUID()) + '/' + std::to_string(secondEntity->GetUID()));
	secondEntity->GetComponent<ProjectileComponent>().m_isTriggered = true;
}

