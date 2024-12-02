#include "PhysicsManager.h"
#include "CollisionCallback.h"
#include "Entity.h"
#include "Transform.h"
#include "Collider.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "CharacterController.h"
#include "Rigidbody.h" 
#include "CharacterController.h" 
#include "PlaneComponent.h" 
#include "CameraComponent.h"
#include "GameEngine.h"
#include "WorldManager.h"
#include "Scene.h"
#include "RaycastQueryFilterCallback.h"
#include "Name.h"

physx::PxFilterFlags CustomFilterShader(physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
	physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
	physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
{
	pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT | physx::PxPairFlag::eNOTIFY_TOUCH_FOUND | physx::PxPairFlag::eNOTIFY_TOUCH_LOST;
	return physx::PxFilterFlag::eDEFAULT;
}

PhysicsManager::PhysicsManager(entt::registry& _registry)
	: m_registry(_registry)
	, m_pFoundation(nullptr)
	, m_pPhysics(nullptr)
	, m_pDispatcher(nullptr)
	, m_pScene(nullptr)
	, m_pMaterial(nullptr)
	, m_pControllerManager(nullptr)
	, m_pEngine(nullptr)
	// 	, m_pCollisionCallback(nullptr)
{
}

PhysicsManager::PhysicsManager(entt::registry& _registry, GameEngine* _pEngine)
	: m_registry(_registry)
	, m_pFoundation(nullptr)
	, m_pPhysics(nullptr)
	, m_pDispatcher(nullptr)
	, m_pScene(nullptr)
	, m_pMaterial(nullptr)
	, m_pControllerManager(nullptr)
	, m_pEngine(_pEngine)
{

}

bool PhysicsManager::Initialize()
{
	std::string strVersion = std::to_string(PX_PHYSICS_VERSION_MAJOR) + "."
		+ std::to_string(PX_PHYSICS_VERSION_MINOR) + "."
		+ std::to_string(PX_PHYSICS_VERSION_BUGFIX);
	DLOG(LOG_INFO, "PhysX version: " + strVersion);

	// Foundation ���� : �޸� �Ҵ� �� ���� �ݹ� ����
	m_pFoundation = PxCreateFoundation(PX_PHYSICS_VERSION, m_allocator, m_errorCallback);
	if (!m_pFoundation)
	{
		DLOG(LOG_ERROR, "PxCreateFoundation Failed.");
		return false;
	}

#ifdef _DEBUG
	// pvd ����
	m_pPvd = physx::PxCreatePvd(*m_pFoundation);
	m_pTransport = physx::PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);

	// PVD�� ��Ʈ��ũ �������� ���� (IP: 127.0.0.1, ��Ʈ: 5425)
	m_pPvd->connect(*m_pTransport, physx::PxPvdInstrumentationFlag::eALL);
#endif

	// Physics ��ü ����
#ifdef _DEBUG
	m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, physx::PxTolerancesScale(), true, m_pPvd);
#else 
	m_pPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pFoundation, physx::PxTolerancesScale(), true);
#endif

	if (!m_pPhysics)
	{
		DLOG(LOG_ERROR, "PxCreatePhysics Failed.");
		return false;
	}

	// Scene ���� : ���� �ùķ��̼� ȯ�� ����
	physx::PxSceneDesc sceneDesc(m_pPhysics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);

	// CPU Dispatcher ���� : �ùķ��̼� �۾��� ���� �����忡 �л�
	m_pDispatcher = physx::PxDefaultCpuDispatcherCreate(4);
	if (!m_pDispatcher)
	{
		DLOG(LOG_ERROR, "PxDefaultCpuDispatcherCreate Failed.");
		return false;
	}
	sceneDesc.cpuDispatcher = m_pDispatcher;
	sceneDesc.filterShader = CustomFilterShader;

	// Collision Callback ����
	auto m_pCollisionCallback = new CollisionCallback();
	sceneDesc.simulationEventCallback = m_pCollisionCallback;

	// 	sceneDesc.

	m_pScene = m_pPhysics->createScene(sceneDesc);
	if (!m_pScene)
	{
		DLOG(LOG_ERROR, "createScene Failed.");
		return false;
	}

	// Material ���� : ����, �ݹ� ��� �� ���� ��ü�� ������ Ư�� ���� // (���� ���� ���, ���� ���� ���, �ݹ� ���)
	m_pMaterial = m_pPhysics->createMaterial(1.f, 1.f, 0.f);
	m_pMaterial->setFrictionCombineMode(physx::PxCombineMode::eMAX);
	if (!m_pMaterial)
	{
		DLOG(LOG_ERROR, "createMaterial Failed.");
		return false;
	}

	// controller manager ����
	m_pControllerManager = PxCreateControllerManager(*m_pScene);
	if (!m_pControllerManager)
	{
		DLOG(LOG_ERROR, "PxCreateControllerManager Failed.");
		return false;
	}

	return true;
}

void PhysicsManager::CreatePhysicsScene(physx::PxSceneDesc _sceneDesc)
{
	/// �� �ʱ�ȭ
	if (m_pScene)
	{
		m_pScene->release();
		m_pScene = nullptr;
	}

	/// Scene �����
	m_pScene = m_pPhysics->createScene(_sceneDesc);
	if (!m_pScene)
	{
		DLOG(LOG_ERROR, "createScene Failed.");
	}
}

physx::PxSceneDesc PhysicsManager::CreateSceneDesc(physx::PxSimulationEventCallback* _callback, physx::PxSimulationFilterShader _filterShader)
{
	// Scene ���� : ���� �ùķ��̼� ȯ�� ����
	physx::PxSceneDesc sceneDesc(m_pPhysics->getTolerancesScale());
	sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);

	sceneDesc.cpuDispatcher = m_pDispatcher;
	sceneDesc.filterShader = _filterShader;

	// Collision Callback ����
	sceneDesc.simulationEventCallback = _callback;

	return sceneDesc;
}

