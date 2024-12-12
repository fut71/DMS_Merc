#pragma once
#include "RenderManager.h"
#include "Transform.h"
#include "AnimationComponent.h"
#include "CameraComponent.h"
#include "MeshRenderer.h"
#include "BoneGroupComponent.h"
#include "TargetBoneComponent.h"
#include "LightComponent.h"
#include "CameraComponent.h"
#include "Texture2D.h"
#include "Texture3D.h"
#include "Text.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"
#include "Graphics/DearsGraphicsEngine.h"
#include "WorldManager.h"
#include "TimeManager.h"
#include "WeaponComponent.h"
#include "EquipmentComponent.h"
#include "Name.h"
#include "OutlineComponent.h"
#include "CubeMapComponent.h"
#include "AlphaBlendComponent.h"
#include "FlowTextureComponent.h"

RenderManager::RenderManager(entt::registry& _registry, std::shared_ptr<DearsGraphicsEngine> _pGraphicsEngine
	, WorldManager* _pWorldManager, TimeManager* _pTimeManager)
	: m_registry(_registry)
	, m_pGraphicsEngine(_pGraphicsEngine)
	, m_pWorldManager(_pWorldManager)
	, m_pTimeManager(_pTimeManager)
	, m_renderModel(true)
	, m_renderGeometry(false)
	, m_pWorldCamera(nullptr)
	, m_pLightCamera{ nullptr, nullptr, nullptr }
{
	mpAnimationManager = nullptr;
	m_pLightObserver = std::make_unique<entt::observer>(m_registry, entt::collector.group<LightComponent>().update<LightComponent>());
	m_pMeshObserver = std::make_unique<entt::observer>(m_registry, entt::collector.group<MeshRenderer>().update<MeshRenderer>());
	m_pTexture3DObserver = std::make_unique<entt::observer>(m_registry, entt::collector.group<Texture3D>().update<Texture3D>());
	m_pTexture2DObserver = std::make_unique<entt::observer>(m_registry, entt::collector.group<Texture2D>().update<Texture2D>());
}

RenderManager::~RenderManager()
{
	delete mpAnimationManager;
}

bool RenderManager::Initialize()
{
	mpAnimationManager = new AnimationManager(m_registry, m_pGraphicsEngine);
	return true;
}

