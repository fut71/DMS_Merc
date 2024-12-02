#include "CustomFunctions.h"

CustomFunctions::CustomFunctions()
{
	// -------------------> DEAD
	mFuncMap["To_Dead"] = [](std::shared_ptr<Entity> _entity)->bool
		{
			if (_entity->HasComponent<PlayerComponent>())
			{
				if (_entity->GetComponent<PlayerComponent>().mHP <= 0)
				{
					// �浹 ���� off
					_entity->RemoveComponent<CapsuleCollider>();
					return true; // newStateEnum = State::DEAD;
				}
			}
			return false; //newStateEnum = State::IDLE;
		};

	mFuncMap["To_DeadE"] = [](std::shared_ptr<Entity> _entity)->bool
		{
			if (_entity->HasComponent<EnemyComponent>())
			{
				if (_entity->GetComponent<EnemyComponent>().mHP <= 0)
				{
					// �浹 ���� off
					_entity->RemoveComponent<CapsuleCollider>();
					return true; // newStateEnum = State::DEAD;
				}
			}
			return false; //newStateEnum = State::IDLE;
		};

	// -------------------> DAMAGED
	mFuncMap["To_Damaged"] = [](std::shared_ptr<Entity> _entity)->bool
		{
			if (_entity->HasComponent<PlayerComponent>())
			{
				if (_entity->GetComponent<PlayerComponent>().mIsAttacked == true)
				{
					return true;
				}
			}
			return false;
		};

	mFuncMap["To_DamagedE"] = [](std::shared_ptr<Entity> _entity)->bool
		{
			if (_entity->HasComponent<EnemyComponent>())
			{
				if (_entity->GetComponent<EnemyComponent>().mIsAttacked == true)
				{
					return true;
				}
			}
			return false;
		};

	// -------------------> Ÿ�� == ��	
	mFuncMap["Target_Is_Money"] = [](std::shared_ptr<Entity> _entity)->bool
		{
			if (_entity->HasComponent<PlayerComponent>())
			{
				return _entity->GetComponent<PlayerComponent>().mpTarget->HasComponent<MoneyComponent>();
			}
		};

	mFuncMap["Target_Is_MoneyE"] = [](std::shared_ptr<Entity> _entity)->bool
		{
			if (_entity->HasComponent<EnemyComponent>())
			{
				return _entity->GetComponent<EnemyComponent>().mpTarget->HasComponent<MoneyComponent>();
			}
		};

	// ------------------->  Ÿ�� == nullptr	
	mFuncMap["None_Target"] = [](std::shared_ptr<Entity> _entity)->bool
		{
			if (_entity->HasComponent<PlayerComponent>())
			{
				return _entity->GetComponent<PlayerComponent>().mpTarget == nullptr;
			}
			return false;
		};

	mFuncMap["None_TargetE"] = [](std::shared_ptr<Entity> _entity)->bool
		{
			if (_entity->HasComponent<EnemyComponent>())
			{
				return _entity->GetComponent<EnemyComponent>().mpTarget == nullptr;
			}
			return false;
		};

	// ------------------->  Ÿ�� == ��(�̰� ���� �Ʊ� ���ֿ��Ը� �� �Լ��� �˴ϴ�.)
	mFuncMap["Target_Is_Enemy"] = [](std::shared_ptr<Entity> _entity)->bool
		{
			if (_entity->HasComponent<PlayerComponent>())
			{
				auto& playerComp = _entity->GetComponent<PlayerComponent>();
				// �ϴ� Ÿ���� ���̸� �� �� + ������ ���� �� �����ؼ� �̸� «
				if (!playerComp.mpTarget->HasComponent<MoneyComponent>())
				{
					if (playerComp.mpTarget->HasComponent<EnemyComponent>())
					{
						return true;
					}
					else
					{
						return false;
					}
				}
			}
			return false;
		};

	mFuncMap["Target_Is_Player"] = [](std::shared_ptr<Entity> _entity)->bool
		{
			if (_entity->HasComponent<EnemyComponent>())
			{
				auto& enemyComp = _entity->GetComponent<EnemyComponent>();
				// �ϴ� Ÿ���� ���̸� �� �� + ������ ���� �� �����ؼ� �̸� «
				if (!enemyComp.mpTarget->HasComponent<MoneyComponent>())
				{
					if (enemyComp.mpTarget->HasComponent<PlayerComponent>())
					{
						return true;
					}
					else
					{
						return false;
					}
				}
			}
			return false;
		};

	/// �� �κ��� ���Ŀ� ���� ��ġ�� �����Ǹ� �����ؾ� �Ѵ�.
	// ���� ������ �浹 (Ÿ�� == ��(����), ���� �浹?)
	mFuncMap["Conflict_Money"] = [this](std::shared_ptr<Entity> _entity)->bool
		{
			if (_entity->HasComponent<PlayerComponent>())
			{
				auto& playerComp = _entity->GetComponent<PlayerComponent>();

				if (playerComp.mpTarget->HasComponent<MoneyComponent>())									   //Ÿ���� ���� ������ �ִ°�?
				{
					auto& a1 = _entity->GetComponent<Transform>().m_localPosition;
					//auto& a2 = playerComp.mpTarget->GetComponent<ProjectileComponent>().m_targetPosition;
					auto& a2 = playerComp.mpTarget->GetComponent<Transform>().m_localPosition;

					//float halfTileScale = mTileScale * 0.5f;
					//Vector2 currentPos = Vector2(static_cast<int>((a1.x + halfTileScale) / mTileScale), static_cast<int>((a1.z + halfTileScale) / mTileScale));
					//Vector2 targetPos = Vector2(static_cast<int>((a2.x + halfTileScale) / mTileScale), static_cast<int>((a2.z + halfTileScale) / mTileScale));

					float distance = pow(a1.x - a2.x, 2) + pow(a1.z - a2.z, 2);

					// ���̽�Ÿ ������ ���̱� ���Ͽ�, distance ������ �߰��ϵ��� �Ѵ�. (�밢�� �Ÿ� 1.414�� ����  ���)
					if (playerComp.mpTarget->GetComponent<ProjectileComponent>().m_isTriggered || 1.0f > distance)
					{
						return true;
					}
				}
			}
			return false;
		};

	mFuncMap["Conflict_MoneyE"] = [this](std::shared_ptr<Entity> _entity)->bool
		{
			if (_entity->HasComponent<EnemyComponent>())
			{
				auto& playerComp = _entity->GetComponent<EnemyComponent>();

				if (playerComp.mpTarget->HasComponent<MoneyComponent>())									   //Ÿ���� ���� ������ �ִ°�?
				{
					auto& a1 = _entity->GetComponent<Transform>().m_localPosition;
					//auto& a2 = playerComp.mpTarget->GetComponent<ProjectileComponent>().m_targetPosition;
					auto& a2 = playerComp.mpTarget->GetComponent<Transform>().m_localPosition;

					//float halfTileScale = mTileScale * 0.5f;
					//Vector2 currentPos = Vector2(static_cast<int>((a1.x + halfTileScale) / mTileScale), static_cast<int>((a1.z + halfTileScale) / mTileScale));
					//Vector2 targetPos = Vector2(static_cast<int>((a2.x + halfTileScale) / mTileScale), static_cast<int>((a2.z + halfTileScale) / mTileScale));

					float distance = pow(a1.x - a2.x, 2) + pow(a1.z - a2.z, 2);

					// ���̽�Ÿ ������ ���̱� ���Ͽ�, distance ������ �߰��ϵ��� �Ѵ�. (�밢�� �Ÿ� 1.414�� ����  ���)
					if (playerComp.mpTarget->GetComponent<ProjectileComponent>().m_isTriggered || 1.5f > distance)
					{
						return true;
					}
				}
			}
			return false;
		};

	// ��Ÿ� ���� �����ϴ°�? attackRange >= distance
	mFuncMap["Enemy_In_Range"] = [](std::shared_ptr<Entity> _entity)->bool
		{
			if (_entity->HasComponent<PlayerComponent>())
			{
				auto& playerComp = _entity->GetComponent<PlayerComponent>();
				if (playerComp.mpTarget->HasComponent<EnemyComponent>())
				{
					float distance = (_entity->GetComponent<Transform>().m_localPosition
						- playerComp.mpTarget->GetComponent<Transform>().m_localPosition).Length();

					if (playerComp.mAttackRange >= distance)
					{
						return true;
					}
					else
					{
						return false;
					}
				}
			}
			return false;
		};

	mFuncMap["Player_In_Range"] = [](std::shared_ptr<Entity> _entity)->bool
		{
			if (_entity->HasComponent<EnemyComponent>())
			{
				auto& enemyComp = _entity->GetComponent<EnemyComponent>();

				if (enemyComp.mpTarget->HasComponent<PlayerComponent>())
				{
					float distance = (_entity->GetComponent<Transform>().m_localPosition
						- enemyComp.mpTarget->GetComponent<Transform>().m_localPosition).Length();

					if (enemyComp.mAttackRange >= distance)
					{
						return true;
					}
					else
					{
						return false;
					}
				}
			}
			return false;
		};
}