void PhysicsManager::FixedUpdate(float _fixedDTime)
{
	// 3D ������Ʈ ��ŷ
	//PickObejct();

	// ������ ť�� ����� ���� ������ ���� ���� ó��
	while (!m_filterDataUpdates.empty())
	{
		auto& update = m_filterDataUpdates.front();

		// PxShape�� �θ��� PxRigidActor�� ������ ��ȿ���� Ȯ��
		physx::PxRigidActor* actor = update.first ? update.first->getActor() : nullptr;

		if (actor )
		{
			update.first->setSimulationFilterData(update.second);
		}
		m_filterDataUpdates.pop();
	}

	// ����� ������ٵ� ������ ���Ϳ� ����
	auto rigidbodyView = m_registry.view<Rigidbody>();
	for (auto entity : rigidbodyView)
	{
		auto rigidbody = m_registry.try_get<Rigidbody>(entity);

		if (rigidbody->m_pRigidActor)
		{
			// ������ ���͸� ������ ���̳������� ĳ����
			if (physx::PxRigidDynamic* dynamic = rigidbody->m_pRigidActor->is<physx::PxRigidDynamic>())
			{
				// ������ ���̳����� ������ �Ӽ� ������Ʈ
				dynamic->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, rigidbody->m_isKinematic);			// Ű�׸�ƽ ����	// ���� �ùķ��̼ǿ��� ���� ���� ������, ���� ��ġ�� ������ �� ����.
				dynamic->setMass(rigidbody->m_mass);																// ���� ����
				dynamic->setLinearDamping(rigidbody->m_drag);														// ���� ��� ����
				dynamic->setAngularDamping(rigidbody->m_angularDrag);												// ȸ�� ���� ��� ����
				dynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !rigidbody->m_useGravity);				// �߷� ����

				// ���Ϳ� ����� ������ ��������
				physx::PxShape* shape = nullptr;
				dynamic->getShapes(&shape, 1);

				// �������� ����� ���׸��� ��������
				if (shape)
				{
					physx::PxMaterial* material = nullptr;
					shape->getMaterials(&material, 1);

					// ���׸����� ������ �Ӽ� ������Ʈ
					if (material)
					{
						material->setDynamicFriction(rigidbody->m_dynamicFriction);   // ���� ���� ���
						material->setStaticFriction(rigidbody->m_staticFriction);     // ���� ���� ���
						material->setRestitution(rigidbody->m_restitution);           // �ݹ� ���
					}
				}

			}
		}
	}

	// �� ���� ��ü �� Ȯ��
	physx::PxU32 nbActors = m_pScene->getNbActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC);

	// ���� ��ü ���� 1�� �̻��� ���, physx �ùķ��̼� ����
	if (nbActors < 1)
	{
		// DLOG(LOG_INFO, "Physics Scene Is Empty. No Simulation Performed.");
	}
	else
	{
		// ���� ���� �ùķ��̼�
		m_pScene->simulate(_fixedDTime);
		m_pScene->fetchResults(true);

		// �� ��ƼƼ���� Transform ������Ʈ
		auto view = m_registry.view<Transform, Rigidbody>();
		for (auto entity : view)
		{
			auto& transform = m_registry.get<Transform>(entity);
			auto& rigidbody = m_registry.get<Rigidbody>(entity);
			auto& actor = rigidbody.m_pRigidActor;
			bool trigger = false;

			if (auto box = m_registry.try_get<BoxCollider>(entity))
			{
				trigger = box->m_isTrigger;
			}
			else if (auto sphere = m_registry.try_get<SphereCollider>(entity))
			{
				trigger = sphere->m_isTrigger;
			}
			else if (auto capsule = m_registry.try_get<CapsuleCollider>(entity))
			{
				trigger = capsule->m_isTrigger;
			}

			if (actor)
			{
				// �ݶ��̴��� trigger on�� ���
				if (trigger)
				{
					physx::PxShape* shape = nullptr;
					actor->getShapes(&shape, 1);
					physx::PxShapeFlags currentFlags = shape->getFlags();
					// 
					if (shape->getFlags() & ~physx::PxShapeFlag::eTRIGGER_SHAPE)
					{
						currentFlags &= ~physx::PxShapeFlag::eSIMULATION_SHAPE;
						currentFlags |= physx::PxShapeFlag::eTRIGGER_SHAPE;
						shape->setFlags(currentFlags);
					}
				}
				// �ݶ��̴��� trigger off �� ���
				else
				{
					physx::PxShape* shape = nullptr;
					actor->getShapes(&shape, 1);
					physx::PxShapeFlags currentFlags = shape->getFlags();
					if (shape->getFlags() & physx::PxShapeFlag::eTRIGGER_SHAPE)
					{
						currentFlags |= physx::PxShapeFlag::eSIMULATION_SHAPE;
						currentFlags &= ~physx::PxShapeFlag::eTRIGGER_SHAPE;
						shape->setFlags(currentFlags & ~physx::PxShapeFlag::eTRIGGER_SHAPE);
					}
				}

				if (!rigidbody.m_isStatic)
				{

					/// �������� �޶����� ���, ������ ����
					if (transform.m_pParent)
					{
						physx::PxShape* shape;
						actor->getShapes(&shape, 1); // ù ��° shape ��������

						// Shape�� Ÿ���� Ȯ��
						physx::PxGeometryType::Enum geoType = shape->getGeometry().getType();

						switch (geoType)
						{
						case physx::PxGeometryType::eBOX:
						{
							// BoxGeometry ó��
							physx::PxBoxGeometry boxGeometry = static_cast<const physx::PxBoxGeometry&>(shape->getGeometry());
							// Transform�� �����ϰ� BoxCollider�� ũ�� ��
							auto& name = m_registry.get<Name>(entity).m_name;
							constexpr float epsilon = std::numeric_limits<float>::epsilon();

							if (fabsf(boxGeometry.halfExtents.x - transform.m_localScale.x / 2.0f) > epsilon ||
								fabsf(boxGeometry.halfExtents.y - transform.m_localScale.y / 2.0f) > epsilon ||
								fabsf(boxGeometry.halfExtents.z - transform.m_localScale.z / 2.0f) > epsilon)
							{
								// Transform�� �������� �ٸ��ٸ� BoxGeometry ������Ʈ
								boxGeometry.halfExtents = physx::PxVec3(transform.m_localScale.x / 2.0f,
									transform.m_localScale.y / 2.0f,
									transform.m_localScale.z / 2.0f);
								shape->setGeometry(boxGeometry); // Shape�� geometry ������Ʈ
							}
							break;
						}
						// 	case physx::PxGeometryType::eCAPSULE:
						// 	{
						// 		// CapsuleGeometry ó��
						// 		physx::PxCapsuleGeometry capsuleGeometry = static_cast<const physx::PxCapsuleGeometry&>(shape->getGeometry());
						// 
						// 		if (capsuleGeometry.radius != transform->m_localScale.x / 2.0f || // x������ ����
						// 			capsuleGeometry.halfHeight != transform->m_localScale.y / 2.0f) // y������ ����
						// 		{
						// 			// Transform�� �°� CapsuleGeometry ������Ʈ
						// 			capsuleGeometry.radius = transform->m_localScale.x / 2.0f;
						// 			capsuleGeometry.halfHeight = transform->m_localScale.y / 2.0f;
						// 			shape->setGeometry(capsuleGeometry); // Shape�� geometry ������Ʈ
						// 		}
						// 		break;
						// 	}
						// 	case physx::PxGeometryType::eSPHERE:
						// 	{
						// 		// SphereGeometry ó��
						// 		physx::PxSphereGeometry sphereGeometry = static_cast<const physx::PxSphereGeometry&>(shape->getGeometry());
						// 		if (sphereGeometry.radius != transform->m_localScale.x / 2.0f) // �������� x�� �����Ϸ� ����
						// 		{
						// 			// Transform�� �°� SphereGeometry ������Ʈ
						// 			sphereGeometry.radius = transform->m_localScale.x / 2.0f;
						// 			shape->setGeometry(sphereGeometry); // Shape�� geometry ������Ʈ
						// 		}
						// 		break;
						// 	}
						default:
							// �ٸ� geometry Ÿ���� ó������ ����
							break;
						}
					}

					/// ������ ��ġ ��ȯ
					if (!rigidbody.m_isKinematic)
					{
						physx::PxTransform pxTransform = actor->getGlobalPose();
						Quaternion qut = Quaternion(pxTransform.q.x, pxTransform.q.y, pxTransform.q.z, pxTransform.q.w);
						Vector3 vec = qut.ToEuler();
						if (auto box = m_registry.try_get<BoxCollider>(entity))
						{
							transform.m_localPosition = Vector3(pxTransform.p.x, pxTransform.p.y, pxTransform.p.z) - box->m_center;
						}
						else if (auto sphere = m_registry.try_get<SphereCollider>(entity))
						{
							transform.m_localPosition = Vector3(pxTransform.p.x, pxTransform.p.y, pxTransform.p.z) - sphere->m_center;
						}
						else if (auto capsule = m_registry.try_get<CapsuleCollider>(entity))
						{
							transform.m_localPosition = Vector3(pxTransform.p.x, pxTransform.p.y, pxTransform.p.z) - capsule->m_center;
						}
					}
					// Ű�׸�ƽ ��ü�� ���, ��ü�� Ʈ������ ������Ʈ�κ��� ���� ��ü�� ��ġ�� �����Ŵ
					else
					{
						// ������Ʈ�κ��� Ʈ������ ������ ������
						Vector3 rot = transform.m_localRotation /** (std::numbers::pi / 180.f)*/;
						Quaternion qut = Quaternion::CreateFromYawPitchRoll(rot);

						physx::PxVec3 pxPos;

						Vector3 center;
						if (auto box = m_registry.try_get<BoxCollider>(entity))
						{
							center = box->m_center;
						}
						else if (auto sphere = m_registry.try_get<SphereCollider>(entity))
						{
							center = sphere->m_center;
						}
						else if (auto capsule = m_registry.try_get<CapsuleCollider>(entity))
						{
							center = capsule->m_center;
						}
						pxPos = physx::PxVec3(transform.m_localPosition.x + center.x
							, transform.m_localPosition.y + center.y
							, transform.m_localPosition.z + center.z);

						physx::PxQuat pxQut = physx::PxQuat(qut.x, qut.y, qut.z, qut.w);
						physx::PxTransform pxTransform = physx::PxTransform(pxPos, pxQut);

						// physx::PxTransform pxTransform = physx::PxTransform(pxPos, pxQut);
						// ���Ϳ� Ʈ������ ����
						actor->setGlobalPose(pxTransform);

					}
				}
			}

		}

		auto charView = m_registry.view<Transform, CharacterController>();
		for (auto entity : charView)
		{
			auto transform = m_registry.try_get<Transform>(entity);
			auto controller = m_registry.try_get<CharacterController>(entity);

			if (transform && controller)
			{
				// PxExtendedVec3�κ��� ��ġ ������ ������
				physx::PxExtendedVec3 pxTransform = controller->m_pController->getPosition();

				// transform�� m_localPosition�� ���� �Ҵ�
				transform->m_localPosition = Vector3(pxTransform.x, pxTransform.y, pxTransform.z);
			}
		}
	}
}

