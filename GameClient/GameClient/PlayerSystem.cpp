#include "PlayerSystem.h"
#include "WeaponComponent.h"
#include "AnimationComponent.h"
#include "ParticleComponent.h"
// �ӽ÷� �߰��ϴ� ���
#include "Text.h"

PlayerSystem::PlayerSystem(entt::registry& _registry, EntityManager* _pEntityManager, PhysicsManager* _pPhysicsManager
	, RenderManager* _pRenderManager, ResourceManager* _pResourceManager, EventManager* _pEventManager
	, SoundManager* _pSoundManager, EquipmentManager* _pEquipmentManager)
	: mRegistry(_registry)
{
	mpStatusManager = new StatusManager(_pEntityManager, _pResourceManager);
	mpEquipmentManager = _pEquipmentManager;
	mpEntityManager = _pEntityManager;
	mpPhysicsManager = _pPhysicsManager;
	mpResourceManager = _pResourceManager;
	mpSoundManager = _pSoundManager;
	mpFSMHelper = new FSMHelper(_pResourceManager);
	mpAllyFSM = new AllyFSM(_pEntityManager, _pPhysicsManager, _pResourceManager, _pEventManager, _pSoundManager, _pRenderManager, mpFSMHelper);
	mpEnemyFSM = new EnemyFSM(_pEntityManager, _pPhysicsManager, _pResourceManager, _pEventManager, _pSoundManager, _pRenderManager, mpFSMHelper);
	mpEnemyPosVec = {};

	// ��ȹ���� �������� ������, �������� ���̴�.
	mMaxSetUnitNum = 20;  // �ִ� ��ġ ������ ���� ��
	mBuffLv1_Limit = 20;
	mBuffLv2_Limit = 50;

	unitDeployPosVec.resize(mMaxSetUnitNum); // ��ġ�� ���� ���� �ִ� ������� �޸� �Ҵ�(���Ҵ��� ���� ����)
	// ������ ����
	mpRenderManager = _pRenderManager;
}

PlayerSystem::~PlayerSystem()
{
	delete mpStatusManager;
}

void PlayerSystem::Initialize(std::vector<std::pair<std::shared_ptr<Entity>, Vector3>>* _pMoneyPosVec, AStar* _pAstar)
{
	// ���⼭ �÷��̾� �ý��ۿ� ���ӵ� ��� �Ŵ����� �ʱ�ȭ�Ѵ�.
	mpStatusManager->Initialize();
	mpEnemyPosVec = mpResourceManager->ParseCSV<EnemyData>("Enemy.csv");

	// FSM �ʱ�ȭ
	mpFSMHelper->Initialize();
	// ����ü �ý����� moneyPosVec* �����ϱ�(Loot���� Vector����)
	mpFSMHelper->SetMoneyVec(_pMoneyPosVec);
	mpFSMHelper->SetAstar(_pAstar);

	mpAllyFSM->Initialize();
	mpEnemyFSM->Initialize();


}

