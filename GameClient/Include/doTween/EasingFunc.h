#pragma once
#include<unordered_map>
#include<string>
#include<functional>

#define PI 3.141592

class EasingFunc
{
public:
	EasingFunc();
	~EasingFunc();
public:
	void Initialize();
	std::unordered_map<std::string, std::function<void(float start, float end, float time, float* result)>> EasingFuncMap;

	bool TimeCheckHelper(float time, float end, float* result);
};

/// Ŭ�󿡼� cpp�� �� �� �����Ƿ� �̸��� �����[Riun]
// ����Ʈ ���� : https://www.researchgate.net/figure/Image-of-set-of-Penners-easing-functions_fig3_308007569
// �ӵ��� ��ȭ ���� : Sine < Cubic < Quint < Circ < Back
// easeInSine : ���� ����
// easeOutSine : ���� ����
// easeInOutSine : ���� �����ϴ� ���� ����
// easeInCubic : ���� ����
// easeOutCubic : ���� ����
// easeInOutCubic : ���� �����ϴ� ���� ����
// easeInQuint : ���� ����
// easeOutQuint : ���� ����
// easeInOutQuint : ���� �����ϴ� ���� ����
// easeInCirc : ���� ����
// easeOutCirc : ���� ����
// easeInOutCirc : ���� �����ϴ� ���� ����
// easeInElastic : �ӵ��� ���� ���� �۴ٰ� �������� �����ϸ� Ŀ��
// easeOutElastic : �ʹݿ� �����ϰ�, �ӵ��� ���� ���� ũ�ٰ� �۾���
// easeInOutElastic : �߹ݿ� �����ϰ�, �ӵ��� ���� ���� �۴� ũ�� �۾���
// easeInBack : ���� ����(�� ���� �ְ����� �ƴ�)
// easeOutBack : ���� ����(�� ���� �ְ����� �ƴ�)
// easeInOutBack : ���� �����ϴ� ���� ����(�� ���� �ְ����� �ƴ�)
// easeInBounce : ���� �� Ƣ�� ����
// easeOutBounce : Ƣ������� ������ �ݴ��� ��
// easeInOutBounce : �߹��� ���� ���� Ƣ������� ��