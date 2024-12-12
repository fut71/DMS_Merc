//#pragma once
//#include <any>
//#include <optional>
//#include <functional>
//#include <unordered_map>
//#include "StateEnum.h"
//#include "IState.h"
//#include "IdleState.h"
//#include "MoveState.h"
//#include "AttackState.h"
//#include "DeadState.h"
//#include "Transition.h"
//
///// 2024.8.8�� FSM ������ �ӽ÷� �Űܵ�.
//// FSM�� 2���� ������ ����
//// 1. ���� FSM�� �� -> State�� ��ƼƼ ���� ������ �����, �޸� ����ȭ but ������ �ۿ� �����
//// 2. ���� FSM�� �� -> �Ϲ����� ���� : ������������ ����� FSM�� ���� ����.
//class BossFSM
//{
//public:
//	BossFSM(ClientAnimator* _animator);
//	~BossFSM();
//
//public:
//	IState* currentSM; // ���� ���� �ӽ� -> FSM���� ���� � �������� ����
//	std::unordered_map<std::string, std::function<bool(std::shared_ptr<Entity> _temp)>> mFuncMap; // �Լ��� ��� �׸��̴�. �̰��� ���������� �ƿ� �ٱ����� ������ ������ ���ɼ��� �ִ�.
//
//private:
//	Transition* mpTransition; // transition�� ������ ����ġ ������ �Ѵ�.
//
//public: //���� ������
//	std::unordered_map<State, IState*> mStatePool; // ������ state�� �����ϴ� pool�̴�.
//
//private:
//	void Initialize(ClientAnimator* _animator); // state���� ���� - fsm�� ����ִ� ������ ��������
//	void MakeFunction(); // �⺻ fsm �Լ��� �����Ѵ�. �߰����� �Լ��� �˾Ƽ� �׶� Ŭ���̾�Ʈ �ʿ��� �߰�����.
//	IState* GetState(State _state); // ������ state���� StatePool���� ã�Ƽ� ������
//	void ChangeState(std::shared_ptr<Entity> _entity, State _newStateEnum); // state�� ���� �������� Ȯ���Ѵ�.
//
//public:
//	void Update(float _dTime, std::shared_ptr<Entity> _entity); // ������Ʈ [��ƼƼ]
//};
//
