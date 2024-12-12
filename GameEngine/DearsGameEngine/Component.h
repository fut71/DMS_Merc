#pragma once
#include "pch.h"
#include "MetaData.hpp"

class Entity;

/// <summary>
/// ������Ʈ�� �������̽� Ŭ�����Դϴ�.
/// </summary>
class Component
{
public:
	//Component() = default;
	virtual ~Component() = default;

	/// <summary>
	/// ������Ʈ�� �������� ��ƼƼ�� �̸�/uid���� �˱� ���� ������
	/// </summary>
	/// <param name="_owner"></param>
	Component(std::shared_ptr<Entity> _owner) :m_pOwner(_owner) {}

	/// <summary>
	/// ������Ʈ ���� �޼���	// ���� ������ �����鰰��
	/// std::shared_ptr<Component> Clone() const override {	return std::make_shared<T>(*this);	}	// Ŭ�� �ż��带 ���� �������
	/// </summary>
	/// <returns></returns>
	virtual std::shared_ptr<Component> Clone() const = 0;

	std::string GetOwnerName() const;

 	std::shared_ptr<Entity> m_pOwner;
};



/// <summary>
/// ������Ʈ�� ����Ʈ ����, ���̽� Ŭ������ Component���� Ȯ���մϴ�.
/// </summary>
template <typename T>
concept IsComponent = std::is_base_of<Component, T>::value;

