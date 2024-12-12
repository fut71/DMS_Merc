#pragma once
#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dxgi.h>                       // DXGIFactory
#include <dxgi1_4.h>                    // DXGIFactory4
#include <string>
#include <directxtk/DDSTextureLoader.h> // ť��� ���� �� �ʿ�
#include <vector>
using Microsoft::WRL::ComPtr;
#include <iostream>

#ifdef _DEBUG
#define DEBUG_LOG(message) std::cout<<message<<std::endl
#else
#define DEBUG_LOG(message)
#endif // DEBUG


namespace Dears
{
	inline void ThrowIfFailed(HRESULT hr)
	{
		if (FAILED(hr)) {
			// ������� �� ���⿡ �ߴ��� ������ ����
			throw std::exception();
		}
	}
}

/// �������� ����, ������ �����ִ� Ŭ����
// 2024.5.20. Riun �������� : �����͸� �Ű������� �׳� �����ͼ�, ������ �ƴ� ���纻�� �ʱ�ȭ�Ǿ���...
// �Ű������� ������ ���������� �ٲپ���.
class RendererHelper
{
public:
	RendererHelper();
	~RendererHelper();

public:
	///����̽��� �����Ѵ�.
	static bool CreateDevice(ComPtr<ID3D11Device>& _pDevice, ComPtr<ID3D11DeviceContext>& _pDeviceContext);

	///IDXGIFactory�� �̿��� CreateSwapChain.
	static bool CreateSwapChain(ComPtr<ID3D11Device>& _pDevice,
		HWND _hWnd, UINT& _numQualityLevels, int _screenWidth, int _screenHeight, ComPtr<IDXGISwapChain>& _pSwapChain);

	///Device���� RenderTargetView�� �����Ѵ�. 
	static bool CreateRenderTargetView(ComPtr<ID3D11Device>& _pDevice,
		ComPtr<IDXGISwapChain>& _pSwapChain,
		ComPtr<ID3D11RenderTargetView>& _pRenderTargetView,
		UINT _index = 0,
		UINT _screenWidth = 0,
		UINT _screenHeight = 0
	);

	///DeviceContext���� ViewPort�� �����Ѵ�.
	static void SetViewPort(ComPtr<ID3D11DeviceContext>& _pDeviceContext,
		int _startScreenWidth, int _startScreenHeight, int _endScreenWidth, int _endScreenHeight,
		D3D11_VIEWPORT& _screenViewPort);

	///�׸��� ������ �ʿ��� ����Ʈ�� �����Ѵ�.
	static void SetShadowViewport(ComPtr<ID3D11DeviceContext>& _pDeviceContext, float m_shadowWidth, float m_shadowHeight,
		D3D11_VIEWPORT& _screenViewPort);

	///Device���� RasterizerState�� �����Ѵ�. 
	static ComPtr<ID3D11RasterizerState> CreateSolidRasterizerState(ComPtr<ID3D11Device>& _pDevice);
	static ComPtr<ID3D11RasterizerState> CreateWireRasterizerState(ComPtr<ID3D11Device>& _pDevice);

	/// Create Depth Buffer state (view)
	static bool CreateDepthStencilBuffer(ComPtr<ID3D11Device>& _pDevice,
		UINT& _numQualityLevels,
		int _screenWidth, int _screenHeight,
		ComPtr<ID3D11DepthStencilView>& _pdepthStencilView);

	/// Create Depth Buffer state (view)
	static bool CreateDepthOnlyBuffer(ComPtr<ID3D11Device>& _pDevice,
		int _screenWidth, int _screenHeight,
		ComPtr<ID3D11DepthStencilView>& _pdepthStencilView,
		ComPtr<ID3D11ShaderResourceView>& _pdepthOnlySRV);


	//VertexBuffer�� �����. 
	template<typename T_VERTEX>
	static ComPtr<ID3D11Buffer> CreateVertexBuffer(ComPtr<ID3D11Device>& _pDevice, unsigned int _mNumVertices, T_VERTEX* _vertices);

	//IndexBuffer�� �����. 
	static ComPtr<ID3D11Buffer> CreateIndexBuffer(ComPtr<ID3D11Device>& _pDevice, const unsigned int _numIndices, unsigned int* _indices);

