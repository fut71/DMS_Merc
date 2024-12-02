#pragma once
#include "Component.h"
#include "StateEnum.h"
#include "ClassTypeEnum.h"
#include "IState.h"

/// <summary>
/// ������ �Ʊ��� ���� �÷��̾� ������Ʈ�� ������ �ִ��� �и��ϰ� ����
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
	ClassType mClassType;						// Ŭ����
	std::shared_ptr<Entity> mpTarget;			// ��ǥ
	std::shared_ptr<Entity> mPreviousTarget;	// ���� ��ǥ : �̵� Ÿ���� ����ƴ��� Ȯ�� ����
	Vector3 mPreviousTargetPos;					// ���� ��ǥ : �̵� ��ġ�� ����ƴ��� Ȯ�� ����

	// �ش� ������ ��ȹ ���� ���� : ���� �ʱ�ȭ�� statusManager���� �ϰ� ó��
	int mbuffLv;				// �÷��̾�� �ٸ� ��
	float mHP;					// �÷��̾�� �ٸ� ��
	float mMaxHP;				// ��
	float mAttackPower;			// ��
	float mAttackSpeed;			// ��
	float mMoveSpeed;			// ��
	float mAttackRange;			// ��
	float mRecogRange;			// �� �� �ν� ����
	int mMoney;					// �÷��̾��� ������
	int mBuffMoney;				// �� ���� ���� ���·� ���� ���� ��

	bool mIsDead; // ������ �׾����� �� �׾����� �Ǻ��ϱ� ���� ����
	bool mIsAttacked; // ���� �ݶ��̴��� �浹�ߴ°�?
	bool mIsShot; // ���Ÿ��� -> ���⸦ ��� ���� ���ݿ�

	// ��� ��ü�� ���θ��� ��� �� �ε����� ������ �Ѵ�.
	std::vector<Vector2>* mpPath;
	size_t mPathIndex;
	Vector2 mAstarPos; // pathIndex�� ��ΰ� �ٲ�� �Ҿ����ؼ� ���� ����صδ� ���� ���Ͱ� ������ �ʴ� ������������ ����.
	Vector3 tempDirection; // ������ transform�� rotation���� �����ϰ� �۵��� �Ͽ� rotation�� ������ ���� �Ͽ���.
	Vector3 pathDirection; // ������ transform�� rotation���� �����ϰ� �۵��� �Ͽ� rotation�� ������ ���� �Ͽ���. (�����̴� ����)

	State mOwnState; // �������� ���¸� �˰� �־�� �Ѵ�.
	IState* mCurrentSM; // FSM������ ���� ����
};

