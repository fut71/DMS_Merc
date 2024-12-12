#pragma once
#include "IManager.h"
#include "KeyInfo.h"

class IInputDevice;

// ����̽��� ����Ʈ ����
// ���ø� Ŭ������ Update �� GetKeyInfo �޼��带 �����ϴ��� Ȯ���մϴ�.
template <typename T>
concept InputDevice = requires(T _device)
{
	{ _device.Update() }->std::same_as<void>;
	{ _device.GetKeyInfo(KEY{}) }->std::same_as<KeyInfo>;
};

/// <summary>
/// Ű����, ���콺 ���� �Է� ����̽����� �����ϴ� Ŭ�����Դϴ�.
/// �ٸ� ����̽��鵵 �߰��� �� �ֽ��ϴ�.
/// 2024.05.17 _Doyo;
/// </summary>
class InputManager
	: public IManager
{
public:
	InputManager() = default;
	~InputManager() = default;

	virtual bool Initialize() override;
	virtual void FixedUpdate(float _fixedDTime) override {}
	virtual void Update(float _dTime) override;
	virtual void LateUpdate(float _dTime) override {}
	virtual void Finalize() override;

	KEY_STATE GetKeyState(KEY _key);

	bool GetKey(KEY _key);
	bool GetKeyDown(KEY _key);
	bool GetKeyUp(KEY _key);

	Vector2 GetMousePos() const;
	Vector2 GetDeltaMousePos() const;

	void AddMouseCursor(const std::string& _file);
	void SetMouseCursor(int _cursorName);
	void SetMouseCursor();

	// imgui�� beginrender���� ���콺 Ŀ�� �ʱ�ȭ�� �ֱ� ������ beginrender���Ŀ� �ٽ� �����������
	HCURSOR GetCurrentCursor() const;

private:
	// �Ŵ����� ����̽��� �߰��ϴ� �Լ��Դϴ�. �ʱ�ȭ �Լ� ������ ȣ���Ͽ� ����̽��� �߰��մϴ�.
	template <InputDevice T>
	void AddDevice(std::shared_ptr<T> _pDevice)
	{
		m_pDevices.push_back(_pDevice);
	}

private:
	std::vector<std::shared_ptr<IInputDevice>> m_pDevices;	// �����ϰ��ִ� ����̽����� �迭
	std::vector<HCURSOR> m_cursors;
	HCURSOR m_currentCursor;
};

