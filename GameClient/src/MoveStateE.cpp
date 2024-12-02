#include "MoveStateE.h"
#include "Transform.h"
#include "EnemyComponent.h"
#include "Rigidbody.h"
#include "MoneyComponent.h"
#include "ProjectileComponent.h"

void MoveStateE::Initialize()
{
}

void MoveStateE::Enter(std::shared_ptr<Entity> _entity)
{
	/// 0. ���� ���� ����
	auto& enemyComp = _entity->GetComponent<EnemyComponent>();
	auto& animationComp = _entity->GetComponent<AnimationComponent>();

	/// 2. �ִϸ��̼� ����
	if (enemyComp.mpTarget)
	{
		auto& tempTarget = enemyComp.mpTarget;
		if (tempTarget->HasComponent<MoneyComponent>())
		{
			animationComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@MutantMove.fbx");
		}
		else
		{
			animationComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@Move.fbx");
		}
	}
	// �ִϸ��̼� ���ǵ� ����
	animationComp.mPlaySpeed = enemyComp.mMoveSpeed / mAnimationRation;

	/// ������
	//std::cout << "Enter Move State" << std::endl;
}

void MoveStateE::Update(float _dTime, std::shared_ptr<Entity> _entity)
{

}

void MoveStateE::Exit(std::shared_ptr<Entity> _entity)
{
	/// 0. ���� ���� ����
	auto& enemyComp = _entity->GetComponent<EnemyComponent>();
	auto& trsComp = _entity->GetComponent<Transform>();

	/// ���ϸ��̼� ���ǵ� ������ ����
	_entity->GetComponent<AnimationComponent>().mPlaySpeed = 1.0f;

	if (enemyComp.mpPath != nullptr)
	{
		enemyComp.mpPath->clear(); // ���� �ִ� ��� ����� : ���Ŀ� exit���� ��ƼƼ�� ������ �ű⿡ �ְ� �� ��..
	}
	enemyComp.mPathIndex = 0;

	auto& tempTarget = enemyComp.mpTarget;
	if (tempTarget == nullptr)
	{
		return;
	}
	if (tempTarget->HasComponent<Transform>())
	{
		// ĳ���Ͱ� ���� �ִ� Ÿ���� ������ �� �� �� ����
		mpFSMHelper->LookAtTarget(trsComp.m_localPosition, tempTarget->GetComponent<Transform>().m_localPosition, enemyComp.tempDirection, trsComp.m_localRotation.y);
	}

	/// ������
	//std::cout << "Exit Move State" << std::endl;
}

