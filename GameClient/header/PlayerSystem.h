#pragma once
#include "EntityManager.h" // �̰� �����ϸ� byte ���� �߻��ϴ� �ߺ��Ǵ��� ������ �� ��
#include "EnemyData.h"
#include "EquipmentManager.h"
#include "StatusManager.h"
#include "FSMHelper.h"
#include "AllyFSM.h"
#include "EnemyFSM.h"

class PlayerSystem
{
public:
	PlayerSystem(entt::registry& _registry, EntityManager* _pEntityManager, PhysicsManager* _pPhysicsManager
		, RenderManager* _pRenderManager, ResourceManager* _pResourceManager, EventManager* _pEventManager
	,SoundManager* _pSoundManager, EquipmentManager* _pEquipmentManager);
	~PlayerSystem();

	// �� pair�� ���ؼ� a�� ��� a�� ��ȯ
	struct DistanceCompare
	{
		bool operator()(const std::pair<shared_ptr<Entity>, float>& a,
			const std::pair<shared_ptr<Entity>, float>& b)
		{
			return a.second > b.second;
		}
	};

	void Initialize(std::vector<std::pair<std::shared_ptr<Entity>, Vector3>>* _pMoneyPosVec, AStar* _pAstar);

	// ��ġ �� ���۵ǰ� ������ ���� �÷��̸� ����Ѵ�.
	void PlayerUnitUpdate(std::shared_ptr<Entity> _entity, float _dTime);
	void EnemyUnitUpdate(std::shared_ptr<Entity> _entity, float _dTime);
	
	void FixedUpdate(std::shared_ptr<Entity> _entity, float _fixedDTime);

	// ������ �����̴�. ���Ŀ� ���� ���� �� ���â�� ���� �����͵��� ������ �Լ��� �� �����̴�.
	void Finalize();

	// �ʱ� ����AI�� �߰�(��ƼƼ, Ŭ���� Ÿ��, ��ġ)
	void AddUnit(std::shared_ptr<Entity> _entity, const std::string& _classType, Vector3 _pos);

	// ��ġ�� ������ �߰�
	void AddDeplyUnit(std::shared_ptr<Entity> _entity, const std::string& _classType);

	// ��ġ �� ���� ����
	void ChangeUnit(std::shared_ptr<Entity> _entity, const std::string& _classType);

	// ��ġ �� ���� ����
	void RemoveUnit(UID _entityUID);

	// ��ġ�� �Ϸ�� �� ������ �����ؾ� ������ �� ����.
	void AddPhysics(std::shared_ptr<Entity> _entity);

	// ���� ������ ���������� �°� �߰�. ���� �Ŵ����� �ִ°� ������ �ӽ÷� ���⿡ ����� �д�. ���� ���������� �����ϴ�.
	void AddSceneEnemyUnits(UID _sceneUID);

	// ���� ������ �Ǳ� ���� FSM�� ���� ù ���¸� �������ش�.
	void SetInitializeState(std::shared_ptr<Entity> _entity, State _state);

	/// <summary>���� ������ �����Ѵ�.(���ݹ����� int�� �ø��ؼ� ������)</summary>
	/// <param name="_entity">�� ����</param> <param name="_otherEntity">��� ����</param> <returns>true : ���� ���� ����, false : ���� �Ұ� ����</returns>
	bool IsAttackable(const Vector2& _myPos, const Vector2& _otherPos, float _attackRange);

	// �ӽ÷� �߰��ϴ� �ؽ�Ʈ �ڵ�
	void ShowState(std::shared_ptr<Entity> _entity);
	void UpdateStateText(std::shared_ptr<Entity> _entity);

	int mMaxSetUnitNum;  // �ִ� ��ġ ������ ���� ��

	void DetectGroundMoney(); //Enemy���� �켱 ȣ���Ѵ�.

	// �Ʊ��� : ���� �߿� ���� ����� ���� Ÿ������ ��´�. (�� ��ü�� Ž���Ѵ�.)
	std::shared_ptr<Entity> TargetNearestEnemy(std::shared_ptr<Entity> _entity);
	std::shared_ptr<Entity> EnemyTargetInRange(std::shared_ptr<Entity> _entity);

	// ������ : �Ʊ� �߿� ���� ����� ���� Ÿ������ ��´�. (�� ��ü�� Ž���Ѵ�.)
	std::shared_ptr<Entity> TargetNearestPlayer(std::shared_ptr<Entity> _entity);
	std::shared_ptr<Entity> PlayerTargetInRange(std::shared_ptr<Entity> _entity);

	std::vector<Vector2> *GetUnitDeployPosVec();

	StatusManager* mpStatusManager; // StatusManager ����(classManager�� Status �� ����)

private:
	entt::registry& mRegistry;
	EntityManager* mpEntityManager;
	PhysicsManager* mpPhysicsManager;
	ResourceManager* mpResourceManager;
	EquipmentManager* mpEquipmentManager; // EquipmentManager ����
	SoundManager* mpSoundManager;

	// ������ �Ŵ�����
	RenderManager* mpRenderManager;

	// FSM �Ŵ��� ����
	FSMHelper* mpFSMHelper;

	AllyFSM* mpAllyFSM; // FSM ����
	EnemyFSM* mpEnemyFSM; // FSM ����
// 	StatusManager* mpStatusManager; // StatusManager ����(classManager�� Status �� ����)

	// ���� ���� �� ��ȭ�� �����ؼ� �׶��� Ÿ�����ϰԲ� �ϱ� ���� ����
	int tempMoneyVecSize;
	// ������ ��ġ�� �ڸ��� ����ϴ� ���� ũ�� �迭�� �����.
	std::vector<Vector2> unitDeployPosVec;

	// ��ȹ���� ������ ��
	int mBuffLv1_Limit; 
	int mBuffLv2_Limit;

private:
	std::pair<std::shared_ptr<Entity>, float> FindClosestEntity(const Vector3& _pos);
	std::vector<EnemyData> mpEnemyPosVec;
};

