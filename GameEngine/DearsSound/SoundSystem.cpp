#include "pch.h"
#include "SoundSystem.h"

class SoundSystem::Impl
{
public:
	Impl()
		: m_pSystem(nullptr)
	{
	}
	~Impl() = default;

	/// <summary>
	/// FMOD System ��ü ������ �ʱ�ȭ, ä�� �׷�� ������ �ʱ�ȭ
	/// </summary>
	/// <returns></returns>
	bool Initialize()
	{
		// System ��ü ����
		FMOD_RESULT fr = FMOD::System_Create(&m_pSystem);
		if (fr != FMOD_OK)
		{
			return false;
		}

		// System ��ü �ʱ�ȭ
		fr = m_pSystem->init(64, FMOD_INIT_NORMAL, nullptr);
		if (fr != FMOD_OK)
		{
			return false;
		}

		// ��� ���尡 ���� �ͽ��Ǵ� ������ ä�� �׷��� �����ɴϴ�.
		fr = m_pSystem->getMasterChannelGroup(&m_pMasterGroup);
		if (fr != FMOD_OK)
		{
			return false;
		}

		// ����� ä�� �׷��� �����մϴ�.
		fr = m_pSystem->createChannelGroup("BackgroundMusicGroup", &m_pBGMGroup);
		if (fr != FMOD_OK)
		{
			return false;
		}

		// ȿ���� ä�� �׷��� �����մϴ�.
		fr = m_pSystem->createChannelGroup("EffectSoundGroup", &m_pSFXGroup);
		if (fr != FMOD_OK)
		{
			return false;
		}

		// ������ ä�� �׷쿡 ����� ä�� �׷�� ȿ���� ä�� �׷��� �߰��մϴ�.
		m_pMasterGroup->addGroup(m_pBGMGroup);
		m_pMasterGroup->addGroup(m_pSFXGroup);

		return true;
	}

	/// <summary>
	/// FMOD System ��ü ������Ʈ
	/// </summary>
	void Update()
	{
		if (m_pSystem)
		{
			// �ý��� ������Ʈ
			m_pSystem->update();
		}
	}

	/// <summary>
	/// FMOD ����� ���� �� �ý��� �ʱ�ȭ, �Ҵ� ����
	/// </summary>
	void Finalize()
	{
		// ��� ���� ���� �ʱ�ȭ
		if (m_pSystem)
		{
			// 			m_pSystem->update();
						// ���� �� �ʱ�ȭ
			for (auto& sound : m_pSounds)
			{
				sound.second->release();
				sound.second = nullptr;
			}
			m_pSounds.clear();

			// ����� ä�� �� �ʱ�ȭ
			for (auto& channel : m_pBGMChannels)
			{
				channel.second->stop();
				channel.second = nullptr;
			}
			m_pBGMChannels.clear();

			// ȿ���� ä�� �� �ʱ�ȭ
			for (auto& channel : m_pSFXChannels)
			{
				channel.second->stop();
				channel.second = nullptr;
			}
			m_pSFXChannels.clear();

			// ����� ä�� �׷� �ʱ�ȭ
			if (m_pBGMGroup)
			{
				m_pBGMGroup->release();
				m_pBGMGroup = nullptr;
			}

			// ȿ���� ä�� �׷� �ʱ�ȭ
			if (m_pSFXGroup)
			{
				m_pSFXGroup->release();
				m_pSFXGroup = nullptr;
			}

			// ������ ä�� �׷� �ʱ�ȭ
			if (m_pMasterGroup)
			{
				m_pMasterGroup->release();
				m_pMasterGroup = nullptr;
			}

			// �ý��� �ʱ�ȭ �� �Ҵ� ����
// 			m_pSystem->update();

			m_pSystem->release();
			m_pSystem = nullptr;
		}
	}

	bool LoadBGM(const std::string& _filename, const std::string& _soundName)
	{
		if (m_pSystem)
		{
			// ����� ����
			if (m_pSounds.find(_soundName) == m_pSounds.end())
			{
				FMOD::Sound* pSound = nullptr;
				FMOD_RESULT fr = m_pSystem->createStream(_filename.c_str(), FMOD_LOOP_NORMAL | FMOD_2D, 0, &pSound);
				if (fr != FMOD_OK)
				{
					return false;
				}
				// ���� �ʿ� ������ ����� �߰�
				m_pSounds[_soundName] = pSound;
				return true;
			}
		}
		return false;
	}

