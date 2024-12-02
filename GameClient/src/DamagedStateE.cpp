#include "DamagedStateE.h"

void DamagedStateE::Initialize()
{
}

void DamagedStateE::Enter(std::shared_ptr<Entity> _entity)
{
	// �� ������ �ش� (����)�ִϸ��̼� ��� �ð� �ʱ�ȭ �� ����
	_entity->GetComponent<AnimationComponent>().mpTargetAnimation = mpResourceManager->Get_Animation("Character@Damaged.fbx");

	// �����ϵ��� ��ġ�� �����ϰ� ���� ���¿��� Ȥ�ó� ���� �ӵ� ������ �� �����Ѵ�.
	mpPhysicsManager->SetFixPosition(_entity, true);
}

void DamagedStateE::Update(float _dTime, std::shared_ptr<Entity> _entity)
{
	/// ��ƼƼ�� ��ġ�� ����
	//mpPhysicsManager->UpdatePosition(_entity, _entity->GetComponent<Transform>().m_localPosition);
	if (_entity->HasComponent<Rigidbody>())
	{
		mpPhysicsManager->SetVelocity(_entity, Vector3());
	}
}

void DamagedStateE::Exit(std::shared_ptr<Entity> _entity)
{
	// ��ġ ������ �����Ѵ�.
	mpPhysicsManager->SetFixPosition(_entity, false);
}
