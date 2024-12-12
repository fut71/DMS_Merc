#pragma once
#include "Scene.h"
#include "LevelManager.h"
#include "EasingFunc.h"
/// <summary>
/// ������ ���� �޴��� �ش��ϴ� ���Դϴ�.
/// </summary>

class MainMenuScene :
	public Scene
{
public:
	/// <summary>
	/// ������ �⺻������ ���Ǵ� ��ü���� �����ڿ��� �������ݴϴ�.
	/// </summary>
	MainMenuScene(entt::registry& _registry, const std::string& _name
		, EventManager* _pEventManager, RenderManager* _pRenderManager
		, PhysicsManager* _pPhysicsManager, InputManager* _pInpueManager
		, WorldManager* _worldManager, UIManager* _pUIManager, EntityManager* _pEntityManager
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
	/// <summary>
	/// ��¡ �Լ��� �������� ����
	/// </summary>
	EasingFunc* tempEasing;

	/// <summary>
	/// ������ ĸ��ȭ�� ���Ѿ��ϳ� ������ �ð��� ������ �� �ߺ����� �����ڴ´�
	/// </summary>
	float mtime = 0;
	float mtime2 = 0;
	float mTimeExit = 0.0f;
	//bool mCanMove = false;

	// ���� �Ŵ����� ��� ���� UI�� ��ġ�� �����մϴ�. ���� �ð��� ������ �� �Ŵ����� �и��ؾ� �մϴ�.
	LevelManager* mpLevelManager;

	// ���� ����� ���õ� ���ҽ� �߰� �Լ�
	void AddExplainUI();
	bool mIsPressedExplainButton = false;
};

