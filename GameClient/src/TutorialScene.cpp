#include "TutorialScene.h"
#include "DearsGameEngineAPI.h"
#include "PhysicsManager.h"
#include "ResourceManager.h"
#include "PlayerComponent.h"
#include "EnemyComponent.h"
#include "WeaponComponent.h"
#include "ProjectileComponent.h"
#include "CustomFilter.h"
#include "MoneyGunComponent.h"
#include "IndicatorComponent.h"
#include "NPCComponent.h"
#include "SceneNumEnum.h"
#include "SceneTable.h"

TutorialScene::TutorialScene(entt::registry& _registry, const std::string& _name
	, EventManager* _pEventManager, RenderManager* _pRenderManager
	, PhysicsManager* _pPhysicsManager, InputManager* _pInputManager, WorldManager* _worldManager
	, UIManager* _UIManager, EntityManager* _pEntityManager, ResourceManager* _pResourceManager
	, SoundManager* _pSoundManager)
	: Scene(_registry, _name, _pEventManager, _pRenderManager, _pPhysicsManager
		, _pInputManager, _worldManager, _UIManager, _pEntityManager, _pResourceManager, _pSoundManager)
{
	mpUnitSystem = nullptr;
	mpLevelManager = nullptr;
	mpAstar = nullptr;
}

