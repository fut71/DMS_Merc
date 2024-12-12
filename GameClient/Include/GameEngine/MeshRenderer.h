#pragma once
#include "Component.h"
#include "GraphicsDefine.h"

/// <summary>
/// ������Ʈ�� ���̳� �ؽ��ĸ� ����ϰ� �������Ҽ� �ִ� ������Ʈ�Դϴ�.
/// <para>
/// MeshRenderer("���ϸ�", "�޽� �̸�", �� ����, �ִϸ��̼� ����)
/// </para>
/// </summary>
struct MeshRenderer
	: public Component
{
public:
	/// <summary>
	/// MeshRenderer �������Դϴ�.
	/// </summary>
	/// <param name="_file">�� ���ϸ�</param>
	/// <param name="_meshName">�޽� �̸�</param>
	/// <param name="_hasBone">�� ����</param>
	/// <param name="_hasAnimation">�ִϸ��̼� ����</param>
	MeshRenderer(std::shared_ptr<Entity> _owner, const std::string& _file = "", const std::string& _meshName = ""
		, bool _hasBone = false, bool _hasAnimation = false, bool _flag = false)
		: Component(_owner), m_file(_file), m_meshName(_meshName)
		, m_hasBone(_hasBone), m_flag(_flag)
		, m_pModel(nullptr), mIs_VSconstant(false), mIs_VSBoneConstant(false), mIs_VSTargetBoneconstant(false), mIs_PSconstant(false), mIs_CubeMap(false)
		, mVSConstantBufferData()
	{
		m_pModel = std::make_shared<ModelBuffer>();
	}

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<MeshRenderer>(*this);
	}

	~MeshRenderer()
	{
// 		if (m_pModel != nullptr)
// 		{
// 			delete m_pModel;
// 		}
	}

public:
	std::string m_file;
	std::string m_meshName;
	std::shared_ptr<ModelBuffer> m_pModel;
	bool m_hasBone;
	bool m_flag;	// �ӽ� �ο� ����, ���� �÷��� �߿� ������Ʈ �������� �ذ��ϱ� ���� �ӽ÷� ����

	bool mIs_VSconstant = true;						 // VSConstantBuffer�� �ִ°�?
	bool mIs_VSBoneConstant;					 // VSBoneConstanatBuffer�� �ִ°�?
	bool mIs_VSTargetBoneconstant;				 // VsTargetBoneConstantBuffer�� �ִ°�?
	bool mIs_PSconstant;						 // PsConstnatBuffer�� �ִ°�?
	bool mIs_CubeMap;							 // �� �޽��� ť������� �����ΰ�?
	bool mIsWeapon = false;
	//---------------------------------ConstantData---------------------------------------���߿� shared_ptr�� �ٱŵ� �ɵ� �ϴ�. 
	/// <summary>
	/// model, view, projection, invTranspose
	/// </summary>
	VSConstantBufferData mVSConstantBufferData;	// �갡 �ٸ� ��������� �޸� �������׾���
	//#sung : ������ �̰��� PSConstantBufferData�� �־���Ѵ�.
};