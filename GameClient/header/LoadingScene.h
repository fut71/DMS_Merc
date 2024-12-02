#pragma once
#include "Scene.h"
#include "LevelManager.h"
#include "EasingFunc.h"
/// <summary>
/// ������ ���� �޴��� �ش��ϴ� ���Դϴ�.
/// </summary>

class LoadingScene :
	public Scene
{
public:
	/// <summary>
	/// ������ �⺻������ ���Ǵ� ��ü���� �����ڿ��� �������ݴϴ�.
	/// </summary>
	LoadingScene(entt::registry& _registry, const std::string& _name
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
	float accumulateTime;
	bool mIsResourceLoaded1=false;
	bool mIsResourceLoaded2=false;
	bool mIsResourceLoaded3=false;
	bool mIsResourceLoaded4=false;
	bool mIsResourceLoaded5=false;
	// �̹� ���ҽ����� �ε� �߾�����
	bool mIsResourcesAlreadyLoaded = false;

	float loadingTextInterval = 1.0f / 3.0f; // 0.33�ʸ��� ����
	float maxTimeforPercetage;
	LevelManager* mpLevelManager;

	void LoadingTextUpdate();
	void LoadingResourceUpdate();

	float tempTime = 0.0f;
	float mLoadingMaxSize;
};

