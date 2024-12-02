#include "UnitSystem.h"

UnitSystem::UnitSystem(entt::registry& _registry, EntityManager* _pEntityManager, PhysicsManager* _pPhysicsManager
	, InputManager* _pInputManager, WorldManager* _pWorldManager, RenderManager* _pRenderManager
	, ResourceManager* _pResourceManager, EventManager* _pEventManager
	, UIManager* _pUIManager, SoundManager* _pSoundManager, AStar* _pAstar)
	: mRegistry(_registry)
{
	mpEntityManager = _pEntityManager;
	mpPhysicsManager = _pPhysicsManager;
	mpInputManager = _pInputManager;
	mpWorldManager = _pWorldManager;
	mpRenderManager = _pRenderManager;
	mpResourceManager = _pResourceManager;
	mpEventManager = _pEventManager;
	mpUIManager = _pUIManager;
	mpSoundManager = _pSoundManager;

	// Astar �����
	mpAstar = _pAstar;

	// ����ü �ý��� ����
	mpProjectileSystem =  new ProjectileSystem(mRegistry, mpEventManager
		, mpEntityManager, mpPhysicsManager, mpRenderManager, mpSoundManager
		, mpInputManager, mpWorldManager);

	// ��� �Ŵ��� ����
	mpEquipmentManager = new EquipmentManager(mRegistry, mpEntityManager, mpPhysicsManager, mpRenderManager, mpResourceManager);

	// �÷��̾� �ý��� ����
	mpPlayerSystem = new PlayerSystem(mRegistry, mpEntityManager, mpPhysicsManager
		, mpRenderManager, mpResourceManager, mpEventManager, mpSoundManager, mpEquipmentManager);

	// NPC �ý��� ����
	mpNPCSystem = new NPCSystem(mpEntityManager, mpRenderManager, mpResourceManager, mpSoundManager, mpUIManager, mpProjectileSystem, mpEquipmentManager);
}

UnitSystem::~UnitSystem()
{
	delete mpNPCSystem;
	delete mpPlayerSystem;
	delete mpEquipmentManager;
	delete mpProjectileSystem;
}

void UnitSystem::Initialize()
{
	// ������ �ý��� �� �Ŵ����� �ʱ�ȭ
	mpProjectileSystem->Initialize();
	mpEquipmentManager->Initialize();
	mpPlayerSystem->Initialize(mpProjectileSystem->GetMoneyPosVec(), mpAstar);
	mpNPCSystem->Initialize();
	mpProjectileSystem->SetAstar(mpAstar);
}

ProjectileSystem* UnitSystem::GetProjectileSystem()
{
	return mpProjectileSystem;
}

EquipmentManager* UnitSystem::GetEquipmentManager()
{
	return mpEquipmentManager;

}

PlayerSystem* UnitSystem::GetPlayerSystem()
{
	return mpPlayerSystem;

}

NPCSystem* UnitSystem::GetNPCSystem()
{
	return mpNPCSystem;
}