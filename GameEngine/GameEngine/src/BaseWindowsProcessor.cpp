#include "pch.h"
#include "BaseWindowsProcessor.h"
#include "Resource.h"
#include "GameEngine.h"

BaseWindowsProcessor::BaseWindowsProcessor(HINSTANCE _hInstance, const WCHAR* _szTitle = L"BaseApp", int _iconResourceId = IDI_ICONDMB)
	:m_hInstance(_hInstance)
	, m_hWnd()
	, m_msg()
	, m_hAccelTable()
	, m_iconResourceId(_iconResourceId)
{
	wcscpy_s(m_szTitle, _szTitle);
}

BaseWindowsProcessor::BaseWindowsProcessor(int _hInstance, const WCHAR* _szTitle = L"BaseApp", int _iconResourceId = IDI_ICONDMB)
	:m_hInstance(reinterpret_cast<HINSTANCE>(_hInstance))
	, m_hWnd()
	, m_msg()
	, m_hAccelTable()
	, m_iconResourceId(_iconResourceId)
{
	wcscpy_s(m_szTitle, _szTitle);
}

bool BaseWindowsProcessor::Initialize()
{
	// �⺻ Ŀ���� �ε��մϴ� (IDC_ARROW).
// 	HCURSOR hArrowCursor = LoadCursor(NULL, IDC_ARROW);
// 
// 	// Ŀ�� �̹����� �����ϸ鼭 ũ�⸦ ����
// 	HCURSOR hCousor = (HCURSOR)CopyImage(hArrowCursor, IMAGE_CURSOR, 64, 64, 0);

	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = BaseWindowsProcessor::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInstance;
	wcex.hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(m_iconResourceId));
	wcex.hCursor = NULL/*hCousor*//*LoadCursor(nullptr, IDC_ARROW)*/;
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = 0;
	wcex.lpszClassName = m_szTitle;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(m_iconResourceId));

	RegisterClassExW(&wcex);
	// ���ϴ� ��Ÿ�Ϸ� ���� (���� ǥ����, �׵θ�, ��ũ�ѹ� ����)
	DWORD dwStyle = WS_POPUP;

	RECT rect = { 0, 0, m_screenWidth, m_screenHeight };
	AdjustWindowRect(&rect, dwStyle, FALSE);
	int windowWidth = rect.right - rect.left;
	int windowHeight = rect.bottom - rect.top;

	m_hWnd = CreateWindowW(m_szTitle, m_szTitle, dwStyle,
		m_screenPosX, m_screenPosY, windowWidth, windowHeight, nullptr, nullptr, m_hInstance, nullptr);

	if (!m_hWnd)
	{
		return false;
	}

	ShowWindow(m_hWnd, SW_SHOWNORMAL);
	UpdateWindow(m_hWnd);

	m_hAccelTable = LoadAccelerators(m_hInstance, MAKEINTRESOURCE(IDI_ICONDMB));

	/// �ý��� ���� �� �ʱ�ȭ
	// ����� ��忡�� �ܼ�â ����
#ifdef _DEBUG	
	CreateConsole();
#endif // _DEBUG

	return true;
}

void BaseWindowsProcessor::OnResize(uint16_t _width, uint16_t _height)
{
	m_screenWidth = _width;
	m_screenHeight = _height;
}

void BaseWindowsProcessor::Run()
{
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
			Update();
		}
	}
}

void BaseWindowsProcessor::Finalize()
{
}

HWND BaseWindowsProcessor::GetHWND() const
{
	return m_hWnd;
}

LRESULT CALLBACK BaseWindowsProcessor::WndProc(HWND _hWnd, UINT _message, WPARAM _wParam, LPARAM _lParam)
{
	// 	BaseWindowsProcessor* processor = reinterpret_cast<BaseWindowsProcessor*>(GetWindowLongPtr(_hWnd, GWLP_USERDATA));
	switch (_message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(_wParam);
		// �޴� ������ ���� �м��մϴ�:
		switch (wmId)
		{
		case IDM_FULLSCREEN:

			break;
		case IDM_EXIT:
			DestroyWindow(_hWnd);
			break;
		default:
			return DefWindowProc(_hWnd, _message, _wParam, _lParam);
		}
	}
	break;
	// 		case WM_SIZE:
	// 			uint16_t width = LOWORD(_lParam);
	// 			uint16_t height = HIWORD(_lParam);
	// 			processor->OnResize(width, height);
	// 			break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		DestroyWindow(_hWnd);
		break;
	default:
		return DefWindowProc(_hWnd, _message, _wParam, _lParam);
	}
	return 0;
}

void BaseWindowsProcessor::Update()
{
}

void BaseWindowsProcessor::CreateConsole()
{
	// �ܼ� â �Ҵ�
	AllocConsole();

	// ǥ�� ����� ���𷺼�
	FILE* dummy;
	_wfreopen_s(&dummy, L"CONIN$", L"r", stdin);

	// ǥ�� ��� ���𷺼�
	_wfreopen_s(&dummy, L"CONOUT$", L"w", stdout);

	// ǥ�� ���� ���𷺼�
	_wfreopen_s(&dummy, L"CONOUT$", L"w", stderr);

	// �ܼ� â �ڵ��� ������
	HWND consoleWindow = GetConsoleWindow();

	// �ܼ� â�� �ʱ� ��ġ ���� (��: �»�ܿ��� 100, 100 ��ġ�� ����)
	SetWindowPos(consoleWindow, NULL, 1500, 500, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}
