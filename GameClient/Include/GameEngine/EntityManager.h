#pragma once
#include "Entity.h"
#include "Rigidbody.h"
#include "Transform.h"

class EntityManager
{
public:
	EntityManager(entt::registry& _registry);
	~EntityManager() {};

	/// <summary>
	/// ��ƼƼ�� ��ȯ�մϴ�.
	/// </summary>
	/// <param name="_uid">��ȯ�� ��ƼƼ�� UID</param>
	/// <returns>��ƼƼ ��ü</returns>
	std::shared_ptr<Entity> GetEntity(const UID& _uid);

	std::shared_ptr<Entity> GetEntity(const entt::entity& _uid);

	/// <summary>
	/// ���� ��ƼƼ ���� ��ȯ�մϴ�.
	/// </summary>
	/// <returns>��ƼƼ ��</returns>
	std::unordered_map<UID, std::shared_ptr<Entity>> GetEntityMap() const;

	/// <summary>
	/// ���ο� ��ƼƼ�� �����մϴ�.
	/// </summary>
	/// <param name="_name">��ƼƼ ��ü�� �̸�</param>
	/// <returns>������ ��ƼƼ ��ü</returns>
	std::shared_ptr<Entity> CreateEntity(const std::string& _name);

	/// <summary>
	/// �� ���� ��ƼƼ �߰�
	/// </summary>
	/// <param name="_entity">��ƼƼ ��ü</param>
	void AddEntity(std::shared_ptr<Entity> _entity);

	/// <summary>
	/// ��ƼƼ�� ������ �����մϴ�.
	/// </summary>
	/// <param name="_uid">������ ��ƼƼ�� UID</param>
	void RemoveEntity(const UID& _uid);

	/// <summary>
	/// ��� ��ƼƼ�� ������ �����մϴ�.
	/// </summary>
	void RemoveAllEntities();

//	std::shared_ptr<Entity> CreateEntity(entt::registry& _registry, const std::string& _name)
//	{
//		auto entity = std::make_shared<Entity>(_registry, _name);
//		m_entities[entity->GetUID()] = entity;
//		return entity;
//	}
//
//	void DestroyEntity(const UID& id)
//	{
//		auto it = m_entities.find(id);
//		if (it != m_entities.end())
//		{
//// 			it->second->Finalize();
//			m_entities.erase(it);
//		}
//	}
//
//	std::shared_ptr<Entity> GetEntity(const UID& id)
//	{
//		auto it = m_entities.find(id);
//		if (it != m_entities.end())
//		{
//			return it->second;
//		}
//		return nullptr;
//	}
//
//	void Update()
//	{
//		for (auto& [id, entity] : m_entities)
//		{
//// 			entity->Update();
//		}
//	}
//
//	void Clear()
//	{
//		for (auto& [id, entity] : m_entities)
//		{
//// 			entity->Finalize();
//		}
//		m_entities.clear();
//	}

private:
	//std::unordered_map<UID, std::shared_ptr<Entity>> m_entities;
	entt::registry& mRegistry;										// ��ƼƼ ������Ʈ��
	std::unordered_map<UID, std::shared_ptr<Entity>> mpEntities;	// ���� �����ϴ� ��ƼƼ ����

};