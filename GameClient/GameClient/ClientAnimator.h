#pragma once
#include <string>
#include <memory>
#include "Entity.h"
#include "EntityManager.h"
#include "AnimationComponent.h"
#include "MeshRenderer.h"
#include "PhysicsManager.h"

class PhysicsManager;

/// <summary>
/// �⺻ �ӵ��� �����ϱ� ���� ������ �߰�
/// 2024.8.13.
/// </summary>
class ClientAnimator 
{
public:
	ClientAnimator(EntityManager* _pEntityManager, PhysicsManager* _pPhysicsManager)
		:mpEntityManager(_pEntityManager), mpPhysicsManager(_pPhysicsManager)
	{};
	~ClientAnimator() {};

public:
	EntityManager* mpEntityManager;
	PhysicsManager* mpPhysicsManager;
};
