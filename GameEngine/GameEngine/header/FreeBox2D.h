#pragma once
#include "Component.h"

/// <summary>
/// ������ �簢�� ui�� ���� ������Ʈ Ŭ����
/// <para>
/// FreeBox2D( Vector2 ��ġ, );
/// </para>
/// </summary>
struct FreeBox2D
	: public Component
{
public:
	FreeBox2D(std::shared_ptr<Entity> _owner, Vector2 _position = Vector2(), int _layer = 0, const bool& _isVisible = true,
		Vector4 _rgba = Vector4(0.0f, 0.0f, 0.0f, 1.0f))
		: Component(_owner), m_position(_position), m_layer(_layer),
		m_isVisible(_isVisible), m_rgba(_rgba), m_hasDepth(false)
	{}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<FreeBox2D>(*this);
	}

public:

	Vector2 m_position;
	int m_layer; // ���ڰ� Ŭ���� �ڿ� �׷����ϴ�.
	bool m_isVisible; // ������������ ����� ������, true�� ���� �׸���.
	Vector4 m_rgba;
	bool m_hasDepth;	// 3D �������� z������ ���̰��� ��� �ϸ� ����Ѵ�.
};