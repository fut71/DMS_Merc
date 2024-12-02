#include "DearsGameEngineAPI.h"
#include "LoadingScene.h"
#include "ResourceManager.h"
#include "LayerEnum.h"
#include "SceneNumEnum.h"
#include "MessageBox2D.h"

/// [Riun] ���� �޴����� �ε��� ������ �� Ȯ���ϴ� �÷��װ� �־�� ��. �� ������ �Ʒ������� �̵��ϵ��� �����ؾ���

LoadingScene::LoadingScene(entt::registry& _registry, const std::string& _name,
	EventManager* _pEventManager, RenderManager* _pRenderManager,
	PhysicsManager* _pPhysicsManager, InputManager* _pInpueManager,
	WorldManager* _worldManager, UIManager* _pUIManager, EntityManager* _pEntityManager,
	ResourceManager* _pResourceManager, SoundManager* _pSoundManager)
	: Scene(_registry, _name, _pEventManager, _pRenderManager, _pPhysicsManager
		, _pInpueManager, _worldManager, _pUIManager, _pEntityManager, _pResourceManager
		, _pSoundManager)
	, tempEasing(nullptr)
{
	mpLevelManager = nullptr;
	accumulateTime = 0;
	maxTimeforPercetage = 6.0f;
	mLoadingMaxSize = 1500.f;
}

bool LoadingScene::Initialize()
{
	AddResource();
	tempEasing = new EasingFunc();
	accumulateTime = 0;

	mpLevelManager = new LevelManager(m_registry, m_pRenderManager, m_pInputManager, m_pUIManager, m_pEntityManager
		, m_pResourceManager, m_pSoundManager);
	mpLevelManager->InitializeforNoneFbx(GetUID());

	// ī�޶� ��ƼƼ ����, ����
	mpLevelManager->AddCamera(Vector3(0.f, 5.f, -50.f), Vector3(0.f, 0.f, 1.f));

	// �ε��� ���� �̹���
	auto ui2 = m_pEntityManager->CreateEntity("UI_L_Img"); // ������ 1080+96 = 1176���� ����ش�.
	std::string randimLodingImg = "UI_img_LoadingImg" + to_string(RandomUtil::RandomInt(1, 10)) + ".png";
	m_pUIManager->AddUI(ui2, randimLodingImg, Vector2(495, -1080), Vector2(930, 797), static_cast<int>(Layer::BG), Vector4(1.f), false);

	// �ε� ������ �޹�� + ������ ���� + �ε� �ؽ�Ʈ
	auto ui3 = m_pEntityManager->CreateEntity("UI_L_Loading");
	m_pUIManager->AddUI(ui3, "UI_img_LoadingBG.png", Vector2(190, -185), Vector2(1540, 100), static_cast<int>(Layer::BG_P), Vector4(1.f), false);
	m_pUIManager->AddMessageBox2D(ui3, "UI_img_LoadingBar.png", Vector2(210, -185), Vector2(1500, 60), Vector4(1.0f), u8"Now Loading...", Vector2(759, -271), 0.8f, "KIMM_Bold(60).ttf", Vector4(1.0f), static_cast<int>(Layer::BG_B), false);

	/// UI
	mpLevelManager->AddAnimationUI(false, static_cast<int>(m_pWorldManager->GetCurrentWorld()->GetPreviousScene()));
	mpLevelManager->SetUIAnimationState(UIAnimationState::PRELOADING); // UI �ִϸ��̼� ���� ����
	tempTime = 0.0f;

	// ���� ���� ��Ʋ���̾����� Ȯ��
	if (m_pWorldManager->GetCurrentWorld()->GetPreviousScene() >= static_cast<uint32_t>(SceneName::TUTORIAL))
	{
		auto view = m_registry.view<Texture2D>();
		for (auto& entity : view)
		{
			auto& name = m_registry.get<Name>(entity).m_name;
			auto& texture = m_registry.get<Texture2D>(entity);
			if (name.find("UI_A_Cloud") != std::string::npos)
			{
				texture.m_position.y = -726.f;
			}
		}
	}

	return Scene::Initialize();
}

