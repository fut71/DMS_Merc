#pragma once

#include <d3d11.h>
#include "wrl.h"
using Microsoft::WRL::ComPtr;

//PipeLineState : �������� �� Context�� ���� ����
/// 2024.5.20
/// Riun PipelineState��� �ϸ� ���ڰ� PS�� pixel shader�� ��ġ�� ������ Object�� �ٿ���. �Ϲ������� PSO�� �Ҹ���.
/// ���� PipelineState�� �ܼ��� ���� �����̰�, ���� �����ϴ� ��ü�� PipelineStateObject�� �Ϲ������� �Ҹ���.
class PipelineStateObject
{
public:
	ComPtr<ID3D11InputLayout> m_pInputLayout;
	ComPtr<ID3D11VertexShader> m_pVertexShader;
	ComPtr<ID3D11PixelShader> m_pPixelShader;

	ComPtr<ID3D11HullShader> m_pHullShader;
	ComPtr<ID3D11DomainShader> m_pDomainShader;
	ComPtr<ID3D11GeometryShader> m_pGeometryShader;

	ComPtr<ID3D11BlendState> m_pBlendState;

	ComPtr<ID3D11DepthStencilState> m_pDepthStencilState;

	ComPtr<ID3D11RasterizerState> m_pRasterizerState;

	ComPtr<ID3D11SamplerState> m_pSamplerState;	


	float m_blendFactor[4] = { 1.f, 1.f, 1.f, 1.f };		
	UINT m_stencilRef = 0;								   //���Ľ� ������

	D3D11_PRIMITIVE_TOPOLOGY m_primitiveTopology =
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

public:
	void operator=(const PipelineStateObject& _pso);
	void SetBlendFactor(const float blendFactor[4]);
};

// ���� ����(m_blendFactor[4])��
// ���� ���꿡�� �ҽ� �Ǵ� ��� ���� �������� ��
// Ư�� D3D11_BLEND_BLEND_FACTOR, D3D11_BLEND_INV_BLEND_FACTOR ó�� ��������� ���� ���͸� ����ϴ� ���� ��带 ������ �� ���.
// �� ���� �Ϲ������� [R, G, B, A] ���·� ��Ÿ����, ������ ä�ο� ������ ������ �����ϰ� �ȴ�.
// ex) m_blendFactor[4] = [0.5, 0.5, 0.5, 0.5]�� ��� �ҽ� �Ǵ� ��� ������ �� ä�� (R, G, B, A)�� 0.5�� ���� ��������� ������ �������� �񼮵Ǵ� ȿ���� �� �� �ִ�. 
// 
// 
//
