#pragma once
#include <any>
#include <optional>
#include <functional>
#include <unordered_map>
#include <string>
#include <memory>
#include <Entity.h>
#include "PlayerComponent.h"
#include "enemyComponent.h"
#include "MoneyComponent.h"
#include "CapsuleCollider.h"
#include "ProjectileComponent.h"
#include "Transform.h"

/// FSM�� State�� �����ϱ� ���� �Լ��� �����ϴ� Ŭ����
class CustomFunctions
{
public:
	CustomFunctions();
	~CustomFunctions() {};

public:
	float mTileScale = 1.0f;
	std::unordered_map<std::string, std::function<bool(std::shared_ptr<Entity> _temp)>> mFuncMap; // �Լ��� ��� �׸��̴�. �̰��� ���������� �ƿ� �ٱ����� ������ ������ ���ɼ��� �ִ�.
};

