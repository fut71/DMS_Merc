#include "ExampleScene.h"
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
#include "MessageBox2D.h"
#include "SceneNumEnum.h"
#include "LayerEnum.h"
#include "FadeInOut.h"
#include "ShrinkRectComponent.h"

ExampleScene::ExampleScene(entt::registry& _registry, const std::string& _name
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

	mTutorialTextVec = m_pResourceManager->ParseCSV<TutotialTextTable>("TutorialText2.csv");
	mTextIdx = 0;
	mTutorialState = TutorialState::FINAL;
	mTime = 0.0f;
}

bool ExampleScene::Initialize()
{
	/// ���ҽ� �߰�
	AddResource();
	m_pRenderManager->SetCubeMap("Atrium_diffuseIBL.dds", "Atrium_specularIBL.dds");

	// �������� ����
	m_pSceneData = new SceneData();

	/// mpAstar �ʱ�ȭ : ������ �� �Ŵ��� �ȿ� �ִ� �� ������ ���� ���� �Ŵ����� ��� �κ� ó���ϰ� �־ ������ ���� ���� ���´�.
	// ���� �ð��� ���� �� ���� ������ �ٲ㺸��.
	mpAstar = new AStar();
	std::vector<std::vector<int>>* astarMap = new std::vector<std::vector<int>>();
	*astarMap = m_pResourceManager->ParseMapCSV("Stage1.csv");
	mpAstar->Initialize(Vector2(-15.0f), Vector2(15.0f), astarMap);
	mStartPoint = mpAstar->GetMapStartPoint();
	mEndPoint = mpAstar->GetMapEndPoint();

	/// �Ŵ��� �� �ý��� �ʱ�ȭ
	// ���� �ý��� ����(����ü, �÷��̾�, NPC �ý��� �� ��� �Ŵ��� �ʱ�ȭ)
	mpUnitSystem = new UnitSystem(m_registry, m_pEntityManager, m_pPhysicsManager, m_pInputManager, m_pWorldManager
		, m_pRenderManager, m_pResourceManager, m_pEventManager, m_pUIManager, m_pSoundManager, mpAstar);
	mpUnitSystem->Initialize();

	mpLevelManager = new LevelManager(m_registry, m_pRenderManager, m_pPhysicsManager, m_pInputManager, m_pUIManager, m_pEntityManager
		, m_pResourceManager, m_pSoundManager, mpUnitSystem, mpAstar);
	mpLevelManager->Initialize(GetUID());
	mpLevelManager->SetGameState(GameState::PRE_PLACEMENT);

	/// �ؽ�Ʈ �ε��� �ʱ�ȭ
	mTextIdx = 0;

	/// �⺻���� ȯ�� ����
	// ī�޶� ��ƼƼ ����, ����
	mpLevelManager->AddCamera(Vector3(-12.f, 4.0f, 3.35f), Vector3(-1.f, -0.2f, 0.0f));
	// ����Ʈ ����
	mpLevelManager->AddLight();
	// ť��� ����
	mpLevelManager->AddCubeMap(1);

	auto camera = mpLevelManager->GetWorldCamera();
	camera->SetEyePos(Vector3(-15.42, 18.06, -17.72));
	camera->SetDirection(Vector3(0.57735, -0.57735, 0.57735));
	m_pRenderManager->CameraSetOrthographic(0.03);

	/// ������Ʈ ����
	// ���� �� ��ġ(���Ÿ� ���� ����)
	mpLevelManager->AddPlaneNWall();
	// �ٴ� ��ǥ�� ��ġ
	mpLevelManager->DrawCoordinatesXZ(true);
	// �ε������� Ÿ�� ����
	mpLevelManager->AddSquareAtPointer("blue.png", "red.png");
	// �� ������ �߰�[��ȹ�� �� �ѹ�]
	mpLevelManager->AddMapData(1);
	// ���ּ��� --------------------------------------------------------
	// ���� ���� ����
	mpUnitSystem->GetPlayerSystem()->AddSceneEnemyUnits(0);

	// NPC ����
	mpUnitSystem->GetNPCSystem()->AddNPC("Chief"); // �Ʊ� �뺴��
	mpUnitSystem->GetNPCSystem()->AddNPC("RVC_C"); // ���� �뺴��
	//
	auto ui1 = m_pEntityManager->CreateEntity("UI_T");
	m_pUIManager->AddUI(ui1, "UI_pnl_MsgBoxClick1.png", Vector2(193, 489), Vector2(432, 210), static_cast<int>(Layer::COM_B), Vector4(1.0f));
	m_pUIManager->AddText(ui1, u8"�ȳ�, ���� ��ġ ��Ŀ����� ��", "KIMM_Bold(60).ttf", Vector2(219, 554), static_cast<int>(Layer::COM_B), true, Vector4(1.0f));
	ui1->GetComponent<Text>().m_scale = 0.33f;

	auto ui2 = m_pEntityManager->CreateEntity("UI_T_Skip");
	m_pUIManager->AddUI(ui2, "UI_btn_Skip.png", Vector2(1812, 997), Vector2(83, 58), static_cast<int>(Layer::COM_B), Vector4(1.0f));
	m_pUIManager->AddButtonAllColor(ui2, Vector4(0.5f, 0.5f, 0.5f, 1.0f), Vector4(1.0f));

	auto ui3 = m_pEntityManager->CreateEntity("UI_PointMoney");
	m_pUIManager->AddUI(ui3, "UI_PointMoney.png", Vector2(915, 490), Vector2(90, 100), static_cast<int>(Layer::EFFECT), Vector4(1.0f), false);

	auto ui4 = m_pEntityManager->CreateEntity("UI_H"); // ��ġ�� ���� ����
	ui4->AddComponent<Transform>(Vector3(5.f, 0.f, 0.f), Vector3(0.f), Vector3(0.7f, 0.005f, 0.7f));
	ui4->AddComponent<MeshRenderer>("box.fbx", "Cube.001");
	ui4->AddComponent<Texture3D>("White.png");
	ui4->AddComponent<AlphaBlendComponent>(0.2);
 	ui4->AddComponent<OutlineComponent>(1.05, Vector3(0, 0, 1));

	// ��ֹ� ��ƼƼ -> �ʹݿ� ���Դٰ� �ٷ� ���ŵ� �༮
	auto ui5 = m_pEntityManager->CreateEntity("UI_H_Obstacle"); // ��ġ�� ���� ����
	ui5->AddComponent<Transform>(Vector3(5.f, 0.f, 0.f), Vector3(0.f), Vector3(0.5f, 1.0f, 0.5f));
	ui5->AddComponent<MeshRenderer>("box.fbx", "Cube.001");
	ui5->AddComponent<Texture3D>("Log.png");

	// �簢�� ����Ʈ ��ƼƼ
	auto effectRect = m_pEntityManager->CreateEntity("UI_Effect_Rect");
	m_pUIManager->AddRect(effectRect, Vector2(925, 410), Vector2(200.f), static_cast<int>(Layer::COM_M), Vector4(0.0f), 8.0f, Vector4(1.0f, 0.38f, 0.0f, 1.0f));
	effectRect->AddComponent<ShrinkRectComponent>(Vector2(200.f), Vector2(100.f), 20);
	effectRect->GetComponent<Box2D>().mIsVisible = false;
	mIsReadyResetShrinkRect = true;

	/*auto ui5 = m_pEntityManager->CreateEntity("sssss");
	m_pUIManager->AddText(ui5, u8"�ȳ�,\n ����\n��ġ \n��Ŀ�����\n ��", "KIMM_Bold(60).ttf", Vector2(0, 0), static_cast<int>(Layer::COM_B), true);
	ui1->GetComponent<Text>().m_scale = 0.7f;*/

	/// UI
	mpLevelManager->AddBasicUI();
	mpLevelManager->AddBattleUI(dynamic_cast<SceneData*>(m_pSceneData));
	mpLevelManager->AddStageNumAndObjects(u8"Ʃ�丮��");
	mpLevelManager->AddClassUI(0, true); // UI Ŭ���� ��ư
	mpLevelManager->AddVerifyPopUpUI();
	mpLevelManager->AddAnimationUI(true, static_cast<int>(m_pWorldManager->GetCurrentWorld()->GetPreviousScene()));

	/// Ʃ�丮�� ���� ����
	mTutorialState = TutorialState::CLOUD;

	/// UI �ִϸ��̼� ���� ����
	mpLevelManager->SetUIAnimationState(UIAnimationState::PRETUTORIAL);

	/// Sound
	m_pSoundManager->StopBGM();
	m_pSoundManager->PlayBGM("Snd_bgm_BeforeBattle");
	return Scene::Initialize();
}

