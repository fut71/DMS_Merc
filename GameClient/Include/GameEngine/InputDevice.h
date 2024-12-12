#pragma once
#include "pch.h"
#include "KeyInfo.h"

/// <summary>
/// �Է� ��ġ �������̽� Ŭ�����Դϴ�.
/// 2024.05.17 _Doyo
/// </summary>
class IInputDevice
{
public:
	virtual ~IInputDevice() = default;
	virtual void Update() = 0;
	virtual KeyInfo GetKeyInfo(KEY _key) const = 0;
};

/// <summary>
/// Ű������ ������ �����ϰ� ������Ʈ�ϴ� Ŭ�����Դϴ�.
/// </summary>
class Keyboard : public IInputDevice
{
public:
	// Ű���忡 �ش��ϴ� Ű �ڵ� �ʱ�ȭ
	Keyboard()
	{
		std::vector<KEY> keys =
		{
			KEY::BACK, KEY::TAB, KEY::RETURN, KEY::SHIFT, KEY::CTRL, KEY::ALT,
			KEY::CAPSLOCK, KEY::ESCAPE, KEY::SPACE, KEY::PGUP, KEY::PGDN,
			KEY::END, KEY::HOME, KEY::LEFT, KEY::UP, KEY::RIGHT, KEY::DOWN,
			KEY::INS, KEY::DEL, KEY::D0, KEY::D1, KEY::D2, KEY::D3, KEY::D4,
			KEY::D5, KEY::D6, KEY::D7, KEY::D8, KEY::D9, KEY::A, KEY::B, KEY::C,
			KEY::D, KEY::E, KEY::F, KEY::G, KEY::H, KEY::I, KEY::J, KEY::K, KEY::L,
			KEY::M, KEY::N, KEY::O, KEY::P, KEY::Q, KEY::R, KEY::S, KEY::T, KEY::U,
			KEY::V, KEY::W, KEY::X, KEY::Y, KEY::Z, KEY::WIN, KEY::F1, KEY::F2,
			KEY::F3, KEY::F4, KEY::F5, KEY::F6, KEY::F7, KEY::F8, KEY::F9, KEY::F10,
			KEY::F11, KEY::F12, KEY::NUMLOCK, KEY::SCROLLLOCK, KEY::LSHIFT, KEY::RSHIFT,
			KEY::LCONTROL, KEY::RCONTROL, KEY::LMENU, KEY::RMENU, KEY::OEM_1, KEY::OEM_PLUS,
			KEY::OEM_COMMA, KEY::OEM_MINUS, KEY::OEM_PERIOD, KEY::OEM_2, KEY::OEM_3, KEY::OEM_4,
			KEY::OEM_5, KEY::OEM_6, KEY::OEM_7
		};

		for (auto& key : keys)
		{
			m_keyInfos[key] = KeyInfo();
		}
	}

	~Keyboard()
	{
		m_keyInfos.clear();
	}

	// Ű���� ���� ������Ʈ
	virtual void Update() override
	{
		HWND hWnd = GetFocus();

		if (hWnd != nullptr)
		{
			for (auto& key : m_keyInfos)
			{
				if (GetAsyncKeyState(static_cast<int>(key.first)) & 0x8000)
				{
					key.second.keyState = key.second.isPushed
						? KEY_STATE::HOLD : KEY_STATE::TAP;
					key.second.isPushed = true;
				}
				else
				{
					key.second.keyState = key.second.isPushed
						? KEY_STATE::AWAY : KEY_STATE::NONE;
					key.second.isPushed = false;
				}
			}
		}
		else
		{
			for (auto& key : m_keyInfos)
			{
				key.second.isPushed = false;
				if (KEY_STATE::TAP == key.second.keyState
					|| KEY_STATE::HOLD == key.second.keyState)
				{
					key.second.keyState = KEY_STATE::AWAY;
				}
				else
				{
					key.second.keyState = KEY_STATE::NONE;
				}
			}
		}
	}

	// Ű�� ������ ��ȯ
	virtual KeyInfo GetKeyInfo(KEY _key) const override
	{
		if (m_keyInfos.find(_key) != m_keyInfos.end())
		{
			return m_keyInfos.find(_key)->second;
		}
	}

private:
	std::map<KEY, KeyInfo> m_keyInfos;

};

