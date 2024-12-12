#pragma once
#include "Logger.h"
#include "PhysicsDefine.h"

/// <summary>
/// ������ �浹 �̺�Ʈ ó���� ���� �ݹ� ��ü�Դϴ�.
/// 2024.06.03 _Doyo;
/// </summary>
class CollisionCallback
	: public physx::PxSimulationEventCallback
{
public:
	CollisionCallback() = default;
	~CollisionCallback() = default;

	/// <summary>
	/// �浹 �̺�Ʈ �ݹ� �޼��� 
	/// </summary>
	/// <param name="_pairHeader">�浹�� �� ��ü�� ���� ������ �����ϴ� ����ü�Դϴ�.</param>
	/// <param name="_pairs">�浹�� ��ü �ֵ��� �迭�Դϴ�. �� �迭 ��Ҵ� �浹 �ֿ� ���� ���� ������ �����մϴ�.</param>
	/// <param name="_nbPairs">�浹�� ��ü ���� ���� ��Ÿ���ϴ�.</param>
	virtual void onContact(const physx::PxContactPairHeader& _pairHeader, const physx::PxContactPair* _pairs, physx::PxU32 _nbPairs) override
	{
		DLOG(LOG_INFO, "onContact()");

		for (physx::PxU32 i = 0; i < _nbPairs; i++)
		{
			const physx::PxContactPair& cp = _pairs[i];
			if (cp.events & physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
			{
			}
		}
	}

	/// <summary>
	/// Ʈ���� �̺�Ʈ �ݹ� �޼��� 
	/// </summary>
	/// <param name="_pairs">Ʈ���ŵ� ��ü �ֵ��� �迭�Դϴ�. �� �迭 ��Ҵ� Ʈ���� �̺�Ʈ�� ���� ������ �����մϴ�.</param>
	/// <param name="_count">Ʈ���ŵ� ��ü ���� ���� ��Ÿ���ϴ�.</param>
	virtual void onTrigger(physx::PxTriggerPair* _pairs, physx::PxU32 _count) override
	{
		DLOG(LOG_INFO, "onTrigger()");
	}

	/// <summary>
	/// ���� �ı� �̺�Ʈ �ݹ� �޼��� 
	/// </summary>
	/// <param name="_constraints">�ı��� ������� �迭�Դϴ�. �� �迭 ��Ҵ� �ı��� ���࿡ ���� ������ �����մϴ�.</param>
	/// <param name="_count">�ı��� ������ ���� ��Ÿ���ϴ�.</param>
	virtual void onConstraintBreak(physx::PxConstraintInfo* _constraints, physx::PxU32 _count) override
	{
		DLOG(LOG_INFO, "onConstraintBreak()");
	}

	/// <summary>
	/// ��ü ��� �̺�Ʈ �ݹ� �޼��� 
	/// </summary>
	/// <param name="_actors">��� ��ü�鿡 ���� ������ �迭�Դϴ�.</param>
	/// <param name="_count">��� ��ü�� ���� ��Ÿ���ϴ�.</param>
	virtual void onWake(physx::PxActor** _actors, physx::PxU32 _count) override
	{
		DLOG(LOG_INFO, "onWake()");
	}

	/// <summary>
	/// ��ü ��� �̺�Ʈ �ݹ� �޼��� 
	/// </summary>
	/// <param name="_actors">��� ��ü�鿡 ���� ������ �迭�Դϴ�.</param>
	/// <param name="_count">��� ��ü�� ���� ��Ÿ���ϴ�.</param>
	virtual void onSleep(physx::PxActor** _actors, physx::PxU32 _count) override
	{
		DLOG(LOG_INFO, "onSleep()");
	}

	/// <summary>
	/// �ùķ��̼� ���� ������Ʈ �̺�Ʈ �ݹ� �޼��� 
	/// </summary>
	/// <param name="_bodyBuffer">������Ʈ�� ��ü ��ü���� ������ �迭�Դϴ�.</param>
	/// <param name="_poseBuffer">�� ��ü ��ü�� ���ο� ��ġ�� ȸ���� ��Ÿ���� �迭�Դϴ�.</param>
	/// <param name="_count">������Ʈ�� ��ü�� ���� ��Ÿ���ϴ�.</param>
	virtual void onAdvance(const physx::PxRigidBody* const* _bodyBuffer, const physx::PxTransform* _poseBuffer, const physx::PxU32 _count) override
	{
		DLOG(LOG_INFO, "onAdvance()");
	}
};