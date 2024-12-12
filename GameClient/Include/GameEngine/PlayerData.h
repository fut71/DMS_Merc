#pragma once
#include "IData.h"

/// <summary>
/// ������ ���� �� �����ϴ� �������� ���� Ŭ�����Դϴ�.
/// ������ ���� ������ ���� ���� �����͸� ��ȣȭ�Ͽ� �����մϴ�.
/// </summary>
class PlayerData : public IData
{
public:
    PlayerData() {}
    ~PlayerData() {}

    void Serialize();
    void Deserialize();

private:
    int m_gold;
    int m_wood;
    int m_iron;
    float m_level;

};

