#pragma once
#include <vector>
#include "Utility.h"
#include "CustomFilter.h"
#include "RenderManager.h"
#include "PhysicsManager.h"
#include "UIManager.h"
#include "EntityManager.h"
#include "SoundManager.h"
#include "UnitSystem.h"
#include "SceneData.h"
#include "MapData.h"
#include "EnemyData.h"
#include "GameStateEnum.h"
#include "AStar.h"
#include "EasingFunc.h"
#include "CameraPosition.h"

/// <summary>
/// �������� �� ��ġ��, �� ������ �ߺ��Ǵ� �Լ��� �����ϴ� �˸� ���� �Ŵ����Դϴ�.
/// Ŭ���� ���õ� bool �� : Ŭ�� ������ ����  true, �Ұ����� ���� false;
/// </summary>
class LevelManager
{
public:
	// ��Ʋ ������ ����ϴ� LevelManager
	LevelManager(entt::registry& _registry, RenderManager* _pRenderManager
		, PhysicsManager* _pPhysicsManager, InputManager* _pInputManager
		, UIManager* _UIManager, EntityManager* _pEntityManager
		, ResourceManager* _pResourceManager, SoundManager* _pSoundManager, UnitSystem* _pUnitSystem, AStar* _pAstar);
	// �Ϲ� ������ ����ϴ� LevelManager -> ������Ʈ���� �����ؼ� ��� ������ ���� ������ ������ �ʱ�ȭ �ؾ� ��
	LevelManager(entt::registry& _registry, RenderManager* _pRenderManager, InputManager* _pInputManager, UIManager* _UIManager
		, EntityManager* _pEntityManager, ResourceManager* _pResourceManager, SoundManager* _pSoundManager);
	~LevelManager();

	// FBX�� ���� ���� �� ���� ������ ����ϴ� �ʱ�ȭ�Դϴ�.
	void InitializeforNoneFbx(const UID& _sceneUID);
	// �ʱ�ȭ (���� �ѹ��� �ϰ�, ��� ���� ���� �ڷḦ csv�� �о �����صд�.
	void Initialize(const UID& _sceneUID);
	// ��������� �߰��� �Լ� (�� �ѹ� ����)
	void SetSceneUID(const UID& _sceneUID);

	// ����� �� �ѹ� �ʱ�ȭ
	void Finalize();

	/// �ʱ�ȭ�� ���Ǵ� �Լ�
	// ī�޶� �����մϴ�.
	void AddCamera(const Vector3& _pos, const Vector3& _viewDir);
	// ����Ʈ�� �����մϴ�.
	void AddLight();
	// ť����� �����մϴ�.
	void AddCubeMap(uint8_t _mapNum);
	// �ʿ� ������Ʈ�� ��ġ�Ѵ�.
	void AddMapData(const int& _sceneNum);
	// �⺻���� ���� �ٴڰ� ������ �ܰ� ���� ���� �����Ѵ�.
	void AddPlaneNWall();
	// �ε������Ϳ� Ÿ���� �����Ѵ�.
	void AddSquareAtPointer(const std::string& _originalTexture, const std::string& _changedTexture);

	// ��ġ ���� ����
	void AddPlaceIndicator();

	/// <summary>
	///��ġ ���� ǥ�� Ȱ��ȭ
	/// </summary>
	/// <param name="_on">true: on / false: off</param>
	void DisablePlaceIndicator();

	/// ���� �⺻���� ���ÿ� ���Ǵ� �Լ�
	// ������ ���Ǵ� UI�� �߰��մϴ�.(ȯ�漳�� ��ư ����)
	void AddBasicUI();
	// ��Ʋ���� ���Ǵ� UI�� �߰��մϴ�.
	void AddBattleUI(SceneData* _pSceneData);
	// ������������ �ٲ�� ��Ҵ� ���� UI�� �߰��մϴ�.
	void AddStageNumAndObjects(const std::u8string& _stageNum);
	// UI Ŭ���� ��ư�� �����մϴ�.[Ȱ��ȭ�� Ŭ������ ����]
	void AddClassUI(const int& _activeClassNum, bool _isToturial);
	// Ȯ�� �˾� â�� ���� UI�Դϴ�.
	void AddVerifyPopUpUI();
	// �⺻���� UIAnimation�� ���� ���Ǵ� UI�Դϴ�.
	void AddAnimationUI(bool _isBettelScene, int _priviusSceneNum);

