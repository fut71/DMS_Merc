#pragma once
#include "AttackState.h"

void AttackState::Initialize()
{

}

void AttackState::Enter(std::shared_ptr<Entity> _entity)
{
	/// 0. ���� ���� ����
	auto& playerComp = _entity->GetComponent<PlayerComponent>();
	auto& animationComp = _entity->GetComponent<AnimationComponent>();
	auto& trsComp = _entity->GetComponent<Transform>();

	/// 1. Ŭ���� ���� -> ���� �ִϸ��̼� ���� : ��ȹ ������ ĳ���� ���̺� ����
	if (playerComp.mClassType == ClassType::ARCHER) // ��ó�� ��
	{
		animationComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@BowShootAttack.fbx");
	}
	else if (playerComp.mClassType == ClassType::MERCENARY) // �뺴�� ��
	{
		animationComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@SlashAttack.fbx");
	}
	// �� �� �ٸ� �͵��� ������ �� �� �־� ���߿� �߰��մϴ�.

	/// 2. �ִϸ��̼� ��Ÿ�� �� ��� �ӵ��� �����ϱ� ���� ���ǵ� ����
	animationComp.mPlaySpeed = playerComp.mAttackSpeed;

	/// 3. ���� ����(��ġ ����)
	mpPhysicsManager->SetFixPosition(_entity, true);

	/// 4. ���� Ÿ�꿡 ���� ��ġ�� attackState�� �Ǹ� Ÿ���� �� �� �ٶ󺸵��� ����
	if (!playerComp.mpTarget)
	{
		return;
	}
	// Ÿ���� �׾����� Ÿ���� nullptr�� �����ϰ� ����
	if (playerComp.mpTarget->HasComponent<EnemyComponent>() && playerComp.mpTarget->GetComponent<EnemyComponent>().mIsDead)
	{
		playerComp.mpTarget = nullptr;
		return;
	}
	auto& tempTarget = playerComp.mpTarget;
	mpFSMHelper->LookAtTarget(trsComp.m_localPosition, tempTarget->GetComponent<Transform>().m_localPosition, playerComp.tempDirection, trsComp.m_localRotation.y);

	/// ������
	//std::cout << "Enter Attack State" << std::endl;
}

void AttackState::Update(float _dTime, std::shared_ptr<Entity> _entity)
{

}

void AttackState::Exit(std::shared_ptr<Entity> _entity)
{
	/// 1. �ִϸ��̼� ���ǵ带 ���� �����Ѵ�.
	_entity->GetComponent<AnimationComponent>().mPlaySpeed = 1.0f;

	/// 2. ���� ����(��ġ ������ ����)
	mpPhysicsManager->SetFixPosition(_entity, false);

	/// 3. ������ 
	//std::cout << "Exit Attack State" << std::endl;
}

void AttackState::FixedUpdate(float _fixedTime, std::shared_ptr<Entity> _entity)
{
	/// 0. Ÿ���� ������ ����
	auto& playerComp = _entity->GetComponent<PlayerComponent>();
	auto& trsComp = _entity->GetComponent<Transform>();

	if (!playerComp.mpTarget)
	{
		return;
	}

	// Ÿ���� �׾����� Ÿ���� nullptr�� �����ϰ� ����
	if (playerComp.mpTarget->HasComponent<EnemyComponent>() && playerComp.mpTarget->GetComponent<EnemyComponent>().mIsDead)
	{
		playerComp.mpTarget = nullptr;
		return;
	}

	/// 1. ���� ����(Ÿ���� �� �ٶ󺸰� �ֵ��� ����)
	auto& previousTarget = playerComp.mPreviousTarget;								// ���� Ÿ�� : ���� ������ Ÿ�ٰ� �������� Ȯ�ο�(move�� �޸� attack���� �� ���� ������ nullptr�� �� �� ����.)
	auto& previousTargetPos = playerComp.mPreviousTargetPos;						// ���� Ÿ���� ��ġ
	auto& tempTarget = playerComp.mpTarget;											// ���� Ÿ��
	auto& targetPos = tempTarget->GetComponent<Transform>().m_localPosition;		// Ÿ�� ��ġ
	auto& position = trsComp.m_localPosition;										// ������ ��ġ
	auto& direction = playerComp.tempDirection;										// ������ ����

	// Ÿ���� ����Ǹ� ���� ����
	if ((tempTarget != previousTarget))
	{
		previousTarget = tempTarget;
		// ĳ���Ͱ� �ٶ󺸰� �ִ� ������ ������Ʈ
		mpFSMHelper->LookAtTarget(position, targetPos, direction, trsComp.m_localRotation.y);
	}

	// Ÿ���� �����̶� �����̸� ���� ����
	if (targetPos != previousTargetPos)
	{
		previousTargetPos = targetPos;
		// ĳ���Ͱ� �ٶ󺸰� �ִ� ������ ������Ʈ
		mpFSMHelper->LookAtTarget(position, targetPos, direction, trsComp.m_localRotation.y);
		//return;
	}

	/// 2. ���Ÿ��� ��� �� Ŭ������ �´� �Լ��� ȣ���Ѵ�.
	if (playerComp.mClassType == ClassType::ARCHER) // ��ó�� ��
	{
		RangedAttack(_entity);
	}
	else if (playerComp.mClassType == ClassType::NINJA) // ������ ��
	{
	}
	else if (playerComp.mClassType == ClassType::WIZARD) // ���ڵ� �϶�
	{
	}

	/// 3. ���� ����(��ġ ����)
	//mpPhysicsManager->UpdatePosition(_entity, pos);
	if (_entity->HasComponent<Rigidbody>())
	{
		mpPhysicsManager->SetVelocity(_entity, Vector3());
	}

	/// 4. ���� ���� ���
	mAttackSoundTime += _fixedTime;
	if (mAttackSoundTime >= playerComp.mAttackSpeed)
	{
		if (playerComp.mClassType == ClassType::ARCHER)
		{
			mpSoundManager->PlaySFX("Snd_sfx_CharaShoot");
		}
		else if (playerComp.mClassType == ClassType::MERCENARY)
		{
			if (RandomUtil::RandomInt(0, 1) == 0)
			{
				mpSoundManager->PlaySFX("Snd_sfx_CharaMelee1", 0.5f);
			}
			else
			{
				mpSoundManager->PlaySFX("Snd_sfx_CharaMelee2", 0.5f);
			}
		}
		mAttackSoundTime -= playerComp.mAttackSpeed;
	}
}

