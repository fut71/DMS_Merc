#pragma once
#include "pch.h"
#include "Singleton.h"

class TimeManager;
class InputManager;
class ResourceManager;
class SoundManager;
class EventManager;
class WorldManager;

/// <summary>
/// ���� ������ �Ŵ������� �����ϴ� �Ŵ��� Ŭ�����Դϴ�.
/// ���� �Ŵ������� ����, �Ҹ� �� �ʱ�ȭ ���� ����մϴ�.
/// 2024.05.20 _Doyo;
/// </summary>
class GameManager 
{
public:
	bool Initialize();
	void FixedUpdate();
	void Update();
	void LateUpdate();
	void Finalize();


private:
    // �Ŵ��� ��ü���� ������
	TimeManager* m_pTimeManager;
	InputManager* m_pInputManager;
	ResourceManager* m_pResourceManager;
	SoundManager* m_pSoundManager;
	EventManager* m_pEventManager;
	WorldManager* m_pWorldManager;


private:
	entt::registry m_registry;

};

