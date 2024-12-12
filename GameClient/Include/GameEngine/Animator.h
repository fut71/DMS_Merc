#pragma once
#include "Entity.h"
#include "MeshRenderer.h"
#include "Graphics/DearsGraphicsEngine.h"

/// <summary>
/// [Riun] 2024.8.31.
/// �� Ŭ������ Ŭ���̾�Ʈ���� �ִϸ��̼� �Ŵ����� ��� ������ �˰� �ϴ� ���� �����ϰ��� ���� Ŭ�����Դϴ�.
/// Ŭ���̾�Ʈ������ �� �ִϸ������� ���븸 �� �� �ְ�, �� �ܿ��� �������� ���ϰ� �մϴ�.
/// </summary>
/// 
class Animator
{
public:
	Animator(std::shared_ptr<DearsGraphicsEngine> _graphicsEngine);
	~Animator();

	float GetCoolTime(std::shared_ptr<Entity> _entity);

	bool IsAnimatonFinished(std::shared_ptr<Entity> _entity);


private:
	std::shared_ptr<DearsGraphicsEngine> mpGraphicsEngine;	// �׷��� ���� ��ü


};