bool TutorialScene::Initialize()
{
	/// ���ҽ� �߰�
	AddResource();
	// 	m_pRenderManager->SetCubeMap("Atrium_diffuseIBL.dds", "Atrium_specularIBL.dds");

		// �������� ����
	m_pSceneData = new SceneData();
	SceneData* pSceneData = dynamic_cast<SceneData*>(m_pSceneData);
	auto sceneTable = m_pResourceManager->ParseCSV<SceneTable>("StageVariables.csv");
	for (auto& table : sceneTable)
	{
		if (m_mapNum == table.sceneNum)
		{
			pSceneData->m_heldMoney = table.sceneMoney;
			pSceneData->m_totalAmount = table.sceneMoney;
			pSceneData->m_rankA = table.standardRankA;
			pSceneData->m_rankB = table.standardRankB;
			pSceneData->m_time = table.time;
		}
	}
	/// mpAstar �ʱ�ȭ : ������ �� �Ŵ��� �ȿ� �ִ� �� ������ ���� ���� �Ŵ����� ��� �κ� ó���ϰ� �־ ������ ���� ���� ���´�.
	// ���� �ð��� ���� �� ���� ������ �ٲ㺸��.
	mpAstar = new AStar();
	std::vector<std::vector<int>>* astarMap = new std::vector<std::vector<int>>();

	*astarMap = m_pResourceManager->ParseMapCSV(m_stageName + std::string(".csv"));

	mpAstar->Initialize(Vector2(-15.0f), Vector2(15.0f), astarMap);

	/// �Ŵ��� �� �ý��� �ʱ�ȭ
	// ���� �ý��� ����(����ü, �÷��̾�, NPC �ý��� �� ��� �Ŵ��� �ʱ�ȭ)
	mpUnitSystem = new UnitSystem(m_registry, m_pEntityManager, m_pPhysicsManager, m_pInputManager, m_pWorldManager
		, m_pRenderManager, m_pResourceManager, m_pEventManager, m_pUIManager, m_pSoundManager, mpAstar);
	mpUnitSystem->Initialize();

	mpLevelManager = new LevelManager(m_registry, m_pRenderManager, m_pPhysicsManager, m_pInputManager, m_pUIManager, m_pEntityManager
		, m_pResourceManager, m_pSoundManager, mpUnitSystem, mpAstar);
	mpLevelManager->Initialize(GetUID());
	/// �⺻���� ȯ�� ����
	// ī�޶� ��ƼƼ ����, ����
	//mpLevelManager->AddCamera(Vector3(-16.46, 18.06, -16.62), Vector3(-1.f, -0.2f, 0.0f));
	mpLevelManager->AddCamera(mpLevelManager->GetCameraPosInfo().startPos, mpLevelManager->GetCameraPosInfo().startRot);

	// ����Ʈ ����
	mpLevelManager->AddLight();
	// ť��� ����
	mpLevelManager->AddCubeMap(m_mapNum);

	/// ������Ʈ ����
	//mpLevelManager->AddCubeMap();
	// ���� �� ��ġ(���Ÿ� ���� ����)
	mpLevelManager->AddPlaneNWall();
	// �ٴ� ��ǥ�� ��ġ
	// mpLevelManager->DrawCoordinatesXZ(true);
	mpLevelManager->DrawCoordinatesXZ(false);
	// �ε������� Ÿ�� ����
	mpLevelManager->AddSquareAtPointer("blue.png", "red.png");
	mpLevelManager->AddPlaceIndicator();

	// �� ������ �߰�[��ȹ�� �� �ѹ�]
	mpLevelManager->AddMapData(m_mapNum);
	// ���ּ��� --------------------------------------------------------
	// ���� ���� ����
	mpUnitSystem->GetPlayerSystem()->AddSceneEnemyUnits(m_mapNum);
	// 	auto enemy = m_pEntityManager->CreateEntity("enemy");
	// 	mpUnitSystem->GetPlayerSystem()->AddUnit(enemy, "RVC_0", Vector3(10.f, 0.f, -10.f));
		//mpUnitSystem->GetPlayerSystem()->AddUnit(enemy, "RVC_R1", Vector3(10.f, 0.f, -10.f));
		// NPC ����
	mpUnitSystem->GetNPCSystem()->AddNPC("Chief"); // �Ʊ� �뺴��
	mpUnitSystem->GetNPCSystem()->AddNPC("RVC_C"); // ���� �뺴��

	// ���� ����
	mpLevelManager->AddFireCracker();

	if (m_mapNum == 4 || m_mapNum == 5)
	{
		mpLevelManager->AddSmog();
	}

	// ����ŸƮ Ȯ��
	if (m_pWorldManager->GetCurrentWorld()->GetCurrentScene()->GetUID() == m_pWorldManager->GetCurrentWorld()->GetPreviousScene())
	{
		mpLevelManager->mIsRestart = true;
	}

	/// UI
	mpLevelManager->AddBasicUI();
	mpLevelManager->AddBattleUI(pSceneData);
	mpLevelManager->AddStageNumAndObjects(m_u8StageName);
	mpLevelManager->AddClassUI(2, false); // UI Ŭ���� ��ư
	mpLevelManager->AddVerifyPopUpUI();
	mpLevelManager->AddAnimationUI(true, static_cast<int>(m_pWorldManager->GetCurrentWorld()->GetPreviousScene()));
	mpLevelManager->SetUIAnimationState(UIAnimationState::PREBATTLE); // UI �ִϸ��̼� ���� ����

	/// Sound
	m_pSoundManager->StopBGM();
	m_pSoundManager->PlayBGM("Snd_bgm_BeforeBattle");
	return Scene::Initialize();
}

void TutorialScene::FixedUpdate(float _dTime)
{
	if (mpLevelManager->GetGameState() == GameState::NORMAL || mpLevelManager->GetGameState() == GameState::RESULT)
	{
		mpLevelManager->UIAniUpdate(_dTime);

		switch (mpLevelManager->GetUIAnimationState())
		{
		case UIAnimationState::VISIBLE:
		{
			auto view = m_registry.view<Texture2D>();
			for (auto& entity : view)
			{
				auto& name = m_registry.get<Name>(entity).m_name;
				auto& texture = m_registry.get<Texture2D>(entity);
				if (name.find("UI_A_Cloud") != std::string::npos)
				{
					texture.m_isVisible = true;
				}
			}
			mpLevelManager->SetUIAnimationState(UIAnimationState::POSTBATTLE);
		}
		break;
		default:
			break;
		}
	}
	else if (mpLevelManager->GetGameState() == GameState::START_ANIMATION)
	{
		mpLevelManager->StartAnimationFixedUpdate(_dTime);
	}
	else if (mpLevelManager->GetGameState() == GameState::PLAY)
	{
		mpLevelManager->PlayFixedUpdate(_dTime);
	}
	else if (mpLevelManager->GetGameState() == GameState::POST_PLAY_ANIMATION)
	{
		mpLevelManager->PostAnimationFixedUpdate(_dTime);
	}
}