void LoadingScene::FixedUpdate(float _dTime)
{
	switch (mpLevelManager->GetUIAnimationState())
	{
	case UIAnimationState::PRELOADING:
	{
		tempTime += _dTime;

		auto view = m_registry.view<Texture2D>();
		for (auto& entity : view)
		{
			auto& name = m_registry.get<Name>(entity).m_name;
			auto& texture = m_registry.get<Texture2D>(entity);
			if (name.find("UI_A_Cloud") != std::string::npos)
			{
				if (m_pWorldManager->GetCurrentWorld()->GetPreviousScene() == static_cast<uint32_t>(SceneName::MAIN))
				{
					tempEasing->EasingFuncMap["easeInSine"](-2983, 1080, tempTime, &texture.m_position.y);
				}
				else // ��ε� �ÿ��� ���� ��ġ�� -726
				{
					tempEasing->EasingFuncMap["easeInSine"](-726, 1080, tempTime, &texture.m_position.y);
				}

				if (texture.m_position.y == 1080)
				{
					mpLevelManager->SetUIAnimationState(UIAnimationState::VISIBLE); // UI �ִϸ��̼� ���� ����
					tempTime = 0.0f;
					return;
				}

			}
		}
	}
	break;
	case UIAnimationState::VISIBLE:	  // ������ �ڵ� :���⼭ �ε��� ���õ� ui�� ���̰� �����մϴ�.
	{
		auto view = m_registry.view<Texture2D>();
		for (auto& entity : view)
		{
			auto& name = m_registry.get<Name>(entity).m_name;
			auto& texture = m_registry.get<Texture2D>(entity);
			if (name.find("UI_L") != std::string::npos)
			{
				texture.m_isVisible = true;
			}
		}

		auto uiView = m_registry.view<MessageBox2D>();
		for (auto& loadingEntity : uiView)
		{
			auto& loadingBar = m_registry.get<MessageBox2D>(loadingEntity);
			auto& name = m_registry.get<Name>(loadingEntity).m_name;
			if (name == "UI_L_Loading")
			{
				loadingBar.mIsImgVisible = true;
				loadingBar.mIsTextVisible = true;
			}

		}
		mpLevelManager->SetUIAnimationState(UIAnimationState::NORMAL); // UI �ִϸ��̼� ���� ����
	}
	break;
	case UIAnimationState::NORMAL:	  // ������ �ڵ�
	{
		tempTime += _dTime;

		auto view = m_registry.view<Texture2D>();
		for (auto& entity : view)
		{
			auto& name = m_registry.get<Name>(entity).m_name;
			auto& texture = m_registry.get<Texture2D>(entity);
			if (name == "UI_L_Img")
			{
				tempEasing->EasingFuncMap["easeOutSine"](-1080, 96, tempTime * 0.5f, &texture.m_position.y);
			}

			if (name == "UI_L_Loading")
			{
				tempEasing->EasingFuncMap["easeOutSine"](-185, 971, tempTime * 0.5f, &texture.m_position.y);
			}
		}

		auto uiView = m_registry.view<MessageBox2D>();
		for (auto& loadingEntity : uiView)
		{
			auto& loadingBar = m_registry.get<MessageBox2D>(loadingEntity);
			auto& name = m_registry.get<Name>(loadingEntity).m_name;
			if (name == "UI_L_Loading")
			{
				tempEasing->EasingFuncMap["easeOutSine"](-185, 991, tempTime * 0.5f, &loadingBar.mTexturePosition.y);
				tempEasing->EasingFuncMap["easeOutSine"](-271, 905, tempTime * 0.5f, &loadingBar.mTextPosition.y);
			}
		}
	}
	break;
	case UIAnimationState::POSTLOATING: // �� �κ��� �ε��� 100% �Ϸ�Ǹ� LoadingResourceUpdate���� ���¸� �Ѱ��ش�.
	{
		tempTime += _dTime;

		auto view = m_registry.view<Texture2D>();
		for (auto& entity : view)
		{
			auto& name = m_registry.get<Name>(entity).m_name;
			auto& texture = m_registry.get<Texture2D>(entity);
			if (name == "UI_L_Img")
			{
				tempEasing->EasingFuncMap["easeOutSine"](96, -1080, tempTime, &texture.m_position.y);
			}

			if (name == "UI_L_Loading")
			{
				tempEasing->EasingFuncMap["easeOutSine"](991, -185, tempTime, &texture.m_position.y);
			}

			if (name.find("UI_A_Cloud") != std::string::npos)
			{
				if (tempTime > 1.0f)
				{
					tempEasing->EasingFuncMap["easeInSine"](1080, -726, (tempTime - 1.0f), &texture.m_position.y);
					if (name == "UI_A_CloudR" && texture.m_position.y == -726)
					{
						mpLevelManager->SetUIAnimationState(UIAnimationState::NEXTSCENE); // UI �ִϸ��̼� ���� ����
						tempTime = 0.0f;
						return;
					}
				}
			}
		}

		auto uiView = m_registry.view<MessageBox2D>();
		for (auto& loadingEntity : uiView)
		{
			auto& loadingBar = m_registry.get<MessageBox2D>(loadingEntity);
			auto& name = m_registry.get<Name>(loadingEntity).m_name;
			if (name == "UI_L_Loading")
			{
				tempEasing->EasingFuncMap["easeOutSine"](991, -185, tempTime, &loadingBar.mTexturePosition.y);
				tempEasing->EasingFuncMap["easeOutSine"](905, -271, tempTime, &loadingBar.mTextPosition.y);
			}
		}
	}
	break;
	case UIAnimationState::NEXTSCENE: // ������ �ڵ�
	{
		tempTime = 0.0f;
		accumulateTime = 0.0f;

		if (m_pWorldManager->GetCurrentWorld()->GetPreviousScene() >= static_cast<uint32_t>(SceneName::SCENE1))
		{
			m_pWorldManager->GetCurrentWorld()->SetScene(m_pWorldManager->GetCurrentWorld()->GetPreviousScene() + 1);
		}
		else if (m_pWorldManager->GetCurrentWorld()->GetPreviousScene() == static_cast<uint32_t>(SceneName::MAIN))
		{
			m_pWorldManager->GetCurrentWorld()->SetScene(static_cast<uint32_t>(SceneName::TUTORIAL)); // [TODO] �� �κ� �ڱⰡ �� �ɷ� �ؼ� �����;� ���ٵ�
		}
	}
	break;
	default:
		break;
	}
}

