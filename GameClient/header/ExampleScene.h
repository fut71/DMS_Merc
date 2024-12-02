#pragma once
#include "Scene.h"
#include "UnitSystem.h"
#include "PlayerSystem.h"
#include "ProjectileSystem.h"
#include "NPCSystem.h"
#include "SceneData.h"
#include "LevelManager.h"
#include "TutotialTextTable.h"
#include "TutorialEnum.h"

class ExampleScene
	: public Scene
{
public:
	/// <summary>
	/// ������ �⺻������ ���Ǵ� ��ü���� �����ڿ��� �������ݴϴ�.
	/// </summary>
	ExampleScene(entt::registry& _registry, const std::string& _name
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

	// �ε������Ϳ� ��ġ������ ���� �� ���� �����ؼ� �۵�
	void SettingDeployIndicator(Vector3 _cursorPos, SceneData* _pSceneData);
	void SettingPlayingIndicator();
	void SettingRealPlayingIndicator(SceneData* _pSceneData);
	 
private:
	UnitSystem* mpUnitSystem;
	LevelManager* mpLevelManager;
	AStar* mpAstar;
	std::vector<TutotialTextTable> mTutorialTextVec;
	size_t mTextIdx; // �ؽ�Ʈ ������ �ε���
	Vector2 mStartPoint;
	Vector2 mEndPoint;

	TutorialState mTutorialState;
	float mTime; // �ؽ�Ʈ �ڵ� ����� ���� Ÿ�� + ui �ִϸ��̼ǿ��� ���
	//float mPressClickTime = 0.0f;

	//Ÿ�� �� �� ��ġ ��ȯ
	void ChangeGuideTile(const std::string& _texFileName, Vector2 _posXZ, Vector2 _sizeXZ);
	void ChangeGuideTileTrs(Vector2 _posXZ, Vector2 _sizeXZ);
	void InvisibleGuideTile();
	void ChangeGuideTileTex(const std::string& _texFileName);

	// �ؽ�Ʈ
	void PrintIntroText(TutorialState _tutorialState);
	void SkipText(int _endIdx);

	// ���� �簢��
	void UpdateShrinkRect(float _dTime);
	void ResettingShrinkRect(Vector2 _targetRectPos, Vector2 _targetSize, int _sizeMultiplier, int _shrinkAmount);
	bool mIsReadyResetShrinkRect;
};