#pragma once
#include "Component.h"

/// <summary>
/// 2D �ؽ��� ������Ʈ�Դϴ�.
/// UI ��½ÿ� ���˴ϴ�.
/// �⺻ ������ ���� �������� �ʰڽ��ϴ�.
/// <para>
/// Texture2D(string "���ϸ�", Vector2 ��ġ, Vector2 ������, int ���̾�, bool �׸��� ����, Vector4 RGBA);
/// </para>
/// </summary>
struct Texture2D
	:public Component
{
public:
	Texture2D(std::shared_ptr<Entity> _owner, const std::string& _file = "", const Vector2& _position = Vector2()
		, const Vector2& _size = Vector2(1.f), const int& _layer = 0, const Vector4& _rgba = Vector4(1.0f, 1.0f, 1.0f, 1.0f)
		, const bool& _isVisible = true, const bool& _hasDepth = false)
		: Component(_owner), m_file(_file), m_position(_position), m_size(_size), m_layer(_layer), 
		m_isVisible(_isVisible), m_rgba(_rgba), m_hasDepth(_hasDepth)
	{}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<Texture2D>(*this);
	}

public:
	std::string m_file; // ���ϸ�
	Vector2 m_position; // ��ġ���� ���� ��� �����̴�.
	Vector2 m_size;		// ����, ����
	int m_layer;		// ���ڰ� Ŭ���� �ڿ� �׷����ϴ�.
	Vector4 m_rgba;		// RGBA ��, �̹����� RGB�� �⺻ 1�̾�� �ϰ�, A�� ���İ��̴�.
	bool m_isVisible;	// ������������ ����� ������, true�� ���� �׸���.
	bool m_hasDepth;	// 3D �������� z������ ���̰��� ��� �ϸ� ����Ѵ�.
};