#pragma once
#include "Scene.h"
#include "UnitSystem.h"
#include "PlayerSystem.h"
#include "ProjectileSystem.h"
#include "NPCSystem.h"
#include "SceneData.h"
#include "LevelManager.h"

class StageScene1_3
	: public Scene
{
public:
	/// <summary>
	/// ������ �⺻������ ���Ǵ� ��ü���� �����ڿ��� �������ݴϴ�.
	/// </summary>
	StageScene1_3(entt::registry& _registry, const std::string& _name
		, EventManager* _pEventManager, RenderManager* _pRenderManager
		, PhysicsManager* _pPhysicsManager, InputManager* _pInpueManager
		, WorldManager* _worldManager, UIManager* _UIManager, EntityManager* _pEntityManager
		, ResourceManager* _pResourceManager, SoundManager* _pSoundManager);

	/// <summary>
	/// ���� �ʱ�ȭ�մϴ�.
	/// </summary>
	/// <returns>__super::Initialize() ��ȯ</returns>
	virtual bool Initialize() override;

	/// <summary>
	/// ������ �ð� �������� ���� ������Ʈ�� �����մϴ�.
	/// </summary>
	/// <param name="_dTime">������ ��ŸŸ��</param>
	virtual void FixedUpdate(float _dTime) override;

	/// <summary>
	/// �� ������ ������Ʈ�� �����մϴ�.
	/// </summary>
	/// <param name="_dTime">������ �ð�</param>
	virtual void Update(float _dTime) override;

	/// <summary>
	/// ��ó�� ������Ʈ�� �����մϴ�.
	/// </summary>
	/// <param name="_dTime">������ �ð�</param>
	virtual void LateUpdate(float _dTime) override;

	/// <summary>
	/// ���� �����ϰ� �����մϴ�.
	/// </summary>
	virtual void Finalize() override;

	/// <summary>
	/// ���� ���ҽ��� �߰��մϴ�.
	/// </summary>
	virtual void AddResource() override;

private:
	UnitSystem* mpUnitSystem;
	LevelManager* mpLevelManager;
	AStar* mpAstar;
};
