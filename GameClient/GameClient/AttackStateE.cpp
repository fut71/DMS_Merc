#include "AttackStateE.h"
#include "EnemyComponent.h"

void AttackStateE::Initialize()
{
}

void AttackStateE::Enter(std::shared_ptr<Entity> _entity)
{
	/// 0. 공통 변수 선언
	auto& enemyComp = _entity->GetComponent<EnemyComponent>();
	auto& animationComp = _entity->GetComponent<AnimationComponent>();
	auto& trsComp = _entity->GetComponent<Transform>();

	/// 1. 클래스 선별 -> 공격 애니메이션 결정 : 기획 문서의 캐릭터 테이블 참조
	if (enemyComp.mClassType == ClassType::RVC_R1) // 궁수 적군
	{
		animationComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@BowShootAttack.fbx");
	}
	else if (enemyComp.mClassType == ClassType::RVC_C) // 아마도 닌자인 적군
	{
		animationComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@ShurikenAttack.fbx");
	}
	else // 그 외 완드를 일반 무기로 드는 적군
	{
		animationComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@SlashAttack.fbx");
	}

	/// 2. 애니메이션 쿨타임 및 재생 속도를 조정하기 위한 스피드 설정
	animationComp.mPlaySpeed = enemyComp.mAttackSpeed;

	/// 3. 물리 설정(위치 고정)
	mpPhysicsManager->SetFixPosition(_entity, true);

	/// 4. 같은 타깃에 같은 위치라도 attackState가 되면 타깃을 한 번 바라보도록 설정
	if (!enemyComp.mpTarget)
	{
		return;
	}
	// 타깃이 죽었으면 타깃을 nullptr로 변경하고 종료
	if (enemyComp.mpTarget->HasComponent<EnemyComponent>() && enemyComp.mpTarget->GetComponent<EnemyComponent>().mIsDead)
	{
		enemyComp.mpTarget = nullptr;
		return;
	}
	auto& tempTarget = enemyComp.mpTarget;
	mpFSMHelper->LookAtTarget(trsComp.m_localPosition, tempTarget->GetComponent<Transform>().m_localPosition, enemyComp.tempDirection, trsComp.m_localRotation.y);

	/// 디버깅용
	//std::cout << "Enter Attack State" << std::endl;
}

void AttackStateE::Update(float _dTime, std::shared_ptr<Entity> _entity)
{

}

void AttackStateE::Exit(std::shared_ptr<Entity> _entity)
{
	/// 1. 애니메이션 스피드를 원상 복구한다.
	_entity->GetComponent<AnimationComponent>().mPlaySpeed = 1.0f;

	/// 2. 물리 설정(위치 고정을 해제)
	mpPhysicsManager->SetFixPosition(_entity, false);

	/// 3. 디버깅용 
	//std::cout << "Exit Attack State" << std::endl;
}

void AttackStateE::FixedUpdate(float _fixedTime, std::shared_ptr<Entity> _entity)
{
	/// 0. 타겟이 없으면 종료
	auto& enemyComp = _entity->GetComponent<EnemyComponent>();
	auto& trsComp = _entity->GetComponent<Transform>();

	if (!enemyComp.mpTarget)
	{
		return;
	}

	// 타깃이 죽었으면 타깃을 nullptr로 변경하고 종료
	if (enemyComp.mpTarget->HasComponent<PlayerComponent>() && enemyComp.mpTarget->GetComponent<PlayerComponent>().mIsDead)
	{
		enemyComp.mpTarget = nullptr;
		return;
	}

	/// 1. 방향 설정(타겟을 늘 바라보고 있도록 설정)
	auto& previousTarget = enemyComp.mPreviousTarget;								// 이전 타겟 : 현재 지정된 타겟과 동일한지 확인용(move와 달리 attack에서 이 값은 무조건 nullptr이 될 수 없다.)
	auto& previousTargetPos = enemyComp.mPreviousTargetPos;							// 이전 타깃의 위치
	auto& tempTarget = enemyComp.mpTarget;											// 현재 타겟
	auto& targetPos = tempTarget->GetComponent<Transform>().m_localPosition;		// 타겟 위치
	auto& position = trsComp.m_localPosition;										// 유닛의 위치
	auto& direction = enemyComp.tempDirection;										// 유닛의 방향

	// 타깃이 변경되면 방향 변경
	if ((tempTarget != previousTarget))
	{
		previousTarget = tempTarget;
		// 캐릭터가 바라보고 있는 방향을 업데이트
		mpFSMHelper->LookAtTarget(position, targetPos, direction, trsComp.m_localRotation.y);
	}

	// 타깃이 조금이라도 움직이면 방향 업뎃
	if (targetPos != previousTargetPos)
	{
		previousTargetPos = targetPos;
		// 캐릭터가 바라보고 있는 방향을 업데이트
		mpFSMHelper->LookAtTarget(position, targetPos, direction, trsComp.m_localRotation.y);
		//return;
	}

	/// 2. 원거리일 경우 각 클래스에 맞는 함수를 호출한다.
	if (enemyComp.mClassType == ClassType::RVC_R1) // 아처일 때
	{
		RangedAttack(_entity);
	}
	else if (enemyComp.mClassType == ClassType::RVC_C) // 용병왕일 때
	{
	}

	/// 3. 물리 설정(위치 고정)
	//mpPhysicsManager->UpdatePosition(_entity, pos);
	if (_entity->HasComponent<Rigidbody>())
	{
		mpPhysicsManager->SetVelocity(_entity, Vector3());
	}

	/// 4. 공격 사운드 재생
	mAttackSoundTime += _fixedTime;
	if (mAttackSoundTime >= enemyComp.mAttackSpeed)
	{
		if (enemyComp.mClassType == ClassType::RVC_R1)
		{
			mpSoundManager->PlaySFX("Snd_sfx_CharaShoot");
		}
		else if (enemyComp.mClassType == ClassType::RVC_0)
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
		mAttackSoundTime -= enemyComp.mAttackSpeed;
	}
}