void PhysicsManager::Update(float _dTime)
{
}

void PhysicsManager::LateUpdate(float _dTime)
{

}

void PhysicsManager::Finalize()
{
	if (m_pScene)
	{
		m_pScene->release();
		m_pScene = nullptr;
	}

	if (m_pDispatcher)
	{
		m_pDispatcher->release();
		m_pDispatcher = nullptr;
	}

	if (m_pPhysics)
	{
		m_pPhysics->release();
		m_pPhysics = nullptr;
	}

#ifdef _DEBUG
	if (m_pTransport)
	{
		m_pTransport->release();
		m_pTransport = nullptr;
	}

	if (m_pPvd)
	{
		m_pPvd->release();
		m_pPvd = nullptr;
	}
#endif

	if (m_pFoundation)
	{
		m_pFoundation->release();
		m_pFoundation = nullptr;
	}

	if (m_pControllerManager)
	{
		m_pControllerManager->release();
		m_pControllerManager = nullptr;
	}
}

void PhysicsManager::AddPhysicsObject(const UID& _uid, physx::PxU32 _groupID /*= 0*/, physx::PxU32 _data /*= 0*/)
{
	//auto view = m_registry.view<Transform, Rigidbody>();
	entt::entity entity = static_cast<entt::entity>(_uid);
	auto& transform = m_registry.get<Transform>(entity);
	if (Rigidbody* rigidbody = m_registry.try_get<Rigidbody>(entity))
	{
		if (rigidbody->m_pRigidActor)
		{
			DLOG(LOG_INFO, "Actor already create.");
		}

		if (auto plane = m_registry.try_get<PlaneComponent>(entity))
		{
			physx::PxPlane pxPlane = physx::PxPlane(plane->m_axes.x, plane->m_axes.y, plane->m_axes.z, plane->m_distance);

			physx::PxMaterial* material = m_pPhysics->createMaterial(rigidbody->m_staticFriction, rigidbody->m_dynamicFriction, rigidbody->m_restitution);
			material->setFrictionCombineMode(physx::PxCombineMode::eMAX);

			rigidbody->m_pRigidActor = PxCreatePlane(*m_pPhysics, pxPlane, *material);

			if (rigidbody->m_pRigidActor)
			{
				// filterdata ����
				physx::PxFilterData filterData;
				filterData.word0 = _groupID;
				filterData.word1 = _data;
				physx::PxShape* shape = nullptr;
				auto num = rigidbody->m_pRigidActor->getNbShapes();
				rigidbody->m_pRigidActor->getShapes(&shape, 1);
				shape->setSimulationFilterData(filterData);

				// name ����
				rigidbody->m_pRigidActor->setName(rigidbody->m_name);

				// ���� ���� �߰�
				m_pScene->addActor(*rigidbody->m_pRigidActor);

				// ���׸��� �Ҵ� ����
				material->release();
			}
		}
		else
		{
			Vector3 center{};
			if (auto collider = m_registry.try_get<BoxCollider>(entity))
			{
				center = collider->m_center;
			}
			else if (auto collider = m_registry.try_get<SphereCollider>(entity))
			{
				center = collider->m_center;
			}
			else if (auto collider = m_registry.try_get<CapsuleCollider>(entity))
			{
				center = collider->m_center;
			}

			// transform ����
			Quaternion qut = Quaternion::CreateFromYawPitchRoll(transform.m_localRotation);
			physx::PxTransform pxTransform(
				physx::PxVec3(
					transform.m_localPosition.x + center.x,
					transform.m_localPosition.y + center.y,
					transform.m_localPosition.z + center.z),
				physx::PxQuat(qut.x, qut.y, qut.z, qut.w)
			);

			// getmetry ����
			std::shared_ptr<physx::PxGeometry> geometry;
			if (auto collider = m_registry.try_get<BoxCollider>(entity))
			{
				if (!transform.m_pParent)
				{
					geometry = std::make_shared<physx::PxBoxGeometry>(collider->m_size.x / 2, collider->m_size.y / 2, collider->m_size.z / 2);
				}
				else
				{
					geometry = std::make_shared<physx::PxBoxGeometry>(transform.m_localScale.x / 2, transform.m_localScale.y / 2, transform.m_localScale.z / 2);
				}
			}
			else if (auto collider = m_registry.try_get<SphereCollider>(entity))
			{
				geometry = std::make_shared<physx::PxSphereGeometry>(collider->m_radius);
			}
			else if (auto collider = m_registry.try_get<CapsuleCollider>(entity))
			{
				geometry = std::make_shared<physx::PxCapsuleGeometry>(collider->m_radius, collider->m_height / 2);
				pxTransform = physx::PxTransform(pxTransform.p, physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0.f, 0.f, 1.f))); // 90�� ȸ��
			}

			// dynamic actor ����
			physx::PxMaterial* material = m_pPhysics->createMaterial(rigidbody->m_staticFriction, rigidbody->m_dynamicFriction, rigidbody->m_restitution);
			material->setFrictionCombineMode(physx::PxCombineMode::eMAX);

			if (!rigidbody->m_isStatic)
			{
				rigidbody->m_pRigidActor = PxCreateDynamic(*m_pPhysics, pxTransform, *geometry, *material, 1.f);
				physx::PxRigidDynamic* rigidDynamic = rigidbody->m_pRigidActor->is<physx::PxRigidDynamic>();
				rigidDynamic->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, rigidbody->m_isKinematic);			// Ű�׸�ƽ ����	// ���� �ùķ��̼ǿ��� ���� ���� ������, ���� ��ġ�� ������ �� ����.
				rigidDynamic->setMass(rigidbody->m_mass);																// ���� ����
				rigidDynamic->setLinearDamping(rigidbody->m_drag);														// ���� ��� ����
				rigidDynamic->setAngularDamping(rigidbody->m_angularDrag);												// ȸ�� ���� ��� ����
				rigidDynamic->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, !rigidbody->m_useGravity);				// �߷� ����
				rigidDynamic->setSleepThreshold(0.5f);																	// ��ü�� �޸� ���·� ��ȯ�Ǵ� �Ӱ谪 ����
				rigidDynamic->setMaxLinearVelocity(100.0f);																// �ְ� �ӵ� ����
				rigidDynamic->setMaxAngularVelocity(100.0f);															// �ְ� ���ӵ� ����

				// ��ü�� X�� �� Z�� ȸ���� �����Ͽ� �ں����� �ʰ� �Ѵ�.
				if (m_registry.try_get<CapsuleCollider>(entity))
				{
					rigidDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, true);
					rigidDynamic->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, true);
				}

			}
			else
			{
				rigidbody->m_pRigidActor = PxCreateStatic(*m_pPhysics, pxTransform, *geometry, *material);
				physx::PxRigidStatic* rigidStatic = rigidbody->m_pRigidActor->is<physx::PxRigidStatic>();
			}

			// name ����
			rigidbody->m_pRigidActor->setName(rigidbody->m_name);

			// filterdata ����
			physx::PxFilterData filterData;
			filterData.word0 = _groupID;
			filterData.word1 = _data;
			physx::PxShape* shape = nullptr;
			auto num = rigidbody->m_pRigidActor->getNbShapes();
			rigidbody->m_pRigidActor->getShapes(&shape, 1);
			shape->setSimulationFilterData(filterData);

			// ���� ���� �߰�
			m_pScene->addActor(*rigidbody->m_pRigidActor);

			// ���׸��� �Ҵ� ����
			material->release();
		}
	}
}

