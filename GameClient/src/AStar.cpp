#include "AStar.h"

AStar::AStar()
{
	// sqrt(2) �߰� �켱
	directions[0] = Vector2(1, 1);
	directions[1] = Vector2(1, -1);
	directions[2] = Vector2(-1, 1);
	directions[3] = Vector2(-1, -1);
	// sqrt(1) �ļ���
	directions[4] = Vector2(1, 0);
	directions[5] = Vector2(-1, 0);
	directions[6] = Vector2(0, 1);
	directions[7] = Vector2(0, -1);

	tempObstacles = nullptr;
}

AStar::~AStar()
{

}

void AStar::Initialize(Vector2 _mapStart, Vector2 _mapEnd, std::vector<std::vector<int>>* _map)
{
	mMapStart = _mapStart;
	mMapEnd = _mapEnd;
	mMap = _map;
	mObstacles = ConvertObstacle(mMapStart, *mMap);
}

std::vector<Vector2>* AStar::BasicAstar(Vector2& _startPoint, Vector2& _goalPoint, const std::vector<Vector2>& obstacles, const Vector2& _mapStart, const Vector2& _mapEnd)
{
	/// ��� �ڵ� -------------------------------------------------------------
	if (_startPoint.x < _mapStart.x || _startPoint.y < _mapStart.y ||
		_startPoint.x > _mapEnd.x || _startPoint.y > _mapEnd.y)
	{
		//std::cout << "Astar() error - ���� ������ ��ǥ ������ ���� ũ�⸦ ����ϴ�." << std::endl;
		//return {};
		_startPoint = MovePos(_startPoint);
	}
	if (_goalPoint.x < _mapStart.x || _goalPoint.y < _mapStart.y ||
		_goalPoint.x > _mapEnd.x || _goalPoint.y > _mapEnd.y)
	{
		_goalPoint = MovePos(_goalPoint);
	}

	for (const auto& obs : obstacles)
	{
		if (obs == _startPoint)
		{
			_startPoint = newPos(_startPoint, *mObstacles);
			//std::cout << "Astar() error - ���� ������ ��ǥ ������ ��ֹ��� ���� ��ġ�� �����Ǿ����ϴ�." << std::endl;
			//return {};
		}
		if (obs == _goalPoint)
		{
			_goalPoint = newPos(_goalPoint, *mObstacles);
			//std::cout << "Astar() error - ���� ������ ��ǥ ������ ��ֹ��� ���� ��ġ�� �����Ǿ����ϴ�." << std::endl;
			//return {};
		}

		if (obs.x < _mapStart.x || obs.y < _mapStart.y || obs.x > _mapEnd.x || obs.y > _mapEnd.y)
		{
			std::cout << "Astar() error - ��ֹ� ��ġ�� ���� ũ�⸦ ����ϴ�." << std::endl;
			return {};
		}
	}

	/// ��� �ڵ� �� -------------------------------------------------------------

	std::priority_queue<Node*, std::vector<Node*>, NodeCompare> mOpenList;
	int width = static_cast<int>(_mapEnd.x - _mapStart.x) + 1;
	int height = static_cast<int>(_mapEnd.y - _mapStart.y) + 1;

	// 2���� �迭�� �������� �Ҵ�
	Node*** nodeArray = new Node * *[width];
	bool** closeList = new bool* [width];
	for (int i = 0; i < width; i++)
	{
		nodeArray[i] = new Node * [height]();
		closeList[i] = new bool[height]();
	}


	// ���� ù ��° ����� Distance(hCost)�� goal�� ���� �� �����̹Ƿ� ���� ū ���� ���� �ȴ�.
	Node* startNode = new Node();
	startNode->position = _startPoint;
	startNode->hCost = Distance(_startPoint, _goalPoint);
	mOpenList.push(startNode);
	nodeArray[static_cast<int>(_startPoint.x - _mapStart.x)][static_cast<int>(_startPoint.y - _mapStart.y)] = startNode;

	// openList�� �� ������ �ݺ��Ѵ�.
	while (!mOpenList.empty())
	{
		Node* currentNode = mOpenList.top();
		mOpenList.pop();

		// ���� closedList���� �ش� ����� ��ġ�� �ִٸ� �� ��ġ�� ���� �ʴ´�.
		if (closeList[static_cast<int>(currentNode->position.x - _mapStart.x)][static_cast<int>(currentNode->position.y - _mapStart.y)])
		{
			continue;
		}
		closeList[static_cast<int>(currentNode->position.x - _mapStart.x)][static_cast<int>(currentNode->position.y - _mapStart.y)] = true;

		if (currentNode->position == _goalPoint)
		{
			std::vector<Vector2>* path = new std::vector<Vector2>();
			while (currentNode)
			{
				path->push_back(currentNode->position);
				currentNode = currentNode->parent;
			}
			std::reverse(path->begin(), path->end());

			for (int i = 0; i < width; ++i)
			{
				for (int j = 0; j < height; ++j)
				{
					delete nodeArray[i][j];
				}
				delete[] nodeArray[i];
				delete[] closeList[i];
			}
			delete[] nodeArray;
			delete[] closeList;

			return path;
		}

		//GetDirections();

		for (const auto& dir : directions)
		{

			Vector2 neighborPos = currentNode->position + dir;

			/// �������� ���ϸ� ����
			if (!IsMovable(neighborPos, obstacles, _mapStart, _mapEnd))
			{
				continue;
			}

			float newGCost = currentNode->gCost + Distance(currentNode->position, neighborPos);

			// ���� ��ġ�� �ʿ��� ã�´�.
			int nx = static_cast<int>(neighborPos.x - _mapStart.x);
			int ny = static_cast<int>(neighborPos.y - _mapStart.y);
			Node* neighborNode = nodeArray[nx][ny];

			if (neighborNode == nullptr) // ��尡 ���ų� �� ����� gCost�� �� ������ ���ο� ��带 ������ openList�� �־��ش�.
			{
				Node* neighborNode = new Node;
				neighborNode->position = neighborPos;
				neighborNode->gCost = newGCost;
				// ������� ���� ���ܵ�
				//std::cout << neighborNode->position.x << " " << neighborNode->position.y << " " << neighborNode->position.z << ", " << neighborNode->gCost << std::endl;
				neighborNode->hCost = Distance(neighborPos, _goalPoint);
				neighborNode->parent = currentNode;
				mOpenList.push(neighborNode);
				nodeArray[nx][ny] = neighborNode;
			}
			else if (newGCost < neighborNode->gCost)
			{
				neighborNode->gCost = newGCost;
				neighborNode->parent = currentNode;
				mOpenList.push(neighborNode);
			}
		}
	}

	for (int i = 0; i < width; ++i)
	{
		for (int j = 0; j < height; ++j)
		{
			delete nodeArray[i][j];
		}
		delete[] nodeArray[i];
		delete[] closeList[i];
	}
	delete[] nodeArray;
	delete[] closeList;

	return nullptr;
}

