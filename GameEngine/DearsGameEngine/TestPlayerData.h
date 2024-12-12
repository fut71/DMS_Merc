#pragma once
#include "IData.h"

/// <summary>
/// ������ ���� �� �����ϴ� �������� ���� Ŭ�����Դϴ�.
/// ������ ���� ������ ���� ���� �����͸� ��ȣȭ�Ͽ� �����մϴ�.
/// </summary>
class TestPlayerData : public IData
{
public:
    TestPlayerData() {}
    ~TestPlayerData() {}

    void Serialize();
    void Deserialize();

private:
    int m_gold;
    int m_wood;
    int m_iron;
    float m_level;

};

