#pragma once
#include "EnemyFSM.h"
#include "CapsuleCollider.h"

EnemyFSM::EnemyFSM(EntityManager* _pEntityManager, PhysicsManager* _pPhysicsManager
	, ResourceManager* _pResourceManager, EventManager* _pEventManager
	, SoundManager* _pSoundManager, RenderManager* _pRenderManager, FSMHelper* _pFSMHelper)
{
	mpEntityManager = _pEntityManager;
	mpPhysicsManager = _pPhysicsManager;
	mpResourceManager = _pResourceManager;
	mpEventManager = _pEventManager;
	mpSoundManager = _pSoundManager;
	mpRenderManager = _pRenderManager;
	mpFSMHelper = _pFSMHelper;
	mpTransition = new Transition();
}

EnemyFSM::~EnemyFSM()
{
	delete mpDeadState;
	delete mpDamagedState;
	delete mpLootState;
	delete mpAttackState;
	delete mpMoveState;
	delete mpIdleState;
	delete mpTransition;
}

void EnemyFSM::Initialize()
{
	//  state ����
	mpIdleState = new IdleStateE(mpEntityManager, mpPhysicsManager, mpResourceManager, mpEventManager, mpSoundManager);
	mpMoveState = new MoveStateE(mpEntityManager, mpPhysicsManager, mpResourceManager, mpEventManager, mpSoundManager, mpFSMHelper);
	mpAttackState = new AttackStateE(mpEntityManager, mpPhysicsManager, mpResourceManager, mpEventManager, mpSoundManager, mpRenderManager, mpFSMHelper);
	mpLootState = new LootStateE(mpEntityManager, mpPhysicsManager, mpResourceManager, mpEventManager, mpSoundManager, mpFSMHelper);
	mpDamagedState = new DamagedStateE(mpEntityManager, mpPhysicsManager, mpResourceManager, mpEventManager, mpSoundManager);
	mpDeadState = new DeadStateE(mpEntityManager, mpPhysicsManager, mpResourceManager, mpEventManager, mpSoundManager);

	mpIdleState->Initialize();
	mpMoveState->Initialize();
	mpAttackState->Initialize();
	mpLootState->Initialize();
	mpDamagedState->Initialize();
	mpDeadState->Initialize();

	// �������� �Լ� ���� �޾ƿ´�.
	mpFuncMap = mpFSMHelper->GetFuctMap();
}

// Transition���� ���� �������� Ȯ���Ѵ�.
void EnemyFSM::ChangeState(std::shared_ptr<Entity> _entity, State _newStateEnum)
{
	/// 0. ���� ���� ����
	auto& enemyComp = _entity->GetComponent<EnemyComponent>();

	/// 1. ���� ������ ���°� �ƴϸ� ����
	if (mpTransition->IsCanTransitionTo(_newStateEnum) == false)
	{
		//return;
		//_entity->GetComponent<AnimationComponent>().mpNextTargetAnimation = mpResourceManager->Get_Animation("Character@Death.fbx");
	}

	/// 2. ���� ���°� nullptr�� �ƴ϶�� ���� ���¸� ������ �� ���·� �� �غ� �Ѵ�.
	if (enemyComp.mCurrentSM != nullptr)
	{
		enemyComp.mCurrentSM->Exit(_entity);
	}

	enemyComp.mCurrentSM = GetState(_newStateEnum);
	enemyComp.mOwnState = _newStateEnum;

	/// 3. ���� ���°� nullptr�� �ƴ϶�� �� ���·� ����.
	if (enemyComp.mCurrentSM != nullptr)
	{
		enemyComp.mCurrentSM->Enter(_entity);
	}
}

