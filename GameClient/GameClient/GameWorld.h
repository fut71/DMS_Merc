#pragma once
#include "World.h"
#include "SceneNumEnum.h"



class GameEngine;
class EventManager;

class GameWorld
	: public World
{
public:
	GameWorld(entt::registry& _registry, GameEngine* _pGameEngine, EventManager* _pEventManager);
	~GameWorld() = default;

	/// <summary>
	/// ���带 �ʱ�ȭ�մϴ�.
	/// </summary>
	/// <returns></returns>
	virtual bool Initialize();

	/// <summary>
	/// ������ ������ ������Ʈ
	/// </summary>
	/// <param name="_dTime">������ ������ ����</param>
	virtual void FixedUpdate(float _dTime);

	/// <summary>
	/// ������Ʈ
	/// </summary>
	/// <param name="_dTime">������ ����</param>
	virtual void Update(float _dTime);

	/// <summary>
	/// ��ó�� ������Ʈ
	/// </summary>
	/// <param name="_dTime">������ ����</param>
	virtual void LateUpdate(float _dTime);

	/// <summary>
	/// ���带 �����ϰ� �����մϴ�.
	/// </summary>
	virtual void Finalize();

 	void SetScene(const SceneName& _sceneName);

private:
	void ChangeScene(const Event& _event);

	void ExitGameProcess(const Event& _event);

	void AddResource();

	void LoadWorldSetting();

	void CreateSceneFromTable();

private:
	GameEngine* m_pEngine;

	bool m_flyingCamera;
	bool m_inputSceneChange;
};

