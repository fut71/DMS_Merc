#include "Transition.h"

Transition::Transition()
{
}

bool Transition::IsCanTransitionTo(State _newStateEnum)
{
	/// �츮 ������ �׳� Dead�� ���� �ٸ� ���·� ��ȯ �Ұ��� ���Ǹ� ������
	if (_newStateEnum == State::DEAD)
	{
		return false;  // Dead ���¿��� �ٸ� ���·� ��ȯ �Ұ�
	}

	// �ٸ� ���� ��ȯ�� ���� ���� �߰��� ��
	return true;
}

bool Transition::IsTransit(State& _currentStateEnum, State& _newStateEnum)
{
	// ���� �޾ƿ� ���°� �ٸ��� true�� ��ȯ�Ѵ�.
	return _currentStateEnum != _newStateEnum;
}
