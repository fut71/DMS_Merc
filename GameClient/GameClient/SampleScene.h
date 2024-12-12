#pragma once
#include "Scene.h"

/// <summary>
/// ���ο� ��ŰŸ���� ���� ������� Scene Ŭ������ ���
/// </summary>
class SampleScene :
	public Scene
{
public:
	/// <summary>
	/// ������ �⺻������ ���Ǵ� ��ü���� �����ڿ��� �������ݴϴ�.
	/// </summary>
	SampleScene(entt::registry& _registry, const std::string& _name
		, EventManager* _pEventManager, RenderManager* _pRenderManager
		, PhysicsManager* _pPhysicsManager, InputManager* _pInpueManager
		, WorldManager* _pWorldManager, UIManager* _UIManager
		, EntityManager* _pEntityManager, ResourceManager* _pResourceManage);

	/// <summary>
	/// �߰��� ����� ����� ��ü�� �ִٸ� �����ڸ� ���� �����մϴ�.
	/// </summary>
	// SampleScene(entt::registry& _registry, const std::string& _name
	// 	, EventManager* _pEventManager, RenderManager* _pRenderManager
	// 	, PhysicsManager* _pPhysicsManager, InputManager* _pInpueManager
	// 	, const int& _i, const float& _f);

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

private:
	void DoSomething(const Event& _event);
};