	//ConstantBuffer�� �����.
	template<typename T_Constant>
	static ComPtr<ID3D11Buffer> CreateConstantBuffer(ComPtr<ID3D11Device>& _pDevice, const T_Constant& _constantBufferStruct);

	//StructedBuffer�� �����.
	template<typename T_Struct>
	static ComPtr<ID3D11Buffer> CreateStructuredBuffer(ComPtr<ID3D11Device>& _pDevice, const T_Struct& _constantBufferStruct, unsigned int _count);

	//UAV�� �����
	static ComPtr<ID3D11UnorderedAccessView> CreateUnorderedAccessView(ComPtr<ID3D11Device>& _pDevice, ComPtr<ID3D11Buffer> _structuredBuffer, unsigned int _count);

	//SRV�� �����
	static ComPtr<ID3D11ShaderResourceView> CreateShaderResourceView(ComPtr<ID3D11Device>& _pDevice, ComPtr<ID3D11Buffer> _structuredBuffer, unsigned int _count);
	
	//������¡ ���۸� �����Ѵ�.
	template<typename S_Struct>
	static void CopyFromStagingBuffer(ComPtr<ID3D11DeviceContext>& context, ComPtr<ID3D11Buffer>& buffer, UINT _size, S_Struct* destination);

	//������¡ ������ ������ structuredbuffer -> GPU�� �����Ѵ�.
	template<typename S_Struct>
	static void UpdateDataOnGPU(ComPtr<ID3D11DeviceContext>& context,
		ComPtr<ID3D11Buffer>& structuredBuffer,  // UAV�� ���ε��� GPU ����
		ComPtr<ID3D11Buffer>& stagingBuffer,      // CPU ���ٿ� ������¡ ����
		UINT _size,
		S_Struct* destination);



	//������¡ ���۸� �����.
	template<typename S_Struct>
	static ComPtr<ID3D11Buffer>CreateStagingBuffer(ComPtr<ID3D11Device>& _pDevice, const unsigned int numElements, const unsigned int sizeElement, S_Struct* _stagingStruct);

	//Texture�� �����. ���� �׷��Ƚ� �����̳ʷ� �������Ѵ�. -> �̰� ���߿� �ٸ������� ���°� �´°� ����.
	static ComPtr<ID3D11ShaderResourceView> Create3DTexture(ComPtr<ID3D11Device>& _pDevice, std::string _filename);
	static ComPtr<ID3D11ShaderResourceView> Create2DTexture(ComPtr<ID3D11Device>& _pDevice, std::string _filename);
	static ComPtr<ID3D11ShaderResourceView> CreateDDSTexture(ComPtr<ID3D11Device>& _pDevice, std::string _filename);

	//���۸� ������Ʈ�Ѵ�. ��κ� ConstantBuffer�� ������Ʈ��.
	template<typename T_Constant>
	static void UpdateBuffer(ComPtr<ID3D11DeviceContext>& _pDeviceContext, const T_Constant& _ConstantBufferStruct, ComPtr<ID3D11Buffer>& _buffer);

	//�׸��� �ʰ�, ���ҽ��並 �����.
	static bool CreateShadowMapAndShaderResourceView(ComPtr<ID3D11Device>& _pDevice, int _screenWidth, int _screenHeight,
		ComPtr<ID3D11DepthStencilView>& _pShadowMap, ComPtr<ID3D11ShaderResourceView>& _pShadowResourceView);

	// Create depth stencil state
	static ComPtr<ID3D11DepthStencilState> CreateDepthStencilState(ComPtr<ID3D11Device>& _pDevice);

	//���÷�������Ʈ�� �����.
	static ComPtr<ID3D11SamplerState> CreateSamplerState(ComPtr<ID3D11Device>& _pDevice);

	//���ؽ� ���̴��� inputlayout�� �����
	static void CreateVertexShaderAndInputLayout(
		ComPtr<ID3D11Device>& device, const std::string& filename,
		const std::vector<D3D11_INPUT_ELEMENT_DESC>& inputElements,
		ComPtr<ID3D11VertexShader>& m_vertexShader,
		ComPtr<ID3D11InputLayout>& m_inputLayout);

	//�ȼ����̴��� �����.
	static void CreatePixelShader(ComPtr<ID3D11Device>& device, const std::string& filename, ComPtr<ID3D11PixelShader>& _pixelShader);

