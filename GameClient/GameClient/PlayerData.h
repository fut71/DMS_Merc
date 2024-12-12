#pragma once
#include "IData.h"

class PlayerData :
	public IData
{
public:
	uint32_t GetPlayerMoney() const { return m_playerMoney; }
	void SetPlayerMoney(const uint32_t& val) { m_playerMoney = val; }
	uint8_t GetMoneyGunLevel() const { return m_moneyGunLevel; }
	void SetMoneyGunLevel(const uint8_t& val) { m_moneyGunLevel = val; }
	std::unordered_set<uint8_t> GetClearedStatges() const { return m_clearedStatges; }

private:
	uint32_t m_playerMoney;     // �������� ��
	uint8_t m_moneyGunLevel;    // �������� �Ӵϰ��� ����;
	std::unordered_set<uint8_t> m_clearedStatges;   // Ŭ������ �������� �ؽ�����

};

 