void PhysicsManager::ApplyForce(std::shared_ptr<Entity>& _pEntity, const Vector3& _force, int _mode)
{
	if (_pEntity->HasComponent<Rigidbody>())
	{
		auto& rigidbody = _pEntity->GetComponent<Rigidbody>();

		auto actor = rigidbody.m_pRigidActor->is<physx::PxRigidDynamic>();
		if (actor)
		{
			// �� ����
			actor->addForce(physx::PxVec3(_force.x, _force.y, _force.z), physx::PxForceMode::eFORCE);
			// 			DLog(LOG_INFO, "addforce");
			// 			DLog(LOG_INFO, std::to_string(actor->getLinearVelocity().x)
			// 				+ std::to_string(actor->getLinearVelocity().y)
			// 				+ std::to_string(actor->getLinearVelocity().z));
		}
	}
}

void PhysicsManager::SetVelocity(std::shared_ptr<Entity>& _pEntity, const Vector3& _velocity)
{
	auto& rigidbody = _pEntity->GetComponent<Rigidbody>();
	if (_pEntity->HasComponent<Rigidbody>())
	{
		auto actor = rigidbody.m_pRigidActor->is<physx::PxRigidDynamic>();
		if (actor)
		{
			actor->setLinearVelocity(physx::PxVec3(_velocity.x, _velocity.y, _velocity.z));
		}
	}
}

