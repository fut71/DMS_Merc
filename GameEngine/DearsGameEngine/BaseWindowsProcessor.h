#pragma once
#include "pch.h"

/// <summary>
/// ���� ������ �⺻ Ŭ�����Դϴ�.
/// ������ �޽��� ������ ���ư����ֽ��ϴ�.
/// 2024.04.18 _Doyo
/// 
/// ������ ������ ���ư��� Ŭ������ ����
/// 2024.06.18
/// </summary>
class BaseWindowsProcessor
{
public:
	// �⺻ ������ ����
	BaseWindowsProcessor() = delete;

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="_hInstance">�ڵ� �ν��Ͻ�</param>
	/// <param name="_szTitle">Ÿ��Ʋ ���ڿ�</param>
	/// <param name="_iconResourceId">������ id</param>
	BaseWindowsProcessor(HINSTANCE _hInstance, const WCHAR* _szTitle, int _iconResourceId);
	BaseWindowsProcessor(int _hInstance, const WCHAR* _szTitle, int _iconResourceId);
	virtual ~BaseWindowsProcessor() = default;

	/// <summary>
	/// ������ ���μ����� �ʱ�ȭ�մϴ�.
	/// </summary>
	/// <returns>���н� false ��ȯ</returns>
	virtual bool Initialize();

	void OnResize(uint16_t _width, uint16_t _height);
	void Run();
	virtual void Finalize();	

	HWND GetHWND() const;

protected:
	static LRESULT CALLBACK WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam);
	virtual void Update();

	void CreateConsole();

protected:
	HWND m_hWnd;
	HINSTANCE m_hInstance;
	HACCEL m_hAccelTable;
	MSG m_msg;
	WCHAR m_szTitle[100];
	int m_iconResourceId;

	uint16_t m_screenWidth = 1920;
	uint16_t m_screenHeight = 1080;
	uint16_t m_screenPosX = 0;
	uint16_t m_screenPosY = 0;

};

