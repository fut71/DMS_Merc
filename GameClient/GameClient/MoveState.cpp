#pragma once
#include "MoveState.h"
#include "Transform.h"
#include "PlayerComponent.h"
#include "Rigidbody.h"
#include "MoneyComponent.h"
#include "ProjectileComponent.h"

void MoveState::Initialize()
{
}

void MoveState::Enter(std::shared_ptr<Entity> _entity)
{
	/// 0. ���� ���� ����
	auto& playerComp = _entity->GetComponent<PlayerComponent>();
	auto& animationComp = _entity->GetComponent<AnimationComponent>();

	/// 2. �ִϸ��̼� ����
	if (playerComp.mpTarget)
	{
		auto& tempTarget = playerComp.mpTarget;
		if (tempTarget->HasComponent<MoneyComponent>())
		{
			animationComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@MutantMove.fbx");
			animationComp.mPlaySpeed = (playerComp.mBuffSpeed) / mAnimationRation;// �ִϸ��̼� ���ǵ� ����
		}
		else
		{
			animationComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@Move.fbx");
			animationComp.mPlaySpeed = playerComp.mMoveOriSpeed / mAnimationRation;// �ִϸ��̼� ���ǵ� ����
		}
	}

	/// ������
	//std::cout << "Enter Move State" << std::endl;
}

/// Ÿ���� ������ ������ ���� ȣ���
void MoveState::Update(float _dTime, std::shared_ptr<Entity> _entity)
{

}

void MoveState::Exit(std::shared_ptr<Entity> _entity)
{
	/// 0. ���� ���� ����
	auto& playerComp = _entity->GetComponent<PlayerComponent>();
	auto& trsComp = _entity->GetComponent<Transform>();

	/// ���ϸ��̼� ���ǵ� ������ ����
	playerComp.mMoveSpeed = playerComp.mMoveOriSpeed;
	_entity->GetComponent<AnimationComponent>().mPlaySpeed = 1.0f;

	if (playerComp.mpPath != nullptr)
	{
		playerComp.mpPath->clear(); // ���� �ִ� ��� ����� : ���Ŀ� exit���� ��ƼƼ�� ������ �ű⿡ �ְ� �� ��..
	}
	playerComp.mPathIndex = 0;

	auto& tempTarget = playerComp.mpTarget;
	if (tempTarget == nullptr)
	{
		return;
	}
	if (tempTarget->HasComponent<Transform>())
	{
		// ĳ���Ͱ� ���� �ִ� Ÿ���� ������ �� �� �� ����
		mpFSMHelper->LookAtTarget(trsComp.m_localPosition, tempTarget->GetComponent<Transform>().m_localPosition, playerComp.tempDirection, trsComp.m_localRotation.y);
	}

	/// ������
	//std::cout << "Exit Move State" << std::endl;
}

