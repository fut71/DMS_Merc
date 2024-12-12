#pragma once
#include<wrl.h>
#include<d3d11.h>
#include "ModelInfo.h"
#include "BufferData.h"
using Microsoft::WRL::ComPtr;

// enum class AnimationPlayType
// {
// 	Loop,
// 	Once,
// };

struct ModelBuffer // 1���� �𵨿� ���� ���� ���� ��ü�� ���� ��츦 ����� ����� ����
{
	ComPtr<ID3D11Buffer> m_pVertexBuffer;					// ���ؽ� ����
	ComPtr<ID3D11Buffer> m_pIndexBuffer;					// �ε��� ����

	ComPtr<ID3D11Buffer> m_pVSConstantBuffer;				// ���ؽ� ������Ʈ ����
	ComPtr<ID3D11Buffer> m_pPSConstantBuffer;				// �ȼ� ������Ʈ ����
	ComPtr<ID3D11Buffer> m_BoneConstantBuffer;				// �� ������Ʈ ����	   
	ComPtr<ID3D11Buffer> m_TargetBoneConstantBuffer;		// Ÿ�� �� ������Ʈ ����	   

	ComPtr<ID3D11Buffer> m_InstanceConstantBuffer;			//�ν��Ͻ��� �Ҷ� �ʿ��� �ν��Ͻ� ������Ʈ ����	   
	unsigned int mNumInstances;								//�ν��Ͻ� ����

	ComPtr<ID3D11Buffer> m_pVSEdgeConstantBuffer;			// �ܰ��� ������Ʈ ����
	ComPtr<ID3D11Buffer> m_pPSEdgeConstantBuffer;			// �ܰ��� ������Ʈ ����

	ComPtr<ID3D11Buffer> m_pVSWaterConstantBuffer;			//�� ������Ʈ ���� -> �Ŀ� �� ���̵����� ������Ų��.

	ComPtr<ID3D11ShaderResourceView> m_diffusetexture;		// �ؽ�ó�� ���̴� ���ҽ� �� ����ü
	ComPtr<ID3D11ShaderResourceView> m_normaltexture;
	ComPtr<ID3D11ShaderResourceView> m_cubeMapDiffuseResourceView;
	ComPtr<ID3D11ShaderResourceView> m_cubeMapSpecularResourceView;
	
	UINT offset = 0;										// �̰Ŵ� Mesh�� �ִ� �� �� ��︮�� �� ������..

	unsigned int mNumIndices = 0;
												
	Model* mpTargetModel;

	Animation* mpTargetAnimation = nullptr;						   //� �ִϸ��̼��� �����ų ������
	Animation* mpNextTargetAnimation = nullptr;					   //� �ִϸ��̼��� �������� �����ų ������

	double mAnimationPlaytime = 0;
	double mNextAnimationPlaytime = 0;

// 	AnimationPlayType mAnimationPlayType = AnimationPlayType::Loop;
// 	AnimationPlayType mNextAnimationPlayType = AnimationPlayType::Loop;

	void operator=(ModelBuffer& _modelBuffer)
	{
		m_pVertexBuffer = _modelBuffer.m_pVertexBuffer;
		m_pIndexBuffer = _modelBuffer.m_pIndexBuffer;
		m_pVSConstantBuffer = _modelBuffer.m_pVSConstantBuffer;
		m_pPSConstantBuffer = _modelBuffer.m_pPSConstantBuffer;
		m_BoneConstantBuffer = _modelBuffer.m_BoneConstantBuffer;
		m_diffusetexture = _modelBuffer.m_diffusetexture;
		m_normaltexture = _modelBuffer.m_normaltexture;
		offset = _modelBuffer.offset;
	}
};