	bool LoadSFX(const std::string& _filename, const std::string& _soundName, bool _loop)
	{
		if (m_pSystem)
		{
			FMOD::Sound* pSound = nullptr;
			// ȿ���� ����
			if (_loop)
			{
				FMOD_RESULT fr = m_pSystem->createSound(_filename.c_str(), FMOD_LOOP_NORMAL, 0, &pSound);
				if (fr != FMOD_OK)
				{
					return false;
				}
			}
			else
			{
				FMOD_RESULT fr = m_pSystem->createSound(_filename.c_str(), FMOD_LOOP_OFF, 0, &pSound);
				if (fr != FMOD_OK)
				{
					return false;
				}
			}
			// ���� �ʿ� ������ ȿ���� �߰�
			m_pSounds[_soundName] = pSound;
			return true;
		}
		return false;
	}

	bool Load3DSound(const std::string& _filename, const std::string& _soundName, bool _loop)
	{
		if (m_pSystem)
		{
			FMOD::Sound* pSound = nullptr;
			// 3D ȿ���� ����
			if (_loop)
			{
				FMOD_RESULT fr = m_pSystem->createSound(_filename.c_str(), FMOD_3D | FMOD_LOOP_NORMAL, 0, &pSound);
				if (fr != FMOD_OK)
				{
					return false;
				}
			}
			else
			{
				FMOD_RESULT fr = m_pSystem->createSound(_filename.c_str(), FMOD_3D | FMOD_LOOP_OFF, 0, &pSound);
				if (fr != FMOD_OK)
				{
					return false;
				}
			}
			// ���� �ʿ� ������ 3D ȿ���� �߰�
			m_pSounds[_soundName] = pSound;
			return true;
		}
		return false;
	}

	void PlayBGM(const std::string& _soundName)
	{
		if (!IsPlaying(_soundName))
		{
			if (m_pSystem && m_pSounds.find(_soundName) != m_pSounds.end())
			{
				FMOD::Channel* pChannel = nullptr;
				// ���ڷ� ���� Ű�� �ش��ϴ� ����� �ݺ� ���
				m_pSystem->playSound(m_pSounds[_soundName], m_pBGMGroup, false, &pChannel);
				if (pChannel)
				{
					// ä�� �ʿ� ���� ������� ä�� �߰�
					m_pBGMChannels[_soundName] = pChannel;
					// ���� ä���� ������� ä�η� ����
					m_pCurrentBGMChannel = pChannel;
				}
			}
		}
	}

	void PauseBGM()
	{
		if (m_pCurrentBGMChannel)
		{
			bool isPaused;
			// ���� ä���� ������� ��������� �˻�
			m_pCurrentBGMChannel->getPaused(&isPaused);
			// ������̸� �Ͻ������ϰ�, �Ͻ����� ���¶�� �ٽ� ���
			m_pCurrentBGMChannel->setPaused(!isPaused);
		}
	}

	void StopBGM()
	{
		if (m_pCurrentBGMChannel)
		{
			// ���� ä���� ������� ����
			m_pCurrentBGMChannel->stop();
			m_pCurrentBGMChannel = nullptr;
		}
	}

	void FadeInBGM(const std::string& _soundName, float _fadeTime)
	{
		PlayBGM(_soundName);
		if (m_pCurrentBGMChannel)
		{
			float targetVolume = 1.0f;
			float volumeStep = targetVolume / (_fadeTime * 60); // �����ϴ� FPS�� ���� ����

			for (int i = 0; i < _fadeTime * 60; ++i)
			{
				float currentVolume;
				m_pCurrentBGMChannel->getVolume(&currentVolume);
				currentVolume += volumeStep;
				m_pCurrentBGMChannel->setVolume(currentVolume);
			}
		}
	}

	void FadeOutBGM(float _fadeTime)
	{
		float currentVolume;
		m_pCurrentBGMChannel->getVolume(&currentVolume);
		float step = currentVolume / (_fadeTime * 60); // ���̵� �ƿ��� ���� �� �����Ӵ� ���ҷ� ���

		// �� ������ ������ ���ݾ� ���̱�
		for (int i = 0; i < _fadeTime * 60; ++i)
		{
			currentVolume -= step;
			if (currentVolume <= 0)
			{
				break;
			}
			m_pCurrentBGMChannel->setVolume(currentVolume);
		}

		m_pCurrentBGMChannel->setVolume(0); // �������� ������ 0���� ����
		StopBGM();
	}

