#pragma once
#include "Component.h"
#include "Collider.h"

/// <summary>
/// ��ü ����� �����ϴ� ������Ʈ�Դϴ�.
/// <para>
/// SphereCollider(bool Ʈ���� ����, Vector3 ����, float ������);
/// </para>
/// </summary>
struct SphereCollider
	: public Collider
{
public:
	SphereCollider(std::shared_ptr<Entity> _owner, bool _isTrigger, const Vector3& _center, float _radius)
		:Collider(_owner, _isTrigger, _center, Vector3(1.f)), m_radius(_radius)
	{}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<SphereCollider>(*this);
	}

public:
	float m_radius;		// 
};