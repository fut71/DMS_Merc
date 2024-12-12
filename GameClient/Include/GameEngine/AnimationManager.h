#pragma once
#include "pch.h"
#include "Entity.h"
#include "MeshRenderer.h"
#include "AnimationComponent.h"
#include "Graphics/DearsGraphicsEngine.h"

/// <summary>
/// �Ŵ������ٴ� ���ۿ� ����� Ŭ�����Դϴ�.
/// [Riun] �� �Ŵ����� ���� �Ŵ����� ���ӵǾ� �ֱ� ������ ����� ���� �ʴ´�.
/// </summary>
class AnimationManager
{
public:
	AnimationManager(entt::registry& _registry, std::shared_ptr<DearsGraphicsEngine> _graphicsEngine);
	~AnimationManager();

	void AnimationUpdate(std::shared_ptr<Entity> _entity, float _dTime);

	/// �ִϸ��̼��� �����ϰ�, �� ���۰� �ִٴ� �����Ͽ��� ����� ��---
	// �ʱ�ȭ, �� ù ������ �ִϸ��̼� ���ÿ� ����ϴ� �Լ��Դϴ�.
	void SetInitialAnimation(std::shared_ptr<Entity> _entity);

	// ������ �߻����� �� �ڵ����� ���� �ִϸ��̼��� ���� �ִϸ��̼����� �ٲٰ�, ���� �ִϸ��̼��� nullptr�� �ٲߴϴ�.
	void InterPolationAnimation(std::shared_ptr<Entity> _entity);
	///--------------------------------------------------------------

private:
	entt::registry& m_registry;
	std::shared_ptr<DearsGraphicsEngine> mpGraphicsEngine;	// �׷��� ���� ��ü

	void SettingLoopAnimation(std::shared_ptr<Entity> _entity);

	float GetCoolTime(std::shared_ptr<Entity> _entity);
};