void PhysicsManager::SetKinematic(std::shared_ptr<Entity>& _pEntity, bool _isTrue)
{

}

void PhysicsManager::AddCharacterController(std::shared_ptr<Entity>& _pEntity)
{
	auto& transform = _pEntity->GetComponent<Transform>();
	auto& collider = _pEntity->GetComponent<CapsuleCollider>();

	if (_pEntity->HasComponent<Transform>() && _pEntity->HasComponent<CapsuleCollider>())
	{
		physx::PxCapsuleControllerDesc desc;
		desc.setToDefault();
		desc.height = collider.m_height;
		desc.radius = collider.m_radius;
		desc.position = physx::PxExtendedVec3(transform.m_localPosition.x, transform.m_localPosition.y, transform.m_localPosition.z);
		desc.material = m_pMaterial;
		desc.behaviorCallback = nullptr;
		desc.reportCallback = nullptr;
		DLOG(LOG_INFO, "Creating Capsule Controller");

		switch (collider.m_direction)
		{
		case 0: // X��
			desc.upDirection = physx::PxVec3(1.0f, 0.0f, 0.0f);
			break;
		case 1: // Y��
			desc.upDirection = physx::PxVec3(0.0f, 1.0f, 0.0f);
			break;
		case 2: // Z��
			desc.upDirection = physx::PxVec3(0.0f, 0.0f, 1.0f);
			break;
		default:
			desc.upDirection = physx::PxVec3(0.0f, 1.0f, 0.0f);
			break;
		}

		desc.contactOffset = 0.1f;
		desc.stepOffset = 0.5f;
		desc.slopeLimit = 0.f;
		desc.climbingMode = physx::PxCapsuleClimbingMode::eEASY;
		desc.density = 10.f;

		physx::PxCapsuleController* controller = static_cast<physx::PxCapsuleController*>(m_pControllerManager->createController(desc));
		if (!controller)
		{
			DLOG(LOG_ERROR, "Failed to create controller.");
			return;
		}

		_pEntity->GetComponent<CharacterController>().m_pController = controller;
		DLOG(LOG_INFO, "Controller created successfully.");
	}
	else
	{
		DLOG(LOG_ERROR, "Entity does not have required components.");
	}
}

void PhysicsManager::MoveCharacterController(std::shared_ptr<Entity>& _pEntity, const Vector3& _displacement, float _dTime)
{
	physx::PxCapsuleController* controller = _pEntity->GetComponent<CharacterController>().m_pController;
	controller->move(physx::PxVec3(_displacement.x, _displacement.y, _displacement.z), 0.001f, _dTime, physx::PxControllerFilters());
}

void PhysicsManager::RemovePhysicsObject(std::shared_ptr<Entity>& _pEntity)
{
	auto actor = _pEntity->GetComponent<Rigidbody>().m_pRigidActor;
	m_pScene->removeActor(*actor);
	actor->release();
	DLOG(LOG_INFO, "Physics object removed: " + _pEntity->GetName());
}

