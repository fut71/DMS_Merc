#pragma once
#include "pch.h"
#include "Scene.h"
#include "EventListener.h"

class IData;
class RenderManager;
class PhysicsManager;

/// <summary>
/// ���� ����Ʈ ����, ���̽� Ŭ������ Scene���� Ȯ���մϴ�.
/// </summary>
template <typename T>
concept IsScene = std::is_base_of<Scene, T>::value
|| std::is_same<Scene, T>::value;

/// <summary>
/// ���� ��ü�� �ش��ϴ� ���� ������Ʈ Ŭ�����Դϴ�.
/// ������ ����� ������ ȯ�� ���� ������ ������ �ֽ��ϴ�.
/// </summary>
class World
	: public EventListener
{
public:
	World() = delete;
	// World(entt::registry& _registry);
	World(entt::registry& _registry, EventManager* _pEventManager);
	virtual ~World() = default;

	/// <summary>
	/// ���带 �ʱ�ȭ�մϴ�.
	/// </summary>
	/// <returns></returns>
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
	/// ���带 �����ϰ� �����մϴ�.
	/// </summary>
	virtual void Finalize();

	/// <summary>
	/// ���忡 ���� �߰��մϴ�.
	/// </summary>
	/// <param name="_pScene">�߰��� �� ��ü</param>
	std::shared_ptr<Scene> AddScene(std::shared_ptr<Scene> _pScene);

	/// <summary>
	/// ���忡 ���ο� ���� �߰��մϴ�.
	/// </summary>
	/// <param name="_name">�� �̸�</param>
	/// <returns>������ �� ��ü</returns>
	//std::shared_ptr<Scene> CreateScene(const std::string& _name);

	/// <summary>
	/// ���忡�� ���� �����մϴ�.
	/// </summary>
	/// <param name="_pScene">������ �� ��ü</param>
	void RemoveScene(std::shared_ptr<Scene> _pScene);

	/// <summary>
	/// ���� ���� _uid�� �ش��ϴ� ������ �����մϴ�.
	/// </summary>
	/// <param name="_uid">������ ���� UID</param>
	void SetScene(const UID& _uid);

	/// <summary>
	/// ���� ���� _uid�� �ش��ϴ� ������ �����մϴ�.
	/// </summary>
	/// <param name="_uid">������ ���� �̸�</param>
	void SetScene(const std::string& _name);

	/// <summary>
	/// �� ���� ��ȯ�մϴ�.
	/// </summary>
	/// <returns></returns>
	std::unordered_map<UID, std::shared_ptr<Scene>> GetSceneMap() const;

	/// <summary>
	/// ���� ���� ��ȯ�մϴ�.
	/// </summary>
	/// <returns>���� �� ��ü</returns>
	std::shared_ptr<Scene> GetCurrentScene();

	/// <summary>
	/// ���� ���� ��ȯ�մϴ�.
	/// </summary>
	/// <returns>���� �� UID</returns>
	UID GetPreviousScene();

	/// <summary>
	/// ������ UID�� ��ȯ�մϴ�.
	/// </summary>
	/// <returns>���� UID</returns>
	UID GetUID() const;

	std::shared_ptr<IData> GetWorldData(const std::string& _name);

protected:
	UID m_uid;													// ��ü�� �����ĺ���
	std::unordered_map<UID, std::shared_ptr<Scene>> m_pScenes;	// ���忡 �����ϴ� �� ����
	std::shared_ptr<Scene> m_pCurrentScene;						// ���� ��
	std::unordered_map<std::string, std::shared_ptr<IData>> m_pWorldData;						// ���忡�� ���Ǵ� �����͵�
	entt::registry& m_registry;									// ��ƼƼ ������Ʈ��
	UID m_previousScene;										// ���� ��
};