bool RenderManager::InitializeScene()
{
	// #sung ���� ���� ī�޶� �ʱ�ȭ, ���� // ����� ���� �ϳ��� ����ī�޶� �����ϰ�����
	//���� ����Ʈ ī�޶� �������ش� -> �׸��ڸ� ����
	auto cameraView = m_registry.view<CameraComponent>();
	for (auto entity : cameraView)
	{
		auto& cameraComponent = m_registry.get<CameraComponent>(entity);
		if (cameraComponent.m_cameraEnum == static_cast<unsigned int>(cameraEnum::WorldCamera))		//����ī�޶��� ��� -> ����ī�޶�� �ϳ��� �����Ͽ����Ѵ�.
		{
			m_pWorldCamera = cameraComponent.m_pCamera;
			m_pGraphicsEngine->SetCamera(m_pWorldCamera);
		}
		if (cameraComponent.m_cameraEnum == static_cast<unsigned int>(cameraEnum::LightCamera))		//����Ʈ ī�޶��� ��� ->����Ʈī�޶� ���� �ִ� MaxLight�� ��ŭ�� �����Ͽ����Ѵ�. 
		{
			m_pLightCamera[cameraComponent.m_lightIndex] = cameraComponent.m_pCamera;
		}
	}

	if (!m_pWorldCamera)
	{
		DLOG(LOG_ERROR, "There are No WorldCamera in the Scene.");
		return false;
	}

	// ���� ���� ����Ʈ �ʱ�ȭ, ����														   
	auto lightView = m_registry.view<LightComponent>();										   //���� �� ������ ���� ���� ������ �ƴϴ�
	PSConstantBufferData lightData;														   //���� ����Ʈ������Ʈ�� ���� ��ü�� �������� ��� �ϳ��� ��ü�� ����Ǳ� �����̴�.
	for (auto entity : lightView)															   //���۵� �������� ����鼭 ������ �����ϴ°� ���ƺ��δ�.
	{
		auto& light = m_registry.get<LightComponent>(entity);
		m_pGraphicsEngine->UpdateCommonConstantBuffer(light.m_commonConstData);
	}

	// �� ���� ����
	auto modelView = m_registry.view<Transform, MeshRenderer>();
	for (auto entity : modelView)
	{
		auto& transform = m_registry.get<Transform>(entity);	//Ư�� ���� Transform*
		auto& model = m_registry.get<MeshRenderer>(entity);	//Ư�� ���� MeshRenderer*
		auto& modelBuffer = model.m_pModel;					//Ư�� MeshRenderer�� modelbuffer
		//auto& name = m_registry.get<Name>(entity).m_name;
		auto& modelConst = model.mVSConstantBufferData;
		auto bone = m_registry.try_get<BoneGroupComponent>(entity);
		auto targetBone = m_registry.try_get<TargetBoneComponent>(entity);
		auto flowTexture = m_registry.try_get<FlowTextureComponent>(entity);

		///#sung ���� ���庯ȯ�� ������� ������Ʈ���ش�.
		modelConst.world = transform.GetTransformMatrix();
		modelConst.invWorld = modelConst.world.Invert();

		// �� ���ؽ�, ���ؽ� ���� ���ε�
		modelBuffer->m_pVertexBuffer = m_pGraphicsEngine->Get_VertexBuffer(model.m_meshName);
		modelBuffer->m_pIndexBuffer = m_pGraphicsEngine->Get_IndexBuffer(model.m_meshName);

		// �� ��� ���� ���� �� ���ε�
		modelBuffer->m_pVSConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(modelConst);
		model.mIs_VSconstant = true;
		modelBuffer->m_pPSConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(lightData);
		model.mIs_PSconstant = true;
		modelBuffer->mNumIndices = m_pGraphicsEngine->Get_NumIndex(model.m_meshName);
		if (!model.m_file.empty())
		{
			modelBuffer->mpTargetModel = m_pGraphicsEngine->Get_ModelInfo(model.m_file);
		}

		// ���� ��ƼƼ�� BoneGroupComponent�� �ִ°�? ���� ��� nullptr��ȯ
		if (bone)
		{
			//���� ���� �� ���ε�
			modelBuffer->m_BoneConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(bone->m_boneData);
			model.mIs_VSBoneConstant = true;
		}

		// 3D �ؽ��� ����
		if (auto texture3d = m_registry.try_get<Texture3D>(entity))
		{
			if (!(texture3d->m_diffuse == ""))
			{
				modelBuffer->m_diffusetexture = m_pGraphicsEngine->Get_Textures(texture3d->m_diffuse);
			}
			if (!(texture3d->m_normal == ""))
			{
				modelBuffer->m_normaltexture = m_pGraphicsEngine->Get_Textures(texture3d->m_normal);
			}
		}

		if (flowTexture)
		{
			modelBuffer->m_pVSWaterConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(flowTexture->m_VSWCSD);
			flowTexture->m_VSWCSD.time = flowTexture->m_time;
			flowTexture->m_VSWCSD.speed = flowTexture->m_speed;
		}

		// �ִϸ��̼� ����
		if (auto animation = m_registry.try_get<AnimationComponent>(entity))
		{
			mpAnimationManager->SetInitialAnimation(animation->m_pOwner);
			//modelBuffer->mpTargetAnimation = m_pGraphicsEngine->Get_Animation(animation->mTargetAnimation);
		}

		// ���� ����
		if (auto weapon = m_registry.try_get<WeaponComponent>(entity))
		{
			// ������ ���庯ȯ�� ������� ������Ʈ���ش�
			auto& weaponModel = weapon->m_pModel;
			auto& weaponConst = weapon->m_weaponConstantBufferData;
			weaponConst.world = transform.GetTransformMatrix();
			weaponConst.invWorld = transform.GetTransformMatrix().Invert();

			// ���� ���ؽ�, ���ؽ� ���� ���ε�
			weaponModel->m_pVertexBuffer = m_pGraphicsEngine->Get_VertexBuffer(weapon->m_meshName);
			weaponModel->m_pIndexBuffer = m_pGraphicsEngine->Get_IndexBuffer(weapon->m_meshName);

			// ���� ��� ���� ���� �� ���ε�
			weaponModel->m_pVSConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(weaponConst);
			weaponModel->m_pPSConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(lightData);
			weaponModel->mNumIndices = m_pGraphicsEngine->Get_NumIndex(weapon->m_meshName);
			weaponModel->mpTargetModel = m_pGraphicsEngine->Get_ModelInfo(weapon->m_fileName);

			auto& weaponEntity = weapon->m_pAttachedEntity;

			weaponEntity->GetComponent<Transform>().m_localPosition = transform.m_localPosition;
			weaponEntity->GetComponent<Transform>().m_localRotation = transform.m_localRotation;
			weaponEntity->GetComponent<Transform>().m_localScale = transform.m_localScale;

			if (weaponEntity->HasComponent<Texture3D>())
			{
				auto& weaponTexture = weaponEntity->GetComponent<Texture3D>();
				if (!(weaponTexture.m_diffuse == ""))
				{
					weaponModel->m_diffusetexture = m_pGraphicsEngine->Get_Textures(weaponTexture.m_diffuse);
				}
				if (!(weaponTexture.m_normal == ""))
				{
					weaponModel->m_normaltexture = m_pGraphicsEngine->Get_Textures(weaponTexture.m_normal);
				}
			}

			weaponConst.world = model.mVSConstantBufferData.world * m_pGraphicsEngine->GetTargetBoneAboveMatrix(model.m_file, weapon->m_boneIndex, weapon->m_scale).Transpose();
			weaponConst.invWorld = weaponConst.world.Invert();

			weapon->m_weaponBoneMatrix = weaponConst.world.Transpose();

			Matrix matrix = Matrix::CreateScale(weapon->m_pAttachedEntity->GetComponent<BoxCollider>().m_size)
				* Matrix::CreateTranslation(weapon->m_pAttachedEntity->GetComponent<BoxCollider>().m_center)
				* weapon->m_weaponBoneMatrix;

			auto& weaponTrs = weapon->m_pAttachedEntity->GetComponent<Transform>();

			// 1.Translation ����
			weaponTrs.m_localPosition = Vector3(matrix._41, matrix._42, matrix._43);
			Vector3 scale;
			// 2. Scale ���� 
			scale.x = Vector3(matrix._11, matrix._12, matrix._13).Length();
			scale.y = Vector3(matrix._21, matrix._22, matrix._23).Length();
			scale.z = Vector3(matrix._31, matrix._32, matrix._33).Length();

			weaponTrs.m_localScale = scale;
			// 3. Rotation ����
			Matrix rotationMatrix = matrix;

			if (scale.x != 0) {
				rotationMatrix._11 /= scale.x;
				rotationMatrix._12 /= scale.x;
				rotationMatrix._13 /= scale.x;
			}
			if (scale.y != 0) {
				rotationMatrix._21 /= scale.y;
				rotationMatrix._22 /= scale.y;
				rotationMatrix._23 /= scale.y;
			}
			if (scale.z != 0) {
				rotationMatrix._31 /= scale.z;
				rotationMatrix._32 /= scale.z;
				rotationMatrix._33 /= scale.z;
			}

			// 4. Rotation ����, ����
			weaponTrs.m_localRotation.x = atan2(rotationMatrix._32, rotationMatrix._33);  // Pitch (X�� ȸ��)
			weaponTrs.m_localRotation.y = atan2(-rotationMatrix._31, sqrt(rotationMatrix._32 * rotationMatrix._32 + rotationMatrix._33 * rotationMatrix._33));  // Yaw (Y�� ȸ��)
			weaponTrs.m_localRotation.z = atan2(rotationMatrix._21, rotationMatrix._11);  // Roll (Z�� ȸ��)

			weaponTrs.m_localMatrix = matrix;
			//parent.m_weaponBoneMatrix = matrix;

		}

		// ��� ����
		if (auto equips = m_registry.try_get<EquipmentComponent>(entity))
		{
			for (auto& equip : equips->m_pEquipments)
			{
				if (!equip->m_meshName.empty())
				{
					auto& buffer = equip->m_pModelBuffer;
					// ��� �� ���ؽ�, ���ؽ� ���� ���ε�
					buffer->m_pVertexBuffer = m_pGraphicsEngine->Get_VertexBuffer(equip->m_meshName);
					buffer->m_pIndexBuffer = m_pGraphicsEngine->Get_IndexBuffer(equip->m_meshName);

					// ����� �� ��� ���� ����, ���ε�
					buffer->m_pVSConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(modelConst);
					buffer->m_pPSConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(lightData);
					buffer->mNumIndices = m_pGraphicsEngine->Get_NumIndex(equip->m_meshName);
					buffer->mpTargetModel = m_pGraphicsEngine->Get_ModelInfo(equip->m_modelFileName);

					equip->m_targetBoneIndex = m_pGraphicsEngine->Get_TargetModelBoneIndex(equip->m_targetModelName, equip->m_boneName);

					//���� ���� �� ���ε�
					buffer->m_TargetBoneConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(equip->m_targetBoneConstantBufferData);

					// ����� �ؽ��� ����
					if (!(equip->m_diffuseTexture == ""))
					{
						buffer->m_diffusetexture = m_pGraphicsEngine->Get_Textures(equip->m_diffuseTexture);
					}
					if (!(equip->m_normalTexture == ""))
					{
						buffer->m_normaltexture = m_pGraphicsEngine->Get_Textures(equip->m_normalTexture);
					}

				}
			}
		}

		// �ܰ��� ����
		if (auto outline = m_registry.try_get<OutlineComponent>(entity))
		{
			modelBuffer->m_pVSEdgeConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(modelConst);
			outline->m_VSEdgeCBD.EdgeScaleMatrix = Matrix::CreateScale(outline->m_thickness);
			outline->m_PSEdgeCBD.color = outline->m_color;
			modelBuffer->m_pPSEdgeConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(outline->m_PSEdgeCBD);
		}
	}
	SetCubeMap("skybox_diffuse.dds", "skybox_specular.dds");
	// ť��� ����
	auto cubemapView = m_registry.view<CubeMapComponent>();
	for (auto& entity : cubemapView)
	{
		auto& cubemap = m_registry.get<CubeMapComponent>(entity);
		auto& cubeModel = cubemap.m_pModel;
		cubeModel->m_pVSConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(cubemap.m_VSCBD);
		cubeModel->m_pPSConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(lightData);
		cubeModel->m_pVertexBuffer = m_pGraphicsEngine->Get_VertexBuffer("CubeMap");
		cubeModel->m_pIndexBuffer = m_pGraphicsEngine->Get_IndexBuffer("CubeMap");
		cubeModel->mNumIndices = m_pGraphicsEngine->Get_NumIndex("CubeMap");
		SetCubeMap(cubemap.m_diffuse, cubemap.m_specular);
	}

	return true;
}

