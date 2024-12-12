#pragma once
#include <directxtk/SimpleMath.h>

using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Vector3;

class Camera
{
public:
	Camera(int _screenWidth, int _screenHeight);
	~Camera();

private:
	int mScreenWidth;	// ��ũ�� �ʺ�
	int mScreenHeight;	// ��ũ�� ����

	int mOrthoGraphicScreenWidth = 192;
	int mOrthoGraphicScreenHeight = 108;

	float mAspect;		// ȭ�� ����
	float mSpeed;		// �����̴� �ӵ�

	int mPrevMouseX;	// �� �����Ӹ��� ���콺�� �����̴� ������ ����ϱ� ���� ����(���콺 x)
	int mPrevMouseY;	// �� �����Ӹ��� ���콺�� �����̴� ������ ����ϱ� ���� ����(���콺 y)
public:
	Vector3 mViewPos;	// ���� ��ǥ�迡�� ī�޶��� ��ġ

	Vector3 mViewDir;	// ī�޶� ���� ����, �ɾ�� ����
	Vector3 mViewUp;	// ���� �ð��� ���� ������ �� ���� 
	Vector3 mRightDir;	// ���� �ð��� ���� ������ ������ ���� (�������� ����)

	// roll, pitch, yaw
	// https://en.wikipedia.org/wiki/Aircraft_principal_axes
	float mPitch;			//x�� ���� ȸ��
	float mYaw;				//y�� ���� ȸ��
	float mRoll;			//z�� ���� ȸ��

	float mProjFovAngleY;
	float mNearZ;
	float mFarZ;

	bool isPerspective;
public:
	//[ȭ�� �ʺ�, ȭ�� ����] ȭ�� ���� ����-> ���� �������� �ٲٱ� ������ ī�޶� ��� ���� ���� �ٲ۴�.
	void SetAspectRatio(int _screenWidth, int _screenHeight);

	// [��ġ]���� ��ġ�� ����
	void SetEyePos(Vector3 _pos);

	// [�ӵ�]ī�޶��� �ӵ� ����
	void SetSpeed(float _speed);
	void SetPerspective();
	void SetOrthgraphic(float _scale);
	// [�� ����, �� ����] ī�޶��� ��, ��, right ����
	void SetDirection(Vector3 _viewDir);
	void SetUpVec(Vector3 _viewUp);

	///�� ģ�� �� ����..
	// ī�޶��� ����� ��ȯ(xy�� ������)
	Matrix GetViewRow();

	// ī�޶��� ������� ��ȯ(yz�� ������)
	Matrix GetProjRow();

	// ī�޶��� �ӵ� ��ȯ
	float GetSpeed();

	//ī�޶��� ���� ��ġ ��ȯ
	Vector3 GetmViewPos();


	// [roll(��), pitch(��), yaw(��)] �װ��� ���� �� ���� // ���⿡�� y : ���� ȸ�� ,  z : �¿� ȸ�� [���ʿ�x]
	void SetAircraftAxes(float _yaw, float _pitch, float _roll);

	// [Y�þ߰�, nearZ, farZ] �������� ��� ������ ���� ����
	void ProjectionSettings(float _angle, float _nearZ, float _farZ);

public:
	// [��ġx, ��ġy] ���콺 ������ ������Ʈ
	void UpdateFrameMouse(float deltaX, float deltaY);
	// ȸ���� mViewDir�� ������ ����Ѵ�.
	void UpdateViewDir();
	// [��Ÿ Ÿ��] ������ �̵�
	void MoveForward(float dt);
	// [��Ÿ Ÿ��] ���� �̵�
	void MoveUp(float dt);
	// [��Ÿ Ÿ��] ���������� �̵�
	void MoveRight(float dt);

public:
	// ī�޶��� ���� ��ġxyz�� ���� ����xyz ���
	void PrintCamInfo();
	// [��ġx, ��ġy] ���콺�� ���� ȭ�� ���� ����
	void OnMouseMove(int _mouseX, int _mouseY);

public:
	bool mIsFirstPersonMode;	// 1��Ī ���� ��� ����
};
