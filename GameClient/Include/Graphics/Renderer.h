#pragma once
#include<windows.h>
#include<wrl.h>
#include<d3d11.h>
#include <directxtk/SimpleMath.h>
#include "ModelBuffer.h"
#include "GraphicsResourceManager.h"
#include "RendererHelper.h"
#include "Camera.h"
#include "PipelineStateObject.h "
#include "GraphicsCommon.h"

using Microsoft::WRL::ComPtr;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector4;
using DirectX::SimpleMath::Quaternion;

///�������� �׷��Ƚ� ������ ������ ���������� ����ϴ� Ŭ����.
//Graphics �������� �����Ѵ�.
class Renderer
{
public:
	//������ ȣ�� �� �������ڵ�� �����ȼ�, ���ȼ��� �޴´�.
	Renderer(HWND _hWnd, int _startScreenWidth, int _startScreenHeight, int _endScreenWidth, int _endScreenHeight,
			ComPtr<ID3D11Device>& _pDevice, ComPtr<ID3D11DeviceContext> _pDeviceContext);
	~Renderer();
private:
	HWND m_hWnd;				//�ڵ� ������
	int m_startScreenWidth;		//ȭ���� ����(���� �ȼ�)
	int m_startScreenHeight;	//ȭ���� ����(���� �ȼ�)
	int m_endScreenWidth;		//ȭ���� ��(���� �ȼ�)
	int m_endScreenHeight;		//ȭ���� ��(���� �ȼ�)

	int m_ScreenWidth;
	int m_ScreenHeight;

	///D3D���� �� �⺻���� ������
public:
	ComPtr<ID3D11Device> m_pDevice;										//����̽�
	ComPtr<ID3D11DeviceContext> m_pDeviceContext;						//����̽� ���ؽ�Ʈ

	ComPtr<IDXGISwapChain> mpSwapChain;									//����ü��

	ComPtr<ID3D11RenderTargetView> m_pRenderTargetView;					//������ Ÿ�ٺ�  //���۵� ������ �� 2�� �̻� �ʿ�
	ComPtr<ID3D11RenderTargetView> m_pTempRednerTargetview;				// ����� ����Ÿ��
	
	D3D11_VIEWPORT m_pD3dScreenViewport;								// �� ��Ʈ ������ �����ϴ� ����ü
	D3D11_VIEWPORT m_pD3dtempViewport;									// ����� �� ��Ʈ ����
	
	ComPtr<ID3D11DepthStencilView> mpDepthStencilView;					// ���� ���� ���ٽ� ��
	
	UINT m_numQualityLevels;

	ComPtr<ID3D11ShaderResourceView> mpCubeMapDiffuseResourceView;
	ComPtr<ID3D11ShaderResourceView> mpCubeMapSpecularResourceView;

	
public:
	GraphicsResourceManager* m_pResourceManager;
	RendererHelper* m_pRenderHelper;
	Camera* mpTargetCamera;

	// Depth buffer ����
	//ComPtr<ID3D11RenderTargetView> m_resolvedRTV;					//�����ʿ��� ����ϴ� ô�ϴ� ������ Ÿ�ٺ�
	//ComPtr<ID3D11Texture2D> m_depthOnlyBuffer;  // No MSAA
	ComPtr<ID3D11DepthStencilView> m_depthOnlyDSV; // No MSAA
	//ComPtr<ID3D11DepthStencilView> m_depthStencilView; // use MSAA
	ComPtr<ID3D11ShaderResourceView> m_depthOnlySRV;

	ComPtr<ID3D11Buffer> mpCommonConstantBuffer;


public:
	void SetCamera(Camera* _pTargetCamera);
public:
	bool Initialize(GraphicsResourceManager* _pResourceManager);
	void BeginRender();

	void SetCommonShaderResource(ComPtr<ID3D11ShaderResourceView> _diffuseTexture,
									ComPtr<ID3D11ShaderResourceView> _specularTexture);

	//���� ���̴� ���ҽ��� GPU�� Set���ش�
	bool SetCommonShaderResourceToGPU();


	//���� ������Ʈ�� Set���ش�.(Render ȣ������ �ʼ�)common
	void SetCommonConstant(ComPtr<ID3D11Buffer>& commonConstsBuffer);
	void Render(ModelBuffer* _modelbuffer);
	void Render_CubeMap(ModelBuffer* _modelbuffer);
	void EndRender();
	void Finalize();
	void SetPipelineState(const PipelineStateObject& _pso);

	void LightUpdate(CommonConstantBufferData* _psBufferData);
	void UpdateCommonConstantBuffer(CommonConstantBufferData& _CommonBufferData);

	void RenderDepthMap(ModelBuffer* _modelbuffer);
	void RenderAniDepthMap(ModelBuffer* _modelbuffer);
	void RenderEquipDepthMap(ModelBuffer* _modelbuffer);

	void RenderEdge(ModelBuffer* _modelbuffer);

	void Rend_InstancedModels(ModelBuffer* _modelBuffers);

private:
	bool InitalizeD3D();

public:


	// ���� �𸣰ڴ� index...?
	unsigned int objectIndex = 0;

	// �ӽ� �޽�

	//m_pDevice�� ������ �ӽ��Լ�
	ComPtr<ID3D11Device> GetDevice() const { return m_pDevice; }
	
};


