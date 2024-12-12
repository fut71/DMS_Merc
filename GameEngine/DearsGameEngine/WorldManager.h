#pragma once
#include "IManager.h"
#include "World.h"
#include "Scene.h"
#include "Entity.h"

/// <summary>
/// ������ ������ �Ҹ�, �� ��ȯ �� ���� ������ �帧�� �����ϴ� Ŭ�����Դϴ�.
/// 2024.06.03 _Doyo;
/// </summary>
class WorldManager
	: public IManager
{
public:
	WorldManager() = delete;
	WorldManager(entt::registry& _registry);
	~WorldManager() = default;

	/// <summary>
	/// ���� �Ŵ����� �ʱ�ȭ�մϴ�.
	/// </summary>
	/// <returns>���н� false ��ȯ</returns>
	virtual bool Initialize() override;

	/// <summary>
	/// ������ ������ �������� ���� ������Ʈ�� �����մϴ�.
	/// </summary>
	/// <param name="_dTime">������ �����ӽð�</param>
	virtual void FixedUpdate(float _fixedDTime) override;

	/// <summary>
	/// �� ������ ������Ʈ�� �����մϴ�.
	/// </summary>
	/// <param name="_dTime">������ �ð�</param>
	virtual void Update(float _dTime) override;

	/// <summary>
	/// ��ó�� ������Ʈ�� �����մϴ�.
	/// </summary>
	/// <param name="_dTime">������ �ð�</param>
	virtual void LateUpdate(float _dTime) override;

	/// <summary>
	/// ���� �Ŵ����� �����ϰ� �����մϴ�.
	/// </summary>
	virtual void Finalize() override;

	/// <summary>
	/// ���ο� ���带 �����մϴ�.
	/// �Ƚ�� �Ƚ�
	/// </summary>
	/// <returns>������ ���� ��ü</returns>
	// std::shared_ptr<World> CreateWorld();

	/// <summary>
	/// ���� �Ŵ����� ���� ��ü�� �߰��մϴ�.
	/// </summary>
	/// <returns>�߰��� ���� ��ü</returns>
	std::shared_ptr<World> AddWorld(std::shared_ptr<World> _world);

	/// <summary>
	/// ���� ���带 ��ȯ�մϴ�.
	/// </summary>
	/// <returns>���� ���� ��ü</returns>
	std::shared_ptr<World> GetCurrentWorld();

	bool ShouldQuit() const;
	void RequestQuit();

	std::shared_ptr<IData> GetWorldData(const std::string& _name);

	IData* GetSceneData();

private:
	entt::registry& m_registry;
	std::unordered_map<UID, std::shared_ptr<World>> m_pWorld;	// ���� ����
	std::shared_ptr<World> m_pCurrentWorld;						// ���� ����

	bool m_shouldQuit = false;	// ������ ������ ������ Ȯ���ϴ� �÷���
};