void RenderManager::SetCubeMap(std::string diffuseTextureName, std::string specularTextureName)
{
	m_pGraphicsEngine->Set_CubeMap(diffuseTextureName, specularTextureName);
}

bool RenderManager::InitailizeEntity(std::shared_ptr<Entity> _entity)
{
	PSConstantBufferData lightData;																//���׸��󶧹��� �ʿ��ϴ�.

	if (_entity->HasComponent<Transform>() && _entity->HasComponent<MeshRenderer>())
	{
		auto& transform = _entity->GetComponent<Transform>();	//Ư�� ���� Transform*
		auto& model = _entity->GetComponent<MeshRenderer>();	//Ư�� ���� MeshRenderer*
		auto& modelBuffer = model.m_pModel;					//Ư�� MeshRenderer�� modelbuffer


		auto& modelConst = model.mVSConstantBufferData;

		///���� ���庯ȯ�� ������� ������Ʈ���ش�.
		modelConst.world = transform.GetTransformMatrix();
		modelConst.invWorld = transform.GetTransformMatrix().Invert();


		// �� ���ؽ�, ���ؽ� ���� ���ε�
		modelBuffer->m_pVertexBuffer = m_pGraphicsEngine->Get_VertexBuffer(model.m_meshName);
		modelBuffer->m_pIndexBuffer = m_pGraphicsEngine->Get_IndexBuffer(model.m_meshName);

		// �� ��� ���� ���� �� ���ε�
		modelBuffer->m_pVSConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(modelConst);
		model.mIs_VSconstant = true;
		modelBuffer->m_pPSConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(lightData);
		model.mIs_PSconstant = true;
		modelBuffer->mNumIndices = m_pGraphicsEngine->Get_NumIndex(model.m_meshName);
		modelBuffer->mpTargetModel = m_pGraphicsEngine->Get_ModelInfo(model.m_file);

		// ���� ��ƼƼ�� BoneGroupComponent�� �ִ°�? ���� ��� nullptr��ȯ
		if (_entity->HasComponent<BoneGroupComponent>())
		{
			//���� ���� �� ���ε�
			modelBuffer->m_BoneConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(_entity->GetComponent<BoneGroupComponent>().m_boneData);
			model.mIs_VSBoneConstant = true;
		}

		// 3D �ؽ��� ����
		if (_entity->HasComponent<Texture3D>())
		{
			if (!(_entity->GetComponent<Texture3D>().m_diffuse == ""))
			{
				modelBuffer->m_diffusetexture = m_pGraphicsEngine->Get_Textures(_entity->GetComponent<Texture3D>().m_diffuse);
			}
			if (!(_entity->GetComponent<Texture3D>().m_normal == ""))
			{
				modelBuffer->m_normaltexture = m_pGraphicsEngine->Get_Textures(_entity->GetComponent<Texture3D>().m_normal);
			}
		}

		// �ִϸ��̼� ����
		if (_entity->HasComponent<AnimationComponent>())
		{
			mpAnimationManager->SetInitialAnimation(_entity);
		}

		// ���� ����
		if (_entity->HasComponent<WeaponComponent>())
		{
			auto& weapon = _entity->GetComponent<WeaponComponent>();
			// ������ ���庯ȯ�� ������� ������Ʈ���ش�
			auto& weaponModel = weapon.m_pModel;
			auto& weaponConst = weapon.m_weaponConstantBufferData;
			weaponConst.world = transform.GetTransformMatrix();
			weaponConst.invWorld = transform.GetTransformMatrix().Invert();

			// ���� ���ؽ�, ���ؽ� ���� ���ε�
			weaponModel->m_pVertexBuffer = m_pGraphicsEngine->Get_VertexBuffer(weapon.m_meshName);
			weaponModel->m_pIndexBuffer = m_pGraphicsEngine->Get_IndexBuffer(weapon.m_meshName);

			// ���� ��� ���� ���� �� ���ε�
			weaponModel->m_pVSConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(weaponConst);
			weaponModel->m_pPSConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(lightData);
			weaponModel->mNumIndices = m_pGraphicsEngine->Get_NumIndex(weapon.m_meshName);
			weaponModel->mpTargetModel = m_pGraphicsEngine->Get_ModelInfo(weapon.m_fileName);

			auto& weaponEntity = weapon.m_pAttachedEntity;

			weaponEntity->GetComponent<Transform>().m_localPosition = transform.m_localPosition;
			weaponEntity->GetComponent<Transform>().m_localRotation = transform.m_localRotation;
			weaponEntity->GetComponent<Transform>().m_localScale = transform.m_localScale;

			if (weaponEntity->HasComponent<Texture3D>())
			{
				auto& weaponTexture = weaponEntity->GetComponent<Texture3D>();
				if (!(weaponTexture.m_diffuse == ""))
				{
					weaponModel->m_diffusetexture = m_pGraphicsEngine->Get_Textures(weaponTexture.m_diffuse);
				}
				if (!(weaponTexture.m_normal == ""))
				{
					weaponModel->m_normaltexture = m_pGraphicsEngine->Get_Textures(weaponTexture.m_normal);
				}
			}

			weaponConst.world = model.mVSConstantBufferData.world * m_pGraphicsEngine->GetTargetBoneAboveMatrix(model.m_file, weapon.m_boneIndex, weapon.m_scale).Transpose();
			weaponConst.invWorld = weaponConst.world.Invert();

			weapon.m_weaponBoneMatrix = weaponConst.world.Transpose();

			Matrix matrix = Matrix::CreateScale(weapon.m_pAttachedEntity->GetComponent<BoxCollider>().m_size)
				* Matrix::CreateTranslation(weapon.m_pAttachedEntity->GetComponent<BoxCollider>().m_center)
				* weapon.m_weaponBoneMatrix;

			auto& weaponTrs = weapon.m_pAttachedEntity->GetComponent<Transform>();

			// 1.Translation ����
			weaponTrs.m_localPosition = Vector3(matrix._41, matrix._42, matrix._43);
			Vector3 scale;
			// 2. Scale ���� 
			scale.x = Vector3(matrix._11, matrix._12, matrix._13).Length();
			scale.y = Vector3(matrix._21, matrix._22, matrix._23).Length();
			scale.z = Vector3(matrix._31, matrix._32, matrix._33).Length();

			weaponTrs.m_localScale = scale;
			// 3. Rotation ����
			Matrix rotationMatrix = matrix;

			if (scale.x != 0) {
				rotationMatrix._11 /= scale.x;
				rotationMatrix._12 /= scale.x;
				rotationMatrix._13 /= scale.x;
			}
			if (scale.y != 0) {
				rotationMatrix._21 /= scale.y;
				rotationMatrix._22 /= scale.y;
				rotationMatrix._23 /= scale.y;
			}
			if (scale.z != 0) {
				rotationMatrix._31 /= scale.z;
				rotationMatrix._32 /= scale.z;
				rotationMatrix._33 /= scale.z;
			}

			// 4. Rotation ����, ����
			weaponTrs.m_localRotation.x = atan2(rotationMatrix._32, rotationMatrix._33);  // Pitch (X�� ȸ��)
			weaponTrs.m_localRotation.y = atan2(-rotationMatrix._31, sqrt(rotationMatrix._32 * rotationMatrix._32 + rotationMatrix._33 * rotationMatrix._33));  // Yaw (Y�� ȸ��)
			weaponTrs.m_localRotation.z = atan2(rotationMatrix._21, rotationMatrix._11);  // Roll (Z�� ȸ��)

			weaponTrs.m_localMatrix = matrix;
			//parent.m_weaponBoneMatrix = matrix;


		}
		// ��� ����
		if (_entity->HasComponent<EquipmentComponent>())
		{
			auto& equips = _entity->GetComponent<EquipmentComponent>();
			for (auto& equip : equips.m_pEquipments)
			{
				if (!equip->m_meshName.empty())
				{
					auto& buffer = equip->m_pModelBuffer;
					// ��� �� ���ؽ�, ���ؽ� ���� ���ε�
					buffer->m_pVertexBuffer = m_pGraphicsEngine->Get_VertexBuffer(equip->m_meshName);
					buffer->m_pIndexBuffer = m_pGraphicsEngine->Get_IndexBuffer(equip->m_meshName);

					// ����� �� ��� ���� ����, ���ε�
					buffer->m_pVSConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(modelConst);
					buffer->m_pPSConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(lightData);
					buffer->mNumIndices = m_pGraphicsEngine->Get_NumIndex(equip->m_meshName);
					buffer->mpTargetModel = m_pGraphicsEngine->Get_ModelInfo(equip->m_modelFileName);

					equip->m_targetBoneIndex = m_pGraphicsEngine->Get_TargetModelBoneIndex(equip->m_targetModelName, equip->m_boneName);

					//���� ���� �� ���ε�
					buffer->m_TargetBoneConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(equip->m_targetBoneConstantBufferData);

					// ����� �ؽ��� ����
					if (!(equip->m_diffuseTexture.empty()))
					{
						buffer->m_diffusetexture = m_pGraphicsEngine->Get_Textures(equip->m_diffuseTexture);
					}
					if (!(equip->m_normalTexture.empty()))
					{
						buffer->m_normaltexture = m_pGraphicsEngine->Get_Textures(equip->m_normalTexture);
					}
				}

			}
		}

		// �ܰ��� ����
		if (_entity->HasComponent<OutlineComponent>())
		{
			auto& outline = _entity->GetComponent<OutlineComponent>();
			modelBuffer->m_pVSEdgeConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(modelConst);
			outline.m_VSEdgeCBD.EdgeScaleMatrix = Matrix::CreateScale(outline.m_thickness);
			outline.m_PSEdgeCBD.color = outline.m_color;
			modelBuffer->m_pPSEdgeConstantBuffer = m_pGraphicsEngine->CreateConstantBuffer(outline.m_PSEdgeCBD);
		}
	}
	return true;
}