std::vector<Vector2>* AStar::Astar(Vector2& _startPoint, Vector2& _goalPoint)
{
	return BasicAstar(_startPoint, _goalPoint, *mObstacles, mMapStart, mMapEnd);
	int a = 0;
}

std::vector<Vector2>* AStar::ConvertObstacle(const Vector2& _startPoint, const std::vector<std::vector<int>> _map)
{
	tempObstacles = new std::vector<Vector2>();
	for (size_t i = 0; i < _map.size(); i++)
	{
		for (size_t j = 0; j < _map[i].size(); j++)
		{
			if (_map[i][j] == 1)
			{
				tempObstacles->push_back({ static_cast<float>(j) + _startPoint.x, static_cast<float>(i) + _startPoint.y });
				std::cout << "[" << static_cast<float>(j) + _startPoint.x << ", " << static_cast<float>(i) + _startPoint.y << "] " << std::endl;
			}
		}
	}
	return tempObstacles;
}

void AStar::PrintAstarMap(const std::vector<std::vector<int>>& _map, const std::vector<Vector2> _path)
{
	int n = 0;
	for (const auto& pos : _path)
	{
		std::cout << "----------- " << n << "��° �̵� -----------" << std::endl;
		for (int i = 0; i < _map.size(); ++i)
		{
			for (int j = 0; j < _map[i].size(); ++j)
			{
				if (_map[i][j] == 1)
				{
					std::cout << "��";
				}
				else if (i == pos.y && j == pos.x)
				{
					std::cout << "��";
				}
				else
				{
					std::cout << "��";
				}
			}
			std::cout << std::endl;
		}
		n++;
	}
}

void AStar::PrintSimpleAstar(const std::vector<Vector2> _path)
{
	for (const auto& pos : _path)
	{
		std::cout << "(" << pos.x << ", " << pos.y << ")\n";
	}
}

