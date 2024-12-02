#pragma once
#include "AStar.h"
#include "ProjectileSystem.h"
#include "EquipmentManager.h"
#include "PlayerSystem.h"
#include "NPCSystem.h"

/// <summary>
/// ���� �� �����ϴ� ��� ������ �����ϴ� �ý����̴�.
/// </summary>
class UnitSystem
{
public:
	UnitSystem(entt::registry& _registry, EntityManager* _pEntityManager, PhysicsManager* _pPhysicsManager
		, InputManager* _pInpueManager, WorldManager* _worldManager
		, RenderManager* _pRenderManager, ResourceManager* _pResourceManager, EventManager* _pEventManager
		, UIManager* _pUIManager, SoundManager* _pSoundManager, AStar* _pAstar);
	~UnitSystem();

public:
	void Initialize();

	//�� ��ü�� �����͸� ��ȯ�Ѵ�.
	ProjectileSystem* GetProjectileSystem();
	EquipmentManager* GetEquipmentManager();
	PlayerSystem* GetPlayerSystem();
	NPCSystem* GetNPCSystem();

private:
	entt::registry& mRegistry;

	EntityManager* mpEntityManager;
	PhysicsManager* mpPhysicsManager;
	InputManager* mpInputManager;
	WorldManager* mpWorldManager;
	ResourceManager* mpResourceManager;
	RenderManager* mpRenderManager;
	EventManager* mpEventManager;
	UIManager* mpUIManager;
	SoundManager* mpSoundManager;

	AStar* mpAstar;
	ProjectileSystem* mpProjectileSystem;
	EquipmentManager* mpEquipmentManager;
	PlayerSystem* mpPlayerSystem;
	NPCSystem* mpNPCSystem;
};

