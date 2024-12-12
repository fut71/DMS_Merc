#pragma once
#include "pch.h"
#include "Component.h"
#include "Utility.h"
#include <mutex>
#include <system_error>

class Scene;

/// <summary>
/// 
/// </summary>
class Entity
	: public std::enable_shared_from_this<Entity>
{
public:
	// �⺻ ������ ����
	Entity() = delete;

	Entity(entt::registry& _registry);

	// ���� �����ڿ� ���� �Ҵ� �����ڸ� �����մϴ�.
	Entity(const Entity&) = delete;
	Entity& operator=(const Entity&) = delete;

	// �̵� �����ڿ� �̵� �Ҵ� �����ڸ� �߰��մϴ�.
	Entity(Entity&&) noexcept = delete;
	Entity& operator=(Entity&&) noexcept = delete;

	~Entity();

	/// <summary>
	/// ��ƼƼ�� �ʱ�ȭ�մϴ�.
	/// </summary>
	/// <returns>�ʱ�ȭ ���н� false ��ȯ</returns>
	bool Initialize(const std::string& _name);

	/// <summary>
	/// ��ƼƼ�� �ı��մϴ�.
	/// </summary>
	void Destroy();

	/// <summary>
	/// ��ƼƼ�� ������Ʈ�� �߰��մϴ�.
	/// </summary>
	/// <typeparam name="T">������Ʈ Ÿ��</typeparam>
	/// <param name="...args">������Ʈ ���� ����</param>
	/// <returns>�߰��� ������Ʈ ��ȯ</returns>
	template <IsComponent T, typename... Args>
	T& AddComponent(Args&&... args)
	{
		if (HasComponent<T>())
		{
			DLOG(LOG_ERROR, "Component already Added.");
			return GetComponent<T>();
		}
		auto self = shared_from_this();
		return m_registry.emplace<T>(m_entity, self, std::forward<Args>(args)...);
	}

	/// <summary>
	/// ��ƼƼ�� �ش� ������Ʈ�� �������ִ��� Ȯ���մϴ�.
	/// </summary>
	/// <typeparam name="T">������Ʈ Ÿ��</typeparam>
	/// <returns>������Ʈ ���� ����</returns>
	template <IsComponent T>
	bool HasComponent() const
	{
		return m_registry.all_of<T>(m_entity);
	}

	/// <summary>
	/// ��ƼƼ�� ������Ʈ�� ��ȯ�մϴ�.
	/// </summary>
	/// <typeparam name="T">������Ʈ Ÿ��</typeparam>
	/// <returns>������Ʈ ����</returns>
	template <IsComponent T>
	T& GetComponent()
	{
		if (HasComponent<T>())
		{
			return m_registry.get<T>(m_entity);
		}
		throw std::runtime_error("Component not found");
	}

	/// <summary>
	/// ��ƼƼ�� ������Ʈ���� ���͸� ��ȯ�մϴ�.
	/// </summary>
	/// <typeparam name="T">������Ʈ Ÿ��</typeparam>
	/// <returns>������Ʈ ����</returns>
// 	template<typename T>
// 	std::vector<std::shared_ptr<T>> GetAllComponents()
// 	{
// 		std::vector<std::shared_ptr<T>> components;
// 		m_registry.view<T>().each([&components](auto entity, T& component) {components.push_back(std::make_shared<T>(component)); });
// 		return components;
// 	}

	/// <summary>
	/// ��ƼƼ�� ������Ʈ�� �����մϴ�.
	/// </summary>
	/// <typeparam name="T">������Ʈ Ÿ��</typeparam>
	template <IsComponent T>
	void RemoveComponent()
	{
		m_registry.remove<T>(m_entity);
	}

	/// <summary>
	/// ��ƼƼ�� �����մϴ�.
	/// ���� ������ ��ƼƼ���� �����Ҷ� ����մϴ�.
	/// </summary>
	/// <returns>����� ��ƼƼ ��ü</returns>
	std::shared_ptr<Entity> Clone() const;

	/// <summary>
	/// �� ��ƼƼ�� �������� ���� ��ȯ�մϴ�.
	/// </summary>
	/// <returns>�������� �� ��ü</returns>
	std::shared_ptr<Scene> GetOwner() const;

	/// <summary>
	/// ���� �ĺ��ڸ� ��ȯ�մϴ�.
	/// </summary>
	/// <returns>��ƼƼ UID</returns>
	UID GetUID() const;

	/// <summary>
	/// ��ƼƼ�� �̸��� ��ȯ�մϴ�.
	/// </summary>
	/// <returns></returns>
	std::string GetName() const;

private:
public:
	// 	UID m_uid;						// ��ü �����ĺ���, ��ƼƼ �����ÿ� �ڵ����� �ο��˴ϴ�.
	// 	std::string m_name;				// ��ü �̸�
	std::weak_ptr<Scene> m_owner;	// �� ������Ʈ�� �����ϰ��ִ� ��
	entt::registry& m_registry;		// ��ƼƼ�� ������Ʈ�� �����ϴ� ��ƼƼ ������Ʈ��
	entt::entity m_entity;			// ��ƼƼ �ڵ�

};

