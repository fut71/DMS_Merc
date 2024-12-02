#include "EventListener.h"
#include "EventManager.h"

// ��ü�� �����ڿ��� �̺�Ʈ �Ŵ����� ������ ����
EventListener::EventListener(EventManager* _eventManager)
	: m_pEventManager(_eventManager)
{
}

// ��ü�� �Ҹ�ɶ�, �ش� ��ü�� ����� ��� �̺�Ʈ���� ��������˴ϴ�.
EventListener::~EventListener()
{
// 	for (const auto& [name, id] : m_registeredImmediateHandlers)
// 	{
// 		m_pEventManager->UnregisterEvent(name, id);
// 	}
// 	for (const auto& [name, id] : m_registeredDelayedHandlers)
// 	{
// 		m_pEventManager->UnregisterEvent(name, id);
// 	}
}


// �ﰢ���� �̺�Ʈ�� �̺�Ʈ �Ŵ����� ����մϴ�.
HandlerID EventListener::RegisterImmediateEvent(const std::string& _eventName, EventHandler _handler)
{
	HandlerID id = m_pEventManager->RegisterImmediateEvent(_eventName, std::move(_handler));
	m_registeredImmediateHandlers[_eventName] = id;
	return id;
}

// ������ �̺�Ʈ�� �̺�Ʈ �Ŵ����� ����մϴ�.
HandlerID EventListener::RegisterDelayedEvent(const std::string& _eventName, EventHandler _handler)
{
	HandlerID id = m_pEventManager->RegisterDelayedEvent(_eventName, std::move(_handler));
	m_registeredDelayedHandlers[_eventName] = id;
	return id;
}

// �̺�Ʈ �Ŵ����� ��ϵǾ��ִ� �̺�Ʈ�� �����մϴ�.
void EventListener::UnregisterEvent(const std::string& _eventName)
{
	auto it = m_registeredImmediateHandlers.find(_eventName);
	if (it != m_registeredImmediateHandlers.end())
	{
		m_pEventManager->UnregisterEvent(_eventName, it->second);
		m_registeredImmediateHandlers.erase(it);
	}
	it = m_registeredDelayedHandlers.find(_eventName);
	if (it != m_registeredDelayedHandlers.end())
	{
		m_pEventManager->UnregisterEvent(_eventName, it->second);
		m_registeredDelayedHandlers.erase(it);
	}
}
