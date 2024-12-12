#pragma once
#include "Component.h"
#include "TextIdxEnum.h"

/// <summary>
/// �޼��� �ڽ��Դϴ�. ������ �ؽ�ó�� �ؽ�Ʈ�� �������� �׸��� ���� �׸� �� �ְ� ���� ������Ʈȭ�մϴ�.
/// </summary>
struct MessageBox3D
	: public Component
{
public:
	MessageBox3D(std::shared_ptr<Entity> _owner)
		: Component(_owner), mFile(""), mImgOffset(Vector2()), mSize(Vector2()), mImgRgba(Vector4()), mIsImgVisible(true)
		, mText(u8""), mTextOffset(Vector2()), mScale(1.0f), mFont(""), mTextRgba(Vector4()), mIsTextVisible(true)
		, mWorldPosition(Vector3()), mLayer(0), mIdx(TEXT_IDX::NONE), mNum1(0)
	{}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<MessageBox3D>(*this);
	}

public:
	std::string mFile; // ���ϸ�
	Vector2 mImgOffset; // ��ġ���� ���� ��� �����̴�.
	Vector2 mSize;		// ����, ����
	Vector4 mImgRgba;		// RGBA ��, �̹����� RGB�� �⺻ 1�̾�� �ϰ�, A�� ���İ��̴�.
	bool mIsImgVisible;	// ������������ ����� ������, true�� ���� �׸���.

	std::u8string mText;
	Vector2 mTextOffset; // 3D ���������� ������
	float mScale;	// ��Ʈ�� ũ��
	std::string mFont;
	Vector4 mTextRgba;		// RGBA ��, �̹����� RGB�� �⺻ 1�̾�� �ϰ�, A�� ���İ��̴�.
	bool mIsTextVisible; // ������������ ����� ������, true�� ���� �׸���.

	Vector3 mWorldPosition; // 3D ������ ��ġ
	int mLayer;		// ���ڰ� Ŭ���� �ڿ� �׷����ϴ�.

	/// <summary>
	///  �ӽ÷� �̷��� ����ϰڽ��ϴ�. ���� �����ϰڽ��ϴ�.
	/// </summary>
	TEXT_IDX mIdx;
	int mNum1; //AddTextwithInt() �Լ��� �μ��Դϴ�.
};