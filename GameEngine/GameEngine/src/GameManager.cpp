#include "GameManager.h"
#include "TimeManager.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "UIManager.h"
#include "SoundManager.h"
#include "EventManager.h"
#include "WorldManager.h"

bool GameManager::Initialize()
{
	m_pTimeManager = new TimeManager();
	m_pInputManager = new InputManager();
	m_pResourceManager = new ResourceManager();
	m_pWorldManager = new WorldManager();
	m_pSoundManager = SoundManager::GetInstance();
	m_pEventManager = new EventManager();

	if (!m_pTimeManager->Initialize()
		|| !m_pInputManager->Initialize()
		|| !m_pResourceManager->Initialize()
		|| !m_pSoundManager->Initialize()
		|| !m_pEventManager->Initialize())
	{
		return false;
	}

	// Sound Test
	m_pSoundManager->LoadBGM("../../Resources/Sound/Seven Nation Army.mp3", "SNA");
	m_pSoundManager->LoadBGM("../../Resources/Sound/rather be.mp3", "RB");
	m_pSoundManager->PlayBGM("RB");

	return true;
}

// ������ ���������� ���������� ������Ʈ �մϴ�.
void GameManager::FixedUpdate()
{
}

// ���� ���� �ý����� ������ ��� �Ŵ������� ������Ʈ�մϴ�.
void GameManager::Update()
{
	m_pTimeManager->Update();
	m_pInputManager->Update();
	m_pSoundManager->Update();
	m_pResourceManager->Update();
	m_pEventManager->Update();
}

// ?
void GameManager::LateUpdate()
{
}

// �����ϰ� �ִ� �Ŵ������� �޸��Ҵ��� �����մϴ�.
void GameManager::Finalize()
{
	m_pEventManager->Finalize();
	m_pSoundManager->Finalize();
	m_pResourceManager->Finalize();
	m_pInputManager->Finalize();
	m_pTimeManager->Finalize();

}
