#pragma once
#include "Component.h"
#include "GraphicsDefine.h"

/// <summary>
/// ĳ���Ͱ� �����ϴ� ��� �ϳ��� �ش��ϴ� ��ü
/// </summary>
struct Equipment
{
	std::string m_modelFileName;	// �� ���� �̸�
	std::string m_meshName;			// �޽� �̸�
	std::string m_diffuseTexture;	// ��ǻ�� �ؽ��� ���ϸ�
	std::string m_normalTexture;	// �븻 �ؽ��� ���ϸ�
	std::string m_targetModelName;	// ������ų ���� �̸�
	std::string m_boneName;			// ���� �̸�
	std::shared_ptr<ModelBuffer> m_pModelBuffer;	// �� ����
	int m_targetBoneIndex;			// �� �ε���
	VSTargetBoneConstantBufferData m_targetBoneConstantBufferData;	// �� ��Ʈ����
};

/// <summary>
/// ������ ���͸� �������ִ� ������Ʈ
/// ��� �������� �Ҷ� ����
/// �浹ü�� �ʿ��� ���� ������ ��ƼƼ�� ������Ʈ�� �̿��� ����
/// </summary>
class EquipmentComponent :
	public Component
{
public:
	EquipmentComponent(std::shared_ptr<Entity> _owner)
		: Component(_owner)
	{}

	~EquipmentComponent();

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<EquipmentComponent>(*this);
	}

	void AddEquipment(const std::string& _fileName = "", const std::string& _meshName = ""
		, const std::string& _targetName = "", const std::string& _boneName = ""
		, const std::string& _diffuse = "", const std::string& _normal = "");

public:
	std::vector<std::shared_ptr<Equipment>> m_pEquipments;	// ������ ����

};

// ĳ������ meshrenderer������Ʈ�� ��ü�� ������ ���� ��
// ����, ���� ������� ������ �ִ� ��ü
// VSConstantBufferData 

// ĳ������ bonegroup������Ʈ�� ��ü�� ������ ���� ��
// �� ��Ʈ�������� ���
// VSBoneConstantBufferData* 