void PhysicsManager::ClearScene()
{
	physx::PxU32 numActors = m_pScene->getNbActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC | physx::PxActorTypeFlag::eRIGID_STATIC);
	physx::PxActor** actors = new physx::PxActor * [numActors];
	m_pScene->getActors(physx::PxActorTypeFlag::eRIGID_DYNAMIC | physx::PxActorTypeFlag::eRIGID_STATIC, actors, numActors);

	for (physx::PxU32 i = 0; i < numActors; i++)
	{
		m_pScene->removeActor(*actors[i]);
		actors[i]->release();
	}

	delete[] actors;
}

void PhysicsManager::RemoveCharacterController(std::shared_ptr<Entity>& _pEntity)
{
	// 	if (m_controllers.find(_pEntity) != m_controllers.end())
	// 	{
	// 		auto controller = m_controllers[_pEntity];
	// 		controller->release();
	// 		m_controllers.erase(_pEntity);
	// 		DLOG(LOG_INFO, "Character controller removed: " + _pEntity->GetName());
	// 	}
}

void PhysicsManager::SetGroupCollisionFlag(uint16_t _gourpID1, uint16_t _gourpID2, bool _enableCollision/* = true*/)
{
	physx::PxSetGroupCollisionFlag(_gourpID1, _gourpID2, _enableCollision);
}

void PhysicsManager::SetKinematicTarget(std::shared_ptr<Entity> _entity, Vector3 _position, Quaternion _rotation)
{
	// ȭ���� ��ġ �� ȸ�� ����
	physx::PxTransform arrowPose(physx::PxVec3(_position.x, _position.y, _position.z), physx::PxQuat(0, 0, 0, 1));

	// Ű�׸�ƽ ������Ʈ�� ��ġ �� ȸ�� ����
	if (_entity->GetComponent<Rigidbody>().m_pRigidActor->is<physx::PxRigidDynamic>())
	{
		physx::PxRigidDynamic* entity = _entity->GetComponent<Rigidbody>().m_pRigidActor->is<physx::PxRigidDynamic>();
		entity->setKinematicTarget(arrowPose);
	}

}

void PhysicsManager::SetFixPosition(std::shared_ptr<Entity> _entity, bool _isTrue)
{
	auto& rigidbody = _entity->GetComponent<Rigidbody>();
	if (_entity->HasComponent<Rigidbody>())
	{
		auto actor = rigidbody.m_pRigidActor->is<physx::PxRigidDynamic>();
		if (actor)
		{
			if (_isTrue)
			{
				// ���� ��ġ�� �����Ͽ� ����
				physx::PxTransform currentPose = actor->getGlobalPose();
				actor->setGlobalPose(currentPose); // ��ü�� ��ġ�� ������ ����
				actor->setLinearVelocity(physx::PxVec3(0, 0, 0)); // �ӵ��� 0���� �����Ͽ� ���߰� ��
				actor->setAngularVelocity(physx::PxVec3(0, 0, 0)); // ȸ�� �ӵ��� 0���� ����
				//pMaterial->setStaticFriction(0.0f);
				//pMaterial->setDynamicFriction(0.0f);
				//pMaterial->setRestitution(0.0f);
			}
			else
			{
				actor->wakeUp();
			}
		}
	}
}

void PhysicsManager::UpdatePosition(std::shared_ptr<Entity> _entity, Vector3 _newPos)
{
	auto& rigidbody = _entity->GetComponent<Rigidbody>();
	if (_entity->HasComponent<Rigidbody>())
	{
		auto actor = rigidbody.m_pRigidActor->is<physx::PxRigidDynamic>();
		if (actor)
		{
			Vector3 center;
			if (_entity->HasComponent<BoxCollider>())
			{
				center = _entity->GetComponent<BoxCollider>().m_center;
			}
			else if (_entity->HasComponent<SphereCollider>())
			{
				center = _entity->GetComponent<SphereCollider>().m_center;
			}
			else if (_entity->HasComponent<CapsuleCollider>())
			{
				center = _entity->GetComponent<CapsuleCollider>().m_center;
			}

			// ���ο� ��ġ�� ����
			physx::PxTransform newTransform(physx::PxVec3(_newPos.x + center.x, _newPos.y + center.y, _newPos.z + center.z)
				, actor->getGlobalPose().q);
			actor->setGlobalPose(newTransform);

			// ������ �ӵ� �� ȸ�� ���� (�ʿ��)
			actor->setLinearVelocity(physx::PxVec3(0, 0, 0));
			actor->setAngularVelocity(physx::PxVec3(0, 0, 0));
		}
	}
}

