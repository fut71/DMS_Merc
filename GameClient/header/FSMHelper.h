#pragma once
#include "Entity.h"
#include "AStar.h"
#include "CustomFunctions.h"
#include "ResourceManager.h"

/// <summary>
/// �� Ŭ������ ��� FSM���� �ߺ����� ���Ǵ� �͵��� �����ϱ� ���ؼ� ���˴ϴ�.
/// 
/// </summary>
class FSMHelper
{
public:
	FSMHelper(ResourceManager* _pResourceManager);
	~FSMHelper();

	void Initialize();

	/// ����ü ���� �Լ�
	void SetMoneyVec(std::vector<std::pair<std::shared_ptr<Entity>, Vector3>>* _pMoneyPosVec);
	std::vector<std::pair<std::shared_ptr<Entity>, Vector3>>* GetMoneyVec();
	//int GetVecSize();

	/// Astar ���� �Լ�
	// Astar ��ü�� �����͸� �����Ѵ�.
	void SetAstar(AStar* _pAstar);
	// Astar�� ��θ� �缳���Ѵ�.
	void CalculatePath(std::vector<Vector2>*& _path, Vector2& _startPoint, Vector2& _goalPoint);
	void CalculatePathforEnemy(std::vector<Vector2>*& _path, Vector2& _startPoint, Vector2& _goalPoint);
	// Vector3�� ��ǥ�� Astar�� ���Ǵ� Ÿ�� ������ �������� Vector2(_xzPos)�� ��ȯ��ŵ�ϴ�.
	void AdjustToTileCenter(const Vector3& _realPos, Vector2& _xzPos);
	std::vector<Vector2>* GetAstarObstacles();

	// customFuction�� funcMap�� ������
	std::unordered_map<std::string, std::function<bool(std::shared_ptr<Entity> _temp)>> GetFuctMap();

	// ������ �����ϴ� �Լ�[�� ��ġ, ��� ��ġ, �� ����, ��ǥ ȸ����]
	void LookAtTarget(const Vector3& _unitPos, const Vector3& _targetPos, Vector3& _direction, float& _realRotationY);
	void LookAtPathDir(const Vector3& _unitPos, const Vector3& _targetPos, std::vector<Vector2>*& _path, const size_t& _currentPathIdx, Vector3& _direction, float& _realRotationY);
	void LookAtPathDir2(const Vector3& _unitPos, const Vector3& _targetPos, std::vector<Vector2>*& _path, const size_t& _currentPathIdx, Vector3& _direction, float& _realRotationY);

private:
	ResourceManager* mpResourceManager;

	AStar* mpAstar; // Astar �����
	AStar* mpAstarforEnemy; // Astar �����
	CustomFunctions* mpConditionFunctions; // FSM�� State�� �����ϱ� ���� �Լ� ���յ�

	std::vector<std::pair<std::shared_ptr<Entity>, Vector3>>* mpMoneyPosVec; // ����ü -> �÷��̾� �ý��ۿ��� �޾ƿ� ������
	int mDesiredOffset; // ���� ������ �ڿ������� �ϱ� ���Ͽ�, Astar�� Ÿ�� ��ġ�� �����ϴ� �Լ� (���� post-Smooth�� ������ ��)
	float mRotationSpeed; // ȸ�� ���� �ӵ��� �����ϱ� ���� ���
};