void MoveStateE::FixedUpdate(float _fixedTime, std::shared_ptr<Entity> _entity)
{
	/// 0. ���� ���� ����
	auto& enemyComp = _entity->GetComponent<EnemyComponent>();
	auto& animationComp = _entity->GetComponent<AnimationComponent>();
	auto& trsComp = _entity->GetComponent<Transform>();

	/// Ÿ���� ���ٸ� ����
	if (!_entity || enemyComp.mpTarget == nullptr)
	{
		return;
	}

	auto& tempTarget = enemyComp.mpTarget;
	if (tempTarget->HasComponent<Transform>())
	{
		auto& previousTarget = enemyComp.mPreviousTarget;				// ���� Ÿ�� : ���� ������ Ÿ�ٰ� �������� Ȯ���Ͽ�, �ٸ��� ��θ� �����ϱ� ���� ������
		auto& previousTargetPos = enemyComp.mPreviousTargetPos;			// ���� Ÿ���� ��ġ
		auto& tempTarget = enemyComp.mpTarget;							// ���� Ÿ�� : ���� ������ Ÿ��
		auto& position = trsComp.m_localPosition;						// ������ ��ġ
		auto& speed = enemyComp.mMoveSpeed;								// ������ �ӵ�
		auto& direction = enemyComp.tempDirection;						// ������ ����
		auto& pathdirection = enemyComp.pathDirection;		 // ������ ����(��� ����)
		size_t& currentPathIndex = enemyComp.mPathIndex;					// path �ε���
		Vector2& currentPos = enemyComp.mAstarPos;						// xz����� ��ġ
		Vector3& targetPos = tempTarget->GetComponent<Transform>().m_localPosition;
		Vector2 goalXZPos = Vector2();

		/// ������ ��ǥ�� Ÿ�� ���� �����ϴ� �������� �����Ѵ�. (Astar������ Ÿ�� �������� ����� �ϱ� �����̴�.)
		mpFSMHelper->AdjustToTileCenter(position, currentPos);
		mpFSMHelper->AdjustToTileCenter(targetPos, goalXZPos);

		/// ���� Ÿ���� �������� �ʴ´ٸ�, ���� Ÿ��� ���� ��ġ�� ���� ������ �־��ش�.
		if (!previousTarget)
		{
			previousTarget = tempTarget;
			previousTargetPos = targetPos;
		}

		/// Ÿ���� ����Ǹ� ����
		if ((tempTarget != previousTarget))
		{
			previousTarget = tempTarget;

			// ���� �ִ� ��� �����
			if (enemyComp.mpPath != nullptr)
			{
				enemyComp.mpPath->clear();
			}

			// ��� ����
			mpFSMHelper->CalculatePathforEnemy(enemyComp.mpPath, currentPos, goalXZPos);
			currentPathIndex = 0;

			// ĳ���Ͱ� �ٶ󺸰� �ִ� ������ ������Ʈ
			mpFSMHelper->LookAtTarget(position, targetPos, direction, trsComp.m_localRotation.y);

			// Ÿ���� �޶����� ���� ���¿����� �ִϸ��̼��� �ٲ��� �Ѵ�. Ÿ���� �ٲ�� Ư�� ��Ȳ������ �ִϸ��̼��� ����ȴ�.
			if (tempTarget->HasComponent<EnemyComponent>())
			{
				animationComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@Move.fbx");
			}
			else
			{
				animationComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@MutantMove.fbx");
			}
		}
		else
		{
			// Ÿ���� ������� �ʾ��� ��� ��, ��ġ�� ����Ǹ� ����(���� Ÿ���� ��ġ�� ��� 1�� �̻����� ����Ǹ� ����)
			if ((targetPos - previousTargetPos).Length() > mThreshold)
			{
				previousTargetPos = targetPos;
				// ��� ����
				mpFSMHelper->CalculatePathforEnemy(enemyComp.mpPath, currentPos, goalXZPos);
				currentPathIndex = 0;

				// ĳ���Ͱ� �ٶ󺸰� �ִ� ������ ������Ʈ
				mpFSMHelper->LookAtPathDir2(position, targetPos, enemyComp.mpPath, currentPathIndex, pathdirection, trsComp.m_localRotation.y);
				return;
			}
		}


		/// path�� ����� 0���� �۰ų� �ε����� �� Ŭ �� �����Ƿ� ��θ� �����Ѵ�. (���� Ÿ���� ���� ��� size�� 0�̶� �̶� ����Ѵ�.)
		if (enemyComp.mpPath == nullptr || enemyComp.mpPath->size() <= 0 || currentPathIndex > enemyComp.mpPath->size())
		{
			// ��� ����
			mpFSMHelper->CalculatePathforEnemy(enemyComp.mpPath, currentPos, goalXZPos);
			currentPathIndex = 0;

			// ĳ���Ͱ� �ٶ󺸰� �ִ� ������ ������Ʈ
			mpFSMHelper->LookAtPathDir2(position, targetPos, enemyComp.mpPath, currentPathIndex, pathdirection, trsComp.m_localRotation.y);
		}
		else
		{
			if (currentPos == (*enemyComp.mpPath)[currentPathIndex])
			{
				// ��� �ڵ� // Ȥ�ö� �Ʒ��� currentPathIndex+1�� ����� �Ѿ�� �� �Ǳ� ������ �õ��Ѵ�.
				if (enemyComp.mpPath->size() == currentPathIndex + 1)
				{
					mpFSMHelper->CalculatePathforEnemy(enemyComp.mpPath, currentPos, goalXZPos);
					currentPathIndex = 0;
					return;
				}
				currentPathIndex++;
				mpFSMHelper->LookAtPathDir2(position, targetPos, enemyComp.mpPath, currentPathIndex, pathdirection, trsComp.m_localRotation.y);
			}
			pathdirection.Normalize();
		}

		// �̵�
		position += pathdirection * speed * testSpeedCoef;
		if (position.y > 0.5f)
		{
			position.y = 0.0f;
		}
		mpPhysicsManager->UpdatePosition(_entity, position);

		/// �����浹�� ���� ��ġ ���ϰ� ��θ� ��Ż���� ��츦 ���� ��θ� �����Ѵ�.
		// �� �� �ϳ��� ä���Ϸ��� �ϰ� �ִ�. �Ʒ��� ���� �� ȿ������ ������ ������ ������ ���� ���� �ִٰ� �Ǵ� ���̴�.
		Vector2 xzUnitPos = Vector2(position.x, position.z);
		if (enemyComp.mpPath != nullptr && enemyComp.mpPath->size() > currentPathIndex)
		{
			if (abs(currentPos.x - (*enemyComp.mpPath)[currentPathIndex].x) > 0.5f || abs(xzUnitPos.y - (*enemyComp.mpPath)[currentPathIndex].y) > 0.5f)
			{
				// ��� ����
				mpFSMHelper->CalculatePathforEnemy(enemyComp.mpPath, currentPos, goalXZPos);
				currentPathIndex = 0;
				mpFSMHelper->LookAtPathDir2(position, targetPos, enemyComp.mpPath, currentPathIndex, pathdirection, trsComp.m_localRotation.y);
				return;
			}
		}
	}
	else
	{
		tempTarget = nullptr;
	}
}
