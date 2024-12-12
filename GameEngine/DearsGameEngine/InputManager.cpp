#include "InputManager.h"
#include "InputDevice.h"

// �Ŵ��� �ʱ�ȭ Ŭ�����Դϴ�. ���� ����̽����� �߰��մϴ�.
bool InputManager::Initialize()
{
	std::shared_ptr<IInputDevice> keyboard = std::make_shared<Keyboard>();
	std::shared_ptr<IInputDevice> mouse = std::make_shared<Mouse>();

	AddDevice(keyboard);
	AddDevice(mouse);

	return true;
}

// ������Ʈ �Լ��Դϴ�. �������� ����̽����� ������Ʈ�մϴ�.
void InputManager::Update(float _dTime)
{
	for (auto& device : m_pDevices)
	{
		device->Update();
	}

	// LBUTTON Ű ���� Ȯ��
	//KEY_STATE state = GetKeyState(KEY::LBUTTON);

	//// LBUTTON Ű ���¸� ����� â�� ���
	//switch (state)
	//{
	//case KEY_STATE::NONE:
	//	DLOG(LOG_INFO, "NONE");
	//	break;
	//case KEY_STATE::AWAY:
	//	DLOG(LOG_INFO, "AWA");
	//	break;
	//case KEY_STATE::TAP:
	//	DLOG(LOG_INFO, "TAP");
	//	break;
	//case KEY_STATE::HOLD:
	//	DLOG(LOG_INFO, "HOLD");
	//	break;
	//}
}

// �������� ����̽����� �޸� �Ҵ��� �����մϴ�.
void InputManager::Finalize()
{
}

/// <summary>
/// Ư�� Ű�� ���¸� ��ȯ�մϴ�.
/// </summary>
/// <param name="_key">���¸� ��ȯ�� Ű</param>
/// <returns>���ڷ� ���� Ű�� ����</returns>
KEY_STATE InputManager::GetKeyState(KEY _key)
{
	KEY_STATE keyState;
	for (const auto& device : m_pDevices)
	{
		auto info = device->GetKeyInfo(_key);
		keyState = info.keyState;
	}
	return keyState;
}

bool InputManager::GetKey(KEY _key)
{
	return GetKeyState(_key) == KEY_STATE::HOLD;
}

bool InputManager::GetKeyDown(KEY _key)
{
	return GetKeyState(_key) == KEY_STATE::TAP;
}

bool InputManager::GetKeyUp(KEY _key)
{
	return GetKeyState(_key) == KEY_STATE::AWAY;
}

// ���콺�� ��ġ�� ��ȯ�մϴ�.
Vector2 InputManager::GetMousePos() const
{
	for (const auto& device : m_pDevices)
	{
		// �������� ����̽��� ���콺�� ���� ���
		if (auto mouse = std::dynamic_pointer_cast<Mouse>(device))
		{
			return mouse->GetMousePos();
		}
	}
	DLOG(LOG_WARN, "There is no mouse device in process.");
	return Vector2();
}

// ���� �����Ӱ� ���� �������� ���콺�� ��ġ ���̸� ��ȯ�մϴ�.
Vector2 InputManager::GetDeltaMousePos() const
{
	for (const auto& device : m_pDevices)
	{
		// �������� ����̽��� ���콺�� ���� ���
		if (auto mouse = std::dynamic_pointer_cast<Mouse>(device))
		{
			return mouse->GetDeltaMousePos();
		}
	}
	DLOG(LOG_WARN, "There is no mouse device in process.");
	return Vector2();
}

void InputManager::AddMouseCursor(const std::string& _file)
{
	std::wstring wFile = std::wstring(_file.begin(), _file.end());
	HCURSOR cursor = (HCURSOR)LoadImage(NULL, wFile.c_str(), IMAGE_CURSOR, 32, 32, LR_LOADFROMFILE);
	m_cursors.push_back(cursor);
}

void InputManager::SetMouseCursor(int _cursorName)
{
	m_currentCursor = m_cursors[_cursorName];
}

void InputManager::SetMouseCursor()
{
	SetCursor(m_currentCursor);
}

HCURSOR InputManager::GetCurrentCursor() const
{
	return m_currentCursor;
}
