#pragma once
#include "IManager.h"

class RenderManager;

class WeaponManager
	: public IManager
{
public:
	/// <summary>
	/// entt::registry& ����� ������� �⺻ ������ ����
	/// </summary>
	WeaponManager() = delete;

	/// <summary>
	/// Ŭ�������� ��ƼƼ���� ������Ʈ�� ����� ��� 
	/// "entt::registry& m_registry" ����� �߰��ϰ�, ������ �߰�
	/// </summary>
	/// <param name="_registry"></param>
	WeaponManager(entt::registry& _registry, RenderManager* _renderManager);

	~WeaponManager() = default;

	/// <summary>
	/// �Ŵ��� �ʱ�ȭ 
	/// </summary>
	/// <returns></returns>
	virtual bool Initialize() override;

	/// <summary>
	/// ���� ������ �ִ� ��� ���⼭ ������Ʈ // ������ ��� ���ҵ�
	/// </summary>
	/// <param name="_fixedDTime">1/60���� ������ ������ ����</param>
	virtual void FixedUpdate(float _fixedDTime) override;

	/// <summary>
	/// �Ϲ����� ������ ������Ʈ 
	/// </summary>
	/// <param name="_dTime">������ ����</param>
	virtual void Update(float _dTime) override;

	/// <summary>
	/// ��ó���� �ʿ��� ������ ������Ʈ
	/// </summary>
	/// <param name="_dTime">������ ����</param>
	virtual void LateUpdate(float _dTime) override;

	/// <summary>
	/// �޸� ����	// ������ �� ������ ������
	/// </summary>
	virtual void Finalize() override;

	void Render();

private:
	entt::registry& m_registry;
	RenderManager* m_pRenderManager;
};

