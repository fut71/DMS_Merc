#pragma once
#include "IManager.h"
#include "PhysicsDefine.h"

class CollisionCallback;
class Entity;
class GameEngine;

/// <summary>
/// ���� �ùķ��̼��� �����ϴ� Ŭ�����Դϴ�.
/// NVIDIA���� �����ϴ� PhysX api�� ����ؼ� ���۵Ǿ����ϴ�.
/// 2024.06.03 _Doyo;
/// 
/// �ӵ� ���� �߰�
/// �ڽ� �̿��� �ݶ��̴��� ����� �۵��ϵ��� ����
/// ĳ���� ��Ʈ�ѷ� ��� ����
/// 
/// �浹 �׷� ���� �߰�
/// ���� ���� ������ �ٲ㼭 ������ϴ� �޼��� �߰� // ���Ŀ� Ŭ���̾�Ʈ���� ���� �� ���� �� �����ϵ��� ������ ����
/// </summary>
class PhysicsManager
	: public IManager
{
public:
	PhysicsManager() = delete;
	PhysicsManager(entt::registry& _registry);
	PhysicsManager(entt::registry& _registry, GameEngine* _pEngine);
	~PhysicsManager() = default;

	/// <summary>
	/// ���� �ý����� �ʱ�ȭ�մϴ�.
	/// </summary>
	/// <returns>�ʱ�ȭ ���н� false�� ��ȯ�մϴ�.</returns>
	virtual bool Initialize();

	/// <summary>
	/// ���� ���� ������մϴ�.
	/// �ϴ��� �� ����ũ ����ü�� �̿��ؼ� ����
	/// <param name="_sceneDesc">�� ����ũ ����ü</param>
	/// </summary>
	void CreatePhysicsScene(physx::PxSceneDesc _sceneDesc);

	/// <summary>
	/// ���� ������ϱ� ���� �� ����ũ�� �����մϴ�.
	/// </summary>
	/// <param name="_callback">�ùķ��̼� �̺�Ʈ �ݹ� ��ü</param>
	/// <param name="PxSimulationFilterShader">�ùķ��̼� ���� ���̴� �Լ� ��ü</param>
	/// <returns>�� ����ũ</returns>
	physx::PxSceneDesc CreateSceneDesc(physx::PxSimulationEventCallback* _callback, physx::PxSimulationFilterShader _filterShader);

	/// <summary>
	/// ������ �ð� �������� ���� ������Ʈ�� �����մϴ�.
	/// </summary>
	/// <param name="_dTime">������ ��ŸŸ��</param>
	virtual void FixedUpdate(float _fixedDTime) override;

	/// <summary>
	/// PhysX �ùķ��̼��� ������Ʈ�ϰ� ��ü�� ��ȯ ����� ������Ʈ�մϴ�.
	/// </summary>
	/// <param name="_dTime">������ �ð�</param>
	virtual void Update(float _dTime) override;

	/// <summary>
	/// ��ó�� ������Ʈ�� �����մϴ�.
	/// </summary>
	/// <param name="_dTime">������ �ð�</param>
	virtual void LateUpdate(float _dTime) override;

	/// <summary>
	/// physx ���� �� �ڿ��� �����մϴ�.
	/// </summary>
	virtual void Finalize() override;

	/// <summary>
	/// ��ƼƼ�� ����Ǿ��ִ� ���� ��ü�� �߰��մϴ�.
	/// </summary>
	/// <param name="_pEntity">�߰��� ��ƼƼ</param>
	/// <param name="_groupID">���� ��ü�� �浹 �׷� ����</param>
	/// <param name="_filterData">���� ��ü�� ���� ������ �߰�</param>
 	//void AddPhysicsObject(std::shared_ptr<Entity>& _pEntity, physx::PxU32 _groupID = 0, physx::PxU32 _data = 0);
	void AddPhysicsObject(const UID& _uid, physx::PxU32 _groupID = 0, physx::PxU32 _data = 0);

	/// <summary>
	/// ����� ���� ��ü�� �����մϴ�.
	/// </summary>
	void RemovePhysicsObject(std::shared_ptr<Entity>& _pEntity);

	void ClearScene();

	/// <summary>	
	/// ��ƼƼ�� ���� �����մϴ�.
	/// </summary>
	/// <param name="_pEntity">���� ������ ��ƼƼ</param>
	/// <param name="_force">������ �� ����</param>
	void ApplyForce(std::shared_ptr<Entity>& _pEntity, const Vector3& _force, int _mode = 0);

	/// <summary>	
	/// ��ƼƼ�� �ӵ��� ���� �����մϴ�.
	/// </summary>
	/// <param name="_pEntity">�ӵ��� ������ ��ƼƼ</param>
	/// <param name="_force">������ �ӵ� ����</param>
	void SetVelocity(std::shared_ptr<Entity>& _pEntity, const Vector3& _velocity);

	/// <summary>
	/// ��ƼƼ�� Ű�׸�ƽ On/Off�� �����մϴ�.
	/// </summary>
	/// <param name="_pEntity">Ű�׸�ƽ On/Off�� ������ ���KƼ</param>
	/// <param name="_isTrue">true�� ��ġ�� ����</param>
	void SetKinematic(std::shared_ptr<Entity>& _pEntity, bool _isTrue);

	/// <summary>
	/// ĳ���� ��Ʈ�ѷ��� �߰��մϴ�.
	/// </summary>
	/// <param name="_pEntity">�߰��� ��ƼƼ</param>
	void AddCharacterController(std::shared_ptr<Entity>& _pEntity);

	/// <summary>
	/// ĳ���� ��Ʈ�ѷ��� �̵���ŵ�ϴ�.
	/// </summary>
	/// <param name="_pEntity">�̵���ų ��ƼƼ</param>
	/// <param name="_displacement">�̵� ����</param>
	/// <param name="_dTime">������ �ð�</param>
	void MoveCharacterController(std::shared_ptr<Entity>& _pEntity, const Vector3& _displacement, float _dTime);

	/// <summary>
	/// ����� ĳ���� ��Ʈ�ѷ��� �����մϴ�.
	/// </summary>
	void RemoveCharacterController(std::shared_ptr<Entity>& _pEntity);

	void SetGroupCollisionFlag(uint16_t _gourp1ID, uint16_t _gourp2ID, bool _enableCollision = true);

	void SetKinematicTarget(std::shared_ptr<Entity> _entity, Vector3 _position, Quaternion _rotation);

	void SetFixPosition(std::shared_ptr<Entity> _entity, bool _isTrue);

	void UpdatePosition(std::shared_ptr<Entity> _entity, Vector3 _newPos);

	Vector3 PickObejct();

	Vector3 PickObejct(const std::string& _name);

// 	void OnOffSimulation(bool _on);

	physx::PxFilterData GetFilterData(std::shared_ptr<Entity> _pEntity);
	physx::PxFilterData GetFilterData(const entt::entity& _entity);
	void SetFilterData(std::shared_ptr<Entity>& _pEntity, const physx::PxFilterData& newFilterData);
	void SetFilterData(physx::PxShape* _pShape, const physx::PxFilterData& newFilterData);
	void SetFilterData(const entt::entity& _entity, const physx::PxFilterData& newFilterData);

	std::shared_ptr<Entity> GetEntityFromActor(physx::PxRigidActor* actor);

	void SetObjectSleep(std::shared_ptr<Entity> _pEntity);
	void SetObjectWake(std::shared_ptr<Entity> _pEntity);

	void SetALLObjectSleep();
	void SetALLObjectWake();

	void ClearFilterUpdate();

private:
	// 	struct Impl;
	// 	std::unique_ptr<Impl> m_pImpl;
	GameEngine* m_pEngine;
	entt::registry& m_registry;

	// physx ���� �����
	physx::PxDefaultAllocator m_allocator;				// �⺻ �޸� �Ҵ���
	physx::PxDefaultErrorCallback m_errorCallback;		// �⺻ ���� �ݹ�
	physx::PxFoundation* m_pFoundation;					// Foundation ��ü, physx�� �������� ���� �� ���ҽ��� ����
	physx::PxPhysics* m_pPhysics;						// Physics ��ü
	physx::PxDefaultCpuDispatcher* m_pDispatcher;		// CPU ������, ���� �ùķ��̼��� ���� �ʿ�
	physx::PxScene* m_pScene;							// physx Scene ��ü
	physx::PxMaterial* m_pMaterial;						// ����Ʈ�� ��� �� physx Material ��ü
	physx::PxControllerManager* m_pControllerManager;	// physx Controller Manager ��ü

	// 
#ifdef _DEBUG
	physx::PxPvd* m_pPvd;
	physx::PxPvdTransport* m_pTransport;
#endif

	//std::unordered_map<std::shared_ptr<Entity>, physx::PxRigidActor*> m_entities;	// ���� ��ü�� ��
 	//std::unordered_map<std::shared_ptr<Entity>, physx::PxCapsuleController*> m_controllers; // ĳ���� ��Ʈ�ѷ��� ��
 
 	//std::vector<physx::PxMaterial*> m_pMaterials;

	bool m_simulationOn = true;

	std::queue<std::pair<physx::PxShape*, physx::PxFilterData>> m_filterDataUpdates;

	std::unique_ptr<entt::observer> m_pRigidbodyObserber;
};

