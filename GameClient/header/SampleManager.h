#pragma once
#include "IManager.h"

/// <summary>
///	���������� ���Ǵ� ������ ��� ���ο� �Ŵ��� Ŭ������ ���� ����
/// </summary>
class SampleManager :
    public IManager
{
public:
	/// <summary>
	/// entt::registry& ����� ������� �⺻ ������ ����
	/// </summary>
	SampleManager() = delete;

	/// <summary>
	/// Ŭ�������� ��ƼƼ���� ������Ʈ�� ����� ��� 
	/// "entt::registry& m_registry" ����� �߰��ϰ�, ������ �߰�
	/// </summary>
	/// <param name="_registry"></param>
	SampleManager(entt::registry& _registry);

	~SampleManager() = default;

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

private:
	entt::registry& m_registry;
};