	static void CreateComputeShader(ComPtr<ID3D11Device>& device, const std::string& filename, ComPtr<ID3D11ComputeShader>& _computeShader);

	//string -> wstring ��ȯ
	static std::wstring StringToWstring(const std::string& str);

	//������ üũ�Ѵ�.
	static void CheckResult(HRESULT hr, ID3DBlob* errorBlob);
};

template<typename T_VERTEX>
static ComPtr<ID3D11Buffer>
RendererHelper::CreateVertexBuffer(ComPtr<ID3D11Device>& _pDevice, unsigned int _mNumVertices, T_VERTEX* _vertices)
{
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE; // �ʱ�ȭ �� ����X
	bufferDesc.ByteWidth = UINT(sizeof(T_VERTEX) * _mNumVertices);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0; // 0 if no CPU access is necessary.
	bufferDesc.StructureByteStride = sizeof(T_VERTEX);

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 }; // MS �������� �ʱ�ȭ�ϴ� ���
	vertexBufferData.pSysMem = _vertices;
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;

	ComPtr<ID3D11Buffer> vertexBuffer;

	HRESULT hr =
		_pDevice->CreateBuffer(&bufferDesc, &vertexBufferData, vertexBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		std::cout << "CreateVertexBuffer() failed. " << std::hex << hr << std::endl;
		return nullptr;
	};
	return vertexBuffer;
}

template<typename T_Constant>
ComPtr<ID3D11Buffer>
RendererHelper::CreateConstantBuffer(ComPtr<ID3D11Device>& _pDevice, const T_Constant& _constantBufferStruct)
{
	// ���ؽ� ������Ʈ ���� ����
	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = sizeof(_constantBufferStruct);
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	// Fill in the subresource data.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &_constantBufferStruct;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	ComPtr<ID3D11Buffer> pConstantBuffer;

	HRESULT hr = _pDevice->CreateBuffer(&cbDesc, &InitData, pConstantBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		std::cout << "CreateConstantBuffer() failed. " << std::hex << hr << std::endl;
		return nullptr; // ���� ó��
	}
	return pConstantBuffer;
}

//StructuredBuffer
//D3D���� ����ȭ�� �����͸� GPU�� �����ϱ� ���� ����, ����ü ���·� ������ �������� �迭�� GPU�� �����Ͽ� ���̴����� ���� �׸��� ������ �� �ֵ��� ����

//UAV = Unordered Access View
//D3D���� GPU�� ���޸𸮿� ���� ���� �����Ӱ� �����Ͽ� �а� �� �� �ֵ��� �ϴ� �� /  
//�ַ� ��ǻƮ ���̴����� ���Ǹ�, �����͸� �а� ���� ������ ������ ���� �ʾ� ���ķ� ���� �����忡�� ���ÿ� �����Ϳ� �����ϰ� ó���Ҽ� �ִ�. 

template<typename T_Struct>
ComPtr<ID3D11Buffer> RendererHelper::CreateStructuredBuffer(ComPtr<ID3D11Device>& _pDevice, const T_Struct& _constantBufferStruct, unsigned int _count)
{
	// ��ƼŬ ���� ����
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;												// GPU�� �ַ� ����ϰ�, CPU���� ���÷� ������Ʈ���� ����
	bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS |								//���۸� UAV�� ����� �� �ֵ���	//Compute Shader
		D3D11_BIND_SHADER_RESOURCE;														// ���۸� SRV�� ����� �� �ֵ���	//Vertex Shader
	bufferDesc.ByteWidth = sizeof(_constantBufferStruct) * _count;						//������ ���� ���� ũ�� ����
	bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;						//Structured buffer�� �� ���̴�.
	bufferDesc.StructureByteStride = sizeof(_constantBufferStruct);						// �� ����ü�� ũ�� ����, �ѹ� �Ի��Ҷ� �󸶸�ŭ �д°�

	///����� sturectured buffer�� D3D11_BIND_VERTEX_BUFFER�� ��� �Ұ�.

	ComPtr<ID3D11Buffer> structuredBuffer;
	HRESULT hr = _pDevice->CreateBuffer(&bufferDesc, nullptr, structuredBuffer.GetAddressOf());
	if (FAILED(hr))
	{
		std::cout << "CreateStructuredBuffer() failed. " << std::hex << hr << std::endl;
		return nullptr; // ���� ó��
	}
	return structuredBuffer;

}