void ExampleScene::FixedUpdate(float _dTime)
{
	// play�� ����Ǵ� �κ��� �߰��ؼ� ������Ʈ �մϴ�.
	if (mTutorialState == TutorialState::THROW
		/*|| mTutorialState == TutorialState::ALLY_Play
		|| mTutorialState == TutorialState::BUFF*/
		|| mpLevelManager->GetGameState() == GameState::PLAY)
	{
		auto playerView = m_registry.view<PlayerComponent>();
		auto enemyView = m_registry.view<EnemyComponent>();
		for (auto& entity : playerView)
		{
			mpUnitSystem->GetPlayerSystem()->FixedUpdate(m_pEntityManager->GetEntity(entity), _dTime); // �Ʊ� ������ ������Ʈ �մϴ�.
		}

		for (auto& entity : enemyView)
		{
			mpUnitSystem->GetPlayerSystem()->FixedUpdate(m_pEntityManager->GetEntity(entity), _dTime); // ���� ������ ������Ʈ �մϴ�.
		}

		//mpUnitSystem->GetProjectileSystem()->FixedUpdate(_dTime);
	}

	if (mTutorialState == TutorialState::ALLY_Play
		|| mTutorialState == TutorialState::BUFF)
	{
		auto playerView = m_registry.view<PlayerComponent>();
		for (auto& entity : playerView)
		{
			mpUnitSystem->GetPlayerSystem()->FixedUpdate(m_pEntityManager->GetEntity(entity), _dTime); // �Ʊ� ������ ������Ʈ �մϴ�.
		}
		//mpUnitSystem->GetProjectileSystem()->FixedUpdate(_dTime);
	}

	mpUnitSystem->GetProjectileSystem()->FixedUpdate(_dTime);

}

