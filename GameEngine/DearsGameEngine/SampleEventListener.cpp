#include "EventListener.h"
#include "EventManager.h"
#include "Event.h"

/// <summary>
/// �̺�Ʈ �������� ��� ���� Ŭ�����Դϴ�.
/// </summary>
class TestObject : public EventListener
{
public:
	TestObject(EventManager* _pEventManager)
		:EventListener(_pEventManager)
	{
		// �ﰢ���� �̺�Ʈ ���
		REGISTER_IMMEDIATE_EVENT("TestImmediateEventHandler", TestImmediateEventHandler);
		// ������ �̺�Ʈ ���
		REGISTER_DELAYED_EVENT("TestDelayedEventHandler", TestDelayedEventHandler);
	}

	void Update()
	{
		// �ﰢ���� �̺�Ʈ Ʈ����
		m_pEventManager->TriggerEvent(Event("TestImmediateEventHandler"));
		// ������ �̺�Ʈ�� �̺�Ʈ ť�� Ǫ��
		m_pEventManager->PushDelayedEvent(Event("TestImmediateEventHandler"), 1);
	}

	// �ﰢ������ ó���� �̺�Ʈ �ڵ鷯 �Լ��Դϴ�.
	void TestImmediateEventHandler(const Event& _event)
	{
		DLOG(LOG_INFO, "TestImmediateEventHandler");
	}

	// �����ǰ� ó���� �̺�Ʈ �ڵ鷯 �Լ��Դϴ�.
	void TestDelayedEventHandler(const Event& _event)
	{
		DLOG(LOG_INFO, "TestDelayedEventHandler");
	}

};