	/// ������Ʈ�� ���Ǵ� �Լ�
	void BasicUIUpdate();
	void PopUpUIUpdate(SceneData* _pSceneData);
	void PopUpUIMainUpdate();
	void PopUPUITutorial();
	void UIAniUpdate(float _dTime);

	std::vector<pair<float, float*>> mEasingVec;
	// ��� ui�� ������ϴ�.
	void PushAllUI();

	void StartNoneUI();
	void PrePlacementUpdate(SceneData* _pSceneData);
	void PlacementUpdate(SceneData* _pSceneData, float _dTime);
	void PreplayUpdate(SceneData* _pSceneData);
	void PlayUpdate(SceneData* _pSceneData, float _dTime);
	void NoneUI();
	void PostplayUpdate(SceneData* _pSceneData);
	void ResultUpdate(SceneData* _pSceneData, float _dTime);

	void UpdateParticle(float _dTime);

	/// ������ ������ ������Ʈ�� ���Ǵ� �Լ���
	void StartAnimationFixedUpdate(float _dTime);
	void PlayFixedUpdate(float _dTime);
	void PostAnimationFixedUpdate(float _dTime);

	/// ���� ��ġ/ ���� �� ����ϴ� �Լ�
	// ������ Ư�� ��ġ�� ��ũ���� Ŭ������ ��, ���� ��ǥ�� ��ȯ���� �ʰ� ���´�. (������ �����̸� false)
	bool AddBlockScreenPos(const Vector2& _mousePos, const Vector2& _PosXlYu, const Vector2& _WH);
	Vector3 GetTargetPosition(Vector2 _mousePos, int _screenWidth, int _screenHeight, Vector3 pointOffset, float cameraDistance);
	bool IsCanClickAreaXZ(const Vector3& _realPos, const Vector2& _PosXlZu, const Vector2& _PosXrZd, const int& _aliveNum);

	/// ���� ������� ���� �Լ�
	void JumpingAndRotUnit();
	// ��ġ�� �÷��̾ �����ϴ� �Լ�
	void ResetPlayer(SceneData* _pSceneData);
	// �÷� ������Ʈ �Լ�
	void UpdateRandomColor(Vector4& _rgba, float _dTime, float _speed);

	/// ���� ���� �߿� ����ϴ� �Լ�
	// ���� ��ֹ� ������ ���콺 ��ġ�� ������, false�� ��ȯ�Ѵ�. (������ �����̸� false)
	bool AddObjectArea(const Vector2& _worldMousePos, const Vector2& _PosXlZu, const Vector2& _PosXrZd);
	// ��ũ �ٸ� ������Ʈ ��Ų��.
	void UpdateRankBar(SceneData* _pSceneData);

	/// ����뿡 ���Ǵ� �Լ�
	void DrawCoordinatesXZ(bool _isVisible);
	void CreateCoordinatesXZ();
	void KillAllEnemies();

	/// �� �� ���� ���¿� ���� ���Ǵ� �Լ�
	GameState GetGameState();
	void SetGameState(GameState _state);
	PopUpState GetPopUpState();
	void SetPopUpState(PopUpState _state);
	UIAnimationState GetUIAnimationState();
	void SetUIAnimationState(UIAnimationState _state);

	// ī�޶� ���� ���� �Լ�
	void CameraWalk(Vector3& _target, const Vector3& _start, const Vector3& _end, float _time);
	void CameraRotate(Vector3& _target, const Vector3& _start, const Vector3& _end, float _time);
	void CameraEasing(const std::string& _fucName, Vector3& _target, const Vector3& _start, const Vector3& _end, float _time);

	// ���� ������� ���� ī�޶� ������
	Camera* GetWorldCamera();

	// ȭ�� ���̵� ��/�ƿ�
	void FadePreSetting(bool _mIsFadeOut);
	void FadeInScreen(float _time);
	void FadeOutScreen(float _time);

	std::vector<EnemyData> GetSceneEnemyDataVec();
	bool mIsNext = false; // �������� ��ư�� ��������?
	bool mIsGoMain = false;
	bool mIsSkip = false;
	bool mIsExit = false;
	bool mIsClear = false;	// ���� �¸���
	bool mIsFinalStage = false;
	bool mIsEndingFade = false;
	bool mIsGameEnding = false;
	bool mIsRestart = false; // Restart �����ΰ�?