void ExampleScene::Update(float _dTime)
{
	SceneData* pSceneData = dynamic_cast<SceneData*>(m_pSceneData);

	//mpLevelManager->BasicUIUpdate(); ����â ��� �Ұ�

	auto skipView = m_registry.view<Button>();
	for (auto& entity : skipView)
	{
		auto& name = m_registry.get<Name>(entity).m_name;
		auto& button = m_registry.get<Button>(entity);

		if (name == "UI_T_Skip")
		{
			if (m_pUIManager->GetButtonState(m_pEntityManager->GetEntity(entity)) == ButtonState::PRESSED)
			{
				mpLevelManager->SetPopUpState(PopUpState::OPEN_SKIP);
				break;
			}
		}
	}

	mpLevelManager->PopUPUITutorial();

	if (mpLevelManager->mIsGoMain == true)
	{
		mpLevelManager->mIsGoMain == false;
		m_pWorldManager->GetCurrentWorld()->SetScene(static_cast<uint32_t>(SceneName::MAIN));
		return;
	}
	else if (mpLevelManager->mIsSkip == true)
	{
		mpLevelManager->mIsSkip = false;
		// Ȥ�ö� ���� ���� �簢�� �� ���̰�
		auto shrinkRectView = m_registry.view<ShrinkRectComponent>();
		for (auto& entity : shrinkRectView)
		{
			auto box2D = m_registry.try_get<Box2D>(entity);
			box2D->mIsVisible = false;
		}
		mTutorialState = TutorialState::END;
		mpLevelManager->SetGameState(GameState::TUTORIALEND);
		mpLevelManager->SetUIAnimationState(UIAnimationState::POSTBATTLE);
		//m_pWorldManager->GetCurrentWorld()->SetScene(static_cast<uint32_t>(SceneName::SCENE1));
		return;
	}

	// ��ũ��
	mpLevelManager->UpdateRankBar(pSceneData);

	/// �÷��̾� �ý��� ������Ʈ
	switch (mpLevelManager->GetGameState())
	{
	case GameState::PRE_PLACEMENT:
	{
		// ���� �ִ� �� ����
		int numEnemy = 0;
		auto enemyView = m_registry.view<EnemyComponent>();
		for (auto entity : enemyView)
		{
			numEnemy++;
		}

		/// ��ġ�� ���õ� ��� �ؽ�Ʈ�� �ؽ�ó �츮��
		// ��ġ�� ���õ� �ؽ�Ʈ ���̱�
		auto textView = m_registry.view<Text>();
		for (auto uiEntity : textView)
		{
			auto& name = m_registry.get<Name>(uiEntity).m_name;
			auto& text = m_registry.get<Text>(uiEntity);

			if (name.find("UI_D") != std::string::npos ||
				name.find("UI_B") != std::string::npos ||
				name == "UI_C_OutSetting" ||
				name.find("UI_T") != std::string::npos)
			{
				text.m_isVisible = true;

				if (name == "UI_B_Money")
				{
					text.m_num1 = dynamic_cast<SceneData*>(pSceneData)->m_heldMoney;
				}

				if (name == "UI_B_StageGuide")
				{
					text.m_num1 = numEnemy;
					text.m_num2 = numEnemy;
				}

				// Ʃ�丮�󿡼� ������� �ʴ� �͵��� ������ �ʰ� �Ѵ�.
				if (name == "UI_D_Guide")
				{
					text.m_isVisible = false;
				}

			}

			// Ʃ�丮�󿡼� ������� �ʴ� �͵��� ������ �ʰ� �Ѵ�.
			if (name == "UI_P_Timer")
			{
				text.m_isVisible = false;
			}
		}

		auto msgView = m_registry.view<MessageBox2D>();
		for (auto uiEntity : msgView)
		{
			auto& name = m_registry.get<Name>(uiEntity).m_name;
			auto& msgComp = m_registry.get<MessageBox2D>(uiEntity);

			if (name == "UI_B_Rank_Gauge")
			{
				msgComp.mIsImgVisible = true;
			}
		}

		// ��ġ�� ���õ� �ؽ�ó ���̱�
		auto textureView = m_registry.view<Texture2D>();
		for (auto uiEntity : textureView)
		{
			auto& name = m_registry.get<Name>(uiEntity).m_name;
			auto& img = m_registry.get<Texture2D>(uiEntity);
			auto button = m_registry.try_get<Button>(uiEntity);

			if (name.find("UI_D") != std::string::npos ||
				name.find("UI_B") != std::string::npos ||
				name == "UI_C_OutSetting" ||
				name.find("UI_T") != std::string::npos)
			{
				img.m_isVisible = true;
				/// ��� ��ư ��Ȱ��ȭ : ����â ��ư�� ��� �����ؾ� �Ѵ�! + ��ŵ ��ư�� ��� �����ؾ� �Ѵ�!
				if (button)
				{
					if (name != "UI_C_OutSetting" && name != "UI_T_Skip")
					{
						button->mIsEnable = false;
					}
				}
			}

			// Ʃ�丮�󿡼� ������� �ʴ� �͵��� ������ �ʰ� �Ѵ�.
			if (name == "UI_P_Timer")
			{
				img.m_isVisible = false;
			}
		}
		mpLevelManager->FadePreSetting(false);
		mpLevelManager->SetGameState(GameState::PLACEMENT);
	}
	break;
	case GameState::PLACEMENT:
	{
		// ���� ��ǥ
		Vector3 cursorFollowPos = m_pPhysicsManager->PickObejct("plane");
		if (cursorFollowPos.x > 0)cursorFollowPos.x += 0.5;
		else cursorFollowPos.x -= 0.5;
		if (cursorFollowPos.z < 0) cursorFollowPos.z -= 0.5;
		bool isPayment = false; /// �̰� ��ġ�� �����ؾ� �� �Ŵ�!

		/// Ʃ�丮�� �ؽ�Ʈ ���
		auto view2 = m_registry.view<Text>();
		for (auto& entity : view2)
		{
			auto& name = m_registry.get<Name>(entity).m_name;
			auto& text = m_registry.get<Text>(entity);
			auto img = m_registry.try_get<Texture2D>(entity);

			if ((name == "UI_T"))
			{
				if (mTutorialTextVec[mTextIdx].idx <= 1) // ��ġ ��Ŀ����
				{
					text.m_position = Vector2(219, 554);
					if (mTutorialTextVec[mTextIdx].idx == 0)
					{
						img->m_file = "UI_pnl_MsgBoxClick1.png";
					}
					else
					{
						img->m_file = "UI_pnl_MsgBox1.png";
					}
					img->m_position = Vector2(193, 489);
					img->m_size = Vector2(432, 210);
				}
				else
				{
					text.m_position = Vector2(392, 176);
					if (mTutorialTextVec[mTextIdx].idx == 2)
					{
						img->m_file = "UI_pnl_MsgBoxClick2.png";
					}
					else
					{
						img->m_file = "UI_pnl_MsgBox2.png";
					}
					img->m_position = Vector2(367, 112);
					img->m_size = Vector2(1182, 170);
				}
				text.m_text = mTutorialTextVec[mTextIdx].text;
			}

			// �÷��̾��� �����ݵ� ������ ������Ʈ �Ѵ�.
			if (name == "UI_B_Money")
			{
				text.m_num1 = pSceneData->m_heldMoney;
			}

			// ��ġ�� �Ʊ��� ���� ������Ʈ�Ѵ�.
			if (name == "UI_D_AliveNum")
			{
				if (9 < text.m_num1 && text.m_num1 < 20)
				{
					text.m_text = u8"%d";
					text.m_position.x = 867.f;
				}
				else if (text.m_num1 == 20)
				{
					text.m_text = u8"%d";
					text.m_position.x = 852.f;
				}
				else
				{
					text.m_text = u8"0%d";
					text.m_position.x = 852.f;
				}
				text.m_num1 = pSceneData->m_aliveAlly;
			}
		}

		/// ���⼭ �뺴���� ������Ʈ �Ѵ�.
		auto npcView = m_registry.view<NPCComponent>();
		for (auto& entity : npcView)
		{
			mpUnitSystem->GetNPCSystem()->TutorialUpdate(m_pEntityManager->GetEntity(entity), mTextIdx);
		}

		/// ���� ������ ���� ������Ʈ
		mpUnitSystem->GetProjectileSystem()->TutirialUpdateMoney(_dTime);

		switch (mTutorialState)
		{
		case TutorialState::CLOUD:
		{
			mpLevelManager->UIAniUpdate(_dTime);
			if (mpLevelManager->GetUIAnimationState() == UIAnimationState::NORMAL)
			{
				mTutorialState = TutorialState::FADE;
			}
		}
		break;
		case TutorialState::FADE:
		{
			/*mpLevelManager->FadeInScreen(_dTime);

			auto view = m_registry.view<FadeInOut>();
			for (auto& entity : view)
			{
				auto& name = m_registry.get<Name>(entity).m_name;
				auto& img = m_registry.get<FadeInOut>(entity);
				if (name == "FaidInOut")
				{
					if (img.IsFadingInFin(_dTime) == true)
					{
						mTutorialState = TutorialState::INTRO;
						return;
					}
				}
			}*/

			uint8_t numEnemy = 0;
			auto enemyView = m_registry.view<EnemyComponent>();
			for (auto& entity : enemyView)
			{
				auto& name = m_registry.get<Name>(entity).m_name;
				auto& enemyComp = m_registry.get<EnemyComponent>(entity);


				if (enemyComp.mIsDead == false)
				{
					numEnemy++;
				}
			}

			auto view2 = m_registry.view<Text>();
			for (auto& entity : view2)
			{
				auto& name = m_registry.get<Name>(entity).m_name;
				auto& text = m_registry.get<Text>(entity);
				if (name == "UI_B_StageGuide")
				{
					text.m_num1 = numEnemy;
				}
			}

			mTutorialState = TutorialState::INTRO;
		}
		break;
		case TutorialState::INTRO: // �۸� �ڵ����� �Ѿ�ϴ�.
		{
			SettingPlayingIndicator();

			PrintIntroText(TutorialState::INTRO);	// Ʃ�丮�� �ؽ�Ʈ ����

			if (mTextIdx == 3)
			{
				mpUnitSystem->GetProjectileSystem()->Update(_dTime);

				auto shrinkRectView = m_registry.view<ShrinkRectComponent>();
				for (auto& entity : shrinkRectView)
				{
					auto& rect = m_registry.get<ShrinkRectComponent>(entity);
					auto box2D = m_registry.try_get<Box2D>(entity);
					box2D->mIsVisible = true;
					rect.Update(*box2D, _dTime);
				}

				/// �̺�Ʈ �߻� : ������ ��ֹ� ��ġ�� ������ �Ѵ�.
				if (m_pInputManager->GetKeyUp(KEY::LBUTTON) && mpLevelManager->AddBlockScreenPos(m_pInputManager->GetMousePos(), Vector2(1812, 997), Vector2(83, 58)))
				{
					if (cursorFollowPos.x >= 5 && cursorFollowPos.x <= 6 && cursorFollowPos.z >= -1 && cursorFollowPos.z <= 1)
					{
						mTime = 0.0f;							 // Ÿ�� ����
						InvisibleGuideTile();					 // �ٴ� ǥ�� �� ���̰� ����
						mpLevelManager->DeleteMoney();			 // ��� ���� ����!
						ResettingShrinkRect(Vector2(1215, 460), Vector2(100, 100), 4, 20);
						mTextIdx = 4;

						auto obsView = m_registry.view<Texture3D>();
						for (auto& obsEntity : obsView)
						{
							auto& name = m_registry.get<Name>(obsEntity).m_name;
							auto& texture = m_registry.get<Texture3D>(obsEntity);
							if (name == "UI_H_Obstacle")
							{
								m_pEntityManager->RemoveEntity(static_cast<UID>(obsEntity));
							}
						}

						mTutorialState = TutorialState::THROW_O; // Ʃ�丮�� ���� ����

						return;
					}
				}
			}
		}
		break;
		case TutorialState::THROW_O:
		{
			SettingPlayingIndicator();

			PrintIntroText(TutorialState::THROW_O);	// Ʃ�丮�� �ؽ�Ʈ ����

			if (mTextIdx == 7)
			{
				mpUnitSystem->GetProjectileSystem()->TutirialUpdateThrow(_dTime);
				//mpUnitSystem->GetProjectileSystem()->TutirialUpdateMoney(_dTime);

				ChangeGuideTile("White.png", Vector2(10, -5), Vector2(0.7, 0.7));

				UpdateShrinkRect(_dTime);

				/// �̺�Ʈ �߻� : ������ ������ �پ�� ���� �ڸ��� ������. + �ٸ� ���� ���� ������ ���� ������Ų��.
				if (m_pInputManager->GetKeyUp(KEY::LBUTTON) && mpLevelManager->AddBlockScreenPos(m_pInputManager->GetMousePos(), Vector2(1812, 997), Vector2(83, 58)))
				{
					if (cursorFollowPos.x >= 10 && cursorFollowPos.x <= 11 && cursorFollowPos.z >= -6 && cursorFollowPos.z <= -5)
					{
						mTime = 0.0f;
						InvisibleGuideTile();					 // �ٴ� ǥ�� �� ���̰� ����

						ResettingShrinkRect(Vector2(631, 959), Vector2(90, 90), 4, 20);
						mTutorialState = TutorialState::THROW_O_WAIT;
					}
					else
					{
						// ���� ��� ����!
						mpLevelManager->DeleteMoney();
					}
				}
			}
		}
		break;
		case TutorialState::THROW_O_WAIT:// ������ ������ ������ ������ ���´�.
		{
			mTime += _dTime;
			//mpUnitSystem->GetProjectileSystem()->TutirialUpdateMoney(_dTime);
			if (mTime > 1.0f)
			{
				mTutorialState = TutorialState::THROW;
				mTextIdx = 8; // ����!\n���� ��Ȯ�� ���� �� ������?
				mTime = 0.0f;
				return;
			}
		}
		break;
		case TutorialState::THROW:
		{
			SettingPlayingIndicator();

			// Ư�� ���ǿ��� ���� �����ؼ� Ÿ���� �ϰ� �ϴ� �Լ���.
			mpUnitSystem->GetPlayerSystem()->DetectGroundMoney();

			PrintIntroText(TutorialState::THROW);	// Ʃ�丮�� �ؽ�Ʈ ����

			// ���� ������Ʈ
			mpLevelManager->EnemyUpdate(_dTime);

			mTime += _dTime;
			if (mTime < 1.0f)
			{
				mTextIdx = 8; // ����!\n���� ��Ȯ�� ���� �� ������?
			}
			else if (1.0f <= mTime && mTime < 3.0f)
			{
				mTextIdx = 9; //���� �������Գ� �����մϴ�.\n������ ���� ���� �̷��� �޷���ŵ��. �Ʊ����.
			}
			else if (3.0f <= mTime && mTime < 3.5f)
			{
				mTextIdx = 10; //��!�ӾҾ�!
			}
			else if (3.5f <= mTime) // ������ ���� �԰� ������ ����
			{
				// �ٰŸ� ���� �뺴 ��ư Ȱ��ȭ
				auto uiView = m_registry.view<Button>();
				for (auto& uiEntity : uiView)
				{
					auto& name = m_registry.get<Name>(uiEntity).m_name;
					auto& button = m_registry.get<Button>(uiEntity);
					auto& img = m_registry.get<Texture2D>(uiEntity);

					if (name == "UI_D_Class")
					{
						if (img.m_file == "UI_btn_Merc01.png")
						{
							img.m_rgba = Vector4(0.8f, 0.8f, 0.8f, 1.0f);
							button.mNormalRGBA = img.m_rgba;
							button.mPressedRGBA = Vector4(1.0f);
							button.mHoveredRGBA = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
							button.mIsEnable = true;
							m_pRenderManager->UpdateEntityTexture(m_pEntityManager->GetEntity(uiEntity));
						}
					}
				}

				// ���� ��� ����!
				auto moneyView = m_registry.view<MoneyComponent>();
				for (auto& entity : moneyView)
				{
					auto& money = m_registry.get<MoneyComponent>(entity);
					m_pEntityManager->RemoveEntity(money.m_pOwner->GetUID());
				}
				mTime = 0.0f;
				mTutorialState = TutorialState::Enemy_Play;
				mTextIdx = 11;
				return;
			}
		}
		break;
		case TutorialState::Enemy_Play:
		{
			SettingDeployIndicator(cursorFollowPos, pSceneData);

			PrintIntroText(TutorialState::Enemy_Play);	// Ʃ�丮�� �ؽ�Ʈ ����

			mpLevelManager->DeleteEnemyAll();// �� �� ���� ������Ʈ ��ü ����

			if (mTextIdx == 13)
			{
				/// �̺�Ʈ �߻� : �ٰŸ� Ŭ���� ��ư�� Ŭ���Ѵ�.
				mpLevelManager->TutorialClassDeploy();

				UpdateShrinkRect(_dTime);

				if (mpLevelManager->mTutorialFlag == true)
				{
					mTutorialState = TutorialState::BUTTON_M;
					mTextIdx = 14;
					ChangeGuideTile("blue.png", Vector2(-12.f, 0.0f), Vector2(2.7f, 9.7f));

					ResettingShrinkRect(Vector2(815, 5), Vector2(290, 105), 4, 20);
					mpLevelManager->mTutorialFlag = false;
				}
			}
		}
		break;
		case TutorialState::BUTTON_M:
		{
			SettingDeployIndicator(cursorFollowPos, pSceneData);

			PrintIntroText(TutorialState::BUTTON_M);	// Ʃ�丮�� �ؽ�Ʈ ����

			/// �̺�Ʈ �߻� : ��ġ ��ġ�� Ŭ��
			mpLevelManager->TutorialClassDeployed(cursorFollowPos, isPayment, pSceneData);
			if (mpLevelManager->mTutorialFlag == true)
			{
				InvisibleGuideTile();					 // �ٴ� ǥ�� �� ���̰� ����
				mTutorialState = TutorialState::DEPLOY_M;
				mpLevelManager->mTutorialFlag = false;

			}
		}
		break;
		case TutorialState::DEPLOY_M:
		{
			SettingPlayingIndicator();
			PrintIntroText(TutorialState::DEPLOY_M);	// Ʃ�丮�� �ؽ�Ʈ ����

			if (mTextIdx == 15)
			{
				UpdateShrinkRect(_dTime);
			}
			if (mTextIdx == 16)
			{
				ResettingShrinkRect(Vector2(735, 600), Vector2(100, 100), 4, 20);
			}

			if (mTextIdx == 18)// �뺴�ܿ����� ���� �ν��ϴ� ������ ���� ������\n�ο�� �͵� ���ΰ� �켱 �ֿ췯 ���ϴ�. ����� ����������!
			{
				ChangeGuideTile("White.png", Vector2(-5.f, 0.f), Vector2(0.7f, 0.7f));
				mpUnitSystem->GetProjectileSystem()->TutirialUpdateThrow(_dTime);
				//mpUnitSystem->GetProjectileSystem()->TutirialUpdateMoney(_dTime);

				UpdateShrinkRect(_dTime);

				/// �̺�Ʈ �߻� : ������ �Ʊ��� �پ�� ���� �ڸ��� ������.
				if (m_pInputManager->GetKeyUp(KEY::LBUTTON) && mpLevelManager->AddBlockScreenPos(m_pInputManager->GetMousePos(), Vector2(1812, 997), Vector2(83, 58)))
				{
					if (cursorFollowPos.x >= -6 && cursorFollowPos.x <= -5 && cursorFollowPos.z >= -1 && cursorFollowPos.z <= 1)
					{
						mTime = 0.0f;
						InvisibleGuideTile();					 // �ٴ� ǥ�� �� ���̰� ����

						ResettingShrinkRect(Vector2(800, 150), Vector2(210, 210), 4, 20);
						mTutorialState = TutorialState::THROW_WAIT;
					}
					else
					{
						// ���� ��� ����!
						mpLevelManager->DeleteMoney();
					}
				}
			}

			auto textView = m_registry.view<Text>();
			for (auto text : textView)
			{
				auto& name = m_registry.get<Name>(text).m_name;
				auto& textComp = m_registry.get<Text>(text);
				if (name == "UI_B_Money")
				{
					textComp.m_num1 = pSceneData->m_heldMoney;
				}

				// ��ġ�� �Ʊ��� ���� ������Ʈ�Ѵ�.
				/*if (name == "UI_D_AliveNum")
				{
					textComp.m_num1 = pSceneData->m_aliveAlly;
				}*/

				if (name == "UI_B_Money")
				{
					if (isPayment == true)
					{
						pSceneData->m_heldMoney -= 100;
						isPayment == false;
					}
					textComp.m_num1 = pSceneData->m_heldMoney;
				}
			}
		}
		break;
		case TutorialState::THROW_WAIT:// ������ ������ ������ ������ ���´�.
		{
			mTime += _dTime;
			//mpUnitSystem->GetProjectileSystem()->TutirialUpdateMoney(_dTime);
			if (mTime > 1.0f)
			{
				// ���⼭ �÷��̾� ���� ���� ������ �����ϱ� ���� ü�¹ٸ� �߰��Ѵ�(�������� �ʰ�)
				auto playerView = m_registry.view<PlayerComponent>();
				for (auto& entity : playerView)
				{
					m_pUIManager->Add3DHPBar(m_pEntityManager->GetEntity(entity), Vector3(0, 3.f, 0), Vector2(50, 5), 5, Vector4(0.2f, 0.2f, 0.2f, 1), Vector4(0.f, 0.f, 1.f, 1));
					m_registry.get<HealthBarComponenet>(entity).mIsVisible = false;
					mpUnitSystem->GetPlayerSystem()->SetInitializeState(m_pEntityManager->GetEntity(entity), State::IDLE);
				}

				mTutorialState = TutorialState::ALLY_Play;
				mTextIdx = 19;
				return;
			}
		}
		break;
		case TutorialState::ALLY_Play:
		{
			SettingPlayingIndicator();

			PrintIntroText(TutorialState::ALLY_Play);	// Ʃ�丮�� �ؽ�Ʈ ����

			// Ư�� ���ǿ��� ���� �����ؼ� Ÿ���� �ϰ� �ϴ� �Լ���.
			mpUnitSystem->GetPlayerSystem()->DetectGroundMoney();
			if (mTextIdx > 19) // �뺴�� ��ź�縦 ���� ���� �� �����Ⱑ �Ұ��մϴ�.
			{
				mpUnitSystem->GetProjectileSystem()->Update(_dTime);
			}

			// �÷��̾� ������Ʈ
			auto playerView = m_registry.view<PlayerComponent>();
			for (auto& entity : playerView)
			{
				auto& playerComp = m_registry.get<PlayerComponent>(entity);

				// �÷��̾� �����Ͽ� ������Ʈ�Ѵ�.
				mpUnitSystem->GetPlayerSystem()->PlayerUnitUpdate(m_pEntityManager->GetEntity(entity), _dTime); // �Ʊ� ������ ������Ʈ �մϴ�.

				physx::PxFilterData filterData = m_pPhysicsManager->GetFilterData(entity);
				filterData.word1 |= 8;
				m_pPhysicsManager->SetFilterData(entity, filterData);
			}

			if (mTextIdx == 21)// ���� ��ư�� �� ������ ������\n�����ؼ� ���� �� �־�!
			{
				if (m_pInputManager->GetKey(KEY::LBUTTON))
				{
					mTime = 0.0f;
					// ��� ���� ����!
					mpLevelManager->DeleteMoney();
					mTutorialState = TutorialState::BUFF;
					return;
					/*mPressClickTime += _dTime;
					if (m_pInputManager->GetKeyUp(KEY::LBUTTON) && mpLevelManager->AddBlockScreenPos(m_pInputManager->GetMousePos(), Vector2(1812, 997), Vector2(83, 58)))
					{
						if (mPressClickTime < 1.5f)
						{
							mPressClickTime = 0.0f;
						}
					}*/
				}

				//if (mPressClickTime > 1.5f)
				//{
				//	mPressClickTime = 0.0f;
				//	mTime = 0.0f;
				//	// ��� ���� ����!
				//	mpLevelManager->DeleteMoney();
				//	mTutorialState = TutorialState::BUFF;
				//	return;

				//}
			}
		}
		break;
		case TutorialState::BUFF:
		{
			SettingPlayingIndicator();
			PrintIntroText(TutorialState::BUFF);	// Ʃ�丮�� �ؽ�Ʈ ����

			// ���� ������
			mpUnitSystem->GetPlayerSystem()->DetectGroundMoney();
			// �÷��̾� ������Ʈ
			mpLevelManager->PlayerUpdate(_dTime);

			if (mTextIdx == 25) // ������ �� ������ �ƴϾ��� ?
			{
				// ���� �뺴 �ٽ� ��ġ
				mpUnitSystem->GetPlayerSystem()->AddSceneEnemyUnits(0);

				// ���� ������Ʈ
				auto enemyView = m_registry.view<EnemyComponent>();
				for (auto& entity : enemyView)
				{
					m_pRenderManager->InitailizeEntity(m_pEntityManager->GetEntity(entity));
				}
				mTutorialState = TutorialState::ENEMY;
				return;
			}
		}
		break;
		case TutorialState::ENEMY:
		{
			void SettingDeployIndicator(Vector3 _cursorPos, SceneData * _pSceneData);
			PrintIntroText(TutorialState::ENEMY);	// Ʃ�丮�� �ؽ�Ʈ ����

			if (mTextIdx == 26)
			{
				UpdateShrinkRect(_dTime);

			}
			else if (mTextIdx == 27)
			{
				if (mIsReadyResetShrinkRect == true)
				{
					ResettingShrinkRect(Vector2(1280, 450), Vector2(210, 210), 4, 20);
					return;
				}
				UpdateShrinkRect(_dTime);
			}
			else if (mTextIdx == 28)
			{
				ResettingShrinkRect(Vector2(185, 20), Vector2(300, 68), 4, 20);
			}
			else if (mTextIdx == 30)
			{
				UpdateShrinkRect(_dTime);
			}
			else if (mTextIdx == 31)
			{
				if (mIsReadyResetShrinkRect == true)
				{
					ResettingShrinkRect(Vector2(185, 80), Vector2(285, 75), 4, 20);
					return;
				}
				UpdateShrinkRect(_dTime);
			}
			else if (mTextIdx == 32)
			{
				ResettingShrinkRect(Vector2(1605, 950), Vector2(90, 90), 4, 20);
				return;
			}
			else if (mTextIdx == 33)// �� ó���̴� �̹����� ���� ���� ��� �־�帮���� ����.
			{
				mTime += _dTime;

				// ��ġ�� ���� ����
				mpLevelManager->ResetPlayer(pSceneData);

				// ��ġ�� ���õ� ��ư ��� Ȱ��ȭ + ���� ������ ����
				auto textureView = m_registry.view<Texture2D>();
				for (auto uiEntity : textureView)
				{
					auto& name = m_registry.get<Name>(uiEntity).m_name;
					auto& img = m_registry.get<Texture2D>(uiEntity);
					auto button = m_registry.try_get<Button>(uiEntity);

					if (name.find("UI_D") != std::string::npos ||
						name.find("UI_B") != std::string::npos)
					{
						/// ��� ��ư Ȱ��ȭ : ���� ���� ��ư�� ��� �Ұ�!
						if (button && name != "UI_D_Start")
						{
							button->mIsEnable = true;
						}

						if (button && img.m_file == "UI_btn_Merc02.png")
						{
							img.m_rgba = Vector4(0.8f, 0.8f, 0.8f, 1.0f);
							button->mNormalRGBA = img.m_rgba;
							button->mPressedRGBA = Vector4(1.0f);
							button->mHoveredRGBA = Vector4(0.5f, 0.5f, 0.5f, 1.0f);
						}
					}

					if (name == "UI_PointMoney")
					{
						img.m_isVisible = true;
						if (img.m_position.x > 215)
						{
							img.m_position.x -= 17.5f; // (915 - 215)/40
						}
						else
						{
							img.m_isVisible = false;
							pSceneData->m_totalAmount = 5000;
							pSceneData->m_heldMoney = 5000;
						}
						if (img.m_position.y > 50)
						{
							img.m_position.y -= 11.f; //( 490 - 50)/40
						}
						if (img.m_size.x > 45)
						{
							img.m_size.x -= 1.f;
						}
						if (img.m_size.y > 50)
						{
							img.m_size.y -= 1.f;
						}
					}
				}
				if (mTime > 2.0f)
				{
					mTutorialState = TutorialState::MONEY;
					mTextIdx = 34;
					mTime = 0.0f;
					/*for (auto uiEntity : textureView)
					{
						auto& name = m_registry.get<Name>(uiEntity).m_name;
						auto& img = m_registry.get<Texture2D>(uiEntity);
						if (name == "UI_PointMoney")
						{
							img.m_isVisible = false;
						}
					}*/
					return;
				}
			}

		}
		break;
		case TutorialState::MONEY:
		{
			SettingDeployIndicator(cursorFollowPos, pSceneData);

		ChangeGuideTile("red.png", Vector2(-12.f, 0.0f), Vector2(2.7f, 9.7f));

			PrintIntroText(TutorialState::MONEY);	// Ʃ�丮�� �ؽ�Ʈ ����
			mpLevelManager->AllDeploy(cursorFollowPos, isPayment, pSceneData);

			ChangeGuideTile("blue.png", Vector2(-12.f, 0.0f), Vector2(2.7f, 9.7f));

			UpdateShrinkRect(_dTime);

			if (mpLevelManager->mTutorialFlag == true)
			{
				mTutorialState = TutorialState::FINAL;

				ResettingShrinkRect(Vector2(300, 955), Vector2(200, 80), 4, 20);
				mpLevelManager->mTutorialFlag = false;
				mTime = 0.0;
			}
			mTime += _dTime;
			if (mTime > 3.0f)
			{
				auto view = m_registry.view<Text>();
				for (auto& entity : view)
				{
					auto& name = m_registry.get<Name>(entity).m_name;
					auto& text = m_registry.get<Text>(entity);
					auto img = m_registry.try_get<Texture2D>(entity);

					if (name == "UI_T")
					{
						text.m_isVisible = false;
						img->m_isVisible = false;
					}
				}
				ResettingShrinkRect(Vector2(1605, 950), Vector2(90, 90), 4, 20);
			}
		}
		break;

		case TutorialState::FINAL:
		{
			PrintIntroText(TutorialState::FINAL);	// Ʃ�丮�� �ؽ�Ʈ ����

			InvisibleGuideTile(); // Ÿ�� �� ���̰�

			UpdateShrinkRect(_dTime);

			auto view = m_registry.view<Text>();
			for (auto& entity : view)
			{
				auto& name = m_registry.get<Name>(entity).m_name;
				auto& text = m_registry.get<Text>(entity);
				auto img = m_registry.try_get<Texture2D>(entity);

				if (name == "UI_T")
				{
					text.m_isVisible = true;
					img->m_isVisible = true;
				}
			}

			// ���� ���� ��ư Ȱ��ȭ
			auto textureView = m_registry.view<Texture2D>();
			for (auto uiEntity : textureView)
			{
				auto& name = m_registry.get<Name>(uiEntity).m_name;
				auto button = m_registry.try_get<Button>(uiEntity);

				if (name.find("UI_D") != std::string::npos ||
					name.find("UI_B") != std::string::npos)
				{
					/// ��� ��ư Ȱ��ȭ : ���� ���� ��ư Ȱ��ȭ!
					if (name == "UI_D_Start")
					{
						button->mIsEnable = true;
						if (m_pUIManager->GetButtonState(button->m_pOwner) == ButtonState::PRESSED)
						{
							if (m_pInputManager->GetKeyUp(KEY::LBUTTON) && mpLevelManager->AddBlockScreenPos(m_pInputManager->GetMousePos(), Vector2(1812, 997), Vector2(83, 58)))
							{
								mpLevelManager->SetGameState(GameState::PRE_PLAY);
								mpLevelManager->SetIsGameStart(true);
								if (mpLevelManager->GetIsClassButtonPressed() == true)
								{
									mpLevelManager->SetmIsOneDeploySet(true);  // ��ġ �Ϸ� �� ���� ����
									/*auto entity = m_pEntityManager->GetEntity(mpLevelManager->GetTempUnitUID());
									entity->GetComponent<PlayerComponent>().mIsDeploy = false;

									m_pEntityManager->RemoveEntity(mpLevelManager->GetTempUnitUID());*/
									mpLevelManager->SetTempUnitUID(-1);
									mpLevelManager->SetIsClassButtonPressed(false);
								}
								m_pSoundManager->PlaySFX("Snd_sfx_BattleStart");

								ResettingShrinkRect(Vector2(925, 410), Vector2(100, 100), 4, 20);
								// 								m_pSoundManager->PlaySFX("Snd_sfx_ClickBtn");
																//mpLevelManager->SetPopUpState(PopUpState::OPEN_UNITDEPLOY);

								mTutorialState = TutorialState::PLAYING;
								return;
							}
						}
					}
				}
			}
		}
		break;
		}
	}
	break;
	case GameState::PRE_PLAY:
	{
		mpLevelManager->PreplayUpdate(pSceneData);
		auto view2 = m_registry.view<Text>();
		for (auto& entity : view2)
		{
			auto& name = m_registry.get<Name>(entity).m_name;
			auto& text = m_registry.get<Text>(entity);
			auto img = m_registry.try_get<Texture2D>(entity);

			if (name == "UI_T")
			{
				text.m_isVisible = false;
				img->m_isVisible = false;
			}

			// Ÿ�̸Ӹ� �մϴ�.
			if (name == "UI_P_Timer")
			{
				text.m_isVisible = true;
				if (img) // �� �̰� �� �̹��� ����
				{
					img->m_isVisible = true;
				}
			}
		}
	}
	break;
	case GameState::PLAY:
	{
		mpUnitSystem->GetProjectileSystem()->Update(_dTime);

		uint8_t numAlly = 0;
		uint8_t numEnemy = 0;

		// Ư�� ���ǿ��� ���� �����ؼ� Ÿ���� �ϰ� �ϴ� �Լ���.
		mpUnitSystem->GetPlayerSystem()->DetectGroundMoney();

		// ��ġ Ÿ�� �� �ٲٱ�
		SettingRealPlayingIndicator(pSceneData);

		// ü�¹�
		auto playerView = m_registry.view<PlayerComponent>();
		auto enemyView = m_registry.view<EnemyComponent>();
		for (auto& entity : playerView)
		{
			auto& playerComp = m_registry.get<PlayerComponent>(entity);
			if (auto helthBar = m_registry.try_get<HealthBarComponenet>(entity)) // ü�¹ٰ� �ִ� �� �� �÷��̾� ������Ʈ�� ���� ���̴�.
			{
				float healthPercent = playerComp.mHP / playerComp.mMaxHP;
				helthBar->mHealthPercentage = healthPercent;
			}

			// �÷��̾� �����Ͽ� ������Ʈ�Ѵ�.
			mpUnitSystem->GetPlayerSystem()->PlayerUnitUpdate(m_pEntityManager->GetEntity(entity), _dTime); // �Ʊ� ������ ������Ʈ �մϴ�.

			if (playerComp.mIsDead == false)
			{
				numAlly++;
			}
			else
			{
				physx::PxFilterData filterData = m_pPhysicsManager->GetFilterData(entity);
				filterData.word1 |= 8;
				m_pPhysicsManager->SetFilterData(entity, filterData);
			}

		}

		for (auto& entity : enemyView)
		{
			auto& enemyComp = m_registry.get<EnemyComponent>(entity);
			if (auto helthBar = m_registry.try_get<HealthBarComponenet>(entity)) // ü�¹ٰ� �ִ� �� �� �÷��̾� ������Ʈ�� ���� ���̴�.
			{
				float healthPercent = enemyComp.mHP / enemyComp.mMaxHP;
				helthBar->mHealthPercentage = healthPercent;
			}

			mpUnitSystem->GetPlayerSystem()->EnemyUnitUpdate(m_pEntityManager->GetEntity(entity), _dTime); // ���� ������ ������Ʈ �մϴ�.

			if (enemyComp.mIsDead == false)
			{
				numEnemy++;
			}
			else
			{
				physx::PxFilterData filterData = m_pPhysicsManager->GetFilterData(entity);
				filterData.word1 |= 8;
				m_pPhysicsManager->SetFilterData(entity, filterData);
			}
		}

		/// ���⼭ �뺴���� ������Ʈ �Ѵ�.
		auto npcView = m_registry.view<NPCComponent>();
		for (auto& entity : npcView)
		{
			mpUnitSystem->GetNPCSystem()->Update(m_pEntityManager->GetEntity(entity), numAlly, numEnemy);
		}

		auto textView = m_registry.view<Text>();
		for (auto& entity : textView)
		{
			auto& name = m_registry.get<Name>(entity).m_name;
			auto& text = m_registry.get<Text>(entity);
			// Ÿ�̸�
			if (name == "UI_P_Timer")
			{
				pSceneData->m_time -= _dTime;

				int minutes = static_cast<int>(pSceneData->m_time) / 60;
				int seconds = static_cast<int>(pSceneData->m_time) % 60;

				std::ostringstream oss;
				oss << std::setfill('0') << std::setw(2) << minutes << ":"
					<< std::setfill('0') << std::setw(2) << seconds;

				// std::u8string���� ��ȯ
				std::string str = oss.str();
				text.m_text = std::u8string(str.begin(), str.end());

				// 10�� ��������, ���ȿ���� ���
				if (std::fabs(pSceneData->m_time - 10.0f) < std::numeric_limits<float>::epsilon())
				{
					m_pSoundManager->PlaySFX("Snd_sfx_TimeLow");
				}

				// �ð��� �� �Ǹ� �ڵ����� �Ѿ
				if (pSceneData->m_time < std::numeric_limits<float>::epsilon())
				{
					pSceneData->m_time = pSceneData->m_totalTime;
					DLOG(LOG_INFO, "Game Over");
					mpLevelManager->SetGameState(GameState::TUTORIALEND);
					return;
				}
			}

			// �÷��̾��� �������� ������Ʈ �Ѵ�.
			if (name == "UI_B_Money")
			{
				text.m_num1 = pSceneData->m_heldMoney;
			}

			if (name == "UI_B_StageGuide")
			{
				text.m_num1 = numEnemy;
			}

		} // End for�� : m_pEntityManager->GetEntityMap()
		if (numAlly == 0 || numEnemy == 0)
		{
			pSceneData->m_aliveAlly = numAlly;
			mpLevelManager->SetGameState(GameState::TUTORIALEND);
			m_pSoundManager->StopBGM();
			if (numAlly == 0)
			{
				m_pSoundManager->PlayBGM("Snd_bgm_AfterBattleLose");
			}
			else
			{
				m_pSoundManager->PlayBGM("Snd_bgm_AfterBattleWin");
			}
			mpLevelManager->FadePreSetting(true);
			auto HPView = m_registry.view<HealthBarComponenet>();
			for (auto entity : HPView)
			{
				auto& hpEntity = m_registry.get<HealthBarComponenet>(entity);
				hpEntity.mIsVisible = false;
				auto& animation = m_registry.get<AnimationComponent>(entity);
				animation.mPaused = true;
			}

			auto rigidView = m_registry.view<Rigidbody>();
			for (auto& entity : rigidView)
			{
				auto& rigid = m_registry.get<Rigidbody>(entity);
				rigid.m_isKinematic = true;
			}

			mTutorialState = TutorialState::POSTPLAYEING;
			return;
		}
	}
	break;
	case GameState::TUTORIALEND:
	{
		mTime += _dTime;
		if (mTutorialState == TutorialState::POSTPLAYEING)
		{
			mTextIdx = 36;
			auto view2 = m_registry.view<Text>();
			for (auto& entity : view2)
			{
				auto& name = m_registry.get<Name>(entity).m_name;
				auto& text = m_registry.get<Text>(entity);
				auto img = m_registry.try_get<Texture2D>(entity);
				if (name == "UI_T")
				{
					text.m_isVisible = true;
					img->m_isVisible = true;
					text.m_position = Vector2(392, 176);
					img->m_file = "UI_pnl_MsgBox2.png";
					img->m_position = Vector2(367, 112);
					img->m_size = Vector2(1182, 170);
					text.m_text = mTutorialTextVec[mTextIdx].text;
				}
			}

			if (mTime > 2.0f)
			{
				mTutorialState = TutorialState::END;
				mpLevelManager->SetUIAnimationState(UIAnimationState::POSTBATTLE);
			}
		}
		else if (mTutorialState == TutorialState::END)
		{
			mpLevelManager->UIAniUpdate(_dTime);

			if (mpLevelManager->GetUIAnimationState() == UIAnimationState::NEXTSCENE)
			{
				m_pWorldManager->GetCurrentWorld()->SetScene(static_cast<uint32_t>(SceneName::SCENE1));
				return;
			}
			/*auto view = m_registry.view<FadeInOut>();
			for (auto& entity : view)
			{
				auto& name = m_registry.get<Name>(entity).m_name;
				auto& img = m_registry.get<FadeInOut>(entity);
				if (name == "FaidInOut")
				{
					if (img.IsFadingOutFin(_dTime / 2, 1.0f) == true)
					{
						m_pWorldManager->GetCurrentWorld()->SetScene(static_cast<uint32_t>(SceneName::SCENE1));
						return;
					}
				}
			}*/

		}
	}
	break;
	default:
		break;
	}

	mpLevelManager->UpdateRankBar(pSceneData);
}