void AttackStateE::RangedAttack(std::shared_ptr<Entity> _entity)
{
	auto& enemyComp = _entity->GetComponent<EnemyComponent>();
	Transform& transform = _entity->GetComponent<Transform>();
	auto& animComponent = _entity->GetComponent<AnimationComponent>();
	float currentAnimTime = animComponent.mAnimationPlaytime; // 현재 애니메이션 재생 시간
	float shootTime = 0.3f;// 이거 전체 시간의 11/20 곱해야 해서 메쉬렌더 까야함

	// 일정 애니메이션 시간에 도달했을 때 화살 발사
	if (currentAnimTime >= shootTime && !enemyComp.mIsShot)
	{
		if (enemyComp.mpTarget)
		{
			auto& targetTransform = enemyComp.mpTarget->GetComponent<Transform>();
			Vector3& targetPos = targetTransform.m_localPosition;

			// 화살 생성 이벤트 트리거
			auto projectile = mpEntityManager->CreateEntity("arrow");
			projectile->AddComponent<Transform>(transform.m_localPosition + Vector3(0, 0.7, 0), transform.m_localRotation, Vector3(0.01f));
			//mpEventManager->TriggerEvent(Event("Create Arrow", projectile->GetUID()));
			projectile->AddComponent<BoxCollider>(true, Vector3(), Vector3(0.03f, 0.03f, 0.3f));
			auto& projectileRigid = projectile->AddComponent<Rigidbody>(50.f, 0.f, 0.f, false, false, 0, 1, 1);
			projectile->AddComponent<MeshRenderer>("Arrow 01.FBX", "Arrow 01_Mesh", false, false, true);
			projectile->AddComponent<Texture3D>("Weapon Brown.png");
			auto& projectileComp = projectile->AddComponent<ProjectileComponent>(0.02, 10, Vector3(0, 0, 0), 50);
			mpPhysicsManager->AddPhysicsObject(projectile->GetUID(), TYPE_PROJECTILE, ATTR_ENEMY);
			mpRenderManager->InitailizeEntity(projectile);
			projectileComp.m_isThrown = true;
			Vector3 start = projectile->GetComponent<Transform>().m_localPosition;
			auto time = projectileComp.m_lifeTime;
			auto mass = projectileRigid.m_mass;
			projectileComp.m_targetPosition = targetTransform.m_localPosition;
			projectileComp.m_damage = enemyComp.mAttackPower;
			/// 
			Vector3 force;
			force.x = targetTransform.m_localPosition.x - start.x;
			force.z = targetTransform.m_localPosition.z - start.z;
			force.Normalize();
			force *= projectileComp.m_speed;

			mpPhysicsManager->SetVelocity(projectile, force);
			//m_pSoundManager->Play3DSound("bow", pos.x, pos.y, pos.z);

			// 화살이 발사되었음을 기록 (한 번만 발사되도록)
			enemyComp.mIsShot = true;
		}
	}

	// 애니메이션이 끝났을 때 화살 발사 가능 상태 초기화
	if (currentAnimTime == 0.0f)
	{
		enemyComp.mIsShot = false;
	}
}