Vector3 PhysicsManager::PickObejct()
{
	POINT mousePos;
	GetCursorPos(&mousePos);
	ScreenToClient(m_pEngine->m_hWnd, &mousePos);

	Vector3 rayNDC;
	rayNDC.x = (2.f * mousePos.x) / m_pEngine->m_width - 1.f;
	rayNDC.y = 1.f - (2.f * mousePos.y) / m_pEngine->m_height;
	rayNDC.z = 1.f;
	Vector4 clipCoords = Vector4(rayNDC.x, rayNDC.y, -1.f, 1.f);

	auto view = m_registry.view<CameraComponent>();
	for (auto entity : view)
	{
		auto& cameraComp = m_registry.get<CameraComponent>(entity);
		if (cameraComp.m_cameraEnum == 0)
		{
			auto& camera = cameraComp.m_pCamera;

			Matrix invProj = camera->GetProjRow().Invert();
			Vector4 eyeCoords = Vector4::Transform(clipCoords, invProj);

			if (cameraComp.m_isPerspective)
			{
				eyeCoords.z = 1.f;
				eyeCoords.w = 0.f;
			}

			Matrix invView = camera->GetViewRow().Invert();
			Vector4 rayWorld = Vector4::Transform(eyeCoords, invView);

			physx::PxVec3 rayDir;
			physx::PxVec3 origin;

			if (cameraComp.m_isPerspective)
			{
				rayDir = physx::PxVec3(rayWorld.x, rayWorld.y, rayWorld.z);
				rayDir.normalize();
				origin = physx::PxVec3(camera->mViewPos.x, camera->mViewPos.y, camera->mViewPos.z);
			}
			else
			{
				origin = physx::PxVec3(rayWorld.x, rayWorld.y, rayWorld.z);
				Vector3 forward = camera->mViewDir;
				//Vector3 forward = Vector3(0, 0, 1);
				rayDir = physx::PxVec3(forward.x, forward.y, forward.z);
			}
			physx::PxRaycastBuffer hit;
			if (m_pScene->raycast(origin, rayDir, 100000.f, hit))
			{
				if (hit.hasBlock)
				{
					// �浹�� ��ü
					physx::PxRigidActor* hitActor = hit.block.actor;
					// �浹 ����
					physx::PxVec3 hitPoint = hit.block.position;

					static float eTime = 1;
					eTime += 1.f / 60.f;

// 					if (eTime >= 1)
// 					{
// 						const auto& name = hitActor->getName();
// 						DLOG(LOG_INFO, "Actor Name: " + std::string(name));
// 						//DLOG(LOG_INFO, "view dir: (" + std::to_string(camera->mViewDir.x) + ", " + std::to_string(camera->mViewDir.y) + ", " + std::to_string(camera->mViewDir.z) + ")");
// 						//DLOG(LOG_INFO, "Origin: (" + std::to_string(origin.x) + ", " + std::to_string(origin.y) + ", " + std::to_string(origin.z) + ")");
// 						//DLOG(LOG_INFO, "Direction: (" + std::to_string(rayDir.x) + ", " + std::to_string(rayDir.y) + ", " + std::to_string(rayDir.z) + ")");
// 						DLOG(LOG_INFO, "Hit Position: (" + std::to_string(hitPoint.x) + ", " + std::to_string(hitPoint.y) + ", " + std::to_string(hitPoint.z) + ")");
// 						//DLOG(LOG_INFO, "Hit distance: " + std::to_string(hit.block.distance));
// 						eTime = 0;
// 					}

					return Vector3(hitPoint.x, hitPoint.y, hitPoint.z);
				}
			}

		}
	}
	return Vector3();
}

Vector3 PhysicsManager::PickObejct(const std::string& _name)
{
	POINT mousePos;
	GetCursorPos(&mousePos);
	ScreenToClient(m_pEngine->m_hWnd, &mousePos);

	Vector3 rayNDC;
	rayNDC.x = (2.f * mousePos.x) / m_pEngine->m_width - 1.f;
	rayNDC.y = 1.f - (2.f * mousePos.y) / m_pEngine->m_height;
	rayNDC.z = 1.f;
	Vector4 clipCoords = Vector4(rayNDC.x, rayNDC.y, -1.f, 1.f);

	auto view = m_registry.view<CameraComponent>();
	for (auto entity : view)
	{
		auto& cameraComp = m_registry.get<CameraComponent>(entity);
		if (m_registry.get<CameraComponent>(entity).m_cameraEnum == 0)
		{
			auto& camera = cameraComp.m_pCamera;
			Matrix invProj = camera->GetProjRow().Invert();
			Vector4 eyeCoords = Vector4::Transform(clipCoords, invProj);

			if (cameraComp.m_isPerspective)
			{
				eyeCoords.z = 1.f;
				eyeCoords.w = 0.f;
			}

			Matrix invView = camera->GetViewRow().Invert();
			Vector4 rayWorld = Vector4::Transform(eyeCoords, invView);

			physx::PxVec3 rayDir;
			physx::PxVec3 origin;

			if (cameraComp.m_isPerspective)
			{
				rayDir = physx::PxVec3(rayWorld.x, rayWorld.y, rayWorld.z);
				rayDir.normalize();
				origin = physx::PxVec3(camera->mViewPos.x, camera->mViewPos.y, camera->mViewPos.z);
			}
			else
			{
				origin = physx::PxVec3(rayWorld.x, rayWorld.y, rayWorld.z);
				Vector3 forward = camera->mViewDir;
				//Vector3 forward = Vector3(0,0,1);
				rayDir = physx::PxVec3(forward.x, forward.y, forward.z);
			}

			// ���� �浹�� ó���ϱ� ���� �ִ� 256���� ��Ʈ ����
			const int maxHits = 256;
			physx::PxRaycastHit hitBuffer[maxHits];

			// ��Ʈ ������ ���� �����͸� ����
			physx::PxRaycastBuffer hitResults(hitBuffer, maxHits);

			// ����� ���� ���� �ݹ��� ����� ��ü ���͸�
			RaycastQueryFilterCallback filterCallback(_name);
			physx::PxQueryFilterData filterData(physx::PxQueryFlag::eSTATIC | physx::PxQueryFlag::eDYNAMIC);
			filterData.flags |= physx::PxQueryFlag::ePREFILTER;

			// ����ĳ��Ʈ ���� (���� �ݹ� ����)
			if (m_pScene->raycast(origin, rayDir, 100000.f, hitResults, physx::PxHitFlag::eDEFAULT, filterData, &filterCallback))
			{
				// �̸��� ��ġ�ϴ� ù ��° ��ü�� �浹 ���� ��ȯ
				if (hitResults.hasBlock)
				{
					// �浹�� ��ü
					physx::PxRigidActor* hitActor = hitResults.block.actor;
					// �浹 ����
					physx::PxVec3 hitPoint = hitResults.block.position;

					static float eTime = 1;
					eTime += 1.f / 60.f;

// 					if (eTime >= 1)
// 					{
// 						const auto& name = hitActor->getName();
// 						//DLOG(LOG_INFO, "Actor Name: " + std::string(name));
// 						//DLOG(LOG_INFO, "view dir: (" + std::to_string(camera->mViewDir.x) + ", " + std::to_string(camera->mViewDir.y) + ", " + std::to_string(camera->mViewDir.z) + ")");
// 						//DLOG(LOG_INFO, "Origin: (" + std::to_string(origin.x) + ", " + std::to_string(origin.y) + ", " + std::to_string(origin.z) + ")");
// 						//DLOG(LOG_INFO, "Direction: (" + std::to_string(rayDir.x) + ", " + std::to_string(rayDir.y) + ", " + std::to_string(rayDir.z) + ")");
// 						//DLOG(LOG_INFO, "Hit Position: (" + std::to_string(hitPoint.x) + ", " + std::to_string(hitPoint.y) + ", " + std::to_string(hitPoint.z) + ")");
// 						//DLOG(LOG_INFO, "Hit distance: " + std::to_string(hit.block.distance));
// 						eTime = 0;
// 					}

					return Vector3(hitPoint.x, hitPoint.y, hitPoint.z);
				}
			}
		}
	}

	// ã�� ���� ��� �⺻ �� ��ȯ
	return Vector3();
}