void ExampleScene::LateUpdate(float _dTime)
{
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
}

void ExampleScene::Finalize()
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

void ExampleScene::AddResource()
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

void ExampleScene::SettingDeployIndicator(Vector3 _cursorPos, SceneData* _pSceneData)
{
	// Ÿ�� �ε������ʹ� ������ �����Ų��.
	auto indicatorView = m_registry.view<IndicatorComponent>();
	for (auto indiEntity : indicatorView)
	{
		if (m_registry.get<Name>(indiEntity).m_name == "selectedSquare")
		{
			auto& texture3dComp = m_registry.get<Texture3D>(indiEntity);
			auto& indicatorComp = m_registry.get<IndicatorComponent>(indiEntity);

			auto outline = m_registry.try_get<OutlineComponent>(indiEntity);
			auto alpha = m_registry.try_get<AlphaBlendComponent>(indiEntity);

			if (alpha->m_alpha < 0.5)
			{
				alpha->m_alpha += 0.01;
			}
			else
			{
				alpha->m_alpha = 0;
			}

			if (_cursorPos.x >= -15 && _cursorPos.x <= 15 && _cursorPos.z >= -15 && _cursorPos.z <= 15)
			{
				if (mpLevelManager->IsCanClickAreaXZ(_cursorPos, Vector2(-15, 10), Vector2(-9, -10), _pSceneData->m_aliveAlly) == true)
				{
					if (texture3dComp.m_diffuse != indicatorComp.mOriginalColor)
					{
						texture3dComp.m_diffuse = indicatorComp.mOriginalColor;
						outline->m_color = Vector3(0, 0, 1);
						m_pRenderManager->UpdateEntityTexture(m_pEntityManager->GetEntity(indiEntity));
					}
				}
				else
				{
					if (texture3dComp.m_diffuse != indicatorComp.mChangedColor)
					{
						texture3dComp.m_diffuse = indicatorComp.mChangedColor;
						outline->m_color = Vector3(1, 0, 0);
						m_pRenderManager->UpdateEntityTexture(m_pEntityManager->GetEntity(indiEntity));
					}
				}
				texture3dComp.m_pOwner->GetComponent<Transform>().m_localPosition = Vector3(static_cast<int>(_cursorPos.x), 0.1f, static_cast<int>(_cursorPos.z));
			}
		}
	}
}

