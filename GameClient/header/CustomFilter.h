#pragma once
#include "PhysicsDefine.h"
#include "FilterEnum.h"

/// <summary>
/// ���� ���� ���͸� �������ִ� Ŭ�����Դϴ�.
/// ���� ���� �����Ҷ� �����ϸ� ��ü�鰣�� �浹�� ��� ������ ������ �����Ҽ� �ֽ��ϴ�.
/// </summary>
class CustomFilter
{
public:
	static physx::PxFilterFlags CustomFilterShader(
		physx::PxFilterObjectAttributes _attributes0, physx::PxFilterData _filterData0
		, physx::PxFilterObjectAttributes _attributes1, physx::PxFilterData _filterData1
		, physx::PxPairFlags& _pairFlags, const void* _constantBlock, physx::PxU32 _constantBlockSize);

// 	static physx::PxFilterFlags CustomFilterShader2(
// 		physx::PxFilterObjectAttributes _attributes0, physx::PxFilterData _filterData0
// 		, physx::PxFilterObjectAttributes _attributes1, physx::PxFilterData _filterData1
// 		, physx::PxPairFlags& _pairFlags, const void* _constantBlock, physx::PxU32 _constantBlockSize);

};