void TutorialScene::Update(float _dTime)
{
	SceneData* pSceneData = dynamic_cast<SceneData*>(m_pSceneData);

	mpLevelManager->BasicUIUpdate();
	mpLevelManager->PopUpUIUpdate(pSceneData);

	if (mpLevelManager->mIsGoMain == true)
	{
		m_pWorldManager->GetCurrentWorld()->SetScene(static_cast<uint32_t>(SceneName::MAIN));
		return;
	}

	/// �÷��̾� �ý��� ������Ʈ
	switch (mpLevelManager->GetGameState())
	{
	case GameState::NORMAL:
	{
		mpLevelManager->StartNoneUI();
	}
	break;
	case GameState::PRE_PLACEMENT:
	{
		mpLevelManager->PrePlacementUpdate(pSceneData);
	}
	break;
	case GameState::PLACEMENT:
	{
		mpLevelManager->PlacementUpdate(pSceneData, _dTime);
	}
	break;
	case GameState::PRE_PLAY:
	{
		mpLevelManager->PreplayUpdate(pSceneData);
	}
	break;
	case GameState::PLAY:
	{
		mpUnitSystem->GetProjectileSystem()->Update(_dTime);
		mpLevelManager->PlayUpdate(pSceneData, _dTime);
		mpLevelManager->UpdateParticle(_dTime);
	}
	break;
	case GameState::NONE_UI:
	{
		mpLevelManager->NoneUI();
		mpUnitSystem->GetProjectileSystem()->Finalize();
		int a = m_pWorldManager->GetCurrentWorld()->GetCurrentScene()->GetUID();
		int b = static_cast<uint32_t>(SceneName::LAST);
		if (static_cast<uint32_t>(m_pWorldManager->GetCurrentWorld()->GetCurrentScene()->GetUID()) == static_cast<uint32_t>(SceneName::LAST) - 1)
		{
			mpLevelManager->mIsFinalStage = true;
		}
	}
	break;
	/*case GameState::POST_PLAY_ANIMATION:
	{
		mpLevelManager->PostAnimationUpdate(_dTime);
	}
	break;*/
	case GameState::POST_PLAY:
	{
		mpLevelManager->PostplayUpdate(pSceneData);
		mpUnitSystem->GetProjectileSystem()->Finalize();
	}
	break;
	case GameState::RESULT:
	{
		mpLevelManager->ResultUpdate(pSceneData, _dTime);

		if (mpLevelManager->mIsGoMain == true)
		{
			m_pWorldManager->GetCurrentWorld()->SetScene(static_cast<uint32_t>(SceneName::MAIN));
			return;
		}

		if (mpLevelManager->GetUIAnimationState() == UIAnimationState::NEXTSCENE)
		{
			m_pWorldManager->GetCurrentWorld()->SetScene(static_cast<uint32_t>(SceneName::LOADING));
			return;
		}

		if (mpLevelManager->mIsGameEnding == true)
		{
			m_pWorldManager->GetCurrentWorld()->SetScene(static_cast<uint32_t>(SceneName::CREDIT));
			return;
		}
	}
	break;
	case GameState::RESTART:
	{
		mpLevelManager->SetUIAnimationState(UIAnimationState::UNVISIBLE);
		m_pWorldManager->GetCurrentWorld()->SetScene(m_pWorldManager->GetCurrentWorld()->GetCurrentScene()->GetUID());
	}
	break;
	default:
		break;
	}

}