void RenderManager::Update(float _dTime)
{
	// ����� �ؽ��� ������Ʈ
	UpdateTexture();

	//����Ʈ ������Ʈ�� ������Ʈ���ش�. -> ���������� ���� ������Ʈ ������ ������
	UpdateLight();

	// ��� ���� ������Ʈ
	UpdateModel(_dTime);

	// ť��� ������Ʈ
	UpdateCubeMap();

	// �ȼ� ���̴� ��� ���� ������Ʈ
	UpdatePSCB();

	m_pGraphicsEngine->Update();

}

void RenderManager::UpdateEntityTexture(std::shared_ptr<Entity> _entity)
{

	if (!_entity->HasComponent<MeshRenderer>() || !_entity->HasComponent<Texture3D>())
	{
		return;
	}
	auto& texture3d = _entity->GetComponent<Texture3D>();
	auto& mesh = _entity->GetComponent<MeshRenderer>();

	//��ǻ�� �ؽ��ĸ� �����´�.
	if (!(texture3d.m_diffuse.empty()))
	{
		auto newTexture3D = m_pGraphicsEngine->Get_Textures(texture3d.m_diffuse);
		mesh.m_pModel->m_diffusetexture = newTexture3D;
	}
	//�븻 �ؽ��ĸ� �����´�.
	if (!(texture3d.m_normal.empty()))
	{
		auto newTextureNomal3D = m_pGraphicsEngine->Get_Textures(texture3d.m_normal);
		mesh.m_pModel->m_normaltexture = newTextureNomal3D;
	}
}

