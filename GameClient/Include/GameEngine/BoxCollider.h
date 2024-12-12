#pragma once
#include "Component.h"
#include "Collider.h"

/// <summary>
/// ���ڸ���� �����ϴ� ������Ʈ�Դϴ�.
/// <para>
/// BoxCollider(bool Ʈ���� ����, Vector3 ����, Vector3 ������);
/// </para>
/// </summary>
struct BoxCollider
	: public Collider
{
public:
	BoxCollider(std::shared_ptr<Entity> _owner, bool _isTrigger = false, const Vector3& _center = Vector3(), const Vector3& _size = Vector3(1.f))
		:Collider(_owner, _isTrigger, _center, _size)
	{}


	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<BoxCollider>(*this);
	}

	/*
	static void Refelct()
	{
		entt::meta<BoxCollider>()
			.type("BoxCollider"_hs)
			.data<&BoxCollider::m_isTrigger>("trigger"_hs)
			.data<&BoxCollider::m_center>("center"_hs)
			.data<&BoxCollider::m_size>("size"_hs);
	}
	*/

public:

};