	void PlaySFX(const std::string& _soundName)
	{
		// ���尡 �̹� ��� ������ Ȯ�� (�ִ� �ν��Ͻ� ���� 40�� ����)
		const int MAX_SFX_INSTANCES = 40;
		int currentInstances = 0;

		if (m_pSystem && m_pSounds.find(_soundName) != m_pSounds.end())
		{
			for (auto& channel : m_pSFXChannels)
			{
				bool isPlaying = false;
				if (channel.second && channel.second->isPlaying(&isPlaying) == FMOD_OK && isPlaying)
				{
					currentInstances++;
				}
			}

			// ���� ��� ���� �ν��Ͻ� ���� �ִ밪�� ���� ������ ���
			if (currentInstances < MAX_SFX_INSTANCES)
			{
				FMOD::Channel* pChannel = nullptr;
				m_pSystem->playSound(m_pSounds[_soundName], m_pSFXGroup, false, &pChannel);
				if (pChannel)
				{
					m_pSFXChannels[_soundName] = pChannel;
				}
			}
		}
	}

	void StopSFX(const std::string& _soundName)
	{
		// ������ ������ ��� �ν��Ͻ��� ����
		if (m_pSFXChannels.find(_soundName) != m_pSFXChannels.end())
		{
			FMOD::Channel* pChannel = m_pSFXChannels[_soundName];
			if (pChannel)
			{
				pChannel->stop(); // ���� ����
				m_pSFXChannels.erase(_soundName); // �ʿ��� ä�� ����
			}
		}
	}

	void Play3DSound(const std::string& _soundName, FMOD_VECTOR _position)
	{
		if (m_pSystem && m_pSounds.find(_soundName) != m_pSounds.end())
		{
			FMOD::Channel* pChannel = nullptr;
			m_pSystem->playSound(m_pSounds[_soundName], 0, false, &pChannel);
			if (pChannel)
			{
				FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f };
				pChannel->set3DAttributes(&_position, &velocity);
				pChannel->setPaused(false);
				m_pSFXChannels[_soundName] = pChannel;
			}
		}
	}

	void SetListenerPosition(FMOD_VECTOR _position)
	{
		if (m_pSystem)
		{
			FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f };
			FMOD_VECTOR forward = { 0.0f, 0.0f, 1.0f };
			FMOD_VECTOR up = { 0.0f, 1.0f, 0.0f };
			m_pSystem->set3DListenerAttributes(0, &_position, &velocity, &forward, &up);
		}
	}

	void SetBGMVolume(float _volume)
	{
		_volume = std::clamp(_volume, 0.f, 1.f);
		m_originVolume = _volume;
		if (m_pBGMGroup)
		{
			m_pBGMGroup->setVolume(_volume);
		}
	}

	void SetSFXVolume(float _volume)
	{
		_volume = std::clamp(_volume, 0.f, 1.f);
		if (m_pSFXGroup)
		{
			m_pSFXGroup->setVolume(_volume);
		}
	}

	void SetMasterVolume(float _volume)
	{
		_volume = std::clamp(_volume, 0.f, 1.f);
		if (m_pMasterGroup)
		{
			m_pMasterGroup->setVolume(_volume);
		}
	}

	bool IsPlaying(const std::string& _soundName)
	{
		bool isPlaying = false;

		// ����� ä�ο��� Ȯ��
		if (m_pBGMChannels.find(_soundName) != m_pBGMChannels.end())
		{
			FMOD::Channel* pChannel = m_pBGMChannels[_soundName];
			isPlaying = false;
			if (pChannel && pChannel->isPlaying(&isPlaying) == FMOD_OK)
			{
				return isPlaying;
			}
		}

		// ȿ���� ä�ο��� Ȯ��
		if (m_pSFXChannels.find(_soundName) != m_pSFXChannels.end())
		{
			FMOD::Channel* pChannel = m_pSFXChannels[_soundName];
			isPlaying = false;
			if (pChannel && pChannel->isPlaying(&isPlaying) == FMOD_OK)
			{
				return isPlaying;
			}
		}

		// ��� ���� ���尡 ���� ��� false ��ȯ
		return isPlaying;
	}

	std::string GetFMODVersion()
	{
		uint32_t version = FMOD_VERSION;

		// ���� ���� ���
		uint32_t major = (version >> 16) & 0xFFFF;
		uint32_t minor = (version >> 8) & 0xFF;
		uint32_t patch = version & 0xFF;
		std::string strVersion = std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(patch);
		return strVersion;
	}

