#pragma once
#include "Component.h"
#include "TextIdxEnum.h"

/// <summary>
/// 텍스트 ui를 위한 텍스트 컴포넌트 클래스
/// <para>
/// Text(u8string "출력할 문자열", string "사용할 폰트", Vector2 위치, int 레이어, Vector4 RGBA);
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
	int m_layer; // 숫자가 클수록 뒤에 그려집니다.
	bool m_isVisible; // 렌더러에서만 사용할 변수로, true일 때만 그린다.
	Vector4 m_rgba;
	float m_scale = 1.f;	// 폰트의 크기

	Vector3 m_worldPosition = Vector3(); // 3D 공간의 위치
	Vector3 m_offset = Vector3(); // 3D 공간에서의 오프셋
	bool m_hasDepth;	// 3D 공간에서 z축으로 깊이감을 줘야 하면 사용한다.

	/// <summary>
	///  임시로 이렇게 사용하겠습니다. 추후 정돈하겠습니다.
	/// </summary>
	TEXT_IDX m_idx = TEXT_IDX::NONE;
	int m_num1 = 0; //AddTextwithInt() 함수의 인수입니다.
	int m_num2 = 0; //AddTextwithInt2()의 두 번째 인수이며, 첫번째 인수는 m_num1입니다.
	float m_num3 = 0; // AddTextwithFloat 함수의 인수입니다.
	float m_num4 = 0; //AddTextwithFloat2의 두 번째 인수이며, 첫번째 인수는 m_num3입니다.
};