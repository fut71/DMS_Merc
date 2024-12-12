#pragma once
#include "PhysicsDefine.h"
#include "pch.h"
#include "physx/PxQueryFiltering.h"

class RaycastQueryFilterCallback :
	public physx::PxQueryFilterCallback
{
public:
	RaycastQueryFilterCallback(const std::string& targetName);

	// �� ������ �浹�� ó���� �� ȣ��˴ϴ�.
	physx::PxQueryHitType::Enum preFilter(const physx::PxFilterData& filterData
		, const physx::PxShape* shape, const physx::PxRigidActor* actor, physx::PxHitFlags& queryFlags) override;

	physx::PxQueryHitType::Enum postFilter(const physx::PxFilterData& filterData
		, const physx::PxQueryHit& hit, const physx::PxShape* shape, const physx::PxRigidActor* actor) override;

private:
	std::string m_targetName;

};