void AStar::AdjustToTileCenter(const Vector3& _realPos, Vector2& _xzPos)
{
	if (_realPos.x < -0.5f)
	{
		_xzPos.x = static_cast<int>((_realPos.x - mHalfTileScale) / mTileScale);
	}
	else if (_realPos.x > 0.5f)
	{
		_xzPos.x = static_cast<int>((_realPos.x + mHalfTileScale) / mTileScale);
	}
	else
	{
		_xzPos.x = static_cast<int>(_realPos.x);
	}

	if (_realPos.z < -0.5f)
	{
		_xzPos.y = static_cast<int>((_realPos.z - mHalfTileScale) / mTileScale);
	}
	else if (_realPos.z > 0.5f)
	{
		_xzPos.y = static_cast<int>((_realPos.z + mHalfTileScale) / mTileScale);
	}
	else
	{
		_xzPos.y = static_cast<int>(_realPos.z);
	}
}

std::vector<std::vector<int>>* AStar::GetAstarMap()
{
	return mMap;
}

Vector2 AStar::GetMapStartPoint()
{
	return mMapStart;
}

Vector2 AStar::GetMapEndPoint()
{
	return mMapEnd;
}

std::vector<Vector2>* AStar::GetObstaclePos()
{
	return tempObstacles;
}

bool AStar::IsObstacle(const Vector2& _pos, const std::vector<Vector2>& _obstacles)
{
	for (const auto& obstacle : _obstacles)
	{
		if (obstacle == _pos)
		{
			return true;
		}
	}
	return false;
}

// ��ó�� ������ ��ġ�� �̵� (����: x�� y�� 1�� ������Ű�鼭 Ȯ��)
Vector2 AStar::newPos(const Vector2& _point, const std::vector<Vector2>& _obstacles)
{
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			Vector2 newGoalPoint(_point.x + i, _point.y + j);
			//if (!IsObstacle(newGoalPoint, _obstacles))
			if (IsMovable(newGoalPoint, _obstacles, mMapStart, mMapEnd))
			{
				return newGoalPoint;
			}
		}
	}
	return _point;
}

Vector2 AStar::MovePos(const Vector2& _point)
{
	Vector2 newPoint = {};
	if (_point.x < mMapStart.x)
	{
		newPoint.x = mMapStart.x;
	}
	if (_point.x > mMapEnd.x)
	{
		newPoint.x = mMapEnd.x;
	}
	if (_point.y < mMapStart.y)
	{
		newPoint.y = mMapStart.y;
	}
	if (_point.y > mMapStart.y)
	{
		newPoint.y = mMapEnd.y;
	}
	return newPoint;
}

float AStar::Distance(const Vector2& _pos1, const Vector2& _pos2)
{
	return (_pos1 - _pos2).Length();
}

bool AStar::IsMovable(const Vector2& _pos, const std::vector<Vector2>& _obstaclePos, const Vector2& _mapStartPos, const Vector2& _mapEndPos)
{
	// ���� ��ġ�� ���� ��ǥ ���� �־�� ��
	int x = static_cast<int>(_pos.x);
	int y = static_cast<int>(_pos.y);

	if (x < static_cast<int>(_mapStartPos.x) || y < static_cast<int>(_mapStartPos.y) ||
		x > static_cast<int>(_mapEndPos.x) || y > static_cast<int>(_mapEndPos.y))
	{
		return false;
	}

	// �� ��ֹ��� ���� �˻�
	for (const auto& obs : _obstaclePos)
	{
		int obsX = static_cast<int>(obs.x);
		int obsY = static_cast<int>(obs.y);

		// ��ó ��ֹ��� �˻��Ѵ�.
		if (_pos.x == obs.x && _pos.y == obs.y && Distance(_pos, obs) <= sqrt(2))
		{
			return false;
		}
	}

	return true;
}

Vector2 AStar::SelectBestDirection(const Vector2& _from, const Vector2& _to, const std::vector<Vector2>& directions)
{
	Vector2 toGoalDirection = _to - _from;
	float toGoalLength = toGoalDirection.Length();
	toGoalDirection /= toGoalLength; // ��� ���� ���ߴ�.


	Vector2 bestDirection = directions[0];

	float maxDot = 0.0f; // ���밪���� ���� �Ŷ� 0�� �ּҰ����� �Ѵ�.
	// ��� ���� ���Ϳ� ���ؼ� ������ ���ؼ� ���� ū ���� ���Ѵ�.
	for (const auto& dir : directions)
	{
		float dotProduct = abs(toGoalDirection.Dot(dir));

		if (maxDot < dotProduct)
		{
			maxDot = dotProduct;
			bestDirection = dir;
		}
	}
	return bestDirection;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                       	return bestDirection;
}