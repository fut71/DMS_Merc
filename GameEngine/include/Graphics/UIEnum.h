#pragma once
#include <string>
#include "UIEnum.h"
#include<directxTK/SimpleMath.h>

using DirectX::SimpleMath::Vector4;
using DirectX::SimpleMath::Vector2;

enum class IMAGE_FLIP
{
	NONE,		// ���� ����
	YFLIP,	// �¿� ����
	XFLIP,	// ���� ����
	XYFLIP, // �����¿� ����
};

struct UIImageElement
{
	Vector2 position;
	Vector2 size;
	IMAGE_FLIP imageType;
	std::string textureName;
	float alpha;
};

struct UIRectElement
{
	int index;
	Vector2 position;
	Vector2 size;
	Vector4 color;
};

struct UILineElement
{
	int index;
	Vector2 position;
	Vector2 size;
	Vector4 color;
};

struct UICircleElement
{
	int index;
	Vector2 position;
	Vector2 size;
	Vector4 color;
};

struct UITextElement
{
	int index;
	Vector2 position;
	std::string text;
	std::string font;
	float fontSize;
	Vector4 color;
};