void LoadingScene::Update(float _dTime)
{
	// ���� �ð��� ���� �ؽ�Ʈ�� �����ϰ�, ���ҽ��� ���� ���̿� �߰��մϴ�.
	accumulateTime += _dTime;

	LoadingTextUpdate();
	LoadingResourceUpdate();
}

void LoadingScene::LateUpdate(float _dTime)
{
}

void LoadingScene::Finalize()
{
	Scene::Finalize();
}

void LoadingScene::AddResource()
{
	//���ҽ� �߰�
	m_pRenderManager->AddModel("../TestAsset/", "box.fbx");
}

void LoadingScene::LoadingTextUpdate()
{
	// �ؽ�Ʈ ����
	auto uiView = m_registry.view<MessageBox2D>();
	for (auto& loadingEntity : uiView)
	{
		auto& loadingBar = m_registry.get<MessageBox2D>(loadingEntity);
		auto img = m_registry.try_get<Texture2D>(loadingEntity);
		auto& name = m_registry.get<Name>(loadingEntity).m_name;
		float nowSize = 0;

		if (accumulateTime > maxTimeforPercetage)
		{
			nowSize = maxTimeforPercetage;
		}
		else
		{
			nowSize = accumulateTime;
		}
		loadingBar.mSize.x = (nowSize / maxTimeforPercetage) * mLoadingMaxSize; 

		if (accumulateTime - static_cast<int>(accumulateTime) > 0)
		{
			loadingBar.mText = u8"Now Loading.";
		}
		if (accumulateTime - static_cast<int>(accumulateTime) > loadingTextInterval)
		{
			loadingBar.mText = u8"Now Loading..";
		}
		if (accumulateTime - static_cast<int>(accumulateTime) > loadingTextInterval * 2)
		{
			loadingBar.mText = u8"Now Loading...";
		}
	}
}

void LoadingScene::LoadingResourceUpdate()
{
	if (mpLevelManager->GetUIAnimationState() == UIAnimationState::POSTLOATING
		|| mpLevelManager->GetUIAnimationState() == UIAnimationState::NEXTSCENE) // POSTLOATIN, NEXTSCENE�� ���� ������ �ʿ� ����
	{
		return;
	}

	if (mIsResourcesAlreadyLoaded && accumulateTime > 5.0f)
	{
		// �ε� �Ϸ� �� �� ��ȯ
		mpLevelManager->SetUIAnimationState(UIAnimationState::POSTLOATING); // UI �ִϸ��̼� ���� ����
		tempTime = 0.0f;
		return;
	}


	// 0~1��: �� �߰�, UI �ؽ�ó �߰�, �ִϸ��̼� �߰�
	if (accumulateTime > 0.0f && mIsResourceLoaded1 == false)
	{
		m_pResourceManager->AddFilesInDirModel("Geometry");
		m_pResourceManager->AddFilesInDirAni("Animation");
		mIsResourceLoaded1 = true;
	}

	// 1~6��: 3D �ؽ�ó �߰�
	if (accumulateTime > 1.0f && mIsResourceLoaded2 == false)
	{
		m_pResourceManager->AddFilesInDir3D("Texture3D");
		m_pResourceManager->AddFilesInDirDDS("CubeMap");
		mIsResourceLoaded2 = true;
	}

	// 5��: ť��� �ؽ�ó �߰�
	if (accumulateTime > 2.0f && mIsResourceLoaded3 == false)
	{
		m_pResourceManager->AddFilesInDirModel("Mercenary");
		m_pResourceManager->AddFilesInDirModel("Archer");
		mIsResourceLoaded3 = true;
	}

	// 6�� ~ 10��: �� �߰� (����), ���� �߰�
	if (accumulateTime > 3.0f && mIsResourceLoaded4 == false)
	{
		m_pResourceManager->AddFilesInDirModel("Enemy");
		m_pResourceManager->AddFilesInDirModel("money");
		m_pResourceManager->AddFilesInDirModel("Chief");
		mIsResourceLoaded4 = true;

	}
	else if (accumulateTime > 5.0f && mIsResourceLoaded5 == false)
	{
		m_pResourceManager->AddFilesInDirModel("Environment");
		mIsResourceLoaded5 = true;
	}
	else if (accumulateTime > 6.0f)
	{
		m_pResourceManager->AddFilesInDirBGMSound("BGM");
		m_pResourceManager->AddFilesInDirSFXSound("SFX"); // ���� �ٸ� ���� �ű�
		// �ε� �Ϸ� �� �� ��ȯ
		mIsResourcesAlreadyLoaded = true;
		mpLevelManager->SetUIAnimationState(UIAnimationState::POSTLOATING); // UI �ִϸ��̼� ���� ����
		tempTime = 0.0f;
		maxTimeforPercetage = 5.0f;
		return;
	}
}
