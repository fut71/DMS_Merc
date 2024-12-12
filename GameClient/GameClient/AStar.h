#pragma once
#pragma once
#include <directxTk/SimpleMath.h>
#include <queue>
#include <iostream>

using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;

class AStar
{
public:
	AStar();
	~AStar();

	// Node ����ü ����
	struct Node
	{
		Vector2 position;
		float gCost; // ���� ��忡�� �ش� �������� ���� �ҿ� ���
		float hCost; // ���谪
		float GetFCost() const { return gCost + hCost; }
		Node* parent = nullptr;
	};

	// Node �� ������ ����
	// std::priority_queue�� �⺻������ �ִ� ���� ����Ͽ� ���� ū ��Ҹ� ��Ʈ�� ����������, 
	// Ŀ���͸���¡�� �� ������ �����Ͽ� �ּ� ���� ����ų� �ٸ� ���ؿ� ���� ����� �켱������ ������ �� �ִ�.
	struct NodeCompare
	{
		bool operator()(const Node* lhs, const Node* rhs)
		{
			if (lhs)
				return lhs->GetFCost() > rhs->GetFCost();
		}
	};

	// �⺻���� ������ ���� �Լ�
	void Initialize(Vector2 _mapStart, Vector2 _mapEnd, std::vector<std::vector<int>>* _map);

	// A* �˰��� ����
	std::vector<Vector2>* Astar(Vector2& _startPoint, Vector2& _goalPoint);

	// Astar�� �ʰ� ��θ� ����Ѵ�.
	void PrintAstarMap(const std::vector<std::vector<int>>& _map, const std::vector<Vector2> _path);

	// Astar�� ��θ� ��ǥ�� ����Ѵ�.
	void PrintSimpleAstar(const std::vector<Vector2> _path);

	// Ÿ���� ���ͷ� �����ϴ� �Լ���.
	void AdjustToTileCenter(const Vector3& _realPos, Vector2& _xzPos);

	// ��ŷ �� Ÿ�� ���� ������ �޾ƿ��� ���� ����� get �Լ�
	std::vector<std::vector<int>>* GetAstarMap();
	Vector2 GetMapStartPoint();
	Vector2 GetMapEndPoint();
	std::vector<Vector2>* GetObstaclePos();

	/// �ӽ÷� �Űܵδ� �Լ���
	// ��ֹ��� �ִ��� ������ Ȯ���ϴ� �Լ�
	bool IsObstacle(const Vector2& _pos, const std::vector<Vector2>& _obstacles);
	// ��ֹ��� ���� �� ���ο� ��ġ�� ����ؼ� �־���[���� posXY]
	Vector2 newPos(const Vector2& _point, const std::vector<Vector2>& _obstacles);
	Vector2 MovePos(const Vector2& _point);

private:
	// Ÿ�� �� ĭ�� ���� ũ�� -> ������ 1�ۿ� �� ������.. ���߿� �� Ȯ���غ���.
	float mTileScale = 1.0f;
	float mHalfTileScale = 0.5f;

	// ���� ���۰� ���� ����
	Vector2 mMapStart = Vector2();
	Vector2 mMapEnd = Vector2();
	std::vector<Vector2>* mObstacles = nullptr;
	std::vector<std::vector<int>>* mMap = nullptr;

	Vector2 directions[8];

	float Distance(const Vector2& _pos1, const Vector2& _pos2);

	bool IsMovable(const Vector2& _pos, const std::vector<Vector2>& _obstaclePos, const Vector2& _mapStartPos, const Vector2& _mapEndPos);

	Vector2 SelectBestDirection(const Vector2& _from, const Vector2& _to, const std::vector<Vector2>& directions);

	// ��ֹ��� Astar�� ���Ǵ� �����ͷ� ��ȯ���ִ� �Լ���.
	std::vector<Vector2>* ConvertObstacle(const Vector2& _startPoint, const std::vector<std::vector<int>> _map);

	// ���� �⺻���� ������ Astar ��� ��ȯ �Լ���.(A* �˰��� ����)
	std::vector<Vector2>* BasicAstar(Vector2& _startPoint, Vector2& _goalPoint, const std::vector<Vector2>& obstacles, const Vector2& _mapStart, const Vector2& _mapEnd);

	// Ŭ�� ��ġ�� ��밡������ Ȯ���ϱ� ���� Astar��
	//std::vector<std::vector<int>>* mAstarMap;

	// ��ֹ��� �����Ѵ�.
	std::vector<Vector2>* tempObstacles;
};

template <>
struct std::hash<Vector2>
{
	size_t operator()(const Vector2& v) const noexcept
	{
		return std::hash<float>()(v.x) ^ (std::hash<float>()(v.y) << 1);
	}
};

