#include "FadeInOut.h"

bool FadeInOut::IsFadingOutFin(float _dTime, float _time)
{
	// ���̵� �� �۾��� �������� true�� ��ȯ�Ѵ�.
	if (state != FadeInOutState::FADE_OUT)
	{
		mTime = 0.0f;
		return true;
	}

	mpTexture2D->m_isVisible = true;
	mTime += _dTime;

	// ���� ���� ��ǥ�� alpha������ �۴ٸ� mTime ���� ���� ���� ����
	if (mAlpha > mTime)
	{
		mpTexture2D->m_rgba.w = mTime;
	}
	else // ��ǥ�� ���İ��̶�� alpha ������ ����
	{
		mpTexture2D->m_rgba.w = mAlpha;
	}

	// ��ο����� ������ Ư�� �ð����� ��ο��� �����ϰ� �ʹٸ�, _time�� 1.0 �̻����� �����Ѵ�.
	if (mTime > _time)
	{
		state = FadeInOutState::NONE;
	}
	return false;
}

bool FadeInOut::IsFadingInFin(float _dTime)
{
	// ���̵� �� �۾��� �������� true�� ��ȯ�Ѵ�.
	if (state != FadeInOutState::FADE_IN)
	{
		mpTexture2D->m_isVisible = false;
		return true;
	}

	mpTexture2D->m_isVisible = true;
	mTime += _dTime;

	// ���� ���� ��ǥ�� alpha������ �۴ٸ� mTime ���� ���� ���� ����
	if (mAlpha < 1.0f - mTime)
	{
		mpTexture2D->m_rgba.w = 1.0f - mTime;
	}
	else // ��ǥ�� ���İ��̶�� alpha ������ ����
	{
		mpTexture2D->m_rgba.w = mAlpha;
		state = FadeInOutState::NONE;
		mTime = 0.0f;
	}
	return false;
}