void AttackState::RangedAttack(std::shared_ptr<Entity> _entity)
{
	auto& playerComp = _entity->GetComponent<PlayerComponent>();
	Transform& transform = _entity->GetComponent<Transform>();
	auto& animComponent = _entity->GetComponent<AnimationComponent>();
	float currentAnimTime = animComponent.mAnimationPlaytime; // ���� �ִϸ��̼� ��� �ð�
	float shootTime = 0.3f;// �̰� ��ü �ð��� 11/20 ���ؾ� �ؼ� �޽����� �����

	// ���� �ִϸ��̼� �ð��� �������� �� ȭ�� �߻�
	if (currentAnimTime >= shootTime && !playerComp.mIsShot)
	{
		if (playerComp.mpTarget)
		{
			auto& targetTransform = playerComp.mpTarget->GetComponent<Transform>();
			Vector3& targetPos = targetTransform.m_localPosition;

			// ȭ�� ���� �̺�Ʈ Ʈ����
			auto projectile = mpEntityManager->CreateEntity("arrow");
			projectile->AddComponent<Transform>(transform.m_localPosition + Vector3(0, 0.7, 0), transform.m_localRotation, Vector3(0.01f));
			//mpEventManager->TriggerEvent(Event("Create Arrow", projectile->GetUID()));
			projectile->AddComponent<BoxCollider>(true, Vector3(), Vector3(0.03f, 0.03f, 0.3f));
			auto& projectileRigid = projectile->AddComponent<Rigidbody>(50.f, 0.f, 0.f, false, false, 0, 1, 1);
			projectile->AddComponent<MeshRenderer>("Arrow 01.FBX", "Arrow 01_Mesh", false, false, true);
			projectile->AddComponent<Texture3D>("Weapon Brown.png");
			auto& projectileComp = projectile->AddComponent<ProjectileComponent>(0.02, 10, Vector3(0, 0, 0), 50);
			mpPhysicsManager->AddPhysicsObject(projectile->GetUID(), TYPE_PROJECTILE, ATTR_ALLY);
			mpRenderManager->InitailizeEntity(projectile);
			projectileComp.m_isThrown = true;
			Vector3 start = projectile->GetComponent<Transform>().m_localPosition;
			auto time = projectileComp.m_lifeTime;
			auto mass = projectileRigid.m_mass;
			projectileComp.m_targetPosition = targetTransform.m_localPosition;
			projectileComp.m_damage = playerComp.mAttackPower;
			/// 
			Vector3 force;
			force.x = targetTransform.m_localPosition.x - start.x;
			force.z = targetTransform.m_localPosition.z - start.z;
			force.Normalize();
			force *= projectileComp.m_speed;

			mpPhysicsManager->SetVelocity(projectile, force);
			//m_pSoundManager->Play3DSound("bow", pos.x, pos.y, pos.z);

			// ȭ���� �߻�Ǿ����� ��� (�� ���� �߻�ǵ���)
			playerComp.mIsShot = true;
		}
	}

	// �ִϸ��̼��� ������ �� ȭ�� �߻� ���� ���� �ʱ�ȭ
	if (currentAnimTime == 0.0f)
	{
		playerComp.mIsShot = false;
	}
}
