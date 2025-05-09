#pragma once
#include "Component.h"
#include "StateEnum.h"
#include "ClassTypeEnum.h"
#include "IState.h"

/// <summary>
/// 적군도 아군과 같은 플레이어 컴포넌트를 가지고 있던걸 분리하고 정돈
/// 2024.10.07
/// </summary>
struct EnemyComponent
	: public Component
{
public:
	EnemyComponent(std::shared_ptr<Entity> _owner, ClassType _classType = ClassType::LAST)
		:Component(_owner), mClassType(_classType), mpTarget(nullptr), mPreviousTarget(nullptr), mPreviousTargetPos(Vector3())
		, mbuffLv(0), mHP(0.0f), mMaxHP(0.0f), mAttackPower(0.0f), mAttackSpeed(0.0f)
		, mMoveSpeed(0.0f), mAttackRange(0.0f), mRecogRange(0.0f),mMoney(0), mBuffMoney(0)
		, mIsDead(false), mIsAttacked(false), mIsShot(false)
		, mpPath(nullptr), mPathIndex(0), mAstarPos(Vector2()), tempDirection(Vector3())
		, mOwnState(State::IDLE), mCurrentSM(nullptr)
	{
	}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<EnemyComponent>(*this);
	}

public:
	ClassType mClassType;						// 클래스
	std::shared_ptr<Entity> mpTarget;			// 목표
	std::shared_ptr<Entity> mPreviousTarget;	// 이전 목표 : 이동 타겟이 변경됐는지 확인 여부
	Vector3 mPreviousTargetPos;					// 같은 목표 : 이동 위치가 변경됐는지 확인 여부

	// 해당 변수는 기획 문서 참고 : 최초 초기화는 statusManager에서 일괄 처리
	int mbuffLv;				// 플레이어마다 다른 값
	float mHP;					// 플레이어마다 다른 값
	float mMaxHP;				// ◆
	float mAttackPower;			// ◆
	float mAttackSpeed;			// ◆
	float mMoveSpeed;			// ◆
	float mAttackRange;			// ◆
	float mRecogRange;			// ◆ 돈 인식 범위
	int mMoney;					// 플레이어의 소지금
	int mBuffMoney;				// ◆ 다음 버프 상태로 가기 위한 돈

	bool mIsDead; // 유닛이 죽었는지 안 죽었는지 판별하기 위한 변수
	bool mIsAttacked; // 무기 콜라이더와 충돌했는가?
	bool mIsShot; // 원거리용 -> 무기를 쏘고 나서 간격용

	// 모든 객체는 본인만의 길과 그 인덱스를 가져야 한다.
	std::vector<Vector2>* mpPath;
	size_t mPathIndex;
	Vector2 mAstarPos; // pathIndex는 경로가 바뀌면 불안정해서 값도 기억해두는 것이 벡터가 터지지 않는 예방차원에서 좋다.
	Vector3 tempDirection; // 물리가 transform의 rotation으로 밀접하게 작동을 하여 rotation을 개별로 갖게 하였다.
	Vector3 pathDirection; // 물리가 transform의 rotation으로 밀접하게 작동을 하여 rotation을 개별로 갖게 하였다. (움직이는 방향)

	State mOwnState; // 스스로의 상태를 알고 있어야 한다.
	IState* mCurrentSM; // FSM에서의 현재 상태
};

