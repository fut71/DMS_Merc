//#include "BossFSM.h"
//#include "CapsuleCollider.h"
//
//BossFSM::BossFSM(ClientAnimator* _animator)
//{
//	Initialize(_animator);
//	currentSM = GetState(State::IDLE);
//	currentSM->Initialize();
//	mpTransition = new Transition(State::IDLE);
//	//currentSM->Enter(); // �� �ּ� ������ ������. ����� �ϴµ� ����� �̷�� ����.
//	MakeFunction();
//}
//
//BossFSM::~BossFSM()
//{
//	delete mpTransition;
//}
//
//void BossFSM::Initialize(ClientAnimator* _animator)
//{
//	mStatePool[State::IDLE] = new IdleState(_animator);
//	mStatePool[State::MOVE] = new MoveState(_animator);
//	mStatePool[State::ATTACK] = new AttackState(_animator);
//	mStatePool[State::DEAD] = new DeadState(_animator);
//}
//
//void BossFSM::MakeFunction()
//{
//	/// ����, �Ʊ� ����
//// IDLE -------------> ATTACK
//	mFuncMap["Idle_To_Attack"] = [](std::shared_ptr<Entity> a)->bool
//		{
//			// is�Ʊ� != Ÿ��is�Ʊ� && ���� ��Ÿ�� <= 0
//			if ((a->GetComponent<PlayerComponent>().mIsAlly != a->GetComponent<PlayerComponent>().mpTarget->GetComponent<PlayerComponent>().mIsAlly)
//				&& a->GetComponent<PlayerComponent>().mAttackCoolTime <= 0)
//			{
//				return true;
//			}
//			return false; // State::IDLE;
//		};
//
//	// IDLE --------------> MOVE
//	mFuncMap["Idle_To_Move"] = [](std::shared_ptr<Entity> a)->bool
//		{
//
//			if (a->GetComponent<PlayerComponent>().mpTarget->GetComponent<PlayerComponent>().mIsMoney == true ||
//				a->GetComponent<PlayerComponent>().mIsAlly != a->GetComponent<PlayerComponent>().mpTarget->GetComponent<PlayerComponent>().mIsAlly)
//			{
//				return true; // State::MOVE;
//			}
//			return false; // State::IDLE;
//		};
//
//	// IDLE --------------> LOOT
//	mFuncMap["Idle_To_Loot"] = [](std::shared_ptr<Entity> a)->bool
//		{
//			// ��Ÿ� ���
//			if ((a->GetComponent<PlayerComponent>().mpTarget->GetComponent<PlayerComponent>().mIsMoney == true))
//			{
//				return true;
//			}
//			return false; // State::LOOT;
//		};
//
//	// MOVE --------------> IDLE  
//	mFuncMap["Move_To_Idle"] = [](std::shared_ptr<Entity> a)->bool
//		{
//			//��ǥ == nullptr ||  ���� ����� ���̰�, ��Ÿ� < ��ǥ���� �Ÿ�)
//			if (!(a->GetComponent<PlayerComponent>().mpTarget) ||
//				a->GetComponent<PlayerComponent>().mIsAlly != a->GetComponent<PlayerComponent>().mpTarget->GetComponent<PlayerComponent>().mIsAlly
//				&& a->GetComponent<PlayerComponent>().mAttackRange > (a->GetComponent<PlayerComponent>().mpTarget->GetComponent<Transform>().m_localPosition - a->GetComponent<Transform>().m_localPosition).Length())
//			{
//				return true;
//			}
//			return false; // State::MOVE;
//		};
//
//	// MOVE ------------> LOOT
//	mFuncMap["Move_To_Loot"] = [](std::shared_ptr<Entity> a)->bool
//		{
//			if ((a->GetComponent<PlayerComponent>().mpTarget->GetComponent<PlayerComponent>().mIsMoney == true))
//			{
//				return true;
//			}
//			return false; // State::LOOT;
//
//		};
//
//	// ATTACK ------------> IDLE
//	mFuncMap["Attack_To_Idle"] = [](std::shared_ptr<Entity> a)->bool
//		{
//			//��ǥ == �� ||  ���� �ִϸ��̼� �ð� ���� -> �� ���� �ִϸ��̼� �ð��� ������ �Ŵ�... AttackState���� ó���ؾ� �ϳ�?
//			// Ÿ���� ������ �ٲ�...?
//			if ((a->GetComponent<PlayerComponent>().mpTarget->GetComponent<PlayerComponent>().mIsMoney == true))
//			{
//				return true;
//			}
//			return false; // State::ATTACK;
//		};
//
//	// LOOT --------------> IDLE
//	mFuncMap["Loot_To_Idle"] = [](std::shared_ptr<Entity> a)->bool
//		{
//			// Ÿ���� ������ && ���� �ð��� ����
//			if (!(a->GetComponent<PlayerComponent>().mpTarget))// &&
//				//������ �ð� > a->GetComponent<PlayerComponent>().mLootingTime)
//			{
//				return true; // State::IDLE;
//			}
//			return false; // State::LOOT;
//		};
//
//	// // -------------------> DEAD
//	mFuncMap["To_Dead"] = [](std::shared_ptr<Entity> _entity)->bool
//		{
//			if (_entity->GetComponent<PlayerComponent>().mHP <= 0)
//			{
//				// �浹 ���� off
//				_entity->RemoveComponent<CapsuleCollider>();/// �̰� �´��� �𸣰ڳ�... �׳� �ٴ� �Ʒ��� ���������� �� ����
//				return true; // newStateEnum = State::DEAD;
//			}
//			else
//			{
//				return false; //newStateEnum = State::IDLE;
//			}
//		};
//
//	// ���� ������ �浹
//	mFuncMap["Conflict_Money"] = [](std::shared_ptr<Entity> _entity)->bool
//		{
//			float distance = (_entity->GetComponent<Transform>().m_localPosition
//				- _entity->GetComponent<PlayerComponent>().mpTarget->GetComponent<Transform>().m_localPosition).Length();
//
//
//			if (distance <= 0.1f)
//			{
//				// �浹 ���� ON
//				return true;
//			}
//			else
//			{
//				return false;
//			}
//		};
//
//	// ���̰�, ��Ÿ� ���� �����ϴ°�?
//	mFuncMap["Enemy_In_Range"] = [](std::shared_ptr<Entity> _entity)->bool
//		{
//			float distance = (_entity->GetComponent<Transform>().m_localPosition
//				- _entity->GetComponent<PlayerComponent>().mpTarget->GetComponent<Transform>().m_localPosition).Length();
//
//
//			if (_entity->GetComponent<PlayerComponent>().mIsAlly != _entity->GetComponent<PlayerComponent>().mpTarget->GetComponent<PlayerComponent>().mIsAlly
//				&& _entity->GetComponent<PlayerComponent>().mAttackRange > distance)
//			{
//				return true;
//			}
//			else
//			{
//				return false;
//			}
//		};
//}
//
//IState* BossFSM::GetState(State _state)
//{
//	auto it = mStatePool.find(_state);
//	if (it != mStatePool.end())
//	{
//		return it->second;
//	}
//	return nullptr;
//}
//
//void BossFSM::ChangeState(std::shared_ptr<Entity> _entity, State _newStateEnum)
//{
//	if (mpTransition->IsCanTransitionTo(_newStateEnum))
//	{
//		if (currentSM != nullptr)
//		{
//			currentSM->Exit(_entity);
//		}
//
//		currentSM = GetState(_newStateEnum);
//		//currentStateEnum = _newStateEnum;
//		_entity->GetComponent<PlayerComponent>().mOwnState = _newStateEnum;
//
//		if (currentSM != nullptr)
//		{
//			currentSM->Enter(_entity);
//		}
//	}
//	else
//	{
//		std::cout << "Transition from " << static_cast<int>(_entity->GetComponent<PlayerComponent>().mOwnState)
//			<< " to " << static_cast<int>(_newStateEnum) << " is not allowed.\n";
//	}
//}
//
//void BossFSM::Update(float _dTime, std::shared_ptr<Entity> _entity)
//{
//	///TODO_Riun : BOSS �߰��� �� + PowerPoint ����
//	if (currentSM == nullptr)
//	{
//		_entity->GetComponent<PlayerComponent>().mOwnState = _entity->GetComponent<PlayerComponent>().mOwnState;
//	}
//	else
//	{
//		currentSM->Update(_dTime, _entity);
//
//		State newStateEnum = _entity->GetComponent<PlayerComponent>().mOwnState;
//
//
//		// -------------------> DEAD
//		if (mFuncMap["To_Dead"](_entity))
//		{
//			newStateEnum = State::DEAD;
//		}
//		///Dead�� �ƴ϶��
//		else if (!mFuncMap.empty())
//		{
//			auto& tempTarget = _entity->GetComponent<PlayerComponent>().mpTarget;
//
//			if (!tempTarget)
//			{
//				newStateEnum = State::IDLE;
//			}
//			else
//			{
//				// ��Ÿ� ���
//				float distance = (_entity->GetComponent<Transform>().m_localPosition
//					- tempTarget->GetComponent<Transform>().m_localPosition).Length();
//
//				switch (newStateEnum)
//				{
//				case State::IDLE:
//				{
//					if (mFuncMap["Conflict_Money"](_entity))
//					{
//						newStateEnum = State::LOOT;
//					}
//					else
//					{
//						if (mFuncMap["Enemy_In_Range"](_entity))
//						{
//							if (_entity->GetComponent<PlayerComponent>().mAttackCoolTime <= 0)
//							{
//								newStateEnum = State::ATTACK;
//							}
//							else
//							{
//								newStateEnum = State::IDLE;
//							}
//						}
//						else
//						{
//							newStateEnum = State::MOVE;
//						}
//					}
//				}
//				break;
//				case State::MOVE:
//				{
//					if (mFuncMap["Conflict_Money"](_entity))
//					{
//						newStateEnum = State::LOOT;
//					}
//					else
//					{
//						if (mFuncMap["Enemy_In_Range"](_entity))
//						{
//							newStateEnum = State::IDLE;
//						}
//						else
//						{
//							newStateEnum = State::MOVE;
//						}
//					}
//				}
//				break;
//				case State::ATTACK:
//				{
//					if (_entity->GetComponent<PlayerComponent>().mAttackCoolTime < 1.3f) /// [��Riun : ���� ������! ��ĥ ��!]
//					{
//						newStateEnum = State::IDLE;
//					}
//					else
//					{
//						newStateEnum = State::ATTACK;
//					}
//				}
//				break;
//				case State::LOOT:
//				{
//					if (_entity->GetComponent<PlayerComponent>().mIsMoney = true &&
//						_entity->GetComponent<PlayerComponent>().mRecogRange > distance)  /// [��Riun : ���� ������! ��ĥ ��!] enter ���۵� ���������� �ð� 
//					{
//						newStateEnum = State::LOOT;
//					}
//					else
//					{
//						newStateEnum = State::IDLE;
//					}
//				}
//				break;
//				default:
//					break;
//				}
//			}
//		}
//
//		if (mpTransition->IsTransit(_entity->GetComponent<PlayerComponent>().mOwnState, newStateEnum))
//		{
//			ChangeState(_entity, newStateEnum);
//		}
//	}
//}