void PlayerSystem::PlayerUnitUpdate(std::shared_ptr<Entity> _entity, float _dTime)
{
	/// ��ġ �� ���۵��� ��
	auto& playerComp = _entity->GetComponent<PlayerComponent>();

	// ���� ������ ������Ʈ ���� �ʴ´�.
	if (playerComp.mIsDead == true)
	{
		return;
	}

	// �� FSM�� ����˴ϴ�.
	mpAllyFSM->Update(_dTime, _entity);

	// Ÿ���� ���� �͵��� Ÿ���� �缳���մϴ�.(PickBestTarget�� ��ġ�� ����� ���ɼ��� ū �Լ��Դϴ�.)
	if (playerComp.mpTarget == nullptr)
	{
		//playerComp.mpTarget = EnemyTargetInRange(_entity);
		playerComp.mpTarget = TargetNearestEnemy(_entity);
	}

	// �������� Ȯ���ؼ� ���� ������ �����մϴ�.(�̰� �������� ����Ǿ�� �Ѵ�.)
	int mTempBuffLv = 0;
	if (playerComp.mMoney >= playerComp.mBuffMoney1) // 30�̻��̸� ���� ���� -> ���� �ݾ� �̻��̸� ����
	{
		mTempBuffLv = 1;
		if (playerComp.mMoney >= playerComp.mBuffMoney1 + playerComp.mBuffMoney2)
		{
			mTempBuffLv = 2;
		}
	}

	if (playerComp.mbuffLv != mTempBuffLv) // �̰� ������ �ٲ��� ���� �����ϴ� �� ����. (���ҽ��� �ٲٴ� �Ŵ�...�ִ��� ȿ��������..)
	{
		// status�� ��� ������ ���Ƴ���.
		std::string stringClasstype = mpStatusManager->ConvertClasstypeToString(playerComp.mClassType);

		mpStatusManager->UpdateAllyStatus(_entity, stringClasstype, mTempBuffLv);
		mpEquipmentManager->UpdateEquipment(_entity, stringClasstype, mTempBuffLv);
		playerComp.mbuffLv = mTempBuffLv;
		mpSoundManager->PlaySFX("Snd_sfx_CharaBuff");
	}
}

void PlayerSystem::EnemyUnitUpdate(std::shared_ptr<Entity> _entity, float _dTime)
{
	/// ��ġ �� ���۵��� ��
	auto& playerComp = _entity->GetComponent<EnemyComponent>();

	// ���� ������ ������Ʈ ���� �ʴ´�.
	if (playerComp.mIsDead == true)
	{
		return;
	}

	// �� FSM�� ����˴ϴ�.
	mpEnemyFSM->Update(_dTime, _entity);

	// Ÿ���� ���� �͵��� Ÿ���� �缳���մϴ�.(PickBestTarget�� ��ġ�� ����� ���ɼ��� ū �Լ��Դϴ�.)
	if (playerComp.mpTarget == nullptr)
	{
		//playerComp.mpTarget = PlayerTargetInRange(_entity);
		playerComp.mpTarget = TargetNearestPlayer(_entity);
	}
}

void PlayerSystem::FixedUpdate(std::shared_ptr<Entity> _entity, float _fixedDTime)
{
	if (_entity->HasComponent<PlayerComponent>())
	{
		mpAllyFSM->FixedUpdate(_fixedDTime, _entity);
	}
	else if (_entity->HasComponent<EnemyComponent>())
	{
		mpEnemyFSM->FixedUpdate(_fixedDTime, _entity);
	}
}

void PlayerSystem::Finalize()
{
}

void PlayerSystem::AddUnit(std::shared_ptr<Entity> _entity, const std::string& _classType, Vector3 _pos)
{
	// ���̺�κ��� �⺻ ���ݰ� �޽��� �߰��Ѵ�.
	mpStatusManager->AddInitStatus(_entity, _classType);
	_entity->GetComponent<Transform>().m_localPosition = _pos;

	// ��� �߰��Ѵ�.
	mpEquipmentManager->AddInitUnitEquipment(_entity, _classType);

	// ���� ��ƼŬ�� �߰��Ѵ�.
	_entity->AddComponent<ParticleComponent>(Vector3(), 0, Vector3(0.78, 0.55, 0.3), 0.5, Vector3(0, 1, 0), 0, Vector3(), 1
		, Vector3(0.1), -0.5, Vector3(), 0.1, Vector3(0.1), Vector3(0.2), Vector3(), Vector3(1, 0.3, 1), Vector3(), 0, Vector3());

	// ���� �߰�
	AddPhysics(_entity);
}

