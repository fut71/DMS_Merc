#pragma once
#include "BufferData.h"
#include <directxTK/SimpleMath.h>
#include <string>

using DirectX::SimpleMath::Vector3;

class LightHelper
{
public:
	LightHelper();
	~LightHelper() {};

private:
	int mLightMaxNumber;

public:
	// [PS���۱���, ������� ���� ����] ���ϴ� Light�� ������ �����, CommonConstantBufferData�� �����ش�.
	void Initialize(CommonConstantBufferData* _psBufferData, UINT _num);

	// [PS���۱���] Light Update, PS ���� ������Ʈ ���� ȣ���Ѵ�.
	void Update(CommonConstantBufferData* _commonBufferData);

	// [PS���۱���, ������� ���� ����] Light�� ���� ���� -> �̰Ÿ� �߰��� ������ ����� �� �� ���ڴ�... -> ����Ʈ�� �̸��� �߰������ �� �� ����.
	void ChangeLightMaxNum(CommonConstantBufferData* _psBufferData, UINT _num);

	// [PS���۱���] ���� ������Ʈ ������ ���� ������ ���� ���
	void PrintInfo(CommonConstantBufferData* _psBufferData);

public:
	// ����Ʈ ���� ���� ��ü ���� [PS���۱���, ����, ����, �� ���� �϶��� ����, �� ���� �϶��� ��, ����, ��ġ, spotPower]
	void SetLightSettingAll(CommonConstantBufferData* _psBufferData, UINT _index, LightEnum _lightType, float _strength, float _fallOffStart, 
		float _fallOffEnd, Vector3 _dir, Vector3 _pos, float _spotPower, Vector3 _color);

	void SetDirLight(CommonConstantBufferData* _psBufferData, UINT _index, float _strength, Vector3 _dir, Vector3 _color);

	void SetPointLight(CommonConstantBufferData* _psBufferData, UINT _index, float _strength, float _fallOffStart,
		float _fallOffEnd, Vector3 _pos, Vector3 _color);

	void SetSpotLight(CommonConstantBufferData* _psBufferData, UINT _index, float _strength, float _fallOffStart,
		float _fallOffEnd, Vector3 _dir, Vector3 _pos, float _spotPower, Vector3 _color);

	// ���� �ε����� ����Ʈ�� NONE���� �ٲߴϴ�.
	void SetLightOff(CommonConstantBufferData* _psBufferData, UINT _index);


	// [PS���۱���, index, ���� ����(0~1)] ���� ���⸦ ����, 0~1�� ���� �ִ´�.
	void SetStrength(CommonConstantBufferData* _psBufferData, UINT _index, float _strength);
	// [PS���۱���, index, ���� ����] ���� ������ ����
	void SetLightType(CommonConstantBufferData* _psBufferData, UINT _index, LightEnum _lightType);
	// [PS���۱���, index, ���� ����] ���� ���� ����, _dir�� ���� ���ͷ� ���� ��.
	void SetDirection(CommonConstantBufferData* _psBufferData, UINT _index, Vector3 _dir);
	// [PS���۱���, index, �� ���� �϶��� ����] ���� ���� ����
	void SetFallOffStart(CommonConstantBufferData* _psBufferData, UINT _index, float _distance);
	// [PS���۱���, index, �� ���� �϶��� ��] ���� ���� ���� : fall off Start ���� ū ��
	void SetFallOffEnd(CommonConstantBufferData* _psBufferData, UINT _index, float _length);
	// [PS���۱���, index, ���� ��ġ] ���� ��ġ�� ����
	void SetPosition(CommonConstantBufferData* _psBufferData, UINT _index, Vector3 _pos);
	// [PS���۱���, index, ���� spotPower(0~100)] ���� spotPower�� ���� : Ŭ���� ���� �����
	void SetSpotPower(CommonConstantBufferData* _psBufferData, UINT _index, float _power);
	// [PS���۱���, index, ���� ����] ���� ���� ����
	void SetLightColor(CommonConstantBufferData* _psBufferData, UINT _index, Vector3 _rgb);
	// [PS���۱���, ������ ��, ����� �ε���] Ư�� �ε����� �� �����ϱ�
	void CopyLight(CommonConstantBufferData* _psBufferData, UINT _copy, UINT _origin);

private:
	// [ PS���۱���, index] �Լ� ���ο��� ��ȿ�� �ε������� Ȯ���ϱ� ���� ���� �Լ�
	bool CheckIndex(CommonConstantBufferData* _psBufferData, UINT _index);
};