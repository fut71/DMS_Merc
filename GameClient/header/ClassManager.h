#pragma once
#include "ClassTypeEnum.h"
#include "Entity.h"
#include "CharacterMeshInfo.h"
#include "PlayerComponent.h"
#include "EnemyComponent.h"
#include "ResourceManager.h"

/// <summary>
/// ������ Ŭ������ �����ϰ�, �ִϸ��̼�, ��, �⺻ �޽��� �־��ݴϴ�.
/// </summary>
class ClassManager
{
public:
	ClassManager(EntityManager* _pEntityManager, ResourceManager* _pResourceManager);
	~ClassManager();

	// ������ Ŭ���� ������ �ҷ��� �����Ѵ�.
	void Initialize();

	// 

	/// <summary> ������ �⺻ �޽� ������ ã�Ƽ� �߰��Ѵ�. </summary>
	/// <param name="_entity">���� ��ƼƼ</param> <param name="_classType">Ŭ����</param>
	void AddUnitMesh(std::shared_ptr<Entity> _entity, const std::string& _classType);

	/// <summary> ��ġ ���¿� Ŭ���� ��ư�� �ٸ� ���� ���� �� ������ �⺻ �޽� ������ �����Ѵ�. </summary>
	/// <param name="_entity">���� ��ƼƼ</param> <param name="_classType">Ŭ����</param>
	void UpdateUnitMesh(std::shared_ptr<Entity> _entity, const std::string& _classType);

	// ClassTypeVec�� ��ȯ�Ѵ�.
	std::vector<std::pair<std::string, ClassType>>* GetClassTypeVec();

public:
	/// <summary> �̳� Ÿ���� Ŭ������ string ������ ��ȯ�մϴ�. </summary>
	/// <param name="_classTypeEnum">ClassType �̳�</param> <returns>std::string</returns>
	std::string ConvertClasstypeToString(ClassType _classTypeEnum);

private:
	std::vector<CharacterMeshInfo> mpClassMeshInfoVec;
	// �����Ͱ� ��� �ʿ��� Vector�� �ٲ��, ���� ������ �������� �ʾ� ���ͷ� ����
	std::vector<std::pair<std::string, ClassType>> mStringToClassTypeVec; // �޾ƿ� ��Ʈ�� ���� Ŭ����Ÿ������ ��ȯ�ϱ� ���� ��
	ClassType ConvertStringToClasstype(const std::string& _classType);
	ResourceManager* mpResourceManager;
};