#pragma once
#include <WinUser.h>
#include "GameEngine/BaseWindowsProcessor.h"

class GameEngine;
class IManager;

class GameProcessor :
    public BaseWindowsProcessor
{
public:
    GameProcessor(HINSTANCE _hInstance, const WCHAR* _szTitle, int _iconResourceId);
    ~GameProcessor() = default;

    /// <summary>
    /// ���� �ʱ⼳��
    /// </summary>
    /// <returns></returns>
    bool Initialize() override;

    /// <summary>
    /// ���� ����
    /// </summary>
    void Run();

    /// <summary>
    /// �޸� ����
    /// </summary>
    void Finalize() override;

private:
	LRESULT CALLBACK WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);

	/// <summary>
	/// ���� ������Ʈ
	/// </summary>
	void Update() override;

private:
    std::unique_ptr<GameEngine> m_pGameEngine;          // ���� ����
    std::vector<std::shared_ptr<IManager>> m_pManagers; // �߰��� �ۼ��� �Ŵ�����

    float m_accumulator = 0;
    float m_accumulator2 = 0;

    float m_fixedDeltaTime;
    float m_deltaTime;
};

