#pragma once
#include "CollisionCallback.h"
#include "EventListener.h"

class PhysicsManager;
class EventManager;
class EntityManager;

/// <summary>
/// ���ӿ��� ������ ���� ���� �ùķ��̼� �̺�Ʈ �ݹ� ��ü�Դϴ�.
/// </summary>
class CustomCallback
	: public physx::PxSimulationEventCallback 
	, public EventListener
{
public:
	CustomCallback(PhysicsManager* _pPhysicsManager, EventManager* _pEventManager, EntityManager* _pEntityManager);
	~CustomCallback();

	/// <summary>
	/// �浹 �̺�Ʈ �ݹ� �޼��� 
	/// </summary>
	/// <param name="_pairHeader">�浹�� �� ��ü�� ���� ������ �����ϴ� ����ü�Դϴ�.</param>
	/// <param name="_pairs">�浹�� ��ü �ֵ��� �迭�Դϴ�. �� �迭 ��Ҵ� �浹 �ֿ� ���� ���� ������ �����մϴ�.</param>
	/// <param name="_nbPairs">�浹�� ��ü ���� ���� ��Ÿ���ϴ�.</param>
	 void onContact(const physx::PxContactPairHeader& _pairHeader, const physx::PxContactPair* _pairs, physx::PxU32 _nbPairs) override;

	/// <summary>
	/// Ʈ���� �̺�Ʈ �ݹ� �޼��� 
	/// </summary>
	/// <param name="_pairs">Ʈ���ŵ� ��ü �ֵ��� �迭�Դϴ�. �� �迭 ��Ҵ� Ʈ���� �̺�Ʈ�� ���� ������ �����մϴ�.</param>
	/// <param name="_count">Ʈ���ŵ� ��ü ���� ���� ��Ÿ���ϴ�.</param>
	 void onTrigger(physx::PxTriggerPair* _pairs, physx::PxU32 _count) override;

	/// <summary>
	/// ���� ���� �ı� �̺�Ʈ �ݹ� �޼��� 
	/// </summary>
	/// <param name="_constraints">�ı��� ������� �迭�Դϴ�. �� �迭 ��Ҵ� �ı��� ���࿡ ���� ������ �����մϴ�.</param>
	/// <param name="_count">�ı��� ������ ���� ��Ÿ���ϴ�.</param>
	 void onConstraintBreak(physx::PxConstraintInfo* _constraints, physx::PxU32 _count) override;

	/// <summary>
	/// ��ü�� ����� �̺�Ʈ �ݹ� �޼��� 
	/// </summary>
	/// <param name="_actors">��� ��ü�鿡 ���� ������ �迭�Դϴ�.</param>
	/// <param name="_count">��� ��ü�� ���� ��Ÿ���ϴ�.</param>
	 void onWake(physx::PxActor** _actors, physx::PxU32 _count) override;

	/// <summary>
	/// ��ü�� ���� �̺�Ʈ �ݹ� �޼��� 
	/// </summary>
	/// <param name="_actors">��� ��ü�鿡 ���� ������ �迭�Դϴ�.</param>
	/// <param name="_count">��� ��ü�� ���� ��Ÿ���ϴ�.</param>
	 void onSleep(physx::PxActor** _actors, physx::PxU32 _count) override;

	/// <summary>
	/// �ùķ��̼� ���� ������Ʈ �̺�Ʈ �ݹ� �޼��� 
	/// </summary>
	/// <param name="_bodyBuffer">������Ʈ�� ��ü ��ü���� ������ �迭�Դϴ�.</param>
	/// <param name="_poseBuffer">�� ��ü ��ü�� ���ο� ��ġ�� ȸ���� ��Ÿ���� �迭�Դϴ�.</param>
	/// <param name="_count">������Ʈ�� ��ü�� ���� ��Ÿ���ϴ�.</param>
	 void onAdvance(const physx::PxRigidBody* const* _bodyBuffer, const physx::PxTransform* _poseBuffer, const physx::PxU32 _count) override;

	 /// <summary>
	 /// pair<PxRigidActor*, PxRigidActor*>�� �̺�Ʈ ���ڷ� ����
	 /// first: TYPE_UNIT, second: TYPE_PROJECTILE
	 /// </summary>
	 /// <param name="_pEvent">std::pair<physx::PxRigidActor*, physx::PxRigidActor*></param>
	 //void HandleProjectileHit(const Event& _pEvent);
	 void HandleProjectileHit(physx::PxRigidActor* _unit, physx::PxRigidActor* _projectile);

	 /// <summary>
	 /// pair<PxRigidActor*, PxRigidActor*>�� �̺�Ʈ ���ڷ� ����
	 /// first: TYPE_UNIT, second: TYPE_WEAPON
	 /// </summary>
	 /// <param name="_pEvent">std::pair<physx::PxRigidActor*, physx::PxRigidActor*></param>	
	 //void HandleMeleeWeaponHit(const Event& _pEvent);
	 void HandleMeleeWeaponHit(physx::PxRigidActor* _unit, physx::PxRigidActor* _weapon);

	 /// <summary>
	 /// pair<PxRigidActor*, PxRigidActor*>�� �̺�Ʈ ���ڷ� ����
	 /// first: TYPE_UNIT, second: TYPE_GOLD
	 /// </summary>
	 /// <param name="_pEvent">std::pair<physx::PxRigidActor*, physx::PxRigidActor*></param>
	 //void HandleCoinCollision(const Event& _pEvent);
	 void HandleCoinCollision(physx::PxRigidActor* _unit, physx::PxRigidActor* _gold);

	 void HandleWallCollision(physx::PxRigidActor* _wall, physx::PxRigidActor* _projectile);

private:

	PhysicsManager* m_pPhysicsManager;
	EntityManager* m_pEntityManager;
};