/// <summary>
/// ���콺�� ������ �����ϰ� ������Ʈ�ϴ� Ŭ�����Դϴ�.
/// </summary>
class Mouse : public IInputDevice
{
public:
	Mouse()
	{
		// ��� ��ư�� �ʱ�ȭ�մϴ�.
		std::vector<KEY> keys =
		{
			KEY::LBUTTON, KEY::RBUTTON, KEY::MBUTTON, KEY::XBUTTON_ONE, KEY::XBUTTON_TWO
		};

		for (auto& key : keys)
		{
			m_keyInfos[key] = KeyInfo();
		}
	}

	~Mouse()
	{
		m_keyInfos.clear();
	}

	virtual void Update() override
	{
		HWND hWnd = GetFocus();

		m_prevPosition = m_currentPosition;
		POINT temp;
		GetCursorPos(&temp);
		ScreenToClient(hWnd, &temp);
		m_currentPosition = Vector2(static_cast<float>(temp.x), static_cast<float>(temp.y));

		// ���콺 ���� ������Ʈ
		if (hWnd != nullptr)
		{
			for (auto& key : m_keyInfos)
			{
				if (GetAsyncKeyState(static_cast<int>(key.first)) & 0x8000)
				{
					key.second.keyState = key.second.isPushed
						? KEY_STATE::HOLD : KEY_STATE::TAP;
					key.second.isPushed = true;
				}
				else
				{
					key.second.keyState = key.second.isPushed
						? KEY_STATE::AWAY : KEY_STATE::NONE;
					key.second.isPushed = false;
				}
			}
		}
		else
		{
			for (auto& key : m_keyInfos)
			{
				key.second.isPushed = false;
				if (KEY_STATE::TAP == key.second.keyState
					|| KEY_STATE::HOLD == key.second.keyState)
				{
					key.second.keyState = KEY_STATE::AWAY;
				}
				else
				{
					key.second.keyState = KEY_STATE::NONE;
				}
			}
		}
	}

	// Ű�� ������ ��ȯ
	virtual KeyInfo GetKeyInfo(KEY _key) const override
	{
		if (m_keyInfos.find(_key) != m_keyInfos.end())
		{
			return m_keyInfos.find(_key)->second;
		}
	}

	// ���콺�� ��ġ ��ȯ
	Vector2 GetMousePos() const
	{
		return m_currentPosition;
	}

	// ���� �����Ӱ� ���� �������� ���콺�� ��ġ ���� ��ȯ
	Vector2 GetDeltaMousePos() const
	{
		return m_currentPosition - m_prevPosition;
	}

private:
	std::map<KEY, KeyInfo> m_keyInfos;
	Vector2 m_currentPosition;
	Vector2 m_prevPosition;

};

/// <summary>
/// �Է� ����̽��� �߰� �� ��� ���� �ڵ��Դϴ�.
/// 2024.05.20 _Doyo;
/// </summary>
class CustomDevice : IInputDevice
{
public:
	CustomDevice()
	{
		// ��� ��ư�� �ʱ�ȭ�մϴ�.
		std::vector<KEY> keys =
		{
			/*����̽��� Ű �ڵ��*/
		};

		for (auto& key : keys)
		{
			m_keyInfos[key] = KeyInfo();
		}
	}

	// ����̽� ���� ������Ʈ
	virtual void Update() override
	{
		HWND hWnd = GetFocus();

		if (hWnd != nullptr)
		{
			for (auto& key : m_keyInfos)
			{
				if (GetAsyncKeyState(static_cast<int>(key.first)) & 0x8000)
				{
					key.second.keyState = key.second.isPushed
						? KEY_STATE::HOLD : KEY_STATE::TAP;
					key.second.isPushed = true;
				}
				else
				{
					key.second.keyState = key.second.isPushed
						? KEY_STATE::AWAY : KEY_STATE::NONE;
					key.second.isPushed = false;
				}
			}
		}
		else
		{
			for (auto& key : m_keyInfos)
			{
				key.second.isPushed = false;
				if (KEY_STATE::TAP == key.second.keyState
					|| KEY_STATE::HOLD == key.second.keyState)
				{
					key.second.keyState = KEY_STATE::AWAY;
				}
				else
				{
					key.second.keyState = KEY_STATE::NONE;
				}
			}
		}
	}

private:
	std::map<KEY, KeyInfo> m_keyInfos;

};