void RenderManager::LateUpdate(float _dTime)
{
	auto boxGeometryView = m_registry.view<BoxCollider, Transform>();
	for (auto entity : boxGeometryView)
	{
		auto& transform = m_registry.get<Transform>(entity);
		auto& collider = m_registry.get<BoxCollider>(entity);
		// 		auto& name = m_registry.get<Name>(entity).m_name;
		// 		auto uid = static_cast<int>(entity);
				// ��� �ݶ��̴�
		if (transform.m_pParent)
		{
			auto parentTrs = transform.m_pParent;
			// 			auto name2 = parentTrs->m_pOwner->GetName();
			if (!parentTrs->m_pOwner.get())
			{
				continue;
			}
			if (parentTrs->m_pOwner->HasComponent<WeaponComponent>())
			{
				auto& parent = parentTrs->m_pOwner->GetComponent<WeaponComponent>();
				auto& parentMesh = parentTrs->m_pOwner->GetComponent<MeshRenderer>();
				Matrix matrix
					= Matrix::CreateScale(collider.m_size)
					* Matrix::CreateTranslation(Vector3(collider.m_center))
					* parent.m_weaponBoneMatrix;
				// 1.Translation ����
				transform.m_localPosition = Vector3(matrix._41, matrix._42, matrix._43);
				Vector3 scale;
				// 2. Scale ���� 
				scale.x = Vector3(matrix._11, matrix._12, matrix._13).Length();
				scale.y = Vector3(matrix._21, matrix._22, matrix._23).Length();
				scale.z = Vector3(matrix._31, matrix._32, matrix._33).Length();

				transform.m_localScale = scale;
				// 3. Rotation ����
				Matrix rotationMatrix = matrix;

				if (scale.x != 0) {
					rotationMatrix._11 /= scale.x;
					rotationMatrix._12 /= scale.x;
					rotationMatrix._13 /= scale.x;
				}
				if (scale.y != 0) {
					rotationMatrix._21 /= scale.y;
					rotationMatrix._22 /= scale.y;
					rotationMatrix._23 /= scale.y;
				}
				if (scale.z != 0) {
					rotationMatrix._31 /= scale.z;
					rotationMatrix._32 /= scale.z;
					rotationMatrix._33 /= scale.z;
				}

				// 4. Rotation ����, ����
				transform.m_localRotation.x = atan2(rotationMatrix._32, rotationMatrix._33);  // Pitch (X�� ȸ��)
				transform.m_localRotation.y = atan2(-rotationMatrix._31, sqrt(rotationMatrix._32 * rotationMatrix._32 + rotationMatrix._33 * rotationMatrix._33));  // Yaw (Y�� ȸ��)
				transform.m_localRotation.z = atan2(rotationMatrix._21, rotationMatrix._11);  // Roll (Z�� ȸ��)

				transform.m_localMatrix = matrix;
				//parent.m_weaponBoneMatrix = matrix;
			}

		}
	}

	// ī�޶� ������Ʈ
	auto cameraView = m_registry.view<CameraComponent>();
	for (auto entity : cameraView)
	{
		auto& camera = cameraView.get<CameraComponent>(entity);
	}
}

void RenderManager::Render(float _dTime)
{
	LightComponent* light = nullptr;
	//����Ʈ ������Ʈ�� ������Ʈ���ش�. -> ���������� ���� ������Ʈ ������ ������
	auto lightComponent = m_registry.view<LightComponent>();
	for (auto entity : lightComponent)
	{
		if (light = m_registry.try_get<LightComponent>(entity))
		{
			// 			m_pGraphicsEngine->UpdateCommonConstantBuffer(light->m_commonConstData);
			// 			light->m_commonConstData.light[0].viewProj = (m_pLightCamera[0]->GetViewRow() * m_pLightCamera[0]->GetProjRow()).Transpose();
		}
	}

	// �׸��� ������
	if (m_renderShadow && m_renderModel && light)
	{
		RenderShadow(light);
	}

	// �� ������
	if (m_renderModel)
	{
		RenderModel(light);
	}

	// ���� ���� �� ����� ���� ������
	if (m_renderGeometry)
	{
		RenderGeometry(light);
	}

	m_pGraphicsEngine->RendParticle();

	/// ����� ���� ���
	if (m_renderGeometry)
	{
		// 		m_pGraphicsEngine->UIStartFontID("B.ttf");
		// 		std::string fpsStr = std::format("{:.2f}", m_pTimeManager->FPS());
		// 		std::u8string fps(fpsStr.begin(), fpsStr.end());
		// 		std::string dTStr = std::format("{:.6f}", m_pTimeManager->DeltaTime());
		// 		std::u8string dTime(dTStr.begin(), dTStr.end());
		// 		m_pGraphicsEngine->UIDrawText(Vector2(0, 0), fps);
		// 		m_pGraphicsEngine->UIDrawText(Vector2(0, 40), dTime + std::u8string(u8"ms"));
		// 		m_pGraphicsEngine->UIFinFontID();
		// 		DLOG(LOG_INFO, fpsStr);
		// 		DLOG(LOG_INFO, dTStr);
	}
}

void RenderManager::BeginRender()
{
	m_pGraphicsEngine->BeginRender();
}

