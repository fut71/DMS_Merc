#include "NPCSystem.h"
#include "Transform.h"
#include "Texture3D.h"
#include "BoneGroupComponent.h"
#include "EquipmentComponent.h"
#include "NPCComponent.h"

NPCSystem::NPCSystem(EntityManager* _pEntityManager, RenderManager* _pRenderManager, ResourceManager* _pResourceManager
	, SoundManager* _pSoundManager, UIManager* _pUIManager, ProjectileSystem* _pProjectileSystem, EquipmentManager* _pEquipmentManager)
{
	mpEntityManager = _pEntityManager;
	mpRenderManager = _pRenderManager;
	mpResourceManager = _pResourceManager;
	mpSoundManager = _pSoundManager;
	mpUIManager = _pUIManager;
	mpEquipmentManager = _pEquipmentManager;
	mpProjectileSystem = _pProjectileSystem;
}

NPCSystem::~NPCSystem()
{

}

void NPCSystem::Initialize()
{
	mpNPCMeshInfoVec = mpResourceManager->ParseCSV<NPCMeshInfo>("NPCMesh.csv");
}

void NPCSystem::AddNPC(const std::string& _classType)
{
	for (const auto& npcMeshInfo : mpNPCMeshInfoVec)
	{
		if (npcMeshInfo.npcName == _classType)
		{
			auto npc = mpEntityManager->CreateEntity("NPC");
			npc->AddComponent<NPCComponent>(npcMeshInfo.npcName);
			npc->AddComponent<Transform>(Vector3(npcMeshInfo.posX, npcMeshInfo.posY, npcMeshInfo.posZ),
				Vector3(0.f, DirectX::XMConvertToRadians(npcMeshInfo.rotY), 0.f), Vector3(npcMeshInfo.scale));
			auto& meshRenderComp = npc->AddComponent<MeshRenderer>(npcMeshInfo.fbxName, npcMeshInfo.meshName, true, true);
			// ���� ����
			//meshRenderComp.mHasWeapon = npcMeshInfo.hasWeapon;
			auto& animationComp = npc->AddComponent<AnimationComponent>();
			animationComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@Idle.fbx");
			animationComp.mIsLoop = false;
			npc->AddComponent<BoneGroupComponent>();
			npc->AddComponent<Texture3D>(npcMeshInfo.textureName);
			npc->AddComponent<EquipmentComponent>();

			if (npcMeshInfo.hasWeapon == true)
			{
				
				//mpEquipmentManager->AddNPCEquipment(npc, npcMeshInfo.npcName);
				mpEquipmentManager->AddInitUnitEquipment(npc, npcMeshInfo.npcName);
			}
			else
			{
				mpEquipmentManager->AddNPCEquipment(npc, npcMeshInfo.npcName);
			}
		}
	}

}

void NPCSystem::Update(std::shared_ptr<Entity> _entity, uint8_t _numAlly, uint8_t _numEnemy)
{
	auto& npcComp = _entity->GetComponent<NPCComponent>();
	auto& animationComp = _entity->GetComponent<AnimationComponent>();
	animationComp.mIsLoop = false;
	float mAnimationCoolTime = animationComp.mpTargetAnimation->mDuration / animationComp.mpTargetAnimation->mTicksPerSecond;

	if (npcComp.mNpcName == "Chief") // �Ʊ�
	{
		// ������ ������ ���, �� ���� ����� �� �� Idle ���� ��ȯ�Ѵ�.
		if (mpProjectileSystem->mIsThrow == true)
		{
			animationComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@MagicAttack.fbx");

			if (mAnimationCoolTime < animationComp.mAnimationPlaytime)
			{
				mpProjectileSystem->mIsThrow = false;
			}
		}
		else
		{
			animationComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@Idle.fbx");
		}
		// �й����� ��
		//if (_numAlly == 0)
		//{
		//	animationComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@Lose.fbx");
		//	animationComp.mIsLoop = false;
		//}
		//// �¸����� ��
		//else if(_numEnemy == 0)
		//{
		//	animationComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@Victory.fbx");
		//	animationComp.mIsLoop = false;
		//}
	}
	else if (npcComp.mNpcName == "RVC_C") // ����
	{
		// ������ 15�� �̻��̸� �ڽŸ�����
		if (_numEnemy >= 15)
		{
			animationComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@Idle.fbx");
		}
		// ������ 10�� ���ϸ� ������
		else if (_numEnemy < 10)
		{
			animationComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@IdleFrust.fbx");
		}
		else // ������ 10�� �̻��̰� 15�� �̸��̸� �ο�� ��� ����
		{
			animationComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@IdleBattle.fbx");
		}
	}

	if (mAnimationCoolTime < animationComp.mAnimationPlaytime)
	{
		animationComp.mAnimationPlaytime = 0.0f;
	}
}

