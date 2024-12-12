#pragma once
#include "DearsGameEngineAPI.h"

/// <summary>
/// ī�޶� �̵� ���⿡ ���� Ư�� ��ġ, ������
/// </summary>
struct CameraPosition
{
	// perspective
	Vector3 mainPositionPerspective = Vector3(-16.46, 18.06, -16.62);
	Vector3 mainRotationPerspective = Vector3(0, -0.8, 0.6);

	// orthographic
	Vector3 mainPosOrtho = Vector3(-16.46, 18.06, -16.62);
	Vector3 mainRotOrtho = Vector3(0.57735, -0.57735, 0.57735);

	// ���� �� ���� ��ġ
	Vector3 startPos = Vector3(0.f, 34.f, -17.5f);
	Vector3 startRot = Vector3(0, -0.93, 0.37);

	// ���� �� 1(�̵�) // ����� Ÿ�� �̵�
	//Vector3 pos1 = Vector3(0, 17, -13.5);
	Vector3 rot1 = Vector3(0, -0.8, 0.6);

	// ���� �� 1(�̵�2) // ����� Ÿ�� �̵�
	Vector3 pos2 = Vector3(0, 6, -1.5);
	Vector3 rot2 = Vector3(0.13, -0.7, 0.52);

	// ���� �� 2(���̵� �ƿ� ��) // �뺴������ ����
	Vector3 pos3 = Vector3(16, 4, 0);
	Vector3 rot3 = Vector3(1, 0, 0);

	// ���� �� 3 �ڷ� ����
	Vector3 pos4 = Vector3(-3, 10, 0);
	Vector3 rot4 = Vector3(0.8, -0.6, 0);

	// ���� ��4(���� ��ȯ ��)
	Vector3 pos5 = Vector3(-16.4, 31.79, -16.77);
	Vector3 rot5 = mainRotOrtho;

	// ������ 5 (���1) : �ڷ� ���� �뺴�� �ٴ� �� ������
	Vector3 pos6 = Vector3(21, 6.4, 0);
	Vector3 rot6 = Vector3(-1, 0, 0);

	// ������ 6 (���2) : �뺴������ ���� ��
	Vector3 pos7 = Vector3(-12.f, 4.0f, 3.35f);
	Vector3 rot7 = Vector3(-1.f, -0.2f, 0.0f);


};