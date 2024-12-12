#pragma once
#include "Component.h"
#include "TextIdxEnum.h"

/// <summary>
/// �ؽ�Ʈ ui�� ���� �ؽ�Ʈ ������Ʈ Ŭ����
/// <para>
/// Text(u8string "����� ���ڿ�", string "����� ��Ʈ", Vector2 ��ġ, int ���̾�, Vector4 RGBA);
/// </para>
/// </summary>
struct Text
	: public Component
{
public:
	Text(std::shared_ptr<Entity> _owner, std::u8string _text = u8"", std::string _font = ""
		, Vector2 _position = Vector2(), int _layer = 0, bool _isVisible = true, 
		Vector4 _rgba = Vector4(0.0f, 0.0f, 0.0f, 1.0f))
		: Component(_owner), m_text(_text), m_font(_font), m_position(_position), m_layer(_layer),
		m_isVisible(_isVisible), m_rgba(_rgba), m_hasDepth(false)
	{}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<Text>(*this);
	}

public:

	std::u8string m_text;
	std::string m_font;
	Vector2 m_position;
	int m_layer; // ���ڰ� Ŭ���� �ڿ� �׷����ϴ�.
	bool m_isVisible; // ������������ ����� ������, true�� ���� �׸���.
	Vector4 m_rgba;
	float m_scale = 1.f;	// ��Ʈ�� ũ��

	Vector3 m_worldPosition = Vector3(); // 3D ������ ��ġ
	Vector3 m_offset = Vector3(); // 3D ���������� ������
	bool m_hasDepth;	// 3D �������� z������ ���̰��� ��� �ϸ� ����Ѵ�.

	/// <summary>
	///  �ӽ÷� �̷��� ����ϰڽ��ϴ�. ���� �����ϰڽ��ϴ�.
	/// </summary>
	TEXT_IDX m_idx = TEXT_IDX::NONE;
	int m_num1 = 0; //AddTextwithInt() �Լ��� �μ��Դϴ�.
	int m_num2 = 0; //AddTextwithInt2()�� �� ��° �μ��̸�, ù��° �μ��� m_num1�Դϴ�.
	float m_num3 = 0; // AddTextwithFloat �Լ��� �μ��Դϴ�.
	float m_num4 = 0; //AddTextwithFloat2�� �� ��° �μ��̸�, ù��° �μ��� m_num3�Դϴ�.
};