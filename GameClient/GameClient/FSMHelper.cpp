#include "FSMHelper.h"
#include "Transform.h"

FSMHelper::FSMHelper(ResourceManager* _pResourceManager)
{
	mpResourceManager = _pResourceManager;
	mpConditionFunctions = nullptr;
	mpMoneyPosVec = nullptr;
	mDesiredOffset = 4;
	mRotationSpeed = 0.1f;
}

FSMHelper::~FSMHelper()
{
	delete mpConditionFunctions;
}

void FSMHelper::Initialize()
{
	mpConditionFunctions = new CustomFunctions();
	mpAstarforEnemy = new AStar();

}

void FSMHelper::SetMoneyVec(std::vector<std::pair<std::shared_ptr<Entity>, Vector3>>* _pMoneyPosVec)
{
	mpMoneyPosVec = _pMoneyPosVec;
}

std::vector<std::pair<std::shared_ptr<Entity>, Vector3>>* FSMHelper::GetMoneyVec()
{
	return mpMoneyPosVec;
}

void FSMHelper::SetAstar(AStar* _pAstar)
{
	mpAstar = _pAstar;
	*mpAstarforEnemy = *mpAstar;
}

void FSMHelper::CalculatePath(std::vector<Vector2>*& _path, Vector2& _startPoint, Vector2& _goalPoint)
{
	if (_path != nullptr)
	{
		delete _path;
		_path = nullptr;
	}

	_path = mpAstar->Astar(_startPoint, _goalPoint);
}

void FSMHelper::CalculatePathforEnemy(std::vector<Vector2>*& _path, Vector2& _startPoint, Vector2& _goalPoint)
{
	if (_path != nullptr)
	{
		delete _path;
		_path = nullptr;
	}

	_path = mpAstarforEnemy->Astar(_startPoint, _goalPoint);
}

void FSMHelper::AdjustToTileCenter(const Vector3& _realPos, Vector2& _xzPos)
{
	mpAstar->AdjustToTileCenter(_realPos, _xzPos);
}

std::vector<Vector2>* FSMHelper::GetAstarObstacles()
{
	return mpAstar->GetObstaclePos();
}

std::unordered_map<std::string, std::function<bool(std::shared_ptr<Entity> _temp)>> FSMHelper::GetFuctMap()
{
	return mpConditionFunctions->mFuncMap;
}
//�������̴���?
//Ÿ���� ����� ��� �ٶ󺸴� ������ �����ִ� �Լ�
void FSMHelper::LookAtTarget(const Vector3& _unitPos, const Vector3& _targetPos, Vector3& _direction, float& _realRotationY)
{
	/// 1. ���� ����(Ÿ���� �� �ٶ󺸰� �ֵ��� ����)
	_direction = Vector3(_targetPos.x - _unitPos.x, 0.0f, _targetPos.z - _unitPos.z);
	//_direction = _targetPos - _unitPos;
	float length = _direction.Length();

	if (length != 0)
	{
		_direction /= length;
		// ĳ���Ͱ� ���� �ִ� ����
		Vector3 characterDir = -Vector3::UnitZ;

		float theta = atan2(_direction.x, _direction.z);

		// ��ǥ ȸ����: theta�� 180��(�Ǵ� �� ����)�� �߰��Ͽ� �̵� ������ ���ϰ� ����
		theta += DirectX::XM_PI;

		float deltaAngle = theta - _realRotationY;
		// deltaAngle�� -�𿡼� �� ������ ������ ����ȭ
		deltaAngle = std::fmod(deltaAngle + DirectX::XM_PI, DirectX::XM_2PI) - DirectX::XM_PI;

		// ��ǥ ȸ����
		float _goalRotationY = _realRotationY + deltaAngle;

		// �ε巴�� ȸ�� (Lerp ���)
		//float newRotationY = _realRotationY + (_goalRotationY - _realRotationY) * mRotationSpeed /** _entity->GetComponent<PlayerComponent>().mMoveSpeed*/;

		// ���ο� ȸ���� ���� (y�� ȸ���� ����)
		_realRotationY = _realRotationY + deltaAngle;
		//_realRotationY = newRotationY;
	}
	else
	{
		_direction = { 0.0f,0.0f,0.0f }; // �и� 0�̸� �� �������� ���ϰ� ��
	}
}

