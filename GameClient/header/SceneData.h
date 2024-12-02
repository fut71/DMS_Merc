#pragma once
#include "IData.h"

class SceneData
	: public IData
{
public:

public:
	float m_totalTime = 90.f;
	float m_time = 90.f;			// ���� ���� �ð�
	uint8_t m_aliveAlly = 0;	// ������ �Ʊ� �뺴 ��
	uint8_t m_totalAlly = 0;	// ������ �Ʊ� �뺴 ��
	uint32_t m_heldMoney = 3000;	// ���� �ݾ�
	uint32_t m_usedAmount = 0;	// ���� �ݾ�
	uint32_t m_remainAmount = 0;// ���� �ݾ�
	uint32_t m_totalAmount = 3000;

	uint16_t m_rankA = 1000;
	uint16_t m_rankB = 500;

};