void NPCSystem::FinalUpdate(std::shared_ptr<Entity> _entity, bool _mIsCleared)
{
	auto& npcComp = _entity->GetComponent<NPCComponent>();
	auto& animationComp = _entity->GetComponent<AnimationComponent>();
	animationComp.mIsLoop = false;
	animationComp.mAnimationPlaytime = 0.0f;
	float mAnimationCoolTime = animationComp.mpTargetAnimation->mDuration / animationComp.mpTargetAnimation->mTicksPerSecond;
	
	if (npcComp.mNpcName == "Chief") // �Ʊ�
	{
		// �й����� ��
		if (_mIsCleared == false)
		{
			animationComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@Lose.fbx");
			animationComp.mIsLoop = false;
		}
		// �¸����� ��
		else
		{
			animationComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@Victory.fbx");
			animationComp.mIsLoop = false;
		}
	}
}

void NPCSystem::TutorialUpdate(std::shared_ptr<Entity> _entity, int _textIdx)
{
	auto& npcComp = _entity->GetComponent<NPCComponent>();
	auto& animationComp = _entity->GetComponent<AnimationComponent>();
	auto& trsComp = _entity->GetComponent<Transform>();
	animationComp.mIsLoop = true;

	if (npcComp.mNpcName == "Chief") // �Ʊ�
	{
		// ������ ������ ���, �� ���� ����� �� �� Idle ���� ��ȯ�Ѵ�.
		switch (_textIdx)
		{
		case 0:
			trsComp.m_localRotation.y = DirectX::XMConvertToRadians(60.f);
			animationComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@Wave Bare Hands.fbx");
		break;
		case 1:
			animationComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@Cast Spell 01.fbx");
			break;
		case 3:
			trsComp.m_localRotation.y = DirectX::XMConvertToRadians(-90.f);
			break;
		case 4:
			animationComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@Shake Head.fbx");
			break;
		case 6:
			animationComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@ShurikenAttack.fbx");
			break;
		case 8:
			animationComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@Nod Head.fbx");
			break;
		case 10:
			trsComp.m_localRotation.y = DirectX::XMConvertToRadians(60.f);
			animationComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@ShurikenAttack.fbx");
			break;
		case 12:
			trsComp.m_localRotation.y = DirectX::XMConvertToRadians(-90.f);
			animationComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@Clapping.fbx");
			break;
		case 19:
			animationComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@Cast Spell 01.fbx");
			break;
		case 23:
			animationComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@Damaged.fbx");
			break;
		case 28:
			trsComp.m_localRotation.y = DirectX::XMConvertToRadians(60.f);
			animationComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@Cast Spell 02.FBX");
			break;
		case 34:
			animationComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@Cast Spell 02.FBX");
			break;
		case 35:
			trsComp.m_localRotation.y = DirectX::XMConvertToRadians(-90.f);
			animationComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@Idle.fbx");
			break;
		default:
			animationComp.mpTargetAnimation = mpResourceManager->Get_Animation("Character@Idle.fbx");
			break;
		} 
	}
}