//�������̴���?
//ĳ���Ͱ� �̵��߿� ���� ������ �������ֱ� ���� �Լ�
void FSMHelper::LookAtPathDir(const Vector3& _unitPos, const Vector3& _targetPos, std::vector<Vector2>*& _path, const size_t& _currentPathIdx, Vector3& _direction, float& _realRotationY)
{
	// ��ΰ� ���ٸ� ������
	if (_path == nullptr)
	{
		return;
	}

	//// ���� ��� ���� Ȯ��
	int remainingPathLength = (*_path).size() - _currentPathIdx;

	// ���� ��� ���̿� ���ϴ� ������ �� ���� ���� ����
	int adjustedOffset = std::min(mDesiredOffset, remainingPathLength - 1);

	// adjustedOffset�� ������ futureIndex ���
	int futureIndex = _currentPathIdx + adjustedOffset;

	// futureIndex�� �ش��ϴ� ��ġ ���
	Vector3 nextPosition((*_path)[futureIndex].x, _unitPos.y, (*_path)[futureIndex].y);

	/// 1. ���� ����(Ư�� ��� ������ �ٶ󺸰� �ֵ��� ����)
	_direction = Vector3(nextPosition.x - _unitPos.x, 0.0f, nextPosition.z - _unitPos.z);
	float length = _direction.Length();

	// ��� �ڵ�, Ȥ�ö� nextPosition�� ���� ������ ���� ���� length�� ����ġ�� ���� ��� ������ �������� �ʴ� ������ ����
	if (length < 1.5f) 
	{
		// ����� ������ ���� ��� ��ǥ ��ġ�� �ٶ󺸵��� ����
		if (_currentPathIdx == (*_path).size() - 1) 
		{
			_direction = Vector3(_targetPos.x - _unitPos.x, 0.0f, _targetPos.z - _unitPos.z);
		}
		else 
		{
			// ����� ���� �� ��° ������ ��� (�ʿ� �� futureIndex�� ���� ����)
			futureIndex = std::min(_currentPathIdx + 2, (*_path).size() - 1);
			nextPosition = Vector3((*_path)[futureIndex].x, 0.0f, (*_path)[futureIndex].y);
			_direction = Vector3(nextPosition.x - _unitPos.x, 0.0f, nextPosition.z - _unitPos.z);
		}
		length = _direction.Length();
	}

	// �ʹ� ����� �Ÿ����� ���� ���� ����
	if (length > 0.01f)
	{
		_direction /= length;
		// ĳ���Ͱ� ���� �ִ� ����
		Vector3 characterDir = -Vector3::UnitZ;

		float theta = atan2(_direction.x, _direction.z);

		// ��ǥ ȸ����: theta�� 180��(�Ǵ� �� ����)�� �߰��Ͽ� �̵� ������ ���ϰ� ����
		theta += DirectX::XM_PI;

		float deltaAngle = theta - _realRotationY;
		// deltaAngle�� -�𿡼� �� ������ ������ ����ȭ
		deltaAngle = std::fmod(deltaAngle + DirectX::XM_PI, DirectX::XM_2PI) - DirectX::XM_PI;

		// ��ǥ ȸ����
		float _goalRotationY = _realRotationY + deltaAngle;

		// �ε巴�� ȸ�� (Lerp ���)
		float newRotationY = _realRotationY + (_goalRotationY - _realRotationY) * mRotationSpeed /** _entity->GetComponent<PlayerComponent>().mMoveSpeed*/;

		// ���ο� ȸ���� ���� (y�� ȸ���� ����)
		//_realRotationY = _realRotationY + deltaAngle;
		_realRotationY = newRotationY;
	}
	else
	{
		_direction = { 0.0f,0.0f,0.0f }; // �и� 0�̸� �� �������� ���ϰ� ��
	}
}

