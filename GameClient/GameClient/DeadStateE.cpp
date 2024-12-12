#include "DeadStateE.h"

void DeadStateE::Initialize()
{
}

void DeadStateE::Enter(std::shared_ptr<Entity> _entity)
{
	// �ִϸ��̼��� �����Ѵ�.
	auto& aniComp = _entity->GetComponent<AnimationComponent>();
	aniComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@Death.fbx");

	// Ÿ���� ���ش�.
	_entity->GetComponent<EnemyComponent>().mpTarget = nullptr;

	// �����ϵ��� ��ġ�� �����ϰ� ���� ���¿��� Ȥ�ó� ���� �ӵ� ������ �� �����Ѵ�.
	mpPhysicsManager->SetFixPosition(_entity, true);
	aniComp.mIsLoop = false;

	// ����
	mpSoundManager->PlaySFX("Snd_sfx_CharaDead1");

	/// ������
	//std::cout << "Enter Enemy Dead State" << std::endl;
}

void DeadStateE::Update(float _dTime, std::shared_ptr<Entity> _entity)
{
	// ��ƼƼ�� ��ġ�� ����
//mpPhysicsManager->UpdatePosition(_entity, _entity->GetComponent<Transform>().m_localPosition);
	if (_entity->HasComponent<Rigidbody>())
	{
		mpPhysicsManager->SetVelocity(_entity, Vector3());
	}

	// �ٽ� �ݺ��� �� �ִϸ��̼��� ������ ���������� ������Ű�� ������ �����Ѵ�.
	auto& aniComp = _entity->GetComponent<AnimationComponent>();
	if (aniComp.mpNextTargetAnimation == nullptr/* && _entity->GetComponent<AnimationComponent>().mAnimationPlaytime >= _entity->GetComponent<AnimationComponent>().mAnimationCoolTime - 0.1f*/)
	{
		// �Ʊ��� ���� ���
		if (_entity->HasComponent<EnemyComponent>())
		{
			// Ÿ�ٿ��� �����ϱ� ���� PlayerComponent�� ���� �÷��׸� Ȱ��ȭ�Ѵ�.
			_entity->GetComponent<EnemyComponent>().mIsDead = true;
		}
		return;
	}

	/// ������
	//std::cout << "Update Enemy Dead State" << std::endl;
}

void DeadStateE::Exit(std::shared_ptr<Entity> _entity)
{
	/// ������
	//std::cout << "Exit Enemy Dead State" << std::endl;
}
