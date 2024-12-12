#pragma once
#include "Component.h"
#include "Texture2D.h"

enum class FadeInOutState
{
	FADE_IN, // �䱸�Ǵ� alpha �� : 0.0f; ���� �����
	NONE,
	FADE_OUT, // �䱸�Ǵ� alpha �� : 1.0f; ���� ��ο���
};

/// <summary>
/// ���̵��� �ƿ��� ����ϴ� ������Ʈ
/// </summary>
class FadeInOut :
	public Component
{
public:
	FadeInOut(std::shared_ptr<Entity> _owner, Texture2D* _pTexture2D = nullptr, float _alpha = 1.0f, FadeInOutState _state = FadeInOutState::NONE)
		: Component(_owner), mpTexture2D(_pTexture2D), mAlpha(_alpha), mTime(0.0f), state(_state)
	{
	}
public:
	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<FadeInOut>(*this);
	}

	// ȭ���� ���� ��ο����� true�� ��ȯ[tTime, _time�� 1.0�� ��� ��ο�������, �� ���¸� �� �����ϰ� ������ �� �̻����� �����ϼ���]
	bool IsFadingOutFin(float _dTime, float _time);

	// ȭ���� ���� ������� true�� ��ȯ(_dTime)
	bool IsFadingInFin(float _dTime);

public:
	Texture2D* mpTexture2D;
	float mAlpha; // ��ǥ�ϴ� fading ������
	float mTime;
	FadeInOutState state;
};

// ��� ����
// alpha���� state ���¸� ���ϴ� �ɷ� ������ ��, ���õ� �Լ��� ȣ���ϸ� ������ �Ϸ�� �� FadeInOutState::NONE�� �ȴ�.
//
/// �ʱ�ȭ �κ�
//auto ui5 = m_pEntityManager->CreateEntity("FaidInOut");
//m_pUIManager->AddUI(ui5, "Black.png", Vector2(0, 0), Vector2(1920, 1080), 2, Vector4(1.f, 1.0f, 1.0f, 1.0f), true);
//ui5->AddComponent<FadeInOut>(&ui5->GetComponent<Texture2D>(), 0.0f, FadeInOutState::FADE_IN);

/// ���� ������Ʈ �κ�
//if (mIsExit == true)
//{
//	if (fadeInOut->IsFadingOutFin(_dTime, 1.5f) == true)
//	{
//		PostQuitMessage(0);
//	}
//}
//else
//{
//	fadeInOut->IsFadingInFin(_dTime) == true;
//	if (fadeInOut->state == FadeInOutState::NONE)
//	{
//		fadeInOut->mAlpha = 1.0f;
//		fadeInOut->state = FadeInOutState::FADE_OUT;
//	}
//}