void FSMHelper::LookAtPathDir2(const Vector3& _unitPos, const Vector3& _targetPos, std::vector<Vector2>*& _path, const size_t& _currentPathIdx, Vector3& _direction, float& _realRotationY)
{
	// ��ΰ� ���ٸ� ������
	if (_path == nullptr)
	{
		return;
	}

	//// ���� ��� ���� Ȯ��
	int remainingPathLength = (*_path).size() - _currentPathIdx;

	// ���� ��� ���̿� ���ϴ� ������ �� ���� ���� ����
	int adjustedOffset = std::min(mDesiredOffset, remainingPathLength - 1);

	// adjustedOffset�� ������ futureIndex ���
	int futureIndex = _currentPathIdx + adjustedOffset;

	// futureIndex�� �ش��ϴ� ��ġ ���
	Vector3 nextPosition((*_path)[futureIndex].x, _unitPos.y, (*_path)[futureIndex].y);

	/// 1. ���� ����(Ư�� ��� ������ �ٶ󺸰� �ֵ��� ����)
	_direction = Vector3(nextPosition.x - _unitPos.x, 0.0f, nextPosition.z - _unitPos.z);
	float length = _direction.Length();

	// ��� �ڵ�, Ȥ�ö� nextPosition�� ���� ������ ���� ���� length�� ����ġ�� ���� ��� ������ �������� �ʴ� ������ ����
	if (length < 1.5f)
	{
		// ����� ������ ���� ��� ��ǥ ��ġ�� �ٶ󺸵��� ����
		if (_currentPathIdx == (*_path).size() - 1)
		{
			_direction = Vector3(_targetPos.x - _unitPos.x, 0.0f, _targetPos.z - _unitPos.z);
		}
		else
		{
			// ����� ���� �� ��° ������ ��� (�ʿ� �� futureIndex�� ���� ����)
			futureIndex = std::min(_currentPathIdx + 2, (*_path).size() - 1);
			nextPosition = Vector3((*_path)[futureIndex].x, 0.0f, (*_path)[futureIndex].y);
			_direction = Vector3(nextPosition.x - _unitPos.x, 0.0f, nextPosition.z - _unitPos.z);
		}
		length = _direction.Length();
	}

	// �ʹ� ����� �Ÿ����� ���� ���� ����
	if (length > 0.01f)
	{
		_direction /= length;
		// ĳ���Ͱ� ���� �ִ� ����
		Vector3 characterDir = -Vector3::UnitZ;

		float theta = atan2(_direction.x, _direction.z);

		// ��ǥ ȸ����: theta�� 180��(�Ǵ� �� ����)�� �߰��Ͽ� �̵� ������ ���ϰ� ����
		theta += DirectX::XM_PI;

		float deltaAngle = theta - _realRotationY;
		// deltaAngle�� -�𿡼� �� ������ ������ ����ȭ
		deltaAngle = std::fmod(deltaAngle + DirectX::XM_PI, DirectX::XM_2PI) - DirectX::XM_PI;

		// ��ǥ ȸ����
		float _goalRotationY = _realRotationY + deltaAngle;

		// �ε巴�� ȸ�� (Lerp ���)
		float newRotationY = _realRotationY + (_goalRotationY - _realRotationY) * mRotationSpeed /** _entity->GetComponent<PlayerComponent>().mMoveSpeed*/;

		// ���ο� ȸ���� ���� (y�� ȸ���� ����)
		//_realRotationY = _realRotationY + deltaAngle;
		_realRotationY = newRotationY;
	}
	else
	{
		_direction = { 0.0f,0.0f,0.0f }; // �и� 0�̸� �� �������� ���ϰ� ��
	}
}
