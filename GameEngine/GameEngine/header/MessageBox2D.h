#pragma once
#include "Component.h"
#include "TextIdxEnum.h"

/// <summary>
/// �޼��� �ڽ��Դϴ�. ������ �ؽ�ó�� �ؽ�Ʈ�� �������� �׸��� ���� �׸� �� �ְ� ���� ������Ʈȭ�մϴ�.
/// </summary>
struct MessageBox2D
	: public Component
{
public:
	MessageBox2D(std::shared_ptr<Entity> _owner)
		: Component(_owner), mFile(""), mTexturePosition(Vector2()), mSize(Vector2()), mImgRgba(Vector4()), mIsImgVisible(true)
		, mText(u8""), mTextPosition(Vector2()), mScale(1.0f), mFont(""), mTextRgba(Vector4()), mIsTextVisible(true)
		, mLayer(0), mIdx(TEXT_IDX::NONE), mNum1(0)
	{}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<MessageBox2D>(*this);
	}

public:
	std::string mFile; // ���ϸ�
	Vector2 mTexturePosition; // ��ġ���� ���� ��� �����̴�.
	Vector2 mSize;		// ����, ����
	Vector4 mImgRgba;		// RGBA ��, �̹����� RGB�� �⺻ 1�̾�� �ϰ�, A�� ���İ��̴�.
	bool mIsImgVisible;	// ������������ ����� ������, true�� ���� �׸���.

	std::u8string mText;
	Vector2 mTextPosition;
	float mScale;	// ��Ʈ�� ũ��
	std::string mFont;
	Vector4 mTextRgba;		// RGBA ��, �̹����� RGB�� �⺻ 1�̾�� �ϰ�, A�� ���İ��̴�.
	bool mIsTextVisible; // ������������ ����� ������, true�� ���� �׸���.

	int mLayer;		// ���ڰ� Ŭ���� �ڿ� �׷����ϴ�.

	/// <summary>
	///  �ӽ÷� �̷��� ����ϰڽ��ϴ�. ���� �����ϰڽ��ϴ�.
	/// </summary>
	TEXT_IDX mIdx;
	int mNum1; //AddTextwithInt() �Լ��� �μ��Դϴ�.
};