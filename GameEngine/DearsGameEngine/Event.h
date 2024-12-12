#pragma once
#include "pch.h"

// �̺�Ʈ ������ Ÿ�� ���� std::any_cast�� ���� Ư�� Ÿ������ ĳ���� ����
// std::optional�� ���� �����Ͱ� ���� ��쵵 ó���Ҽ� �ֽ��ϴ�.
using EventData = std::optional<std::any>;

/// <summary>
/// �̺�Ʈ �̸��� �̺�Ʈ �����͸� ������ �ִ� �̺�Ʈ Ŭ�����Դϴ�.
/// </summary>
class Event
{
public:
	Event() = default;	// ����Ʈ ������

	// �̺�Ʈ�� �̸��� �̺�Ʈ �����͸� �Ű������� �޴� ������, data�� ����Ʈ���� null�Դϴ�.
	Event(const std::string& _name, EventData _data = std::nullopt)
		:m_name(_name), m_data(_data)
	{
	}

	template <typename T>
	std::optional<T> GetDataAs() const
	{
		if (m_data->has_value())
		{
			try
			{
				return std::any_cast<T>(m_data.value());
			}
			catch (const std::bad_any_cast& e)
			{
				DLOG(LOG_WARN, std::string("Bad any cast: ") + std::string(e.what()));
			}
		}
		return std::nullopt;
	}	

	std::string m_name;	// �̺�Ʈ �̸�

private:
	EventData m_data = std::nullopt;	// �̺�Ʈ ó���� ���Ǵ� ������

	friend class EventManager;
	friend class EventListener;

};