physx::PxFilterData PhysicsManager::GetFilterData(std::shared_ptr<Entity> _pEntity)
{
	physx::PxShape* shape[1];
	_pEntity->GetComponent<Rigidbody>().m_pRigidActor->getShapes(shape, 1);
	return shape[0]->getSimulationFilterData();
}

physx::PxFilterData PhysicsManager::GetFilterData(const entt::entity& _entity)
{
	physx::PxShape* shape[1];
	m_registry.get<Rigidbody>(_entity).m_pRigidActor->getShapes(shape, 1);
	return shape[0]->getSimulationFilterData();
}

void PhysicsManager::SetFilterData(std::shared_ptr<Entity>& _pEntity, const physx::PxFilterData& newFilterData)
{
	physx::PxShape* shape[1];
	_pEntity->GetComponent<Rigidbody>().m_pRigidActor->getShapes(shape, 1);
	m_filterDataUpdates.push(std::make_pair(shape[0], newFilterData));
}

void PhysicsManager::SetFilterData(physx::PxShape* _pShape, const physx::PxFilterData& newFilterData)
{
	m_filterDataUpdates.push(std::make_pair(_pShape, newFilterData));
}

void PhysicsManager::SetFilterData(const entt::entity& _entity, const physx::PxFilterData& newFilterData)
{
	physx::PxShape* shape[1];
	m_registry.get<Rigidbody>(_entity).m_pRigidActor->getShapes(shape, 1);
	m_filterDataUpdates.push(std::make_pair(shape[0], newFilterData));
}

std::shared_ptr<Entity> PhysicsManager::GetEntityFromActor(physx::PxRigidActor* actor)
{
	auto view = m_registry.view<Rigidbody>();

	for (auto entity : view)
	{
		auto rigidbody = m_registry.try_get<Rigidbody>(entity);
		if (rigidbody->m_pRigidActor == actor)
		{
			return rigidbody->m_pOwner;
		}
	}

	return nullptr;  // �ش��ϴ� ��ƼƼ�� ã�� ���� ���
}

void PhysicsManager::SetObjectSleep(std::shared_ptr<Entity> _pEntity)
{
	// RigidDynamic ������Ʈ�� sleep ���·� ��ȯ
	physx::PxRigidDynamic* dynamic = _pEntity->GetComponent<Rigidbody>().m_pRigidActor->is<physx::PxRigidDynamic>();
	if (dynamic)
	{
		dynamic->putToSleep();
	}
}

void PhysicsManager::SetObjectWake(std::shared_ptr<Entity> _pEntity)
{
	// RigidDynamic ������Ʈ�� wake ���·� ��ȯ
	physx::PxRigidDynamic* dynamic = _pEntity->GetComponent<Rigidbody>().m_pRigidActor->is<physx::PxRigidDynamic>();
	if (dynamic)
	{
		dynamic->wakeUp();
	}
}

void PhysicsManager::SetALLObjectSleep()
{
	// ��� RigidDynamic ������Ʈ�� sleep ���·� ��ȯ
	auto view = m_registry.view<Rigidbody>();
	for (auto entity : view)
	{
		auto rigidbody = m_registry.try_get<Rigidbody>(entity);
		if (rigidbody && rigidbody->m_pRigidActor && !rigidbody->m_isKinematic)
		{
			physx::PxRigidDynamic* dynamicActor = rigidbody->m_pRigidActor->is<physx::PxRigidDynamic>();
			if (dynamicActor)
			{
				dynamicActor->putToSleep();  // Sleep ���·� ��ȯ
			}
		}
	}
}

void PhysicsManager::SetALLObjectWake()
{
	// ��� RigidDynamic ������Ʈ�� wake ���·� ��ȯ
	auto view = m_registry.view<Rigidbody>();
	for (auto entity : view)
	{
		auto rigidbody = m_registry.try_get<Rigidbody>(entity);
		if (rigidbody && rigidbody->m_pRigidActor && !rigidbody->m_isKinematic)
		{
			physx::PxRigidDynamic* dynamicActor = rigidbody->m_pRigidActor->is<physx::PxRigidDynamic>();
			if (dynamicActor)
			{
				dynamicActor->wakeUp();  // Wake ���·� ��ȯ
			}
		}
	}
}

void PhysicsManager::ClearFilterUpdate()
{
	while (!m_filterDataUpdates.empty())
	{
		m_filterDataUpdates.pop();
	}
}