void PlayerSystem::AddDeplyUnit(std::shared_ptr<Entity> _entity, const std::string& _classType)
{
	// ���̺�κ��� �⺻ ���ݰ� �޽��� �߰��Ѵ�.
	mpStatusManager->AddStatus(_entity, _classType, 0);

	// ���� ��ƼŬ�� �߰��Ѵ�.
	_entity->AddComponent<ParticleComponent>(Vector3(), 0, Vector3(0.78, 0.55, 0.3), 0.5, Vector3(0,1,0), 0, Vector3(), 1
		, Vector3(0.1), -0.5, Vector3(), 0.1, Vector3(0.1), Vector3(0.2), Vector3(), Vector3(1, 0.3, 1), Vector3(), 0, Vector3());

	// ��� �߰��Ѵ�.
	mpEquipmentManager->AddUnitEquip(_entity, _classType, 0);
	mpRenderManager->InitailizeEntity(_entity);
}

void PlayerSystem::ChangeUnit(std::shared_ptr<Entity> _entity, const std::string& _classType)
{
	// ���� Ŭ������ �ƴ� ��츸 ������
	if (_classType != mpStatusManager->ConvertClasstypeToString(_entity->GetComponent<PlayerComponent>().mClassType))
	{
		// ���̺�κ��� �⺻ ���ݰ� �޽��� �����Ѵ�.
		mpStatusManager->UpdateStautsNClass(_entity, _classType);

		// ��� �����Ѵ�.
		mpEquipmentManager->UpdateEquipment(_entity, _classType, 0);
	}
}

void PlayerSystem::RemoveUnit(UID _entityUID)
{
	auto unitEntity = mpEntityManager->GetEntity(_entityUID);
	// 	auto& myEntityVec = unitEntity->GetComponent<AnimationComponent>().myEntityVec;

	// 	for (auto& equipments : myEntityVec)
	// 	{
	// 		mpEntityManager->RemoveEntity(equipments->GetUID());
	// 	}
	mpEntityManager->RemoveEntity(_entityUID);
}

void PlayerSystem::AddPhysics(std::shared_ptr<Entity> _entity)
{
	// ������ ������ �߰��Ѵ�.
	_entity->AddComponent<CapsuleCollider>(false, Vector3(0, 0.78f, 0), 0.3f, 0.38f);
	//_entity->GetComponent<CapsuleCollider>().m_radius = 30 * _entity->GetComponent<Transform>().m_localScale.x/*0.4 * 100 * pow(mBuffUnitSizeCoef, _buffLv)*/;
	_entity->AddComponent<Rigidbody>(50.f, 0.f, 0.f, true, false, 0.5, 0.8, 0.8);

	// ������ ���� ������Ʈ
	if (_entity->HasComponent<PlayerComponent>())
	{
		//_entity->GetComponent<Tr
		//_entity->GetComponent<PlayerComponent>().mAstarPos = Vector2(_entity->GetComponent<Transform>().m_localPosition.x, _entity->GetComponent<Transform>().m_localPosition.z);
		mpPhysicsManager->AddPhysicsObject(_entity->GetUID(), TYPE_UNIT, ATTR_ALLY);
	}
	else if (_entity->HasComponent<EnemyComponent>())
	{
		mpPhysicsManager->AddPhysicsObject(_entity->GetUID(), TYPE_UNIT, ATTR_ENEMY);
	}
	//ShowState(_entity);

	mpPhysicsManager->SetFixPosition(_entity, false);
}

void PlayerSystem::AddSceneEnemyUnits(UID _sceneUID)
{
	for (const auto& enemyData : mpEnemyPosVec)
	{
		if (_sceneUID == UID(enemyData.sceneNum))
		{
			auto enemy = mpEntityManager->CreateEntity("enemy");
			AddUnit(enemy, enemyData.className, Vector3(enemyData.posX, 0.0f, enemyData.posZ));
		}
	}
}

