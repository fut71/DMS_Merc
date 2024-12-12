#pragma once
#include "IState.h"
#include "StateEnum.h"
#include "FSMHelper.h"
#include "AStar.h"

class MoveState : public IState
{
public:
	MoveState(EntityManager* _pEntityManager, PhysicsManager* _pPhysicsManager
		, ResourceManager* _pResourceManager, EventManager* _pEventManager
		, SoundManager* _pSoundManager, FSMHelper* _pFSMHelper)
		: IState(_pEntityManager, _pPhysicsManager, _pResourceManager, _pEventManager, _pSoundManager)
		, mpFSMHelper(_pFSMHelper) {};
	~MoveState() {};

public:
	void Initialize() override;
	void Enter(std::shared_ptr<Entity> _entity) override;
	void Update(float _dTime, std::shared_ptr<Entity> _entity) override;
	void Exit(std::shared_ptr<Entity> _entity) override;
	void FixedUpdate(float _fixedTime, std::shared_ptr<Entity> _entity) override;

private:
	FSMHelper* mpFSMHelper;

	float mAnimationRation = 0.3f; // ��ȹ���� ������ ������, ���� �⺻ ���� ��

	float mThreshold = 1.0f; // 1.0�̻� ��, �̰� ���� ���� �� �����ϴ�. 
	float mLengthThresHold = 0.06f; // Ÿ��� ��ġ�� �ʹ� ����� ���� ���� ���� ������ �����ϰ��� �ִ� ������ �ڵ��.

	// ȸ�� �ӵ�
	//float mRotationSpeed = 0.3f;

	float testSpeedCoef = 0.1f; // �׳� �ù��� �ӵ� �����ϰ� ���� �� ���� �����.
};

