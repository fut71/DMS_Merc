#pragma once
#include "Scene.h"

/// <summary>
/// ���� �� Ŭ�����Դϴ�.
/// </summary>
class BattleScene
	: public Scene
{
public:
	/// <summary>
	/// ������ �⺻������ ���Ǵ� ��ü���� �����ڿ��� �������ݴϴ�.
	/// </summary>
	BattleScene(entt::registry& _registry, const std::string& _name
		, EventManager* _pEventManager, RenderManager* _pRenderManager
		, PhysicsManager* _pPhysicsManager, InputManager* _pInpueManager
		, WorldManager* _worldManager, UIManager* _UIManager, EntityManager* _pEntityManager
		, ResourceManager* _pResourceManager);

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


	void CreateSomething(const Event& _event);

};

