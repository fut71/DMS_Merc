#include "IdleStateE.h"
#include "Transform.h"

void IdleStateE::Initialize()
{
}

void IdleStateE::Enter(std::shared_ptr<Entity> _entity)
{
	// �� ������ �ش� (����)�ִϸ��̼� ��� �ð� �ʱ�ȭ �� ����
	_entity->GetComponent<AnimationComponent>().mpTargetAnimation = mpResourceManager->Get_Animation("Character@Idle.fbx");

	///������
	//std::cout << "Enter Idle State" << std::endl;
}

void IdleStateE::Update(float _dTime, std::shared_ptr<Entity> _entity)
{
	// ��ƼƼ�� ��ġ�� ����
	mpPhysicsManager->UpdatePosition(_entity, _entity->GetComponent<Transform>().m_localPosition);
}

void IdleStateE::Exit(std::shared_ptr<Entity> _entity)
{
	///������
	//std::cout << "Exit Idle State" << std::endl;
}