void ExampleScene::SettingPlayingIndicator()
{
	// ��ġ Ÿ�� �� �ٲٱ�
	auto indicatorView = m_registry.view<IndicatorComponent>();
	for (auto& entity : indicatorView)
	{
		if (m_registry.get<Name>(entity).m_name == "selectedSquare")
		{
			auto& tile = m_registry.get<Texture3D>(entity);
			Vector3 cursorPos = m_pPhysicsManager->PickObejct("plane");
			if (cursorPos.x > 0)cursorPos.x += 0.5;
			else cursorPos.x -= 0.5;
			if (cursorPos.z < 0) cursorPos.z -= 0.5;

			if (cursorPos.x >= -15 && cursorPos.x <= 15 && cursorPos.z >= -15 && cursorPos.z <= 15)
			{
				cursorPos = Vector3(static_cast<int>(cursorPos.x), 0.1f, static_cast<int>(cursorPos.z));
				Vector2 astarPos = {};
				mpAstar->AdjustToTileCenter(cursorPos, astarPos);
				if ((*mpAstar->GetAstarMap())[static_cast<int>(astarPos.y + abs(mStartPoint.y))][static_cast<int>(astarPos.x + abs(mStartPoint.x))] == 0) // ��ֹ��� ���� ���� Ŭ�� ����
				{
					tile.m_diffuse = "blue.png";
					m_pRenderManager->UpdateEntityTexture(m_pEntityManager->GetEntity(entity));
				}
				else
				{
					tile.m_diffuse = "red.png";
					m_pRenderManager->UpdateEntityTexture(m_pEntityManager->GetEntity(entity));
				}
				tile.m_pOwner->GetComponent<Transform>().m_localPosition = cursorPos;
			}
		}
	}
}

