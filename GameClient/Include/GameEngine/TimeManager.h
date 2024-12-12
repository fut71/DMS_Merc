#pragma once
#include "pch.h"

/// <summary>
/// ���� �������� �ð��� �����ϴ� �Ŵ��� Ŭ�����Դϴ�.
/// QueryPerformance�� ����� Windows ȯ�濡�� ����ϵ��� �ۼ��Ǿ����ϴ�.
/// ���� chrono ���̺귯���� ����ؼ� �÷��� Ȯ���� ����Ҽ� �ֽ��ϴ�.
/// 2024.04.22 _Doyo;
/// </summary>
class TimeManager
{
public:
	TimeManager() = default;
	~TimeManager() = default;

	bool Initialize();
	void Update();
	void Finalize();

	float TotalTime() const;
	float DeltaTime() const;
	float FixedDeltaTime() const;
	float FPS() const;

	void Reset();
	void Start();
	void Stop();

	// 30, 60, 120, 144
	void SetTargetFPS(const float _targetFPS);

	void SetFixedDeltaTime(const float _fDTime);
	void SetDeltaTime(const float _dTime);

	std::chrono::system_clock::time_point GetSystemTime();

	float GetTargetFPS() const { return m_targetFPS; }
private:
	double m_secPerCount = 0;
	double m_deltaTime = 0;
	double m_fixedDeltaTime = 1.0 / 60.0;
	float m_maxDeltaTime = 0.1f;  // ��Ÿ Ÿ���� �ִ� 0.1�ʷ� ����

	uint64_t m_baseTime = 0;
	uint64_t m_pausedTime = 0;
	uint64_t m_stopTime = 0;
	uint64_t m_prevTime = 0;
	uint64_t m_curTime = 0;

	bool m_stopped = false;
	float m_targetFPS = 144.f;	// ��ǥ fps
	float m_currentFPS = 0.f;     // ���� FPS�� ����
	int m_frameCount = 0;
	double m_elapsedTime = 0.0;
};