void RenderManager::EndRender()
{
	m_pGraphicsEngine->EndRender();
}

void RenderManager::Finalize()
{
	m_pGraphicsEngine->Finalize(); // ������
}

void RenderManager::SetRenderMode(bool _model, bool _geometry, bool _shadow)
{
	m_renderModel = _model;
	m_renderGeometry = _geometry;
	m_renderShadow = _shadow;
}

void RenderManager::ChangeCamera(const uint8_t _index)
{
	auto cameraView = m_registry.view<CameraComponent>();
	for (auto entity : cameraView)
	{
		auto cameraComponent = m_registry.try_get<CameraComponent>(entity);
		// 		if (cameraComponent->m_index == _index)
		// 		{
		// 			m_pCurrentCamera = cameraComponent->m_pCamera;
		// 
		// 			m_pGraphicsEngine->SetCamera(m_pCurrentCamera);
		// 		}
	}
}

int RenderManager::GetScreenWidth()
{
	return m_pGraphicsEngine->GetScreenWidth();
}

int RenderManager::GetScreenHeight()
{
	return m_pGraphicsEngine->GetScreenHeight();
}

void RenderManager::AddModel(std::string _basePath, std::string _fileName)
{
	m_pGraphicsEngine->AddModel(_basePath, _fileName);
}

void RenderManager::AddAnimation(std::string _basePath, std::string _fileName)
{
	m_pGraphicsEngine->AddAnimation(_basePath, _fileName);
}

Animation* RenderManager::Get_Animation(std::string _animeName)
{
	return m_pGraphicsEngine->Get_Animation(_animeName);
}

void RenderManager::Erase_Animation(std::string _animName)
{
	m_pGraphicsEngine->Erase_Animation(_animName);
}

void RenderManager::Add3DTexture(std::string _basePath, std::string _fileName)
{
	m_pGraphicsEngine->Add3DTexture(_basePath, _fileName);
}

void RenderManager::AddDDSTexture(std::string _basePath, std::string _fileName)
{
	m_pGraphicsEngine->AddDDSTexture(_basePath, _fileName);
}

void RenderManager::Erase_Textures(std::string _textureName)
{
	m_pGraphicsEngine->Erase_Textures(_textureName);
}

int RenderManager::Get_TargetModelBoneIndex(std::string _modelName, std::string _boneName)
{
	return m_pGraphicsEngine->Get_TargetModelBoneIndex(_modelName, _boneName);
}

Matrix RenderManager::GetTargetBoneAboveMatrix(std::string _targetModel, std::string _targetBoneName, float _scale /*= 1.0f*/)
{
	return m_pGraphicsEngine->GetTargetBoneAboveMatrix(_targetModel, _targetBoneName, _scale);
}

AABB RenderManager::Get_AABB(std::string __targetModel)
{
	return m_pGraphicsEngine->Get_AABB(__targetModel);
}

void RenderManager::LightInitialize(CommonConstantBufferData* _ccBufferData, UINT _num)
{
	m_pGraphicsEngine->LightInitialize(_ccBufferData, _num);
}

void RenderManager::SetDirLight(CommonConstantBufferData* _ccBufferData, UINT _index, float _strength, Vector3 _dir, Vector3 _color /*= Vector3(1.0f, 1.0f, 1.0f)*/)
{
	m_pGraphicsEngine->SetDirLight(_ccBufferData, _index, _strength, _dir, _color);
}

void RenderManager::PrintLightInfo(CommonConstantBufferData* _ccBufferData)
{
	m_pGraphicsEngine->PrintLightInfo(_ccBufferData);
}

void RenderManager::CameraSetPerspective()
{
	auto cameraView = m_registry.view<CameraComponent>();
	for (auto entity : cameraView)
	{
		auto& cameraComponent = m_registry.get<CameraComponent>(entity);
		if (cameraComponent.m_cameraEnum == 0)
		{
			cameraComponent.m_pCamera->SetPerspective();
			cameraComponent.m_isPerspective = true;
		}
	}

}

void RenderManager::CameraSetOrthographic(float _scale)
{
	auto cameraView = m_registry.view<CameraComponent>();
	for (auto entity : cameraView)
	{
		auto& cameraComponent = m_registry.get<CameraComponent>(entity);
		if (cameraComponent.m_cameraEnum == 0)
		{
			cameraComponent.m_pCamera->SetOrthgraphic(_scale);
			cameraComponent.m_isPerspective = false;
		}
	}
}

void RenderManager::SetOpacityFactor(const Vector4& _value)
{
	m_opacityFactor[0] = _value.x;
	m_opacityFactor[1] = _value.y;
	m_opacityFactor[2] = _value.z;
	m_opacityFactor[3] = _value.w;
	m_pGraphicsEngine->SetOpacityFactor(m_opacityFactor);
}

void RenderManager::AddParticle(uint16_t _num, CSParticleData& _particleData) const
{
	m_pGraphicsEngine->m_pParticleManager->AddParticle(_num, _particleData);
}

void RenderManager::RenderShadow(LightComponent* _light)
{
	m_pGraphicsEngine->SetCamera(m_pLightCamera[0]);
	m_pGraphicsEngine->UpdateCommonConstantBuffer(_light->m_commonConstData);

	auto view = m_registry.view<MeshRenderer>();
	for (auto entity : view)
	{
		auto& mesh = m_registry.get<MeshRenderer>(entity);
		auto equips = m_registry.try_get<EquipmentComponent>(entity);
		auto weapon = m_registry.try_get<WeaponComponent>(entity);
		auto buffer = mesh.m_pModel.get();

		if (auto alpha = m_registry.try_get<AlphaBlendComponent>(entity))
		{
			continue;
		}

		if (mesh.m_pModel == nullptr)
		{
			DLOG(LOG_ERROR, "Model Pointer is Nullptr");
		}
		else
		{
			if (mesh.mIs_VSBoneConstant)
			{
				m_pGraphicsEngine->mpRenderer->RenderAniDepthMap(buffer);
			}
			else
			{
				if (mesh.mIs_VSTargetBoneconstant)
				{
					if (!mesh.m_meshName.empty())
					{
						//m_pGraphicsEngine->mpRenderer->RenderEquipDepthMap(buffer);
					}
				}
				else
				{
					m_pGraphicsEngine->mpRenderer->RenderDepthMap(buffer);
				}
			}
		}
		if (weapon)
		{
			m_pGraphicsEngine->mpRenderer->RenderDepthMap(weapon->m_pModel.get());
		}
		if (equips)
		{
			for (auto& equip : equips->m_pEquipments)
			{
				if (!mesh.m_meshName.empty())
				{
					//m_pGraphicsEngine->mpRenderer->RenderEquipDepthMap(equip->m_pModelBuffer.get());
				}
			}
		}
	}
}

