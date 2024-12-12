#pragma once
#include "Entity.h"
#include "StatusTable.h"
#include "Transform.h"
#include "PlayerComponent.h"
#include "EnemyComponent.h"
#include "HealthBarComponenet.h"
#include "ClassManager.h"

/// <summary>
/// ������ �������ͽ��� �����ϴ� Ŭ�����Դϴ�.
/// </summary>
class StatusManager
{
public:
	StatusManager(EntityManager* _pEntityManager, ResourceManager* _pResourceManager);
	~StatusManager();

	// ������ Ŭ���� ������ �ҷ��� �����Ѵ�.
	void Initialize();

	/// <summary> �Ʊ�(��ġ�Ǵ� ��) �������ͽ��� �߰��Ѵ�. </summary>
	/// <param name="_entity">���� ��ƼƼ</param> <param name="_classType">Ŭ����</param> <param name="_level">���� ����</param>
	void AddStatus(std::shared_ptr<Entity> _entity, const std::string& _classType, const int& _buffLv);

	/// <summary> ����(�̸� ������ ����) �������ͽ��� �߰��Ѵ�. </summary>
	/// <param name="_entity">���� ��ƼƼ</param> <param name="_classType">Ŭ����</param>
	void AddInitStatus(std::shared_ptr<Entity> _entity, const std::string& _classType);

	/// <summary> �������ͽ��� �ٲ�� ������Ʈ�Ѵ�. </summary>
	/// <param name="_entity">���� ��ƼƼ</param> <param name="_classType">Ŭ����</param> <param name="_buffLv">���� ����</param>
	void UpdateAllyStatus(std::shared_ptr<Entity> _entity, const std::string& _classType, const int& _buffLv);

	/// <summary> Ŭ������ ����Ǵ� Ư���� ��쿡 ������Ʈ�Ѵ�.</summary>
	/// <param name="_entity">���� ��ƼƼ</param> <param name="_classType">Ŭ����</param>
	void UpdateStautsNClass(std::shared_ptr<Entity> _entity, const std::string& _classType);

	/// <summary> �̳� Ÿ���� Ŭ������ string ������ ��ȯ�մϴ�. </summary>
	/// <param name="_classTypeEnum">ClassType �̳�</param> <returns>std::string</returns>
	std::string ConvertClasstypeToString(ClassType _classTypeEnum);

	/// <summary> ClassTypeMap�� Ŭ���� �Ŵ������� �������� �Լ� </summary>
	/// <returns>std::vector&lt;std::pair&lt;std::string, ClassType&gt;&gt;*</returns>
	std::vector<std::pair<std::string, ClassType>>* GetClassTypeVec();

	/// <summary> ���⼭���ʹ� ���� ����� �����Ѵ�.(��ȹ ��) </summary>
	float mMoneyBuffCoef = 1.5f;	// ������ �� ���Ǵ� ��ġ
	float mBuffUnitSizeCoef = 1.4f; // ���� ����� �� ���� �� ���Ǵ� ��ġ

private:
	ClassManager* mpClassManager;
	ResourceManager* mpResourceManager;

	// CSV �ļ��� �ҷ��� ���̺� ����
	std::vector<StatusTable> mpStatusTableVec;
};

