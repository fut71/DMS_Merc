#include "DearsGameEngineAPI.h"
#include "GameProcessor.h"
#include "GameWorld.h"
#include "WeaponManager.h"

GameProcessor::GameProcessor(HINSTANCE _hInstance, const WCHAR* _szTitle, int _iconResourceId)
	: BaseWindowsProcessor(_hInstance, _szTitle, _iconResourceId)
	, m_pGameEngine(nullptr)
	, m_fixedDeltaTime()
	, m_deltaTime()
{
	m_screenPosX = 0;
	m_screenPosY = 0;
}

bool GameProcessor::Initialize()
{

	if (!__super::Initialize())
	{
		return false;
	}

	/// ���� ���� �ʱ�ȭ
	m_pGameEngine = std::make_unique<GameEngine>(m_hWnd, m_screenWidth, m_screenHeight);
	m_pGameEngine->Initialize();
	m_pGameEngine->GetTimeManager()->SetTargetFPS(60);

	/// �߰��� �ۼ��� �Ŵ��� Ŭ���� ����, �ʱ�ȭ
// 	std::shared_ptr<IManager> weaponManager = std::make_shared<WeaponManager>(m_pGameEngine->m_registry, m_pGameEngine->GetRenderManager());
// 	weaponManager->Initialize();
// 	m_pManagers.push_back(weaponManager);

	/// ���� ����
	auto gameWorld = std::make_shared<GameWorld>(m_pGameEngine->m_registry, m_pGameEngine.get()
		, m_pGameEngine->GetEventManager());
	m_pGameEngine->GetWorldManager()->AddWorld(gameWorld);
	gameWorld->Initialize();
	m_pGameEngine->GetWorldManager()->GetCurrentWorld()->SetScene(0);

	return true;
}

void GameProcessor::Run()
{
	auto timer = m_pGameEngine->GetTimeManager();

	while (true)
	{
		if (PeekMessage(&m_msg, NULL, 0, 0, PM_REMOVE))
		{
			if (m_msg.message == WM_QUIT)
			{
				break;
			}

			if (!TranslateAccelerator(m_msg.hwnd, m_hAccelTable, &m_msg))
			{
				TranslateMessage(&m_msg);
				DispatchMessage(&m_msg);
			}
		}
		else
		{
			// ���� �������� ���� ��û Ȯ��
			if (m_pGameEngine->GetWorldManager()->ShouldQuit())
			{
				// Finalize ȣ�� �� �����ϰ� PostQuitMessage ȣ��
				Finalize();
				PostQuitMessage(0);
				break;
			}

			// Ÿ�̸� ������Ʈ
			timer->Update();
			double frameTime = 1.0 / timer->GetTargetFPS();
			double deltaTime = timer->DeltaTime();

			// ������ ������ ���� ���� �ð��� ������ ��ٸ�
			if (deltaTime < frameTime)
			{
				double waitTime = frameTime - deltaTime;
				uint64_t startWait = timer->GetSystemTime().time_since_epoch().count();
				uint64_t endWait = startWait + static_cast<uint64_t>(waitTime * 1e9); // ������ ����

				while (timer->GetSystemTime().time_since_epoch().count() < endWait)
				{
					// ���� �ð� ���� �ƹ��͵� ���� �ʰ� ���
					//Sleep(1);
				}

				// Ÿ�̸� �ٽ� ������Ʈ (��� �� ��Ȯ�� �ð� ���)
				timer->Update();
			}

			// ���� ������Ʈ �� ������
			Update();
		}
	}
}

void GameProcessor::Finalize()
{
	__super::Finalize();
	m_pGameEngine->Finalize();
}

LRESULT CALLBACK GameProcessor::WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	switch (_message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(_wParam);
		// �޴� ������ ���� �м��մϴ�:
		switch (wmId)
		{
		default:
			return DefWindowProc(_hWnd, _message, _wParam, _lParam);
		}
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		// ���� Ȯ�� �޽����� ���ų�, �۾��� ������ �� �ִ� ��ȸ�� ����
		if (MessageBox(_hWnd, L"������ �����Ͻðڽ��ϱ�?", L"���� Ȯ��", MB_OKCANCEL) == IDOK)
		{
			DestroyWindow(_hWnd); // â�� �ı��Ͽ� WM_DESTROY�� �߻���Ŵ
		}
		break;
	default:
		return DefWindowProc(_hWnd, _message, _wParam, _lParam);
	}
	return 0;
}

void GameProcessor::Update()
{
	// Ÿ�̸� ������Ʈ
	auto timer = m_pGameEngine->GetTimeManager();
	timer->Update();

	m_fixedDeltaTime = timer->FixedDeltaTime();

	m_deltaTime = timer->DeltaTime();
	m_accumulator += m_deltaTime;

	// ������ ������ ������Ʈ
	if(m_accumulator >= m_fixedDeltaTime)
	{
		m_pGameEngine->FixedUpdate(m_fixedDeltaTime);

		// �߰� �Ŵ��� ���� ������ ������Ʈ
		for (auto& manager : m_pManagers)
		{
			manager->FixedUpdate(m_fixedDeltaTime);
		}

		m_accumulator -= m_fixedDeltaTime; // ���������� ������ ������ �ð���ŭ ��
	}

	// ������Ʈ
	m_pGameEngine->Update(m_deltaTime);

	// �߰� �Ŵ��� ������Ʈ
	for (auto& manager : m_pManagers)
	{
		manager->Update(m_deltaTime);
	}

	// ��ó�� ������Ʈ
	m_pGameEngine->LateUpdate(m_deltaTime);

	// �߰� �Ŵ��� ��ó�� ������Ʈ
	for (auto& manager : m_pManagers)
	{
		manager->LateUpdate(m_deltaTime);
	}

	// ������
	m_pGameEngine->BeginRender();
	
	// ingui���� �� ������ BeginRender�ÿ� ���콺 Ŀ���� �ʱ�ȭ��Ű�⶧���� ���⿡ �߰���
	m_pGameEngine->GetInputManager()->SetMouseCursor();	
	
	m_pGameEngine->Render(m_deltaTime);
	m_pGameEngine->EndRender();

}