void RenderManager::RenderModel(LightComponent* _light)
{
	m_pGraphicsEngine->SetCamera(m_pWorldCamera);
	if (_light)
	{
		m_pGraphicsEngine->UpdateCommonConstantBuffer(_light->m_commonConstData);
	}

	auto cubeView = m_registry.view<CubeMapComponent>();
	for (auto& entity : cubeView)
	{
		auto& cube = m_registry.get<CubeMapComponent>(entity);
		m_pGraphicsEngine->Rend_CubeMap(cube.m_pModel.get());
	}

	auto view = m_registry.view<MeshRenderer>();
	for (auto& entity : view)
	{
		auto& mesh = m_registry.get<MeshRenderer>(entity);
		auto equips = m_registry.try_get<EquipmentComponent>(entity);
		auto weapon = m_registry.try_get<WeaponComponent>(entity);
		auto buffer = mesh.m_pModel.get();
		auto& name = m_registry.get<Name>(entity).m_name;
		auto outline = m_registry.try_get<OutlineComponent>(entity);

		if (auto alpha = m_registry.try_get<AlphaBlendComponent>(entity))
		{
			if (outline)
			{
				m_pGraphicsEngine->Rend_EdgeModel(buffer);
			}
			SetOpacityFactor(Vector4(alpha->m_alpha));
			m_pGraphicsEngine->Rend_OpacitiyModel(buffer);
			continue;
		}

		if (auto flowTex = m_registry.try_get<FlowTextureComponent>(entity))
		{
			m_pGraphicsEngine->Rend_Water(buffer);
			continue;
		}

		if (mesh.m_pModel == nullptr)
		{
			DLOG(LOG_ERROR, "Model Pointer is Nullptr");
		}
		else
		{
			if (mesh.mIs_CubeMap)
			{
				m_pGraphicsEngine->Rend_CubeMap(buffer);
			}
			else
			{
				if (mesh.mIs_VSBoneConstant)
				{
					m_pGraphicsEngine->Rend_AnimateModel(buffer);
				}
				else
				{
					if (mesh.mIs_VSTargetBoneconstant)
					{
						if (mesh.m_meshName != "")
						{
							m_pGraphicsEngine->Rend_EquipmentModel(buffer);
						}
					}
					else
					{
						m_pGraphicsEngine->Rend_Model(buffer);
					}
				}
			}
		}

		if (weapon)
		{
			m_pGraphicsEngine->Rend_Model(weapon->m_pModel.get());
		}

		if (equips)
		{
			for (auto& equip : equips->m_pEquipments)
			{
				if (!equip->m_meshName.empty())
				{
					m_pGraphicsEngine->Rend_EquipmentModel(equip->m_pModelBuffer.get());
				}
			}
		}

		if (outline)
		{
			m_pGraphicsEngine->Rend_EdgeModel(buffer);
		}
	}

}

void RenderManager::RenderGeometry(LightComponent* _light)
{
	// ����Ʈ�� ��ġ�� ����.
	if (_light)
	{
		m_pGraphicsEngine->Rend_DebugSphere({ 1,1,1 }, { 0,0,0 }, m_pLightCamera[0]->GetmViewPos());
	}

	auto boxGeometryView = m_registry.view<BoxCollider, Transform>();
	for (auto entity : boxGeometryView)
	{
		auto& transform = m_registry.get<Transform>(entity);
		auto& collider = m_registry.get<BoxCollider>(entity);

		// ��� �ݶ��̴�
		if (transform.m_pParent)
		{
			// �ݶ��̴� ������
			m_pGraphicsEngine->Rend_DebugBox(
				Matrix()
				, Matrix()
				, transform.m_localMatrix
			);
		}
		else
		{
			m_pGraphicsEngine->Rend_DebugBox(collider.m_size
				, transform.m_localRotation
				, transform.m_localPosition + collider.m_center);
		}
	}
	auto sphereGeometryView = m_registry.view<SphereCollider, Transform>();
	for (auto entity : sphereGeometryView)
	{
		auto& transform = m_registry.get<Transform>(entity);
		auto& collider = m_registry.get<SphereCollider>(entity);

		m_pGraphicsEngine->Rend_DebugSphere(Vector3(collider.m_radius), transform.m_localRotation
			, transform.m_localPosition + collider.m_center);
	}
	auto capsuleGeometryView = m_registry.view<CapsuleCollider, Transform>();
	for (auto entity : capsuleGeometryView)
	{
		auto& transform = m_registry.get<Transform>(entity);
		auto& collider = m_registry.get<CapsuleCollider>(entity);
		m_pGraphicsEngine->Rend_DebugCapsule(Vector3(collider.m_radius, collider.m_height, collider.m_radius)
			, transform.m_localRotation
			, transform.m_localPosition + collider.m_center);
	}
}

void RenderManager::UpdateLight()
{
	auto lightComponent = m_registry.view<LightComponent>();
	for (auto entity : lightComponent)
	{
		auto& light = m_registry.get<LightComponent>(entity);
		m_pGraphicsEngine->UpdateCommonConstantBuffer(light.m_commonConstData);
		light.m_commonConstData.light[0].viewProj = (m_pLightCamera[0]->GetViewRow() * m_pLightCamera[0]->GetProjRow()).Transpose();
	}
}

