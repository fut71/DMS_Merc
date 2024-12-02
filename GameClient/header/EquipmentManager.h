#pragma once
#include "ClassTypeEnum.h"
#include "FilterEnum.h"
#include "EquipmentMeshInfo.h"
#include "WeaponMeshInfo.h"
#include "EntityManager.h"
#include "PhysicsManager.h"
#include "RenderManager.h"
#include "ResourceManager.h"

/// <summary>
/// ���� �Ŵ����� �߰��߽��ϴ�... ���Ŀ� ���ӿ����� �����ϰ� ���⼭�� �̾Ƴ��ϴ�.
/// </summary>
class EquipmentManager
{
public:
	EquipmentManager(entt::registry& _registry, EntityManager* _pEntityManager
		, PhysicsManager* _pPhysicsManager, RenderManager* _pRenderManager, ResourceManager* _pResourceManager);
	~EquipmentManager();

public:
	// ������ Ŭ���� ������ �ҷ��� �����Ѵ�.
	void Initialize();

	// �̹� ��ġ�� ������ ������ ��� �߰� �Լ�
	void AddInitUnitEquipment(std::shared_ptr<Entity> _entity, const std::string& _classType);

	// NPC�� ������ ��� �߰� �Լ�
	void AddNPCEquipment(std::shared_ptr<Entity> _entity, const std::string& _classType);

	// ��ġ�� �� ���� ��� �߰� �Լ�
	void AddUnitEquip(std::shared_ptr<Entity> _entity, const std::string& _classType, const int& _buffLv);

	// ����� ��ġ�� �Ϸ��ؾ߸� �����ǰԲ��Ѵ�.
	void AddWeapon(std::shared_ptr<Entity> _entity, const std::string& _classType);

	// ����� ��ü�� ���õ� �Լ�
	void UpdateEquipment(std::shared_ptr<Entity> _entity, const std::string& _classType, const int& _buffLv);

	// �ӽ÷� ����� �ļ� �Լ�
		// Csv �ļ� -------------------------------------------------------
	/// <summary> ù ���� �����ϰ� �д� string ���� ���Ե� cvs ������ ���� ��� ���� �ļ� </summary>
	/// <typeparam name="T">���̺� Info</typeparam> <param name="_fileName">���� �̸�</param> <returns>std::vector<T></returns>
	template <typename T>
	std::vector<T> ParseEmptyCSV(const std::string& _fileName);

private:
	entt::registry& mRegistry;
	EntityManager* mpEntityManager;
	std::map<UID, std::vector<UID>> mUnitEquipmentMap; // ���ֿ� �����ϴ� ������ ���� ��(���� ������ ���ĵǴ� �� ���Ƽ� Map���� ����)


	std::vector<EquipmentMeshInfo> mpEquipmentMeshVec;
	std::vector<WeaponMeshInfo> mpWeaponMeshVec;

	// �迭
	std::array<std::pair<int, std::string>, 14> mEquipPart;

	int asc[2];

	PhysicsManager* mpPhysicsManager;
	RenderManager* mpRenderManager;
	ResourceManager* mpResourceManager;

	// ���⸦ ������ ��� ������ �Լ�
	void TestWearEquipment(std::shared_ptr<Entity> _entity, const EquipmentMeshInfo& _info, const std::string& _targetBoneName, bool _isInitalize);

	// ���⸦ �����ϴ� �Լ�
	void WearWeapon(std::shared_ptr<Entity> _entity, const WeaponMeshInfo& _info, bool _isInitalize);
};

template <typename T>
void DEBUG_Print(const T& _message)
{
#ifdef _DEBUG
	std::cout << _message << std::endl;
#endif
}

template <typename T>
std::vector<T> EquipmentManager::ParseEmptyCSV(const std::string& _fileName)
{
	std::vector<T> units = {};
	std::string filepath = mpResourceManager->GetFileManager()->GetFullFilePath(_fileName);
	std::ifstream file(filepath);
	std::string line;

	// ���� ���� ���� �� �� ���� ��ȯ
	if (!file.is_open())
	{
		std::cerr << "Failed to open file: " << filepath << std::endl;
		return units;
	}

	int columnCount = 0;
	// ù ��° ��(���)�� �о���� -> Į�� ���� ī��Ʈ
	if (std::getline(file, line))
	{
		std::stringstream ss(line);
		std::string column;

		// ��ǥ�� ���е� �� �ʵ带 ���� Į�� ���� ī��Ʈ
		while (std::getline(ss, column, ','))
		{
			columnCount++;
		}
	}

	// �� ��° �ٺ��� �о��ش�.
	while (std::getline(file, line))
	{
		std::stringstream ss(line);
		std::string cell;
		std::vector<std::string> row;
		int currentColumnCount = 0; // ������ ó���ϱ� ���� �ҿ����� ���� ��ŵ�ϴ� ���� �����ϱ� ���� ���� �߰�

		// ��ǥ�� ���е� �� �ʵ带 ó��
		while (std::getline(ss, cell, ','))
		{
			if (cell.empty())
			{
				row.push_back(""); // �� ���� ""�� ����
			}
			else
			{
				row.push_back(cell);
			}
		}
		// ���� ������ Į���� �ִٸ� ""�� ä���.
		while (currentColumnCount < columnCount)
		{
			row.push_back("");  // ������ Į���� ""�� ä��
			currentColumnCount++;
		}

		if (row.size() >= columnCount)
		{  // ��� Į���� �ִ��� Ȯ��
			units.emplace_back(row);
		}
		else
		{
			std::cerr << "Incomplete row found and skipped." << std::endl;
		}
	}
	return units;
}