void PlayerSystem::SetInitializeState(std::shared_ptr<Entity> _entity, State _state)
{
	// �� �÷��̾��� FSM ���� �ʱ�ȭ
	if (_entity->HasComponent<PlayerComponent>())
	{
		auto& playerComponent = _entity->GetComponent<PlayerComponent>();
		playerComponent.mCurrentSM = mpAllyFSM->GetState(_state);
		playerComponent.mCurrentSM->Enter(playerComponent.m_pOwner);
	}
	else if (_entity->HasComponent<EnemyComponent>())
	{
		auto& enemyComponent = _entity->GetComponent<EnemyComponent>();
		enemyComponent.mCurrentSM = mpEnemyFSM->GetState(_state);
		enemyComponent.mCurrentSM->Enter(enemyComponent.m_pOwner);
	}
}

bool PlayerSystem::IsAttackable(const Vector2& _myPos, const Vector2& _otherPos, float _attackRange)
{
	// x�� �Ÿ��� y�� �Ÿ��� attackRange�� �����̶�� ���� ���� ���´�.
	if (abs(_otherPos.x - _myPos.x) <= _attackRange &&
		abs(_otherPos.y - _myPos.y) <= _attackRange)
	{
		return true;
	}
	return false;
}

void PlayerSystem::ShowState(std::shared_ptr<Entity> _entity)
{
	// ��Ʈ �߰�
	auto& textComp = _entity->AddComponent<Text>(u8" ", "KIMM_Bold(60).ttf", Vector2(), 2, true, Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	textComp.m_hasDepth = true;
	textComp.m_idx = TEXT_IDX::FLOAT2;
	textComp.m_offset = Vector3(0, 5, 0);
	textComp.m_scale = 0.4f;
}

void PlayerSystem::UpdateStateText(std::shared_ptr<Entity> _entity)
{
	std::u8string stateText = u8"";
	State currentState;

	if (_entity->HasComponent<PlayerComponent>())
	{
		currentState = _entity->GetComponent<PlayerComponent>().mOwnState;
	}
	else
	{
		currentState = _entity->GetComponent<EnemyComponent>().mOwnState;
	}

	switch (currentState)
	{
	case State::IDLE:
		stateText = u8"IDLE\n(%.2f, %.2f)";
		break;
	case State::MOVE:
		stateText = u8"MOVE\n(%.2f, %.2f)";
		break;
	case State::ATTACK:
		stateText = u8"ATTACK\n(%.2f, %.2f)";
		break;
	case State::LOOT:
		stateText = u8"LOOT\n(%.2f, %.2f)";
		break;
	case State::DAMAGED:
		stateText = u8"DAMAGED\n(%.2f, %.2f)";
		break;
	case State::DEAD:
		stateText = u8"DEAD\n(%.2f, %.2f)";
		break;
	default:
		break;
	}

	// std::string�� std::u8string���� ��ȯ
	auto& textComp = _entity->GetComponent<Text>();
	textComp.m_text = stateText;
	Vector3& playerPos = _entity->GetComponent<Transform>().m_localPosition;
	textComp.m_worldPosition = playerPos;
	textComp.m_num3 = playerPos.x;
	textComp.m_num4 = playerPos.z;
}

void PlayerSystem::DetectGroundMoney()
{
	auto& moneyVec = *mpFSMHelper->GetMoneyVec(); // �� ���� ���� ���
	//auto& astarObsPosVec = *mpFSMHelper->GetAstarObstacles(); // ��ֹ� ���� ���� ���

	// ���� �� ��ġ�� ������ ������ ����� 0�� �ƴϰ�, Ȥ�� ����� �ٲ�ٸ�
	if (moneyVec.size() != tempMoneyVecSize || moneyVec.size() > 0)
	{
		//for (auto it = moneyVec.begin(); it != moneyVec.end(); )
		//{
		//	if (it->first.x == astarObsPosVec.x) // Ư�� Ÿ�ٰ� ��ġ�ϴ� ��Ҹ� ã��
		//	{
		//		it = moneyVec.erase(it); // �ش� ��� ���� ��, ���� ��ҷ� �̵�
		//	}
		//	else
		//	{
		//		++it; // ��ġ���� ������ ���� ��ҷ� �̵�
		//	}
		//}



		// ����� �������ش�.
		tempMoneyVecSize = mpFSMHelper->GetMoneyVec()->size();

		auto playerView = mRegistry.view<Transform, PlayerComponent>();
		for (auto& entity : playerView)
		{
			auto& playerComp = mRegistry.get<PlayerComponent>(entity);
			auto& trsComp = mRegistry.get<Transform>(entity);
			if (playerComp.mOwnState != State::DEAD) // Ÿ���� �׾����� �ൿ���� �ʴ´�.
			{
				// ���� ���ְ� ���� ����� ���� ��ƼƼ�� �Ÿ����� �켱���� ť�� �̾ƿ´�.
				std::pair<std::shared_ptr<Entity>, float> nearlistMoney = FindClosestEntity(trsComp.m_localPosition);

				// ���� �νĹ��� �̳��̰�, ���� nullptr�� �ƴϸ� Ÿ������ �����Ѵ�. �� �κп��� ground�� ����ϴ� ����, �켱���� ť���� ���͸��� Get�� �ϴ� �ͺ��� ���⼭ �� �� �ϴ� �� ���� �����̴�..
				if (playerComp.mRecogRange > nearlistMoney.second && nearlistMoney.first != nullptr)
				{
					playerComp.mpTarget = nearlistMoney.first;
				}
				else
				{
					// ���� �� ��ġ�� ������ ������ ����� 0�̶�� Ÿ���� ������ �ƴ� �͵��� ���� Ÿ���� nullptr�� �����.
					if (tempMoneyVecSize == 0 && playerComp.mpTarget && playerComp.mpTarget->HasComponent<EnemyComponent>() == false)
					{
						playerComp.mpTarget = nullptr;
					}
				}
			}
		}

		auto enemyView = mRegistry.view<Transform, EnemyComponent>();
		for (auto& entity : enemyView)
		{
			//auto& enemyComp = mRegistry.get<EnemyComponent>(entity);
			auto enemyComp = mRegistry.try_get<EnemyComponent>(entity);
			auto& trsComp = mRegistry.get<Transform>(entity);
			//if (enemyComp.mOwnState != State::DEAD) // Ÿ���� �׾����� �ൿ���� �ʴ´�.
			//{
			//	// ���� ���ְ� ���� ����� ���� ��ƼƼ�� �Ÿ����� �켱���� ť�� �̾ƿ´�.
			//	std::pair<std::shared_ptr<Entity>, float> nearlistMoney = FindClosestEntity(trsComp.m_localPosition);

			//	// ���� �νĹ��� �̳��̰�, ���� nullptr�� �ƴϸ� Ÿ������ �����Ѵ�.
			//	if (enemyComp.mRecogRange > nearlistMoney.second && nearlistMoney.first != nullptr)
			//	{
			//		enemyComp.mpTarget = nearlistMoney.first;
			//	}
			//	else
			//	{
			//		// ���� �� ��ġ�� ������ ������ ����� 0�̶�� Ÿ���� ������ �ƴ� �͵��� ���� Ÿ���� nullptr�� �����.
			//		if (tempMoneyVecSize == 0 && enemyComp.mpTarget && enemyComp.mpTarget->HasComponent<PlayerComponent>() == false)
			//		{
			//			enemyComp.mpTarget = nullptr;
			//		}
			//	}
			//}
			if (enemyComp->mOwnState != State::DEAD) // Ÿ���� �׾����� �ൿ���� �ʴ´�.
			{
				// ���� ���ְ� ���� ����� ���� ��ƼƼ�� �Ÿ����� �켱���� ť�� �̾ƿ´�.
				std::pair<std::shared_ptr<Entity>, float> nearlistMoney = FindClosestEntity(trsComp.m_localPosition);

				// ���� �νĹ��� �̳��̰�, ���� nullptr�� �ƴϸ� Ÿ������ �����Ѵ�.
				if (enemyComp->mRecogRange > nearlistMoney.second && nearlistMoney.first != nullptr)
				{
					enemyComp->mpTarget = nearlistMoney.first;
				}
				else
				{
					// ���� �� ��ġ�� ������ ������ ����� 0�̶�� Ÿ���� ������ �ƴ� �͵��� ���� Ÿ���� nullptr�� �����.
					if (tempMoneyVecSize == 0 && enemyComp->mpTarget && enemyComp->mpTarget->HasComponent<PlayerComponent>() == false)
					{
						enemyComp->mpTarget = nullptr;
					}
				}
			}
		}
	}
}

std::shared_ptr<Entity> PlayerSystem::TargetNearestEnemy(std::shared_ptr<Entity> _entity)
{
	//		 ������ �Ÿ� <= �ν� ����
	//			|				\
	//			|				 \
	//		  �Ʊ��ΰ�?         Ÿ�� ���� : ���� ����� ��
	//		|		  \					  ��
	//		|		   \				  |
	//	������<���Ѽ�?	Ÿ�� ���� : ��	  |
	//		|	\______________��__________|
	//		|__________________|

	auto& position = _entity->GetComponent<Transform>().m_localPosition;

	// �Ÿ� ������ �����ϱ� ���� �ָӴ� ����
	std::vector<std::pair<std::shared_ptr<Entity>, float>> entities;

	/// ����� ��(�Ʊ�, ���� ����)�� Ÿ������ �����մϴ�.
	auto PlayerView = mRegistry.view<EnemyComponent>();
	for (auto& entity : PlayerView)
	{
		auto& enemyComponent = mRegistry.get<EnemyComponent>(entity);
		// ���� ������ ������Ʈ ���� �ʴ´�.
		if (enemyComponent.mIsDead == true)
		{
			continue;
		}

		// ���� ����� Ÿ���� ����Ѵ�.
		Vector3& notAllyUnit = mRegistry.get<Transform>(entity).m_localPosition; // Ÿ�� ������ ��ġ
		float distance = (notAllyUnit - position).Length();
		entities.emplace_back(enemyComponent.m_pOwner, distance);
	}

	// ���� ����� ��ƼƼ ��ȯ
	if (!entities.empty())
	{
		std::sort(entities.begin(), entities.end(), [](const auto& lhs, const auto& rhs)
			{return lhs.second < rhs.second; });

		return entities.front().first;
	}

	/// �ƹ��͵� ������ nullptr�� ��ȯ
	return nullptr;
}

std::shared_ptr<Entity> PlayerSystem::EnemyTargetInRange(std::shared_ptr<Entity> _entity)
{
	auto& playerComp = _entity->GetComponent<PlayerComponent>();

	/// ���� ���� ������ �����ϴ��� Ȯ���մϴ�.
	auto PlayerView = mRegistry.view<EnemyComponent>();
	for (auto& entity : PlayerView)
	{
		auto& enemyComponent = mRegistry.get<EnemyComponent>(entity);
		// ���� ������ Ÿ�ٿ��� �����Ѵ�.
		if (enemyComponent.mIsDead == true)
		{
			continue;
		}

		//	// �� ���� �ø� ó���Ͽ� ��Ʈ�� ó���Ѵ�.
		int attackRange = static_cast<int>(ceil(playerComp.mAttackRange));

		// ���� �ȿ� ���� Ÿ���� �����Ѵ�.
		if (IsAttackable(playerComp.mAstarPos, enemyComponent.mAstarPos, attackRange) == true)
		{
			return mpEntityManager->GetEntity(entity);
		}
	}

	// �� ã������ nullptr��
	return nullptr;
}

std::shared_ptr<Entity> PlayerSystem::TargetNearestPlayer(std::shared_ptr<Entity> _entity)// _entity�� ����, entity�� ���̾�
{
	auto& position = _entity->GetComponent<Transform>().m_localPosition;

	// �Ÿ� ������ �����ϱ� ���� �ָӴ� ����
	std::vector<std::pair<std::shared_ptr<Entity>, float>> entities;

	/// ����� ��(�Ʊ�)�� Ÿ������ �����մϴ�.
	auto PlayerView = mRegistry.view<PlayerComponent>();
	for (auto& entity : PlayerView)
	{
		auto& playerComponent = mRegistry.get<PlayerComponent>(entity);
		{
			// ���� ������ ������Ʈ ���� �ʴ´�.
			if (playerComponent.mIsDead == true)
			{
				continue;
			}

			// ���� ����� Ÿ���� ����Ѵ�.
			Vector3& notAllyUnit = mRegistry.get<Transform>(entity).m_localPosition; // Ÿ�� ������ ��ġ
			float distance = (notAllyUnit - position).Length();
			entities.emplace_back(playerComponent.m_pOwner, distance);
		}
	}

	// ���� ����� ��ƼƼ ��ȯ
	if (!entities.empty())
	{
		std::sort(entities.begin(), entities.end(), [](const auto& lhs, const auto& rhs)
			{return lhs.second < rhs.second; });

		return entities.front().first;
	}

	/// �ƹ��͵� ������ nullptr�� ��ȯ
	return nullptr;
}

std::shared_ptr<Entity> PlayerSystem::PlayerTargetInRange(std::shared_ptr<Entity> _entity)
{
	auto& enemyComp = _entity->GetComponent<EnemyComponent>();

	/// ���� ���� �÷��̾ �����ϴ��� Ȯ���մϴ�.
	auto PlayerView = mRegistry.view<PlayerComponent>();
	for (auto& entity : PlayerView)
	{
		auto& playerComp = mRegistry.get<PlayerComponent>(entity);
		// ���� ������ Ÿ�ٿ��� �����Ѵ�.
		if (playerComp.mIsDead == true)
		{
			continue;
		}

		//	// �� ���� �ø� ó���Ͽ� ��Ʈ�� ó���Ѵ�.
		int attackRange = static_cast<int>(ceil(enemyComp.mAttackRange));

		// ���� �ȿ� ���� Ÿ���� �����Ѵ�.
		if (IsAttackable(enemyComp.mAstarPos, playerComp.mAstarPos, attackRange) == true)
		{
			return mpEntityManager->GetEntity(entity);
		}
	}

	// �� ã������ nullptr��
	return nullptr;
}

std::vector<Vector2>* PlayerSystem::GetUnitDeployPosVec()
{
	return &unitDeployPosVec;
}

std::pair<std::shared_ptr<Entity>, float> PlayerSystem::FindClosestEntity(const Vector3& _pos)
{
	// �켱���� ť ����
	std::priority_queue<std::pair<std::shared_ptr<Entity>, float>,
		std::vector<std::pair<std::shared_ptr<Entity>, float>>, DistanceCompare> closetList;

	// mpMoneyPosVec�� �ִ� ��ƼƼ��� _pos ���� �Ÿ� ��� �� �켱���� ť�� �߰�
	for (const auto& e : *mpFSMHelper->GetMoneyVec())
	{
		float distance = (_pos - e.second).Length(); // _pos�� e�� ��ġ ���� ���
		closetList.emplace(e.first, distance);		 // ��ƼƼ�� �Ÿ��� ť�� �߰�
	}

	// ť�� ������� �ʴٸ� ���� ����� ��ƼƼ�� �Ÿ� ��ȯ
	if (!closetList.empty())
	{
		return closetList.top(); // ��ƼƼ�� �Ÿ� �� ��� ��ȯ
	}

	// ť�� ������� ���� �⺻ ��(Ȥ�� nullptr)�� ��ȯ
	return { nullptr, 30.0f };
}
