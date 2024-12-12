#pragma once
#include "Component.h"

/// <summary>
/// ü�¹� ui�� ���� ������Ʈ Ŭ����
/// <para>
/// HealthBarComponenet(Vector2 ��ġ, Vector2 ������, int ���̾�, Vector4 ����, Vector4 ü�»�, float ü�º���, bool ���� ����, bool 3D���̰� ����);
// �׵θ��� �����ϴ�. �ְ� �Ϸ��� ui �Ŵ��� ������ �߰��� ��
/// </para>
/// </summary>
struct HealthBarComponenet
	: public Component
{
public:
	HealthBarComponenet(std::shared_ptr<Entity> _owner, Vector2 _position = {}, Vector3 _offSet = {}, Vector2 _sizeWH = {}, int _layer = 0,
		Vector4 _backgroundRgba = Vector4(0.0f, 0.0f, 0.0f, 1.0f), Vector4 _foregroundRgba = Vector4(0.0f, 0.0f, 0.0f, 1.0f), float _HealthPercentage = 1.0f,
		const bool& _isVisible = true, const bool& _hasDepth = false)
		: Component(_owner), mPosition(_position), mOffSet(_offSet), mSize(_sizeWH), mLayer(_layer), mBackgroundColor(_backgroundRgba),
		 mForegroundColor(_foregroundRgba), mHealthPercentage(_HealthPercentage), mIsVisible(_isVisible), mHasDepth(_hasDepth)
	{}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<HealthBarComponenet>(*this);
	}

public:
	Vector2 mPosition;
	Vector3 mOffSet;
	Vector2 mSize;
	int mLayer; // ���ڰ� Ŭ���� �ڿ� �׷����ϴ�.
	Vector4 mBackgroundColor;
	Vector4 mForegroundColor;
	float mHealthPercentage;
	bool mIsVisible; // ������������ ����� ������, true�� ���� �׸���.
	bool mHasDepth;	// 3D �������� z������ ���̰��� ��� �ϸ� ����Ѵ�.
};