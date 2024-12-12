#pragma once
#include "pch.h"

// Ű �ڵ� ������
enum class KEY
{
	NONE = 0,

	/// Mouse
	LBUTTON = 0x01,	// ���� ���콺 ��ư
	RBUTTON = 0x02, // ������ ���콺 ��ư
	MBUTTON = 0x04, // ���콺 ��� ��ư
	XBUTTON_ONE = 0x05, // X1 ���콺 ��ư
	XBUTTON_TWO = 0x06, // X2 ���콺 ��ư

	/// KeyBoard
	BACK = 0x08,	// �齺���̽� Ű
	TAB = 0x09,	// Tab Ű
	RETURN = 0x0D,	// Enter Ű

	SHIFT = 0x10, // Shift Ű
	CTRL = 0x11, // Ctrl Ű
	ALT = 0x12, // Alt Ű

	CAPSLOCK = 0x14, // Caps Lock Ű

	ESCAPE = 0x1B, // Esc Ű

	SPACE = 0x20, // �����̽���
	PGUP = 0x21, // Page Up Ű
	PGDN = 0x22, // Page Down Ű
	END = 0x23, // End Ű
	HOME = 0x24, // Home Ű
	LEFT = 0x25, // ���� ȭ��ǥ Ű
	UP = 0x26, // ���� ȭ��ǥ Ű
	RIGHT = 0x27, // ������ ȭ��ǥ Ű
	DOWN = 0x28, // �Ʒ��� ȭ��ǥ Ű

	INS = 0x2D, // Ins Ű
	DEL = 0x2E, // DEL Ű

	D0 = 0x30, // 0 Ű
	D1 = 0x31, // 1 Ű
	D2 = 0x32, // 2 Ű
	D3 = 0x33, // 3 Ű
	D4 = 0x34, // 4 Ű
	D5 = 0x35, // 5 Ű
	D6 = 0x36, // 6 Ű
	D7 = 0x37, // 7 Ű
	D8 = 0x38, // 8 Ű
	D9 = 0x39, // 9 Ű

	A = 0x41, // A Ű
	B = 0x42, // B Ű
	C = 0x43, // C Ű
	D = 0x44, // D Ű
	E = 0x45, // E Ű
	F = 0x46, // F Ű
	G = 0x47, // G Ű
	H = 0x48, // H Ű
	I = 0x49, // I Ű
	J = 0x4A, // J Ű
	K = 0x4B, // K Ű
	L = 0x4C, // L Ű
	M = 0x4D, // M Ű
	N = 0x4E, // N Ű
	O = 0x4F, // O Ű
	P = 0x50, // P Ű
	Q = 0x51, // Q Ű
	R = 0x52, // R Ű
	S = 0x53, // S Ű
	T = 0x54, // T Ű
	U = 0x55, // U Ű
	V = 0x56, // V Ű
	W = 0x57, // W Ű
	X = 0x58, // X Ű
	Y = 0x59, // Y Ű
	Z = 0x5A, // Z Ű

	WIN = 0x5B, // ���� Windows Ű

	F1 = 0x70, // F1 Ű
	F2 = 0x71, // F2 Ű
	F3 = 0x72, // F3 Ű
	F4 = 0x73, // F4 Ű
	F5 = 0x74, // F5 Ű
	F6 = 0x75, // F6 Ű
	F7 = 0x76, // F7 Ű
	F8 = 0x77, // F8 Ű
	F9 = 0x78, // F9 Ű
	F10 = 0x79, // F10 Ű
	F11 = 0x7A, // F11 Ű
	F12 = 0x7B, // F12 Ű

	NUMLOCK = 0x90, // Num Lock Ű
	SCROLLLOCK = 0x91, // Scroll Lock Ű

	LSHIFT = 0xA0, // ���� Shift Ű
	RSHIFT = 0xA1, // ������ Shift Ű
	LCONTROL = 0xA2, // ���� Ctrl Ű
	RCONTROL = 0xA3, // ������ Ctrl Ű
	LMENU = 0xA4, // ���� Alt Ű
	RMENU = 0xA5, // ������ Alt Ű

	OEM_1 = 0xBA,		// ;: Ű
	OEM_PLUS = 0xBB,	// + Ű
	OEM_COMMA = 0xBC,	// , Ű
	OEM_MINUS = 0xBD,	// - Ű
	OEM_PERIOD = 0xBE,	// . Ű
	OEM_2 = 0xBF,		// /?  Ű
	OEM_3 = 0xC0,		// `~  Ű
	OEM_4 = 0xDB,		// [{  Ű
	OEM_5 = 0xDC,		// \|  Ű
	OEM_6 = 0xDD,		// ]}  Ű
	OEM_7 = 0xDE,		// '"  Ű

	LAST = 0xFF
};

// Ű ���� ������
enum class KEY_STATE
{
	NONE,
	AWAY,
	TAP,
	HOLD
};

// Ű ���� ����ü
struct KeyInfo
{
	KeyInfo()
		: keyState(KEY_STATE::NONE), isPushed(false)
	{}
	KeyInfo(KEY_STATE keyState, bool isPushed)
		: keyState(keyState), isPushed(isPushed)
	{}
	~KeyInfo() = default;

	KEY_STATE keyState;	// Ű ����
	bool isPushed;		// ���� �����ӿ� Ű�� ���Ⱦ�����
};

