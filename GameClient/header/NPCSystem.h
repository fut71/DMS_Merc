#pragma once
#include "EntityManager.h" // �̰� �����ϸ� byte ���� �߻��ϴ� �ߺ��Ǵ��� ������ �� ��
#include "Entity.h"
#include "RenderManager.h"
#include "ResourceManager.h"
#include "SoundManager.h"
#include "UIManager.h"
#include "EquipmentManager.h"
#include "ProjectileSystem.h"
#include "NPCMeshInfo.h"

/// <summary>
/// NPC�� ���õ� Ŭ�����Դϴ�.
/// </summary>
class NPCSystem
{
public:
	NPCSystem(EntityManager* _pEntityManager, RenderManager* _pRenderManager, ResourceManager* _pResourceManager
		, SoundManager* _pSoundManager, UIManager* _pUIManager, ProjectileSystem* _pProjectileSystem, EquipmentManager* _pEquipmentManager);
	~NPCSystem();

public:
	
	void Initialize();

	// ������ �߰��մϴ�. (�̶� ��� �׳� �� �����մϴ�.)
	void AddNPC(const std::string& _classType);

	// �ִϸ��̼��� �� �۵��ؾ� �մϴ�. TODO : �δ��� �Ǵ� 0.5�ʿ� �� �� ������Ʈ�� �սô�.
	void Update(std::shared_ptr<Entity> _entity, uint8_t _numAlly, uint8_t _numEnemy);
	void FinalUpdate(std::shared_ptr<Entity> _entity, bool _mIsCleared);

	void TutorialUpdate(std::shared_ptr<Entity> _entity, int _textIdx);

	// ���� �� �� �־�� �մϴ�.
private:
	//UI �Ŵ����� ������ �մϴ�.
	// ��ƼƼ �Ŵ���, ���� �Ŵ���, ���ҽ� �Ŵ���, ���� �Ŵ��� ������ �մϴ�.
	EntityManager* mpEntityManager;
	RenderManager* mpRenderManager;
	ResourceManager* mpResourceManager;
	SoundManager* mpSoundManager;
	UIManager* mpUIManager;
	EquipmentManager* mpEquipmentManager;
	ProjectileSystem* mpProjectileSystem;

	std::vector<NPCMeshInfo> mpNPCMeshInfoVec;
};

