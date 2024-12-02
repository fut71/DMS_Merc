#pragma once
#include "IData.h"
#include <nlohmann/json.hpp>
#include <fstream>

class UserData 
	: public IData
{
public:
	struct GraphicsData								// �׷��� ����
	{
		std::string screenResolution = "1920x1080"; // ��ũ�� ũ�� ����
		bool fullscreen = false;                    // Ǯ��ũ�� ���� ����
		bool vsync = false;                         // VSync ����
		bool drawShadow = true;                     // �׸��� ������
	} graphics;

	struct AudioData								// ����� ����
	{
		int masterVolume = 3;						// ������ ����
		int bgmVolume = 3;							// ����� ����
		int sfxVolume = 3;							// ȿ���� ����
	} audio;

	struct ConfigData								
	{
		int fps = 60;								// ������ ����
	} config;

	struct GameplayData								// �ΰ��� �÷��� ���� ����
	{
		int moneyAmount = 30;						// �� ��ô�ݾ�
		float moneySpeed = 1.5f;					// �� �ӵ�
		float projectileSpeed = 1.0f;				// ����ü �ӵ�
	} gameplay;

	struct DebugData								// ����׿� ��� Ȱ��ȭ/��Ȱ��ȭ
	{               
		bool drawCollider = false;					// �ݶ��̴� ������
		bool drawFps = false;						// FPS �� ��ŸŸ�� ���
		bool drawCameraInfo = false;				// ī�޶� ���� ���
		bool flyingCamera = false;					// �ö��� ī�޶� �� ���� ��ȯ
		bool inputSceneChange = true;				// Ű �Է����� �� ��ȯ
		bool logging = true;						// �α� �ۼ�
	} debug;

	UserData(const std::string& filePath)
		: m_filePath(filePath) 
	{
	}

	void Load();
	void Save() const;

private:
	std::string m_filePath;

};
