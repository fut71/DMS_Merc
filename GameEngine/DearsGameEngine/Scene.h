#pragma once
#include "pch.h"
#include "Utility.h"
#include "EventListener.h"
#include "IData.h"

// �ӽ÷� ���� static
static bool sIsMain = true;

class Entity;
class System;
class RenderManager;
class PhysicsManager;
class InputManager;
class WorldManager;
class UIManager;
class EntityManager;
class SoundManager;
class ResourceManager;

/// <summary>
/// ���� ���� ���� ���� ��Ÿ���� Ŭ�����Դϴ�. ���� ��ƼƼ���� �迭�� �����˴ϴ�.
/// </summary>
class Scene
	:public EventListener
{
public:
	Scene() = delete;
	Scene(entt::registry& _registry, const std::string& _name
		, EventManager* _pEventManager, RenderManager* _pRenderManager
		, InputManager* _pInpueManager, WorldManager* _pWorldManager);
	Scene(entt::registry& _registry, const std::string& _name
		, EventManager* _pEventManager, RenderManager* _pRenderManager
		, PhysicsManager* _pPhysicsManager, InputManager* _pInpueManager
		, WorldManager* _pWorldManager, UIManager* _pUIManager
		, EntityManager* _pEntityManager, ResourceManager* _pResourceManager
		, SoundManager* _pSoundManager = nullptr);
	virtual ~Scene() = default;

	/// <summary>
	/// ���� �ʱ�ȭ�մϴ�.
	/// </summary>
	/// <returns>�ʱ�ȭ ���н� false ��ȯ</returns>
	virtual bool Initialize();

	/// <summary>
	/// ������ �ð� �������� ���� ������Ʈ�� �����մϴ�.
	/// </summary>
	/// <param name="_dTime">������ ��ŸŸ��</param>
	virtual void FixedUpdate(float _dTime);

	/// <summary>
	/// �� ������ ������Ʈ�� �����մϴ�.
	/// </summary>
	/// <param name="_dTime">������ �ð�</param>
	virtual void Update(float _dTime);

	/// <summary>
	/// ��ó�� ������Ʈ�� �����մϴ�.
	/// </summary>
	/// <param name="_dTime">������ �ð�</param>
	virtual void LateUpdate(float _dTime);

	/// <summary>
	/// ���� �����ϰ� �����մϴ�.
	/// </summary>
	virtual void Finalize();

	///// <summary>
	///// ��ƼƼ�� ��ȯ�մϴ�.
	///// </summary>
	///// <param name="_uid">��ȯ�� ��ƼƼ�� UID</param>
	///// <returns>��ƼƼ ��ü</returns>
	//std::shared_ptr<Entity> GetEntity(const UID& _uid);

	///// <summary>
	///// ���� ��ƼƼ ���� ��ȯ�մϴ�.
	///// </summary>
	///// <returns>��ƼƼ ��</returns>
	//std::unordered_map<UID, std::shared_ptr<Entity>> GetEntityMap() const;

	/*/// <summary>
	/// ���ο� ��ƼƼ�� �����մϴ�.
	/// </summary>
	/// <param name="_name">��ƼƼ ��ü�� �̸�</param>
	/// <returns>������ ��ƼƼ ��ü</returns>
	std::shared_ptr<Entity> CreateEntity(const std::string& _name);*/

	///// <summary>
	///// �� ���� ��ƼƼ �߰�
	///// </summary>
	///// <param name="_entity">��ƼƼ ��ü</param>
	//void AddEntity(std::shared_ptr<Entity> _entity);

	///// <summary>
	///// ��ƼƼ�� ������ �����մϴ�.
	///// </summary>
	///// <param name="_uid">������ ��ƼƼ�� UID</param>
	//void RemoveEntity(const UID& _uid);

	///// <summary>
	///// ��� ��ƼƼ�� ������ �����մϴ�.
	///// </summary>
	//void RemoveAllEntities();

	///// <summary>
	///// ���� �ý����� �߰��մϴ�.
	///// </summary>
	///// <param name="_pSystem">�߰��� �ý��� ��ü</param>
	////void AddSystem(const std::shared_ptr<System>& _pSystem);

	/// <summary>
	/// ���� ���ҽ��� �߰��մϴ�.
	/// </summary>
	virtual void AddResource();

	/// <summary>
	/// ���� UID�� ��ȯ�մϴ�.
	/// </summary>
	/// <returns>�� UID</returns>
	UID GetUID() const;

	/// <summary>
	/// ���� �̸��� ��ȯ�մϴ�.
	/// </summary>
	/// <returns>�� �̸�</returns>
	std::string GetName() const;

	IData* GetSceneData() const;

protected:
	UID m_uid;														// ���� ���� �ĺ���
	std::string m_sceneName;										// ���� �̸�
	//std::unordered_map<UID, std::shared_ptr<Entity>> m_pEntities;	// ���� �����ϴ� ��ƼƼ ����
	entt::registry& m_registry;										// ��ƼƼ ������Ʈ��
	// 	std::vector<std::shared_ptr<System>> m_pSystems;				// ���� �����ϴ� �ý��� ���

	RenderManager* m_pRenderManager;
	PhysicsManager* m_pPhysicsManager;
	InputManager* m_pInputManager;
	WorldManager* m_pWorldManager;
	UIManager* m_pUIManager;
	SoundManager* m_pSoundManager;
	EntityManager* m_pEntityManager;
	ResourceManager* m_pResourceManager;

	IData* m_pSceneData;
};

