#include "StatusManager.h"
#include "CapsuleCollider.h"

StatusManager::StatusManager(EntityManager* _pEntityManager, ResourceManager* _pResourceManager)
{
	mpStatusTableVec = {};
	mpResourceManager = _pResourceManager;
	mpClassManager = new ClassManager(_pEntityManager, _pResourceManager);
}

StatusManager::~StatusManager()
{
	delete mpClassManager;
}

void StatusManager::Initialize()
{
	mpClassManager->Initialize(); // Ŭ���� string ������ csv���� �ҷ��´�.
	mpStatusTableVec = mpResourceManager->ParseCSV<StatusTable>("Status.csv");
}

void StatusManager::AddStatus(std::shared_ptr<Entity> _entity, const std::string& _classType, const int& _buffLv)
{
	for (const auto& statusTable : mpStatusTableVec)
	{
		if (statusTable.className == _classType && statusTable.buffLv == _buffLv)
		{
			// �÷��̾� ������Ʈ �߰�
			auto& playerComp =_entity->AddComponent<PlayerComponent>();
			playerComp.mPlaceMoney = statusTable.placeMoney;
			playerComp.mMaxHP = statusTable.maxHP;
			playerComp.mHP = playerComp.mMaxHP;
			_entity->AddComponent<Transform>(Vector3(), Vector3(0.0f, DirectX::XMConvertToRadians(-90.0f), 0.0f), Vector3(statusTable.unitSize));
			playerComp.mAttackPower = statusTable.attackPower;
			playerComp.mAttackSpeed = statusTable.attackSpeed;
			playerComp.mMoveOriSpeed = statusTable.moveSpeed;
			playerComp.mMoveSpeed = playerComp.mMoveOriSpeed;
			playerComp.mAttackRange = statusTable.attackRange;
			playerComp.mRecogRange = statusTable.recognitionRange;
			playerComp.mBuffSpeed = statusTable.buffSpeed;
			playerComp.mBuffMoney1 = statusTable.buffMoney1;
			playerComp.mBuffMoney2 = statusTable.buffMoney2;
		}
	}

	// Ŭ���� �Ŵ������� �׸� ������ �޾ƿ´�.
	mpClassManager->AddUnitMesh(_entity, _classType);
}

void StatusManager::AddInitStatus(std::shared_ptr<Entity> _entity, const std::string& _classType)
{
	// ���̺�κ��� �⺻ �������ͽ��� �޾ƿ´�.
	for (const auto& statusTable : mpStatusTableVec)
	{
		if (statusTable.className == _classType && statusTable.buffLv == 0)
		{
			// ���� ������Ʈ �߰�
			auto& enemyComp = _entity->AddComponent<EnemyComponent>();
			enemyComp.mMaxHP = statusTable.maxHP;
			enemyComp.mHP = enemyComp.mMaxHP;
			_entity->AddComponent<Transform>(Vector3(), Vector3(0.0f, DirectX::XMConvertToRadians(90.0f), 0.0f), Vector3(statusTable.unitSize));
			enemyComp.mAttackPower = statusTable.attackPower;
			enemyComp.mAttackSpeed = statusTable.attackSpeed;
			enemyComp.mMoveSpeed = statusTable.moveSpeed;
			enemyComp.mAttackRange = statusTable.attackRange;
			enemyComp.mRecogRange = statusTable.recognitionRange;
		}
	}

	// Ŭ���� �Ŵ������� �׸� ������ �޾ƿ´�.
	mpClassManager->AddUnitMesh(_entity, _classType);
}


void StatusManager::UpdateAllyStatus(std::shared_ptr<Entity> _entity, const std::string& _classType, const int& _buffLv)
{
	for (const auto& statusTable : mpStatusTableVec)
	{
		if (statusTable.className == _classType && statusTable.buffLv == _buffLv)
		{
			auto& playerComp = _entity->GetComponent<PlayerComponent>();
			// ���� ü�� �ۼ�Ʈ ����
			float currentHP = _entity->GetComponent<HealthBarComponenet>().mHealthPercentage;
			// ü�� ���� ���� �� ���� ü�� ������ �����ϰ� ����
			playerComp.mMaxHP = statusTable.maxHP;
			playerComp.mHP = playerComp.mMaxHP * currentHP;
			_entity->GetComponent<Transform>().m_localScale = Vector3(statusTable.unitSize * pow(mBuffUnitSizeCoef, _buffLv));
			//_entity->GetComponent<CapsuleCollider>().m_radius = 30.f * _entity->GetComponent<Transform>().m_localScale.x * pow(mBuffUnitSizeCoef, _buffLv);
			playerComp.mAttackPower = statusTable.attackPower;
			playerComp.mAttackSpeed = statusTable.attackSpeed;
			playerComp.mMoveOriSpeed = statusTable.moveSpeed;
			playerComp.mMoveSpeed = playerComp.mMoveOriSpeed;
			playerComp.mAttackRange = statusTable.attackRange;
			playerComp.mRecogRange = statusTable.recognitionRange;
			playerComp.mBuffSpeed = statusTable.buffSpeed;
		}
	}
}

void StatusManager::UpdateStautsNClass(std::shared_ptr<Entity> _entity, const std::string& _classType)
{
	for (const auto& statusTable : mpStatusTableVec)
	{
		if (statusTable.className == _classType)
		{
			auto& playerComp = _entity->GetComponent<PlayerComponent>();
			playerComp.mPlaceMoney = statusTable.placeMoney;
			playerComp.mMaxHP = statusTable.maxHP;
			playerComp.mHP = playerComp.mMaxHP;
			_entity->GetComponent<Transform>().m_localScale = Vector3(statusTable.unitSize * pow(mBuffUnitSizeCoef, 0));
			playerComp.mAttackPower =  statusTable.attackPower;
			playerComp.mAttackSpeed = statusTable.attackSpeed;
			playerComp.mMoveOriSpeed = statusTable.moveSpeed;
			playerComp.mMoveSpeed = playerComp.mMoveOriSpeed;
			playerComp.mAttackRange = statusTable.attackRange;
			playerComp.mRecogRange = statusTable.recognitionRange;
			playerComp.mBuffSpeed = statusTable.buffSpeed;
			playerComp.mBuffMoney1 = statusTable.buffMoney1;
			playerComp.mBuffMoney2 = statusTable.buffMoney2;
		}
	}

	mpClassManager->UpdateUnitMesh(_entity, _classType);
}

std::string StatusManager::ConvertClasstypeToString(ClassType _classTypeEnum)
{
	 return mpClassManager->ConvertClasstypeToString(_classTypeEnum);
}

std::vector<std::pair<std::string, ClassType>>* StatusManager::GetClassTypeVec()
{
	return mpClassManager->GetClassTypeVec();
}