	/// Ʃ�丮�󿡼� ����� �Լ���
	void DeleteMoney();// ���� ����
	void EnemyUpdate(float _dTime);// ���� ������Ʈ
	void PlayerUpdate(float _dTime);// �Ʊ� ������Ʈ
	void DeleteEnemyAll(); // ���� ������Ʈ ����
	void TutorialClassDeploy();	// �ٰŸ� Ŭ���� �ϳ� ��ġ�ϴµ� ���� �Լ�
	void TutorialClassDeployed(Vector3& _cursorPos, bool _isPayment, SceneData* _pSceneData);
	void AllDeploy(Vector3& _cursorPos, bool _isPayment, SceneData* _pSceneData);
	
	// ����
	bool mTutorialFlag = false; // Ʃ�丮�� Ư�� �޼��� �� ������Ʈ ������ ���� �����ϴ� �÷���

	/// GET/SET -> Ʃ�丮�󿡼� ���
	bool GetIsGameStart();
	void SetIsGameStart(bool _isGameStart);
	bool GetmIsOneDeploySet();
	void SetmIsOneDeploySet(bool _isOneDeploySet);
	bool GetIsClassButtonPressed();
	void SetIsClassButtonPressed(bool _isClassButtonPressed);
	UID GetTempUnitUID();
	void SetTempUnitUID(UID _tempUnitID);
	Vector2 GetStartPoint();
	Vector2 GetEndPoint();
	std::string GetUnitClassName();
	void SetClassName(const std::string& _className);
	EasingFunc* mpEasingFunc;

	void AddFireCracker();
	CameraPosition GetCameraPosInfo();

	void AddSmog();
	void SmogUpdate(float _dTime);

private:
	entt::registry& mRegistry;

	std::vector<MapData> mMapDataVec; // ��� ���� ȯ���� �����ϴ� �����Դϴ�.
	std::vector<EnemyData> mEnemyDataVec; // ��� ���� ���� ��ġ�� �����ϴ� �����Դϴ�.
	UID mCurrentSceneUID;	// ������ �� ������ ����� �Ŷ�� �ʱ�ȭ �� �� �ѹ��� �ʱ�ȭ ���ش�.

	RenderManager* mpRenderManager;
	PhysicsManager* mpPhysicsManager;
	InputManager* mpInputManager;
	UIManager* mpUIManager;
	EntityManager* mpEntityManager;
	ResourceManager* mpResourceManager;
	SoundManager* mpSoundManager;
	AStar* mpAstar;
	UnitSystem* mpUnitSystem;
	

	// ���� ���� ���¸� ���� -> ���� �� ����ȭ �� �� ���� -> ��ġ �Ϸ�� ���� ���� �� ���� ���� �͵�.
	GameState currentLevelState;
	SettingState currentSettingState = SettingState::NORMAL;
	PopUpState currentPopUpState = PopUpState::NORMAL;
	UIAnimationState currentUIAniState = UIAnimationState::NORMAL;

	std::list<std::shared_ptr<Entity>> coordinateList;

	bool mIsGameStart = false; // ���� ������ �˸��� �÷��� : ��ġ�� ���� ���� ������ ����� ���� �÷���
	bool mIsOneDeploySet = false; // �ϳ��� ������ ��ġ�� �Ϸ�Ǿ������� Ȯ���ϴ� �÷���
	UID mTempUnitUID = -1;// Ŭ���� ������ ���� ���� ���� ��ƼƼ ������ ��´�. // �Ұ�... �츮�� ��ƼƼ �Ŵ����� UID�� ã�� �� ��...��..
	bool mIsClassButtonPressed = false;
	std::string mClassName = "";
	Vector2 mStartPoint;
	Vector2 mEndPoint;
	CameraPosition mCameraPosInfo;


	// ������ �����Ͽ� �����ϴ� �Լ�
	void SetVolume(const std::string& _fileName, const shared_ptr<Entity>& _entity, int& _intNum, int& _whatVolume);
	int BGVolume = 3;
	int ESVolume = 3;
	float tempTime = 0.f;
	bool mPlayWarningSound = false;
	float mUIAnimationTime = 0.0f;

	// �˾����� ������ �� �������� ó���ϴ� �Լ�
	void ManagePopupDisplay(PopUpState _state, PopUpState _state2 = PopUpState::LAST);

	float mResultEasingTimer = 0;
};

