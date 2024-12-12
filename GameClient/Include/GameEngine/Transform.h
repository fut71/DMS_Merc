#pragma once
#include "Component.h"

/// <summary>
/// ������Ʈ�� ��ġ, ȸ��, ũ�⸦ ������ �� �ִ� ������Ʈ�Դϴ�.
/// <para>
/// Transform(Vector3 ��ġ, Vector3 ȸ����, Vector3 ũ��, Vector3 �θ� ��ġ, Quaternion �θ� ȸ��, Vector3 �θ� ũ��);
/// </para>
/// </summary>
struct Transform
	: public Component
{
public:
	Transform(std::shared_ptr<Entity> _owner, Vector3 _localPosition = Vector3(), Vector3 _localRotation = Vector3(), Vector3 _localScale = Vector3(1.f)
		, Vector3 _parentPosition = Vector3(), Vector3 _parentRotation = Vector3(), Vector3 _parentScale = Vector3(1.f))
		: Component(_owner), m_localPosition(_localPosition), m_localRotation(_localRotation), m_localScale(_localScale)
		, m_worldPosition(_parentPosition), m_worldRotation(_parentRotation), m_worldScale(_parentScale)
		, m_pParent(nullptr)
	{
	}

	virtual std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<Transform>(*this);
	}

	Matrix GetTransformMatrix();

public:
	Transform* m_pParent;

	// ���� ��ü �������� ��ġ, ȸ��, ũ�⸦ ��Ÿ���ϴ�.
	Vector3 m_localPosition;
	Vector3 m_localRotation;
	Vector3 m_localScale;

	// �θ� ��ü�� ��ġ, ȸ��, ũ�⸦ ��Ÿ���ϴ�.
	Vector3 m_worldPosition;
	Vector3 m_worldRotation;
	Vector3 m_worldScale;

	Matrix m_localMatrix;
};