void MoveState::FixedUpdate(float _fixedTime, std::shared_ptr<Entity> _entity)
{
	/// 0. ���� ���� ����
	auto& playerComp = _entity->GetComponent<PlayerComponent>();
	auto& animationComp = _entity->GetComponent<AnimationComponent>();
	auto& trsComp = _entity->GetComponent<Transform>();

	/// Ÿ���� ���ٸ� ����
	if (!_entity || playerComp.mpTarget == nullptr)
	{
		return;
	}

	auto& tempTarget = playerComp.mpTarget;
	if (tempTarget->HasComponent<Transform>())
	{
		auto& previousTarget = playerComp.mPreviousTarget; // ���� Ÿ�� : ���� ������ Ÿ�ٰ� �������� Ȯ���Ͽ�, �ٸ��� ��θ� �����ϱ� ���� ������
		auto& previousTargetPos = playerComp.mPreviousTargetPos;			 // ���� Ÿ���� ��ġ
		auto& tempTarget = playerComp.mpTarget;			 // ���� Ÿ�� : ���� ������ Ÿ��
		auto& position = trsComp.m_localPosition;			 // ������ ��ġ
		auto& speed = playerComp.mMoveSpeed;				 // ������ �ӵ�
		auto& direction = playerComp.tempDirection;		 // ������ ����
		auto& pathdirection = playerComp.pathDirection;		 // ������ ����(��� ����)
		size_t& currentPathIndex = playerComp.mPathIndex;	 // path �ε���
		Vector2& currentPos = playerComp.mAstarPos;	 // tempRawPath
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
			if (playerComp.mpPath != nullptr)
			{
				playerComp.mpPath->clear();
			}

			// ��� ����
			mpFSMHelper->CalculatePath(playerComp.mpPath, currentPos, goalXZPos);
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
				mpFSMHelper->CalculatePath(playerComp.mpPath, currentPos, goalXZPos);
				currentPathIndex = 0;

				// ĳ���Ͱ� �ٶ󺸰� �ִ� ������ ������Ʈ
				mpFSMHelper->LookAtPathDir(position, targetPos, playerComp.mpPath, currentPathIndex, pathdirection, trsComp.m_localRotation.y);
				return;
			}
		}

		/// path�� ����� 0���� �۰ų� �ε����� �� Ŭ �� �����Ƿ� ��θ� �����Ѵ�. (���� Ÿ���� ���� ��� size�� 0�̶� �̶� ����Ѵ�.)
		if (playerComp.mpPath == nullptr || playerComp.mpPath->size() <= 0 || currentPathIndex >= playerComp.mpPath->size())
		{
			// ��� ����
			mpFSMHelper->CalculatePath(playerComp.mpPath, currentPos, goalXZPos);
			currentPathIndex = 0;

			// ĳ���Ͱ� �ٶ󺸰� �ִ� ������ ������Ʈ
			mpFSMHelper->LookAtPathDir(position, targetPos, playerComp.mpPath, currentPathIndex, pathdirection, trsComp.m_localRotation.y);
		}
		else
		{
			if (currentPos == (*playerComp.mpPath)[currentPathIndex])
			{
				// ��� �ڵ� // Ȥ�ö� �Ʒ��� currentPathIndex+1�� ����� �Ѿ�� �� �Ǳ� ������ �õ��Ѵ�.
				if (playerComp.mpPath->size() == currentPathIndex + 1)
				{
					mpFSMHelper->CalculatePath(playerComp.mpPath, currentPos, goalXZPos);
					currentPathIndex = 0;
					return;
				}

				currentPathIndex++;
				mpFSMHelper->LookAtPathDir(position, targetPos, playerComp.mpPath, currentPathIndex, pathdirection, trsComp.m_localRotation.y);
			}
			pathdirection.Normalize();
		}

		// �̵�
		if (tempTarget->HasComponent<MoneyComponent>())
		{
			speed = playerComp.mBuffSpeed;				 // ������ �ӵ� ����
		}
		else
		{
			speed = playerComp.mMoveOriSpeed;				 // ������ �ӵ� ����
		}
		position += pathdirection * speed * testSpeedCoef;
		if (position.y > 0.5f)
		{
			position.y = 0.0f;
		}
		mpPhysicsManager->UpdatePosition(_entity, position);

		/// �����浹�� ���� ��ġ ���ϰ� ��θ� ��Ż���� ��츦 ���� ��θ� �����Ѵ�.
		// �� �� �ϳ��� ä���Ϸ��� �ϰ� �ִ�. �Ʒ��� ���� �� ȿ������ ������ ������ ������ ���� ���� �ִٰ� �Ǵ� ���̴�.
		Vector2 xzUnitPos = Vector2(position.x, position.z);
		if (playerComp.mpPath != nullptr && playerComp.mpPath->size() > currentPathIndex)
		{
			if (abs(currentPos.x - (*playerComp.mpPath)[currentPathIndex].x) > 0.5f || abs(currentPos.y - (*playerComp.mpPath)[currentPathIndex].y) > 0.5f)
			{
				// ��� ����
				mpFSMHelper->CalculatePath(playerComp.mpPath, currentPos, goalXZPos);
				currentPathIndex = 0;
				mpFSMHelper->LookAtPathDir(position, targetPos, playerComp.mpPath, currentPathIndex, pathdirection, trsComp.m_localRotation.y);
				return;
			}
		}


		// ���� ��ġ���� ���� ����� ��� ��� ã��
		/*for (int i = 0; i < mPath.size(); i++)
		{
			if ((xzUnitPos - mPath[i]).Length() > 2.0f)
			{
				currentPathIndex = i;
				break;
			}
		}*/
	}
	else
	{
		tempTarget = nullptr;
	}
}