void EnemyFSM::Update(float _dTime, std::shared_ptr<Entity> _entity)
{
	/// 0. ���� ���� ����
	auto& enemyComp = _entity->GetComponent<EnemyComponent>();

	/// 2. State ���� : ��ȹ ���� ����
	// ������ ���� ���°� �������� �ʴ´ٸ�, ����� �����κ��� ���¸� �ҷ��ͼ� �����Ѵ�.
	if (enemyComp.mCurrentSM == nullptr)
	{
		enemyComp.mCurrentSM = GetState(enemyComp.mOwnState);
	}
	else //������ ���� ���°� �����Ѵٸ�
	{
		// ���� ���¿� ���ο� ���¸� ���ϱ� ���Ͽ�, ���ο� ������Ʈ ���¸� �����Ѵ�.
		State newStateEnum = enemyComp.mOwnState;

		// Ÿ���� ������ trs�� �־�� �ϹǷ� ���ٸ� Ÿ���� ������ �����ϰ� nullptr�� �ٲ��ش�.
		if (enemyComp.mpTarget != nullptr && enemyComp.mpTarget->HasComponent<Transform>() == false)
		{
			enemyComp.mpTarget = nullptr;
			newStateEnum = State::IDLE;
			enemyComp.mCurrentSM = mpIdleState;
		}

		// ������ ���� ���¿� �´� state ������ Update �Լ��� �����Ų��.
		enemyComp.mCurrentSM->Update(_dTime, _entity);

		// -------------------> DEAD
		if (mpFuncMap["To_DeadE"](_entity))
		{
			newStateEnum = State::DEAD;
		}
		///Dead�� �ƴ϶��
		else if (!mpFuncMap.empty())
		{
			auto& tempTarget = enemyComp.mpTarget;

			if (!tempTarget)
			{
				newStateEnum = State::IDLE;
			}
			else
			{
				if (mpFuncMap["To_DamagedE"](_entity))
				{
					newStateEnum = State::DAMAGED;
				}
				else
				{
					switch (newStateEnum)
					{
					case State::IDLE:
					{
						if (mpFuncMap["Target_Is_MoneyE"](_entity))
						{
							if (mpFuncMap["Conflict_MoneyE"](_entity)) //f
							{
								newStateEnum = State::LOOT;
							}
							else
							{
								newStateEnum = State::MOVE; // a
							}
						}
						else if (mpFuncMap["Target_Is_Player"](_entity))
						{
							if (mpFuncMap["Player_In_Range"](_entity)) //b
							{
								newStateEnum = State::ATTACK;
							}
							else
							{
								newStateEnum = State::MOVE;
							}
						}
					}
					break;
					case State::MOVE: // ��, ��, ��
					{
						if (mpFuncMap["None_TargetE"](_entity)) // ��
						{
							newStateEnum = State::IDLE;
						}
						else if (mpFuncMap["Target_Is_MoneyE"](_entity) && mpFuncMap["Conflict_MoneyE"](_entity))
						{
							newStateEnum = State::LOOT;
						}
						else if (mpFuncMap["Target_Is_Player"](_entity) && mpFuncMap["Player_In_Range"](_entity)) // ��
						{
							newStateEnum = State::ATTACK;
						}
					}
					break;
					case State::ATTACK: // ��, e, d
					{
						if (mpFuncMap["None_TargetE"](_entity)) // ��
						{
							newStateEnum = State::IDLE;
						}
						else if (mpFuncMap["Target_Is_MoneyE"](_entity))
						{
							if (mpFuncMap["Conflict_MoneyE"](_entity))
							{
								newStateEnum = State::LOOT;
							}
							else
							{
								newStateEnum = State::MOVE;
							}
						}
						else if (mpFuncMap["Target_Is_Player"](_entity))
						{
							if (!mpFuncMap["Player_In_Range"](_entity))
							{
								newStateEnum = State::MOVE;
							}
						}
					}
					break;
					case State::LOOT: // c, ��, ��
					{
						if (mpFuncMap["None_TargetE"](_entity)) // ��
						{
							newStateEnum = State::IDLE;
						}
						else if (mpFuncMap["Target_Is_Player"](_entity))
						{
							if (mpFuncMap["Player_In_Range"](_entity))
							{
								newStateEnum = State::ATTACK; // ��
							}
							else
							{
								newStateEnum = State::MOVE;// c
							}
						}
						else if (mpFuncMap["Target_Is_MoneyE"](_entity))
						{
							if (!mpFuncMap["Conflict_MoneyE"](_entity))
							{
								newStateEnum = State::MOVE; //c
							}
						}
					}
					break;
					case State::DAMAGED: // ��, ��, ��, ��
					{
						if (mpFuncMap["None_TargetE"](_entity)) // ��
						{
							newStateEnum = State::IDLE;
						}
						else if (mpFuncMap["Target_Is_Player"](_entity))
						{
							if (mpFuncMap["Player_In_Range"](_entity))
							{
								newStateEnum = State::ATTACK; // ��
							}
							else
							{
								newStateEnum = State::MOVE;// ��
							}
						}
						else if (mpFuncMap["Target_Is_MoneyE"](_entity))
						{
							if (mpFuncMap["Conflict_MoneyE"](_entity))
							{
								newStateEnum = State::LOOT; // ��
							}
							else
							{
								newStateEnum = State::MOVE;// ��
							}
						}
					}
					break;
					default:
						break;
					}
				}
			}
		}

		if (mpTransition->IsTransit(enemyComp.mOwnState, newStateEnum))
		{
			ChangeState(_entity, newStateEnum);
		}
	}
}

IState* EnemyFSM::GetState(State _state)
{
	switch (_state)
	{
	case State::IDLE:
		return mpIdleState;
		break;
	case State::MOVE:
		return mpMoveState;
		break;
	case State::ATTACK:
		return mpAttackState;
		break;
	case State::LOOT:
		return mpLootState;
		break;
	case State::DAMAGED:
		return mpDamagedState;
		break;
	case State::DEAD:
		return mpDeadState;
		break;
	default:
		break;
	}
}

void EnemyFSM::FixedUpdate(float _fixedTime, std::shared_ptr<Entity> _entity)
{
	if (_entity->GetComponent<EnemyComponent>().mCurrentSM)
	{
		_entity->GetComponent<EnemyComponent>().mCurrentSM->FixedUpdate(_fixedTime, _entity);
	}
}
