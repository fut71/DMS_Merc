#include "RaycastQueryFilterCallback.h"

RaycastQueryFilterCallback::RaycastQueryFilterCallback(const std::string& targetName)
	: m_targetName(targetName)
{
}

physx::PxQueryHitType::Enum RaycastQueryFilterCallback::preFilter(const physx::PxFilterData& filterData
	, const physx::PxShape* shape, const physx::PxRigidActor* actor, physx::PxHitFlags& queryFlags)
{
	const char* actorName = actor->getName();
	if (actorName && m_targetName == actorName)
	{
		// �̸��� ��ġ�ϸ� ��� �浹�� ����
		return physx::PxQueryHitType::eBLOCK;
	}
	else
	{
		// �̸��� ��ġ���� ������ ���̸� ��� ����
		return physx::PxQueryHitType::eTOUCH;
	}
}

physx::PxQueryHitType::Enum RaycastQueryFilterCallback::postFilter(const physx::PxFilterData& filterData
	, const physx::PxQueryHit& hit, const physx::PxShape* shape, const physx::PxRigidActor* actor)
{
	return physx::PxQueryHitType::eNONE;
}
