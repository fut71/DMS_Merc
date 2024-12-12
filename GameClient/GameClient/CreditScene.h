#pragma once
#include "Scene.h"
#include "LevelManager.h"
#include "EasingFunc.h"

class CreditScene :
	public Scene
{
public:
	/// <summary>
	/// ������ �⺻������ ���Ǵ� ��ü���� �����ڿ��� �������ݴϴ�.
	/// </summary>
	CreditScene(entt::registry& _registry, const std::string& _name
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
	float accumulateTime;
	bool mIsCreditFinish = false;
	float mBGMFadeOutVolume = 0.5f;
	// ���� �Ŵ����� ��� ���� UI�� ��ġ�� �����մϴ�. ���� �ð��� ������ �� �Ŵ����� �и��ؾ� �մϴ�.
	LevelManager* mpLevelManager;
};