void RenderManager::UpdateModel(float _dTime)
{
	auto viewAnim = m_registry.view<MeshRenderer>();
	for (auto& entity : viewAnim)
	{
		auto& mesh = m_registry.get<MeshRenderer>(entity);
		auto ani = m_registry.try_get<AnimationComponent>(entity);
		auto bone = m_registry.try_get<BoneGroupComponent>(entity);
		//auto& name = m_registry.get<Name>(entity).m_name;
		auto& transform = m_registry.get<Transform>(entity);
		auto equips = m_registry.try_get<EquipmentComponent>(entity);
		auto outline = m_registry.try_get<OutlineComponent>(entity);
		auto flowTexture = m_registry.try_get<FlowTextureComponent>(entity);

		ModelBuffer* modelBuffer = mesh.m_pModel.get();
		if (ani)
		{
			mpAnimationManager->AnimationUpdate(mesh.m_pOwner, _dTime);
		}

		mesh.mVSConstantBufferData.world = transform.GetTransformMatrix();
		mesh.mVSConstantBufferData.invWorld = mesh.mVSConstantBufferData.world.Invert();

		if (mesh.mIs_VSconstant == true)
		{
			m_pGraphicsEngine->UpdateConstantBuffer(modelBuffer, mesh.mVSConstantBufferData);
		}

		// �ִϸ��̼� ������Ʈ�� ���� �����Ѵ�.
		if (ani)
		{

			if (ani->mpNextTargetAnimation) //if (modelBuffer->mpNextTargetAnimation)
			{
				if (!m_pGraphicsEngine->UpdateTransitionBoneConstantBuffer(modelBuffer, bone->m_boneData))			  ///false�� ��ȯ�Ǿ��°�? -> ���� �ִϸ��̼��� ������ �ִϸ��̼����� ��ü�Ѵ�.
				{
					mpAnimationManager->InterPolationAnimation(mesh.m_pOwner);
				}
			}
			else
			{
				m_pGraphicsEngine->UpdateBoneConstantBuffer(modelBuffer, bone->m_boneData);
			}
		}

		// ���� ������Ʈ
		if (auto weapon = m_registry.try_get<WeaponComponent>(entity))
		{
			if (weapon->m_pAttachedEntity)
			{
				auto& temp = weapon->m_weaponConstantBufferData;
				// temp.world = weapon->m_pAttachedEntity->GetComponent<Transform>().m_localMatrix.Transpose();
				// temp.invWorld = weapon->m_pAttachedEntity->GetComponent<Transform>().m_localMatrix.Transpose().Invert();

				temp.world = mesh.mVSConstantBufferData.world * m_pGraphicsEngine->GetTargetBoneAboveMatrix(mesh.m_file, weapon->m_boneIndex, weapon->m_scale).Transpose();
				temp.invWorld = temp.world.Invert();

				weapon->m_weaponBoneMatrix = temp.world.Transpose();
				m_pGraphicsEngine->UpdateConstantBuffer(weapon->m_pModel.get(),
					temp);
			}

		}

		// ��� ������Ʈ
		if (equips)
		{
			auto boneGroup = m_registry.try_get<BoneGroupComponent>(entity);

			for (auto& equip : equips->m_pEquipments)
			{
				if (!equip->m_meshName.empty())
				{
					m_pGraphicsEngine->UpdateConstantBuffer(equip->m_pModelBuffer.get(), mesh.mVSConstantBufferData);

					equip->m_targetBoneConstantBufferData.targrtBoneMatrix = boneGroup->m_boneData.bone[equip->m_targetBoneIndex];
					m_pGraphicsEngine->UpdateTargetBoneConstantBuffer(equip->m_pModelBuffer.get(), equip->m_targetBoneConstantBufferData);
				}
			}
		}

		// �ܰ��� ������Ʈ
		if (outline)
		{
			outline->m_PSEdgeCBD.color = outline->m_color;
			m_pGraphicsEngine->UpdateVSEdgeConstantBuffer(modelBuffer, outline->m_VSEdgeCBD);
			m_pGraphicsEngine->UpdatePSEdgeConstantBuffer(modelBuffer, outline->m_PSEdgeCBD);
		}

		if (flowTexture)
		{
			flowTexture->m_VSWCSD.time += _dTime;
			m_pGraphicsEngine->UpdateVSWaterConstantBuffer(modelBuffer, flowTexture->m_VSWCSD);
		}
	}
}

void RenderManager::UpdateCubeMap()
{
	auto cubeView = m_registry.view<CubeMapComponent>();
	for (auto& entity : cubeView)
	{
		auto& cube = m_registry.get<CubeMapComponent>(entity);
		auto& transform = m_registry.get<Transform>(entity);
		cube.m_VSCBD.world = transform.GetTransformMatrix();
		cube.m_VSCBD.invWorld = cube.m_VSCBD.world.Invert();
		m_pGraphicsEngine->UpdateConstantBuffer(cube.m_pModel.get(), cube.m_VSCBD);
		PSConstantBufferData lightData;
		m_pGraphicsEngine->UpdateConstantBuffer(cube.m_pModel.get(), lightData);
	}
}

void RenderManager::UpdatePSCB()
{
	auto view = m_registry.view<MeshRenderer, Transform>();
	for (auto& entity : view)
	{
		auto& transform = m_registry.get<Transform>(entity);
		auto& mesh = m_registry.get<MeshRenderer>(entity);
		ModelBuffer* modelBuffer = mesh.m_pModel.get();

		//������ �������Ǵ� ��ü���� ������ �־���ϳ� ����� ���ǻ� �ϳ��� ��ģ��.
		PSConstantBufferData lightData;
		if (mesh.mIs_PSconstant)
		{
			m_pGraphicsEngine->UpdateConstantBuffer(modelBuffer, lightData);
		}
	}
}

void RenderManager::UpdateTexture()
{
	// 	// ����� 3D �ؽ��� ������Ʈ
	// 	m_pTexture3DObserver->each([this](entt::entity entity)
	// 		{
	// 			if (m_registry.any_of<MeshRenderer, Texture3D>(entity))
	// 			{
	// 				auto& name = m_registry.get<Name>(entity).m_name;
	// 				auto mesh = m_registry.try_get<MeshRenderer>(entity);
	// 				auto texture = m_registry.try_get<Texture3D>(entity);
	// 				if (mesh && texture)
	// 				{
	// 					if (!texture->m_diffuse.empty())
	// 					{
	// 						mesh->m_pModel->m_diffusetexture = m_pGraphicsEngine->Get_Textures(texture->m_diffuse);
	// 					}
	// 					if (!texture->m_normal.empty())
	// 					{
	// 						mesh->m_pModel->m_normaltexture = m_pGraphicsEngine->Get_Textures(texture->m_normal);
	// 					}
	// 					if (!texture->m_specular.empty())
	// 					{
	// 						mesh->m_pModel->m_cubeMapSpecularResourceView = m_pGraphicsEngine->Get_Textures(texture->m_specular);
	// 					}
	// 				}
	// 			}
	// 		});
	// 	// ������ �ʱ�ȭ
	// 	m_pTexture3DObserver->clear();
}
