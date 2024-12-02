#pragma once
#include <windows.h>

class TimeManager;

class GameInfo
{
public:
	GameInfo(TimeManager* _pTimeManager);
	~GameInfo();

	float GetFPS() const;
	float GetFrameTime() const;
	float GetCPUInfo();

	// GPU ��뷮
	double GetVRAMInfo() const;

	// RAM ��뷮
	unsigned int GetRAMInfo() const;

	// 	// �������� �ҿ�� �ð�
	// 	float GetRenderTime() const { return m_RenderTime; }
	// 	void SetRenderTime(const float val) { m_RenderTime = val; }
	// 
	// 	// ���� ���꿡 �ҿ�� �ð�
	// 	float GetPhysicsTime() const { return m_physicsTime; }
	// 	void SetPhysicsTime(const float val) { m_physicsTime = val; }

private:
	TimeManager* m_pTimeManager;

	float m_FPS = 0;			// �ʴ� �������ϴ� ������ ��
	float m_FrameTime = 0;		// �� �������� �������ϴ� �� �ɸ��� �ð�
	float m_CPUInfo = 0;		// CPU ��뷮
	// 	float m_RenderTime;		// ������ �۾��� �ҿ�Ǵ� �ð�
	// 	float m_physicsTime;	// ���� ���꿡 �ҿ�Ǵ� �ð�

	ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
	int numProcessors;
	HANDLE self;
};