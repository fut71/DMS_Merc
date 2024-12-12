#pragma once
#include "IManager.h"
#include "EventListener.h"
#include "Entity.h"
#include "AStar.h"
#include "UserData.h"

class EntityManager;
class PhysicsManager;
class RenderManager;
class SoundManager;
class InputManager;
class WorldManager;
class SceneData;

/// <summary>
/// ����ü ���� �ý���
/// </summary>
class ProjectileSystem
	: public IManager, public EventListener
{
public:
	/// <summary>
	/// entt::registry& ����� ������� �⺻ ������ ����
	/// </summary>
	ProjectileSystem() = delete;

	/// <summary>
	/// Ŭ�������� ��ƼƼ���� ������Ʈ�� ����� ��� 
	/// "entt::registry& m_registry" ����� �߰��ϰ�, ������ �߰�
	/// </summary>
	/// <param name="_registry"></param>
	ProjectileSystem(entt::registry& _registry, EventManager* _pEventManager
		, EntityManager* _pEntityManager, PhysicsManager* _pPhysicsManager
		, RenderManager* _pRenderManagerm, SoundManager* _pSoundManager
		, InputManager* _pInputManager, WorldManager* _pWorldManager);

	~ProjectileSystem() = default;

	/// <summary>
	/// �Ŵ��� �ʱ�ȭ 
	/// </summary>
	/// <returns></returns>
	virtual bool Initialize() override;

	/// <summary>
	/// ���� ������ �ִ� ��� ���⼭ ������Ʈ // ������ ��� ���ҵ�
	/// </summary>
	/// <param name="_fixedDTime">1/60���� ������ ������ ����</param>
	virtual void FixedUpdate(float _fixedDTime) override;

	/// <summary>
	/// �Ϲ����� ������ ������Ʈ 
	/// </summary>
	/// <param name="_dTime">������ ����</param>
	virtual void Update(float _dTime) override;

	/// <summary>
	/// ��ó���� �ʿ��� ������ ������Ʈ
	/// </summary>
	/// <param name="_dTime">������ ����</param>
	virtual void LateUpdate(float _dTime) override;

	/// <summary>
	/// �޸� ����	// ������ �� ������ ������
	/// </summary>
	virtual void Finalize() override;

	void ThrowCoin(const Event& _event);

	void CreateMoney(const Event& _event);
	// 
	// 	void ShootProjectile(const Event& _event);

	void CreateProjectile(const Event& _event);

	void TutirialUpdateThrow(float _dTime);
	void TutirialUpdateMoney(float _dTime);


	void SetAstar(AStar* _pAstar);

	// ���� ��ġ�� ����� ���͸� �������� �Լ�
	std::vector<std::pair<std::shared_ptr<Entity>, Vector3>>* GetMoneyPosVec();

	bool mIsThrow = false; // NPC�� �뺴�տ��� �ִϸ��̼��� �õ��϶�� �÷���

private:
	entt::registry& m_registry;
	EntityManager* m_pEntityManager;
	PhysicsManager* m_pPhysicsManager;
	RenderManager* m_pRenderManager;
	SoundManager* m_pSoundManager;
	InputManager* m_pInputManager;
	WorldManager* m_pWorldManager;

	// ���� �Ʊ� ��ΰ� Ȯ���ϴ� ���� ��ġ ���� ����
	std::vector<std::pair<std::shared_ptr<Entity>, Vector3>> mMoneyPosVec;
	std::vector<std::vector<int>>* mAstarMap;
	Vector2 mStartPoint;
	int mMapwidth;
	AStar* mpAstar;
	SceneData* m_pSceneData;
	std::shared_ptr<UserData> m_pWorldData;
	float spawnTime = 0.f;
	float mTutoSpawnTime = 0.f;

	float m_coinSpeed = 1;
	float m_arrowSpeed = 1;
};
