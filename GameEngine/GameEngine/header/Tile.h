#pragma once
#include "Component.h"

/// <summary>
/// ���簢�� ui�� ���� ������Ʈ Ŭ����
/// <para>
/// Tile(Vector2 ��ġ, Vector2 ������, int ���̾�, Vector4 RGBA, , float �β�, Vector4 �׵θ� ����, bool ���� ����, bool 3D���̰� ����);
/// </para>
/// </summary>
struct Tile
	: public Component
{
public:
	Tile(std::shared_ptr<Entity> _owner, Vector2 _position = Vector2(), Vector2 _sizeWH = Vector2(), int _layer = 0,
		Vector4 _rgba = Vector4(0.0f, 0.0f, 0.0f, 1.0f), float _thickness = 0.0f, Vector4 _BorderRgba = Vector4(), const bool& _isVisible = true, const bool& _hasDepth = false)
		: Component(_owner), mPosition(_position), mSize(_sizeWH), mLayer(_layer),
		mRgba(_rgba), mThickness(_thickness), mBorderRgba(_BorderRgba), mIsVisible(_isVisible), mHasDepth(_hasDepth)
	{}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<Tile>(*this);
	}

public:

	Vector2 mPosition;
	Vector2 mSize;
	int mLayer; // ���ڰ� Ŭ���� �ڿ� �׷����ϴ�.
	Vector4 mRgba;
	float mThickness;
	Vector4 mBorderRgba;
	bool mIsVisible; // ������������ ����� ������, true�� ���� �׸���.
	bool mHasDepth;	// 3D �������� z������ ���̰��� ��� �ϸ� ����Ѵ�.
};