/// <summary>
/// 
/// </summary>
/// <typeparam name="S_Struct"></typeparam>
/// <param name="_pDevice"></param>
/// <param name="_stagingdBuffer"></param>
/// <param name="numElements">����� ����</param>
/// <param name="sizeElement">����� ������ / sizeof(element)</param>
/// <param name="_destination">������</param>
/// <returns></returns>
template<typename S_Struct>
ComPtr<ID3D11Buffer> RendererHelper::CreateStagingBuffer(ComPtr<ID3D11Device>& _pDevice,
	const unsigned int numElements, const unsigned int sizeElement, S_Struct* _destination)
{
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.ByteWidth = numElements * sizeElement;
	desc.Usage = D3D11_USAGE_STAGING;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	desc.StructureByteStride = sizeElement;

	ComPtr<ID3D11Buffer> stagingBuffer;
	//�ʱ� ���� ���� ���
	if (_destination)
	{
		D3D11_SUBRESOURCE_DATA bufferData;
		ZeroMemory(&bufferData, sizeof(bufferData));
		bufferData.pSysMem = _destination;

		HRESULT hr = _pDevice->CreateBuffer(&desc, &bufferData, stagingBuffer.GetAddressOf());
		if (FAILED(hr))
		{
			std::cout << "CreateStagingBuffer() failed. " << std::hex << hr << std::endl;
			return nullptr;
		}
	}
	//�ʱⰪ�� ���� ��� -> ���� GPU�� CPU���� �����͸� ���� �����ϰų� �����ؾ��Ѵ�. 
	else 
	{
		HRESULT hr = _pDevice->CreateBuffer(&desc, NULL, stagingBuffer.GetAddressOf());
	
		if (FAILED(hr))
		{
			std::cout << "CreateStagingBuffer() failed. " << std::hex << hr << std::endl;
			return nullptr;
		}
	}
	return stagingBuffer;

}

template<typename T_Constant>
void RendererHelper::UpdateBuffer(ComPtr<ID3D11DeviceContext>& _pDeviceContext, const T_Constant& _ConstantBufferStruct, ComPtr<ID3D11Buffer>& _buffer)
{
	D3D11_MAPPED_SUBRESOURCE ms;
	_pDeviceContext->Map(_buffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, &_ConstantBufferStruct, sizeof(_ConstantBufferStruct));
	_pDeviceContext->Unmap(_buffer.Get(), NULL);
}

/// <summary>
/// 
/// </summary>
/// <typeparam name="S_Struct"></typeparam>
/// <param name="context"></param>
/// <param name="buffer">����� �����͹���</param>
/// <param name="size">������ �������� ũ��</param>
/// <param name="destination">������ �������� �ּҰ�</param>
template<typename S_Struct>
void RendererHelper::CopyFromStagingBuffer(ComPtr<ID3D11DeviceContext>& context, ComPtr<ID3D11Buffer>& buffer, UINT _size, S_Struct* destination)
{
	D3D11_MAPPED_SUBRESOURCE ms;
	context->Map(buffer.Get(), NULL, D3D11_MAP_READ, NULL, &ms);
	memcpy(destination, ms.pData, _size);
	context->Unmap(buffer.Get(), NULL);
}

template<typename S_Struct>
void RendererHelper::UpdateDataOnGPU(ComPtr<ID3D11DeviceContext>& context, 
									ComPtr<ID3D11Buffer>& structuredBuffer,		// UAV�� ���ε��� GPU ����
									ComPtr<ID3D11Buffer>& stagingBuffer,		// CPU ���ٿ� ������¡ ����
									UINT _size, S_Struct* destination)
{
	// ������¡ ���ۿ� ������ ����
	D3D11_MAPPED_SUBRESOURCE ms;
	HRESULT hr = context->Map(stagingBuffer.Get(), 0, D3D11_MAP_WRITE, 0, &ms);
	if (FAILED(hr)) {
		throw std::runtime_error("Failed to map staging buffer for write.");
	}

	// CPU ������ ����
	memcpy(ms.pData, destination,  _size);
	context->Unmap(stagingBuffer.Get(), 0);

	// UAV ����ȭ ���۷� ������ ����
	context->CopyResource(structuredBuffer.Get(), stagingBuffer.Get());

}