void TutorialScene::LateUpdate(float _dTime)
{
	auto cameraView = m_registry.view<CameraComponent>();
	for (auto entity : cameraView)
	{
		auto& camera = m_registry.get<CameraComponent>(entity);
		// ī�޶��� ���콺 ��ġ ������Ʈ
		if (camera.m_cameraEnum == 0)
		{
			// 			camera.m_pCamera->OnMouseMove(static_cast<int>(m_pInputManager->GetMousePos().x), static_cast<int>(m_pInputManager->GetMousePos().y));
			// 			if (m_pInputManager->GetKey(KEY::F))
			// 			{
			// 				if (camera.m_pCamera->mIsFirstPersonMode)
			// 				{
			// 					camera.m_pCamera->mIsFirstPersonMode = false;
			// 				}
			// 				else
			// 				{
			// 					camera.m_pCamera->mIsFirstPersonMode = true;
			// 					camera.m_pCamera->PrintCamInfo();
			// 				}
			// 			}
		}
	}

	if (m_pInputManager->GetKeyUp(KEY::M))
	{
		auto camera = mpLevelManager->GetWorldCamera();
		camera->SetEyePos(Vector3(0, 17, -17.5));
		camera->SetDirection(Vector3(0, -0.8, 0.6));

		m_pRenderManager->CameraSetPerspective();
	}
	if (m_pInputManager->GetKeyUp(KEY::N))
	{
		auto camera = mpLevelManager->GetWorldCamera();
		camera->SetEyePos(Vector3(-15.42, 18.06, -17.72));
		camera->SetDirection(Vector3(0.57735, -0.57735, 0.57735));

		m_pRenderManager->CameraSetOrthographic(0.03);
	}

	/// �ӽ÷� ������ѳ��� ���� ����� ���� ��ư
	if (m_pInputManager->GetKeyUp(KEY::RIGHT))
	{
		mpUnitSystem->GetPlayerSystem()->mpStatusManager->mMoneyBuffCoef += 0.05f;
	}
	if (m_pInputManager->GetKeyUp(KEY::LEFT))
	{
		mpUnitSystem->GetPlayerSystem()->mpStatusManager->mMoneyBuffCoef -= 0.05f;
	}

}

void TutorialScene::Finalize()
{
	mpUnitSystem->GetPlayerSystem()->Finalize();
	mpLevelManager->Finalize();
	Scene::Finalize();
	if (mpLevelManager)
	{
		delete mpLevelManager;
	}
	if (mpUnitSystem)
	{
		delete mpUnitSystem;
	}
	if (mpAstar)
	{
		delete mpAstar;
	}
	if (m_pSceneData)
	{
		delete m_pSceneData;
	}
}

void TutorialScene::AddResource()
{
	//m_pResourceManager->GetFileManager()->PrintAll(false);
	/// ���ҽ� �߰�
	// �� �߰�
	//m_pRenderManager->AddModel("../TestAsset/", "box.fbx");

	/////UI
	//m_pUIManager->AddTexture2D("../Resources/Texture/", "blue.png");

	//// �ִϸ��̼� �߰�
	//m_pResourceManager->AddFilesInDirAni("Animation");

	//// 3D �ؽ�ó �߰�
	//m_pResourceManager->AddFilesInDir3D("Texture3D");

	//// ť��� �ؽ�ó �߰�
	//m_pResourceManager->AddFilesInDirDDS("CubeMap");

	//// Fbx �߰� -> ���� Ŭ���� ���� �ɰ� ��
	//m_pResourceManager->AddFilesInDirModel("Mercenary");
	//m_pResourceManager->AddFilesInDirModel("Environment");
	//m_pResourceManager->AddFilesInDirModel("Archer");
	//m_pResourceManager->AddFilesInDirModel("Enemy");
	//m_pResourceManager->AddFilesInDirModel("money");
	//m_pResourceManager->AddFilesInDirModel("Chief");
}

void TutorialScene::SetMapInfo(uint8_t _num, string _stageName)
{
	m_mapNum = _num;
	m_stageName = _stageName;
	m_u8StageName = u8string(m_stageName.begin(), m_stageName.end());
}
