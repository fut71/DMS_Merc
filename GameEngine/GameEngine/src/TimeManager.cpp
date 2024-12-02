#include "TimeManager.h"

bool TimeManager::Initialize()
{
	uint64_t countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec); //�ý����� ī���� ��.
	m_secPerCount = 1.0 / static_cast<double>(countsPerSec);

	uint64_t curTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&curTime);	// ���� �ý��� �ð�
	m_prevTime = curTime;

	return true;
}

void TimeManager::Update()
{
	if (m_stopped)
	{
		m_deltaTime = 0.f;
		return;
	}

	uint64_t curTime{};
	QueryPerformanceCounter((LARGE_INTEGER*)&curTime);	// ���� �ý��� �ð�
	m_curTime = curTime;

	// ��Ÿ Ÿ�� ��� (���� �ð����� ���� �ð� ���̸� �ʷ� ��ȯ)
	m_deltaTime = (m_curTime - m_prevTime) * m_secPerCount;

	// ���� �ð��� ���� �ð����� ������Ʈ
	m_prevTime = m_curTime;

	// �ʹ� ū ��Ÿ Ÿ���� �ִ밪�� ���� �ʵ��� ���� (������ ����� ���� �� ����)
	if (m_deltaTime > m_maxDeltaTime)
	{
		m_deltaTime = m_maxDeltaTime;
	}

	// FPS ���
	m_frameCount++;
	m_elapsedTime += m_deltaTime;

	if (m_elapsedTime >= 1.0)
	{
		m_currentFPS = static_cast<float>(m_frameCount) / m_elapsedTime;
		m_frameCount = 0;
		m_elapsedTime = 0.0;
	}

	// ������ ������ ���� Ÿ�̹� ����
	double targetFrameTime = 1.0 / m_targetFPS;
	if (m_deltaTime < targetFrameTime)
	{
		// ��� �ð� ��� (������ �ӵ��� �ʹ� ���� �� ����)
		double timeToWait = targetFrameTime - m_deltaTime;
		// CPU ���ϸ� ���̱� ���� ��� �ð� ���� Ÿ�̸Ӹ� ���� ��Ȯ�� �ð� ����
		uint64_t endWaitTime = curTime + (timeToWait / m_secPerCount);
		while (curTime < endWaitTime)
		{
			QueryPerformanceCounter((LARGE_INTEGER*)&curTime);
		}
		m_curTime = curTime;
		m_deltaTime = targetFrameTime;
	}
}

void TimeManager::Finalize()
{
	Stop();
}

float TimeManager::TotalTime() const
{
	auto totalTime = m_stopped ? ((m_stopTime - m_pausedTime) - m_baseTime) * m_secPerCount
		: ((m_curTime - m_pausedTime) - m_baseTime) * m_secPerCount;
	return totalTime;
}

float TimeManager::DeltaTime() const
{
	return static_cast<float>(m_deltaTime);
}

float TimeManager::FixedDeltaTime() const
{
	return static_cast<float>(m_fixedDeltaTime);
}

float TimeManager::FPS() const
{
// 	static int frameCount = 0;
// 	static float elapsedTime = 0.f;
// 	static float currentFPS = 0;
// 
// 	frameCount++;
// 	elapsedTime += m_deltaTime;
// 
// 	if (elapsedTime >= 1.f)
// 	{
// 		currentFPS = static_cast<float>(frameCount) / elapsedTime;
// 		frameCount = 0;
// 		elapsedTime = 0.f;
// 	}
// 
// 	return currentFPS;
	return (1.f / m_deltaTime);
// 	return m_currentFPS;
}

void TimeManager::Reset()
{
	LARGE_INTEGER currTime;
	QueryPerformanceCounter(&currTime);

	m_baseTime = currTime.QuadPart;
	m_prevTime = currTime.QuadPart;
	m_stopTime = 0;
	m_stopped = false;
	m_pausedTime = 0;
}

void TimeManager::Start()
{
	uint64_t startTime{};
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	if (m_stopped)
	{
		m_pausedTime += startTime - m_stopTime;
		m_prevTime = startTime;
		m_stopTime = 0;
		m_stopped = false;
	}
}

void TimeManager::Stop()
{
	if (!m_stopped)
	{
		uint64_t curTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&curTime);
		m_stopTime = curTime;
		m_stopped = true;
	}
}

void TimeManager::SetTargetFPS(const float _targetFPS)
{
	m_targetFPS = _targetFPS;
	m_fixedDeltaTime = 1.0 / static_cast<double>(_targetFPS);
}

void TimeManager::SetFixedDeltaTime(const float _fDTime)
{
	m_fixedDeltaTime = _fDTime;
}

void TimeManager::SetDeltaTime(const float _dTime)
{
	m_deltaTime = _dTime;
}

std::chrono::system_clock::time_point TimeManager::GetSystemTime()
{
	return std::chrono::system_clock::now();
}