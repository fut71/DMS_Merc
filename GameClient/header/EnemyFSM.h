#pragma once
#include "StateEnum.h"
#include "IState.h"
#include "IdleStateE.h"
#include "MoveStateE.h"
#include "AttackStateE.h"
#include "LootStateE.h"
#include "DamagedStateE.h"
#include "DeadStateE.h"
#include "Transition.h"
#include "MoneyComponent.h"
#include "FSMHelper.h"

/// ĳ������ ���� ���� �ӽ�
// FSM�� 2���� ������ ����
// 1. ���� FSM�� �� -> State�� ��ƼƼ ���� ������ �����, �޸� ����ȭ but ������ �ۿ� �����
// 2. ���� FSM�� �� -> �Ϲ����� ���� : ������������ ����� FSM�� ���� ����.
class EnemyFSM
{
public:
	EnemyFSM(EntityManager* _pEntityManager, PhysicsManager* _pPhysicsManager
		, ResourceManager* _pResourceManager, EventManager* _pEventManager
		, SoundManager* _pSoundManager, RenderManager* _pRenderManager, FSMHelper* _pFSMHelper);
	~EnemyFSM();

public:
	void Initialize(); // state���� ���� - fsm�� ����ִ� ������ ��������
	void Update(float _dTime, std::shared_ptr<Entity> _entity); // ������Ʈ [��ƼƼ]
	IState* GetState(State _state); // ������ state���� StatePool���� ã�Ƽ� ������

	void FixedUpdate(float _fixedTime, std::shared_ptr<Entity> _entity);

private:
	void ChangeState(std::shared_ptr<Entity> _entity, State _newStateEnum); // state�� ���� �������� Ȯ���Ѵ�.
	Transition* mpTransition; // transition�� ������ ����ġ ������ �Ѵ�.

	IState* mpIdleState;
	IState* mpMoveState;
	IState* mpAttackState;
	IState* mpLootState;
	IState* mpDamagedState;
	IState* mpDeadState;

	EntityManager* mpEntityManager;
	PhysicsManager* mpPhysicsManager;
	ResourceManager* mpResourceManager;
	EventManager* mpEventManager;
	SoundManager* mpSoundManager;
	RenderManager* mpRenderManager;
	FSMHelper* mpFSMHelper;

	std::unordered_map<std::string, std::function<bool(std::shared_ptr<Entity> _temp)>> mpFuncMap; // �ſ� ���� �Ἥ get �Լ��� �Ź� �������� ���� �� �Ҹ��ϹǷ� �ʹݿ� �����ͷ� �����ؼ� ����Ѵ�.
};