void ExampleScene::SettingRealPlayingIndicator(SceneData* _pSceneData)
{
	auto indicatorView = m_registry.view<IndicatorComponent>();
	for (auto indiEntity : indicatorView)
	{
		if (m_registry.get<Name>(indiEntity).m_name == "selectedSquare")
		{
			auto& texture3dComp = m_registry.get<Texture3D>(indiEntity);
			auto& indicatorComp = m_registry.get<IndicatorComponent>(indiEntity);
			auto outline = m_registry.try_get<OutlineComponent>(indiEntity);
			auto alpha = m_registry.try_get<AlphaBlendComponent>(indiEntity);
			Vector3 cursorFollowPos = m_pPhysicsManager->PickObejct("plane");
			if (cursorFollowPos.x > 0)cursorFollowPos.x += 0.5;
			else cursorFollowPos.x -= 0.5;
			if (cursorFollowPos.z < 0) cursorFollowPos.z -= 0.5;

			if (alpha->m_alpha < 0.5)
			{
				alpha->m_alpha += 0.01;
			}
			else
			{
				alpha->m_alpha = 0;
			}

			if (cursorFollowPos.x >= -15 && cursorFollowPos.x <= 15 && cursorFollowPos.z >= -15 && cursorFollowPos.z <= 15)
			{
				cursorFollowPos = Vector3(static_cast<int>(cursorFollowPos.x), 0.1f, static_cast<int>(cursorFollowPos.z));
				Vector2 astarPos = {};
				mpAstar->AdjustToTileCenter(cursorFollowPos, astarPos);
				if ((*mpAstar->GetAstarMap())[static_cast<int>(astarPos.y + abs(mStartPoint.y))][static_cast<int>(astarPos.x + abs(mStartPoint.x))] == 0) // ��ֹ��� ���� ���� Ŭ�� ����
				{
					texture3dComp.m_diffuse = "blue.png";
					outline->m_color = Vector3(0, 0, 1);
					m_pRenderManager->UpdateEntityTexture(m_pEntityManager->GetEntity(indiEntity));
				}
				else
				{
					texture3dComp.m_diffuse = "red.png";
					outline->m_color = Vector3(1, 0, 0);
					m_pRenderManager->UpdateEntityTexture(m_pEntityManager->GetEntity(indiEntity));
				}
				texture3dComp.m_pOwner->GetComponent<Transform>().m_localPosition = cursorFollowPos;
			}
		}
	}
}

