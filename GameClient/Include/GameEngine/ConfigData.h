#pragma once
#include "IData.h"

/// ������ ���� �������� Ŭ�����Դϴ�.
class ConfigData : public IData
{
public:

private:
    float m_volumeMaster;
    float m_volumeBGM;
    float m_volumeSFX;

    bool m_windowed;
    uint16_t m_windowWidth;
    uint16_t m_windowHeight;

    uint8_t m_graphicsQuality;

    float m_frame;

};