private:
	FMOD::System* m_pSystem;	// FMOD �ý��� ��ü
	FMOD::ChannelGroup* m_pMasterGroup;	// ��� ä���� �����ϴ� ä�� �׷�
	FMOD::ChannelGroup* m_pBGMGroup;	// ����� ä�ε��� �׷�
	FMOD::ChannelGroup* m_pSFXGroup;	// ȿ���� ä�ε��� �׷�
	FMOD::Channel* m_pCurrentBGMChannel;	// ���� ������� ����� ä��

	// �ý��ۿ� ��ϵǾ��ִ� ���� ��ü�� <������ ���ڿ� Ű, ���� ��ü�� ������>
	std::unordered_map<std::string, FMOD::Sound*> m_pSounds;

	// �ý��ۿ� ��ϵǾ��ִ� ���� ��ü�� <ä���� ���ڿ� Ű, ä�� ��ü�� ������>
	std::unordered_map<std::string, FMOD::Channel*> m_pBGMChannels;	// ����� ä�� ��ü��

	// �ý��ۿ� ��ϵǾ��ִ� ���� ��ü�� <ä���� ���ڿ� Ű, ä�� ��ü�� ������>
	std::unordered_map<std::string, FMOD::Channel*> m_pSFXChannels;	// ȿ���� ä�� ��ü��

	float m_originVolume = 1.f;
};



SoundSystem::SoundSystem()
	:m_pImpl(std::make_unique<Impl>())
{
}

SoundSystem::~SoundSystem() = default;

bool SoundSystem::Initialize()
{
	return m_pImpl->Initialize();
}

void SoundSystem::Update()
{
	m_pImpl->Update();
}

void SoundSystem::Finalize()
{
	m_pImpl->Finalize();
}

bool SoundSystem::LoadBGM(const std::string& _filename, const std::string& _soundName)
{
	return m_pImpl->LoadBGM(_filename, _soundName);
}

void SoundSystem::PlayBGM(const std::string& _soundName)
{
	m_pImpl->PlayBGM(_soundName);
}

void SoundSystem::PauseBGM()
{
	m_pImpl->PauseBGM();
}

void SoundSystem::StopBGM()
{
	m_pImpl->StopBGM();
}

void SoundSystem::FadeInBGM(const std::string& _soundName, float _fadeTime)
{
	m_pImpl->FadeInBGM(_soundName, _fadeTime);
}

void SoundSystem::FadeOutBGM(float _fadeTime)
{
	m_pImpl->FadeOutBGM(_fadeTime);
}

bool SoundSystem::LoadSFX(const std::string& _filename, const std::string& _soundName, bool _loop)
{
	return m_pImpl->LoadSFX(_filename, _soundName, _loop);
}

bool SoundSystem::Load3DSound(const std::string& _filename, const std::string& _soundName, bool _loop)
{
	return m_pImpl->Load3DSound(_filename, _soundName, _loop);
}

void SoundSystem::PlaySFX(const std::string& _soundName)
{
	m_pImpl->PlaySFX(_soundName);
}

void SoundSystem::StopSFX(const std::string& _soundName)
{
	m_pImpl->StopSFX(_soundName);
}

void SoundSystem::Play3DSound(const std::string& _soundName, float _x, float _y, float _z)
{
	m_pImpl->Play3DSound(_soundName, FMOD_VECTOR(_x, _y, _z));
}

void SoundSystem::SetListenerPosition(float _x, float _y, float _z)
{
	m_pImpl->SetListenerPosition(FMOD_VECTOR(_x, _y, _z));
}

void SoundSystem::SetBGMVolume(float _volume)
{
	m_pImpl->SetBGMVolume(_volume);
}

void SoundSystem::SetSFXVolume(float _volume)
{
	m_pImpl->SetSFXVolume(_volume);
}

void SoundSystem::SetMasterVolume(float _volume)
{
	m_pImpl->SetMasterVolume(_volume);
}

bool SoundSystem::IsPlaying(const std::string& _soundName)
{
	return m_pImpl->IsPlaying(_soundName);
}

std::string SoundSystem::GetFMODVersion()
{
	return m_pImpl->GetFMODVersion();
}
