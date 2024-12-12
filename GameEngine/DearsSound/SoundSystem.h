#pragma once
#include "framework.h"

#ifdef SOUNDSYSTEM_EXPORTS
#define SOUNDSYSTEM_API __declspec(dllexport)
#else
#define SOUNDSYSTEM_API __declspec(dllimport)
#endif

/// <summary>
/// ���� �ý��� Ŭ�����Դϴ�.
/// ĸ��ȭ�� ���� Impl ��ü�� ���� ������ ������ϴ�.
/// dll main���� �ϳ��� ���μ������� �ϳ��� �����ؼ� ����մϴ�.
/// �����, ȿ����, 3Dȿ����, ���� ���� ��ɵ��� �����մϴ�.
/// 2024.05.21 _Doyo;
/// </summary>
class SOUNDSYSTEM_API SoundSystem
{
public:
	SoundSystem();
	~SoundSystem();

	/// <summary>
	/// SoundSystem �ʱ�ȭ
	/// </summary>
	/// <returns></returns>
	bool Initialize();

	/// <summary>
	/// SoundSystem ������Ʈ
	/// ����� ����, 3D ���� ó��, ȿ�� �� DSP ó��, ä�� ����, �ݹ� ó�� ���� ������ �մϴ�.
	/// </summary>
	void Update();

	/// <summary>
	/// SoundSystem�� ���ҽ����� �����մϴ�.
	/// </summary>
	void Finalize();

	/// <summary>
	/// ��������� ����� ���带 ����� ���ۿ� �����մϴ�.
	/// </summary>
	/// <param name="_filename">���ϸ�</param>
	/// <param name="_soundName">���ڿ� Ű</param>
	/// <returns></returns>
	bool LoadBGM(const std::string& _filename, const std::string& _soundName);

	/// <summary>
	/// ȿ�������� ����� ���带 ����� ���ۿ� �����մϴ�.
	/// </summary>
	/// <param name="_filename">���ϸ�</param>
	/// <param name="_soundName">���ڿ� Ű</param>
	/// <returns></returns>
	bool LoadSFX(const std::string& _filename, const std::string& _soundName, bool _loop);

	/// <summary>
	/// 3D ȿ�������� ����� ���带 ����� ���ۿ� �����մϴ�.
	/// </summary>
	/// <param name="_filename">���ϸ�</param>
	/// <param name="_soundName">���ڿ� Ű</param>
	/// <returns></returns>
	bool Load3DSound(const std::string& _filename, const std::string& _soundName, bool _loop);

	/// <summary>
	/// ������� �ݺ�����մϴ�.
	/// </summary>
	/// <param name="_soundName">���ڿ� Ű</param>
	void PlayBGM(const std::string& _soundName);

	/// <summary>
	/// ���� ������� ������� �Ͻ� �����մϴ�.
	/// </summary>
	void PauseBGM();

	/// <summary>
	/// ���� ������� ������� �����մϴ�.
	/// </summary>
	void StopBGM();

	/// <summary>
	/// ������� ���̵� �� �մϴ�.
	/// </summary>
	/// <param name="_soundName">���ڿ� Ű</param>
	/// <param name="_fadeTime">���̵� �� �Ǵ� �ð�</param>
	void FadeInBGM(const std::string& _soundName, float _fadeTime);

	/// <summary>
	/// ���� ������� ������� ���̵� �ƿ� �մϴ�.
	/// </summary>
	/// <param name="_fadeTime">���̵� �� �Ǵ� �ð�</param>
	void FadeOutBGM(float _fadeTime);

	/// <summary>
	/// ȿ������ ����մϴ�.
	/// </summary>
	/// <param name="_soundName">���ڿ� Ű</param>
	void PlaySFX(const std::string& _soundName);

	/// <summary>
	/// ȿ������ �����մϴ�.
	/// </summary>
	/// <param name="_soundName"></param>
	void StopSFX(const std::string& _soundName);

	/// <summary>
	/// 3D ȿ������ ����մϴ�.
	/// </summary>
	/// <param name="_soundName">���ڿ� Ű</param>
	/// <param name="_x">ȿ������ x��ǥ</param>
	/// <param name="_y">ȿ������ y��ǥ</param>
	/// <param name="_z">ȿ������ z��ǥ</param>
	void Play3DSound(const std::string& _soundName, float _x, float _y, float _z);

	/// <summary>
	/// 3D ȿ������ �����ϱ� ���� �������� ��ġ�� �����մϴ�.
	/// </summary>
	/// <param name="_x">�������� x��ǥ</param>
	/// <param name="_y">�������� y��ǥ</param>
	/// <param name="_z">�������� z��ǥ</param>
	void SetListenerPosition(float _x, float _y, float _z);

	/// <summary>
	/// ������� ������ �����մϴ�.
	/// </summary>
	/// <param name="_volume">����ũ�� (0.0 ~ 1.0)</param>
	void SetBGMVolume(float _volume);

	/// <summary>
	/// ȿ������ ������ �����մϴ�.
	/// </summary>
	/// <param name="_volume">����ũ�� (0.0 ~ 1.0)</param>
	void SetSFXVolume(float _volume);

	/// <summary>
	/// ������ ������ �����մϴ�.
	/// </summary>
	/// <param name="_volume">����ũ�� (0.0 ~ 1.0)</param>
	void SetMasterVolume(float _volume);

	/// <summary>
	/// Ư�� ���尡 ��������� Ȯ���մϴ�.
	/// </summary>
	/// <param name="_soundName">Ȯ���� ���� �̸�</param>
	/// <returns>������̸� true ��ȯ</returns>
	bool IsPlaying(const std::string& _soundName);

	/// <summary>
	/// ���� ������� FMOD ���̺귯���� ������ �����ɴϴ�.
	/// </summary>
	/// <returns>FMOD�� ���� ���ڿ�</returns>
	std::string GetFMODVersion();

private:
	class Impl;
	std::unique_ptr<Impl> m_pImpl;	// ���� �ý��� ĸ��ȭ�� ���� Impl ��ü
};

extern "C"
{
	SOUNDSYSTEM_API SoundSystem* GetSoundSystem();
	SOUNDSYSTEM_API void ReleaseSoundSystem();
}