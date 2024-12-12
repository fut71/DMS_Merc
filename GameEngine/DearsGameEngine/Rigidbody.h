#pragma once
#include "Component.h"
#include "physx/PxRigidActor.h"
#include "Entity.h"

/// <summary>
/// ������Ʈ�� ���� ��Ģ�� �����Ͽ� ����ó�� �����̰ų� �����ϰ� �ϴ� ������Ʈ�Դϴ�.
/// <para>
/// Rigidbody(float ����, float ���� ����(0~1), float �� ����, bool �߷�, bool Ű�׸�ƽ, float �ݹ� ���(0~1), float ���� ���� ���(0~1)
/// , float ���� ���� ���(0~1), ������ ��ü����, Vector3 ��ġ ���� �÷���, Quaternion ȸ�� ���� �÷���)
/// </para>
/// </summary>
struct Rigidbody
	: public Component
{
public:
	Rigidbody(std::shared_ptr<Entity> _owner, float _mass = 1.f, float _drag = 0.f, float _angularDrag = 0.f, bool _useGravity = true, bool _isKinematic = false
		, float _restitution = 0.f, float _staticFriction = 1.f, float _dynamicFriction = 1.f, bool _static = false
		, const Vector3& _freezePosition = Vector3(1000.f), const Quaternion& _freezeRotation = Quaternion(360.f, 360.f, 360.f, 1.f))
		: Component(_owner), m_mass(_mass), m_drag(_drag), m_angularDrag(_angularDrag), m_useGravity(_useGravity), m_isKinematic(_isKinematic)
		, m_restitution(_restitution), m_staticFriction(_staticFriction), m_dynamicFriction(_dynamicFriction), m_isStatic(_static)
		, m_freezePosition(_freezePosition), m_freezeRotation(_freezeRotation), m_pRigidActor(nullptr)
	{
		const std::string& ownerName = _owner->GetName();
		m_name = new char[ownerName.size() + 1];  // ���ڿ� ���� + null terminator
		strcpy_s(m_name, ownerName.size() + 1, ownerName.c_str());  // ������ ���ڿ� ����
	}
	
	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<Rigidbody>(*this);
	}

public:

	Vector3 m_freezePosition;	// ��ġ ���� �÷���
	Quaternion m_freezeRotation;	// ȸ�� ���� �÷���
	float m_mass;				// ����
	float m_drag;				// ���� ����
	float m_angularDrag;		// �� ����
	float m_restitution;		// �ݹ� ���			// 0 ~ 1
	float m_staticFriction;		// ���� ���� ���	// 0 ~ 1
	float m_dynamicFriction;	// ���� ���� ���	// 0 ~ 1
	bool m_useGravity;			// �߷� ��� ����
	bool m_isKinematic;			// Ű�׸�ƽ ����
	bool m_isStatic;			// ���� ����
	physx::PxRigidActor* m_pRigidActor;
	char* m_name;
};