void ExampleScene::ChangeGuideTile(const std::string& _texFileName, Vector2 _posXZ, Vector2 _sizeXZ)
{
	auto view = m_registry.view<Texture3D>();
	for (auto& entity : view)
	{
		auto& name = m_registry.get<Name>(entity).m_name;
		auto& img = m_registry.get<Texture3D>(entity);
		if (name == "UI_H")
		{
			auto trs = m_registry.try_get<Transform>(entity);
			img.m_diffuse = _texFileName;
			trs->m_localPosition.x = _posXZ.x;
			trs->m_localPosition.y = 0.005f;
			trs->m_localPosition.z = _posXZ.y;
			trs->m_localScale.x = _sizeXZ.x;
			trs->m_localScale.z = _sizeXZ.y;
			m_pRenderManager->UpdateEntityTexture(m_pEntityManager->GetEntity(entity));
			return;
		}
	}
}

void ExampleScene::ChangeGuideTileTrs(Vector2 _posXZ, Vector2 _sizeXZ)
{
	auto view = m_registry.view<Texture3D>();
	for (auto& entity : view)
	{
		auto& name = m_registry.get<Name>(entity).m_name;
		auto& img = m_registry.get<Texture3D>(entity);
		if (name == "UI_H")
		{
			auto trs = m_registry.try_get<Transform>(entity);
			trs->m_localPosition.x = _posXZ.x;
			trs->m_localPosition.y = 0.005f;
			trs->m_localPosition.z = _posXZ.y;
			trs->m_localScale.x = _sizeXZ.x;
			trs->m_localScale.z = _sizeXZ.y;
			return;
		}
	}
}

