#pragma once
#include "Component.h"
#include "Text.h"
#include "Texture2D.h"

enum class ButtonState
{
	NORMAL,
	HOVERED,
	PRESSED
};

/// <summary>
/// UI ��ư ������Ʈ Ŭ����
/// Ŭ���̾�Ʈ���� ���������� �������� �ʰ�, UI�Ŵ����� ���� �����ϵ��� �մϴ�.
/// ���� �̹����� ��Ʈ�� ��ư ��� �����մϴ�. (�ٸ� Box2D ���� �Ŵ� �� ��)
// �巡�� ���� Press�� �̹����� ����մϴ�.
// ȣ������ Ŭ�� �Լ��� ���ÿ� ������� ������. �� ƽ ƨ��ϴ�.
/// <para>
/// Button(u8string "����� ���ڿ�", string "����� ��Ʈ", Vector2 ��ġ, int ���̾�, Vector4 RGBA);
/// </para>
/// </summary>
struct Button
	: public Component
{
public:
	Button(std::shared_ptr<Entity> _owner)
		: Component(_owner), mUIPosition(Vector2()), mTextPosition(Vector2()), mUIPressedPosition(Vector2()), mTextPressedPosition(Vector2())
		, mNormalUIName(""), mNormalTextFont(""), mNormalText(u8""), mNormalRGBA(Vector4(1.0f)), mUIHoveringPosition(Vector2()), mTextHoveringPosition(Vector2())
		, mHoveredUIName(""), mHoveredTextFont(""), mHoveredText(u8""), mHoveredRGBA(Vector4(0.0f)), mPressedUIName(""), mPressedTextFont(""), mPressedText(u8"")
		, mPressedRGBA(Vector4(0.0f)), mButtonState(ButtonState::NORMAL), mIsDrag(false), mOrder(0), mIsEnable(true), mCanDrag(false)
		
	{}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<Button>(*this);
	}

public:
	Vector2 mUIPosition;		// ���� UI�� ������ �����صд�.
	Vector2 mUISize;		// ���� UI�� ������ �����صд�.
	Vector2 mTextPosition;		// ���� Text�� ������ �����صд�.

	std::string mNormalUIName;	// 
	std::string mNormalTextFont;
	std::u8string mNormalText;
	Vector4 mNormalRGBA;

	Vector2 mUIHoveringPosition;
	Vector2 mUIHoveringSize;
	Vector2 mTextHoveringPosition;

	Vector2 mUIPressedPosition;
	Vector2 mUIPressedSize;
	Vector2 mTextPressedPosition;

	std::string mHoveredUIName;
	std::string mHoveredTextFont;
	std::u8string mHoveredText;
	Vector4 mHoveredRGBA;

	std::string mPressedUIName;
	std::string mPressedTextFont;
	std::u8string mPressedText;
	Vector4 mPressedRGBA;

	ButtonState mButtonState;
	bool mIsDrag; // �巡�׸� ������� ������ Ȱ��ȭ�� ��


	std::function<void()> mOnClick; // ��, �̰� �ݹ��Լ���µ� ����� ���� �� �� �����ؾߵ� �� ����. ������ �����ϱ� �����.
	int mOrder; // ��ư�� ������ ��Ÿ���� ���� (���̾�� ����ϰ�, ������ �����ؼ� �ٸ� ��ư�� ���ÿ� ������ �ʰ� �Ѵ�.)
	bool mIsEnable; // ��ư�� Ȱ��ȭ ���θ� ��Ÿ���� �÷��� : Ŭ�� �Ұ� �� �ð������ε� ������ ����
	bool mCanDrag; // UI �Ŵ��� ���ο��� ����ϴ� �÷����̹Ƿ� �ǵ��� �� �� 
};