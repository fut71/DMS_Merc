#pragma once
#include "AllyFSM.h"
#include "CapsuleCollider.h"

AllyFSM::AllyFSM(EntityManager* _pEntityManager, PhysicsManager* _pPhysicsManager
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

AllyFSM::~AllyFSM()
{
	delete mpDeadState;
	delete mpDamagedState;
	delete mpLootState;
	delete mpAttackState;
	delete mpMoveState;
	delete mpIdleState;
	delete mpTransition;
}

void AllyFSM::Initialize()
{
	//  state ����
	mpIdleState = new IdleState(mpEntityManager, mpPhysicsManager, mpResourceManager, mpEventManager, mpSoundManager);
	mpMoveState = new MoveState(mpEntityManager, mpPhysicsManager, mpResourceManager, mpEventManager, mpSoundManager, mpFSMHelper);
	mpAttackState = new AttackState(mpEntityManager, mpPhysicsManager, mpResourceManager, mpEventManager, mpSoundManager, mpRenderManager, mpFSMHelper);
	mpLootState = new LootState(mpEntityManager, mpPhysicsManager, mpResourceManager, mpEventManager, mpSoundManager, mpFSMHelper);
	mpDamagedState = new DamagedState(mpEntityManager, mpPhysicsManager, mpResourceManager, mpEventManager, mpSoundManager);
	mpDeadState = new DeadState(mpEntityManager, mpPhysicsManager, mpResourceManager, mpEventManager, mpSoundManager);

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
void AllyFSM::ChangeState(std::shared_ptr<Entity> _entity, State _newStateEnum)
{
	/// 0. ���� ���� ����
	auto& playerComp = _entity->GetComponent<PlayerComponent>();

	/// 1. ���� ������ ���°� �ƴϸ� ����
	if (mpTransition->IsCanTransitionTo(_newStateEnum) == false)
	{
		//return;
		//_entity->GetComponent<AnimationComponent>().mpNextTargetAnimation = mpResourceManager->Get_Animation("Character@Death.fbx");
	}

	/// 2. ���� ���°� nullptr�� �ƴ϶�� ���� ���¸� ������ �� ���·� �� �غ� �Ѵ�.
	if (playerComp.mCurrentSM != nullptr)
	{
		playerComp.mCurrentSM->Exit(_entity);
	}

	playerComp.mCurrentSM = GetState(_newStateEnum);
	playerComp.mOwnState = _newStateEnum;

	/// 3. ���� ���°� nullptr�� �ƴ϶�� �� ���·� ����.
	if (playerComp.mCurrentSM != nullptr)
	{
		playerComp.mCurrentSM->Enter(_entity);
	}
}

void AllyFSM::Update(float _dTime, std::shared_ptr<Entity> _entity)
{
	/// 0. ���� ���� ����
	auto& playerComp = _entity->GetComponent<PlayerComponent>();

	/// 2. State ���� : ��ȹ ���� ����
	// ������ ���� ���°� �������� �ʴ´ٸ�, ����� �����κ��� ���¸� �ҷ��ͼ� �����Ѵ�.
	if (playerComp.mCurrentSM == nullptr)
	{
		playerComp.mCurrentSM = GetState(playerComp.mOwnState);
	}
	else //������ ���� ���°� �����Ѵٸ�
	{
		// ���� ���¿� ���ο� ���¸� ���ϱ� ���Ͽ�, ���ο� ������Ʈ ���¸� �����Ѵ�.
		State newStateEnum = playerComp.mOwnState;

		// Ÿ���� ������ trs�� �־�� �ϹǷ� ���ٸ� Ÿ���� ������ �����ϰ� nullptr�� �ٲ��ش�.
		if (playerComp.mpTarget != nullptr && playerComp.mpTarget->HasComponent<Transform>() == false)
		{
			playerComp.mpTarget = nullptr;
			newStateEnum = State::IDLE;
			playerComp.mCurrentSM = mpIdleState;
		}

		// ������ ���� ���¿� �´� state ������ Update �Լ��� �����Ų��.
		playerComp.mCurrentSM->Update(_dTime, _entity);

		// -------------------> DEAD
		if (mpFuncMap["To_Dead"](_entity))
		{
			newStateEnum = State::DEAD;
		}
		// Dead�� �ƴ϶��
		else if (!mpFuncMap.empty())
		{
			auto& tempTarget = playerComp.mpTarget;

			// Ÿ���� �������� �ʴ´ٸ� IDLE ����
			if (!tempTarget)
			{
				newStateEnum = State::IDLE;
			}
			else
			{
				// �ǰ��� ���ߴٸ� �ǰ� ����
				if (mpFuncMap["To_Damaged"](_entity))
				{
					newStateEnum = State::DAMAGED;
				}
				else // �� ���� ���¶��, state�� Ȯ���ϰ� ���� �������� üũ�Ѵ�.
				{
					switch (newStateEnum)
					{
					case State::IDLE:
					{
						if (mpFuncMap["Target_Is_Money"](_entity))
						{
							if (mpFuncMap["Conflict_Money"](_entity)) //f
							{
								newStateEnum = State::LOOT;
							}
							else
							{
								newStateEnum = State::MOVE; // a
							}
						}
						else if (mpFuncMap["Target_Is_Enemy"](_entity))
						{
							if (mpFuncMap["Enemy_In_Range"](_entity)) //b
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
						if (mpFuncMap["None_Target"](_entity)) // ��
						{
							newStateEnum = State::IDLE;
						}
						else if (mpFuncMap["Target_Is_Money"](_entity) && mpFuncMap["Conflict_Money"](_entity))
						{
							newStateEnum = State::LOOT;
						}
						else if (mpFuncMap["Target_Is_Enemy"](_entity) && mpFuncMap["Enemy_In_Range"](_entity)) // ��
						{
							newStateEnum = State::ATTACK;
						}
					}
					break;
					case State::ATTACK: // ��, e, d
					{
						if (mpFuncMap["None_Target"](_entity)) // ��
						{
							newStateEnum = State::IDLE;
						}
						else if (mpFuncMap["Target_Is_Money"](_entity))
						{
							if (mpFuncMap["Conflict_Money"](_entity))
							{
								newStateEnum = State::LOOT;
							}
							else
							{
								newStateEnum = State::MOVE;
							}
						}
						else if (mpFuncMap["Target_Is_Enemy"](_entity))
						{
							if (!mpFuncMap["Enemy_In_Range"](_entity))
							{
								newStateEnum = State::MOVE;
							}
						}
					}
					break;
					case State::LOOT: // c, ��, ��
					{
						if (mpFuncMap["None_Target"](_entity)) // ��
						{
							newStateEnum = State::IDLE;
						}
						else if (mpFuncMap["Target_Is_Enemy"](_entity))
						{
							if (mpFuncMap["Enemy_In_Range"](_entity))
							{
								newStateEnum = State::ATTACK; // ��
							}
							else
							{
								newStateEnum = State::MOVE;// c
							}
						}
						else if (mpFuncMap["Target_Is_Money"](_entity))
						{
							if (!mpFuncMap["Conflict_Money"](_entity))
							{
								newStateEnum = State::MOVE; //c
							}
						}
					}
					break;
					case State::DAMAGED: // ��, ��, ��, ��
					{
						if (mpFuncMap["None_Target"](_entity)) // ��
						{
							newStateEnum = State::IDLE;
						}
						else if (mpFuncMap["Target_Is_Enemy"](_entity))
						{
							if (mpFuncMap["Enemy_In_Range"](_entity))
							{
								newStateEnum = State::ATTACK; // ��
							}
							else
							{
								newStateEnum = State::MOVE;// ��
							}
						}
						else if (mpFuncMap["Target_Is_Money"](_entity))
						{
							if (mpFuncMap["Conflict_Money"](_entity))
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

		if (mpTransition->IsTransit(playerComp.mOwnState, newStateEnum))
		{
			ChangeState(_entity, newStateEnum);
		}
	}
}

IState* AllyFSM::GetState(State _state)
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

void AllyFSM::FixedUpdate(float _fixedTime, std::shared_ptr<Entity> _entity)
{
	if (_entity->GetComponent<PlayerComponent>().mCurrentSM)
	{
		_entity->GetComponent<PlayerComponent>().mCurrentSM->FixedUpdate(_fixedTime, _entity);
	}

}
