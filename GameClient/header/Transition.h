#pragma once
#include <functional>
#include "StateEnum.h"
/// FSM�� ������ȯ�� ��ȯ ���� ���θ� Ȯ���ϴ� Ŭ����
class Transition
{
public:
	Transition();
	~Transition() {};

	// ��ȯ ���� ���� : ��ȯ�� �Ͼ�� �������� �ִ� <- ���� ���� �ܹ���
	bool IsCanTransitionTo(State _newStateEnum);
	// ��ȯ �߻� ���� : ��ȯ�� ������ �Ͼ�� ���� �����Ѵ�.
	/// �� ���� �ſ�ſ� ������ �Լ��� �� �����̴�. ��� �ؾ� ���� ����ϰ� �������� �ɱ�.
	//bool IsTransitionNeeded(State& _currentStateEnum, State& _newStateEnum);
	bool IsTransit(State& _currentStateEnum, State& _newStateEnum);
};