void ExampleScene::InvisibleGuideTile()
{
	auto view = m_registry.view<Texture3D>();
	for (auto& entity : view)
	{
		auto& name = m_registry.get<Name>(entity).m_name;
		auto& img = m_registry.get<Texture3D>(entity);
		if (name == "UI_H")
		{
			auto trs = m_registry.try_get<Transform>(entity);
			trs->m_localPosition.y = -0.1f;
			return;
		}
	}
}

void ExampleScene::ChangeGuideTileTex(const std::string& _texFileName)
{
	auto view = m_registry.view<Texture3D>();
	for (auto& entity : view)
	{
		auto& name = m_registry.get<Name>(entity).m_name;
		auto& img = m_registry.get<Texture3D>(entity);
		if (name == "UI_H")
		{
			img.m_diffuse = _texFileName;
			m_pRenderManager->UpdateEntityTexture(m_pEntityManager->GetEntity(entity));
			return;
		}
	}
}

void ExampleScene::PrintIntroText(TutorialState _tutorialState)
{

	switch (_tutorialState)
	{
	case TutorialState::INTRO:
		SkipText(3);
		// 1 : �ȳ�, ���� ��ġ ��Ŀ����� �� // ���� ���� ����, ��¥ ������ ����!
		// 2 : �׸��� ���� ��Ȯ�� ���ϴ� ���� ���� ���� �� �־�. ����?
		// 3 : ���콺 ���� ��ư���� ������ ���� �� �ֽ��ϴ�. ������ ��ġ�� ������ ����������.
		break;
	case TutorialState::THROW_O:
		SkipText(7);
		// 4 : ��ֹ����� ���� �� ����. �� �뺴���� ��ģ�ٰ�.
		// 5 : �׷� �� ������ ��������..
		// 6 : ��!\n�� �Ʊ�� ���� �� �ۿ��� ����!\n�뺴�鿡�� �� ������ ���̾�!
		// 7 : �׷� �̰��� �������ô�.
		break;
	case TutorialState::THROW:
		SkipText(9);
		// 8 : ����!\n���� ��Ȯ�� ���� �� ������?
		// 9 : ���� �������Գ� �����մϴ�.\n������ ���� ���� �̷��� �޷���ŵ��. �Ʊ����.
		break;
	case TutorialState::Enemy_Play:
		SkipText(13);
		// 10 : ��! �ӾҾ�!
		// 11 : ��ġ ��Ŀ�� ���� ���ϴ� ��� �Ʊ� �뺴���� ���� �ຸ���� ����.\n�Ʊ� �뺴�� ��ȯ�غ��ô�.
		// 12 : ���Ͷ�! ���Ÿ� �뺴!
		// 13 : �ٰŸ� �뺴���� ��ġ�սô�.\n�ϴܿ� ù��° ��ư�� ���������?
		break;
	case TutorialState::BUTTON_M:
		SkipText(14);
		// 14 : ���߾��.\n���������� ǥ�õ� ������ ��ġ�սô�.
		break;
	case TutorialState::DEPLOY_M:
		SkipText(18);
		// 15 : ���� ���� �� �� ��ġ�ߴ��� ������ �� �� �־�.
		// 16 : Ŭ���� ��ư�� �̹� ���� ä��� ��ġ ������ �����Ӱ� �� ��ġ�� �� �־��.
		// 17 : ���Ÿ� �뺴 ��ư�� ������ ���Ÿ� �뺴�� ��ġ�� �� ����!
		// 18 : �뺴�ܿ����� ���� �ν��ϴ� ������ ���� ������\n�ο�� �͵� ���ΰ� �켱 �ֿ췯 ���ϴ�. ����� ����������!
		break;
	case TutorialState::ALLY_Play:
		SkipText(21);
		// 19 : ũ! �� �뺴!\n�� �����̴� ��!\n4����̶��! \n���� 100G�� ��ġ�� �־�!
		// 20 : ���� ��� ���̸� ���� ������ �޽��ϴ�.\n��� ���� ����������.
		// 21 : ���� ��ư�� �� ������ ������\n�����ؼ� ���� �� �־�!
		break;
	case TutorialState::BUFF:
		SkipText(25);
		// 22 : ������ ������ ���ݷµ� �������� ü�µ� �������ϴ�.\n������ �� 3�ܰ�� �� �̻��� ���� �Ծ �ҿ��� �����.
		// 23 : �̷�����!\n�뺴���� ���� ��� �������ٰ� �ߴµ�!
		// 24 : ��, �׷� ���� �ٰŸ��� ���Ÿ� �뺴�� ���������� ��ġ�غ���.\n������ �뺴 Ÿ���� Ȯ���ϰ� ��ġ�Ѵٸ� �·��� �� �ö󰡰���?
		// 25 : ������ �� ������ �ƴϾ��� ?
		break;
	case TutorialState::ENEMY:
		mIsReadyResetShrinkRect = false;
		SkipText(33);
		// 26 : ���� ����⸦ �� �뺴�� �ٰŸ� �뺴�̰�,
		// 27 : ���� Ȱ�� �� �뺴�� ���Ÿ� �뺴�̿���. 
		// 28 : �뺴 �ϳ��� 100G�ۿ� ���ϴϱ�\n20�� �� ��ġ����!
		// 29 : ��ġ ��Ŀ������ �̹��� ���� �� ���� 3000G�ۿ� �� �ȴٴ� �� �ؾ��׿�. 
		// 30 : �´�!\n���� ������ �� �������� Ȯ���� �� �־�.
		// 31 : ������ �������� ���� ��ũ ������ Ȯ���� �� �ֽ��ϴ�.\n���� ���� �������� ���� ��ũ�� ���� �� �־��.
		// 32 : ���� ���ݸ� ��߰���?
		// 33 : �� ó���̴� �̹����� ���� ���� ��� �־�帮���� ����.
		break;
	case TutorialState::MONEY:
		mIsReadyResetShrinkRect = true;
		SkipText(34);
		// 34 : ��ȣ!\n20�� �� ��ġ����!\n������ �Ʒ� ������ ��ư��\n������ ��ġ�� �ٽ� �� �� �־�!
		break;
	case TutorialState::FINAL:
		SkipText(35);
		// 35 : ������ �����ϼ���!
		break;
	case TutorialState::PLAYING:
		break;
	case TutorialState::END:
		SkipText(36);
		// 36 : ��ġ ��Ŀ�� ���� �� ��Ź�մϴ�.
		break;
	case TutorialState::LAST:
		break;
	default:
		break;
	}


}

void ExampleScene::SkipText(int _endIdx)
{
	while (mTextIdx < _endIdx && m_pInputManager->GetKeyUp(KEY::LBUTTON) && mpLevelManager->AddBlockScreenPos(m_pInputManager->GetMousePos(), Vector2(1812, 997), Vector2(83, 58)))
	{
		mTextIdx++;
		mIsReadyResetShrinkRect = true;
		return;
	}
}

void ExampleScene::UpdateShrinkRect(float _dTime)
{
	auto shrinkRectView = m_registry.view<ShrinkRectComponent>();
	for (auto& entity : shrinkRectView)
	{
		auto& rect = m_registry.get<ShrinkRectComponent>(entity);
		auto box2D = m_registry.try_get<Box2D>(entity);
		box2D->mIsVisible = true;
		rect.Update(*box2D, _dTime);
	}
}

void ExampleScene::ResettingShrinkRect(Vector2 _targetRectPos, Vector2 _targetSize, int _sizeMultiplier, int _shrinkAmount)
{
	if (mIsReadyResetShrinkRect == true)
	{
		Vector2 adjustPos = _targetRectPos - _targetSize * (_sizeMultiplier - 1) * 0.5f;
		auto shrinkRectView = m_registry.view<ShrinkRectComponent>();
		for (auto& entity : shrinkRectView)
		{
			auto& rect = m_registry.get<ShrinkRectComponent>(entity);
			auto box2D = m_registry.try_get<Box2D>(entity);
			box2D->mIsVisible = false;
			rect.Resetting(*box2D, adjustPos, _targetSize * _sizeMultiplier, _targetSize, _shrinkAmount);
		}
	}
}

