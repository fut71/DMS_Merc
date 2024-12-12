#pragma once
#include<wrl.h>
#include<d3d11.h>
#include <unordered_map>
#include<string>

#include "BufferData.h"
#include "ObjectPool.h"

using Microsoft::WRL::ComPtr;

class ParticleManager
{
public:
	ParticleManager() = delete;
	ParticleManager(ComPtr<ID3D11Device> _pDevice, ComPtr<ID3D11DeviceContext> _pDeviceContext, unsigned int _scale);
private:
	ComPtr<ID3D11Device> m_pDevice;											//����̽�
	ComPtr<ID3D11DeviceContext> m_pDeviceContext;							//����̽� ���ؽ�Ʈ
	//ComPtr<ID3D11DeviceContext1> m_pDeviceContext;						//����̽� ���ؽ�Ʈ
public:
	ObjectPool<CSParticleData>* m_pParticleObjectpool;

	ComPtr<ID3D11Buffer> m_pParticleStructedBuffer;				//��ƼŬ�� ����� structedBuffer
	ComPtr<ID3D11UnorderedAccessView> m_pParticleUAV;			//��ƼŬ�� ����� UnorderedAccessView
	ComPtr<ID3D11ShaderResourceView> m_pParticleSRV;			//��ƼŬ�� ����� UnorderedAccessView

	ComPtr<ID3D11Buffer> m_pParticleIndexBuffer;				//��ƼŬ�� ����� IndexStructedBuffer
	ComPtr<ID3D11UnorderedAccessView> m_pParticleIndexUAV;		//��ƼŬ�� ����� UnorderedAccessView
	ComPtr<ID3D11ShaderResourceView> m_pParticleIndexSRV;		//��ƼŬ�� ����� UnorderedAccessView

	
	ComPtr<ID3D11Buffer> m_pParticleVertexinfoBuffer;
	ComPtr<ID3D11UnorderedAccessView> m_pParticleVertexInfoUAV;
	ComPtr<ID3D11ShaderResourceView> m_pParticleVertexInfoSRV;

	//������¡ ���۵�
	ComPtr<ID3D11Buffer> m_pParticleInfoStagingBuffer;
	ComPtr<ID3D11Buffer> m_pIndexStagingBuffer;

	ComPtr<ID3D11Buffer> mVertexBuffer;
	ComPtr<ID3D11Buffer> mIndexBuffer;
	unsigned int mNumIndices = 0;

public:
	void Initialize();
	void CreateParticleStructedBufferandUAVandSRV();
	void CreateStagingBuffer();
	void Update();
	void Render();
	void ComputeShaderBarrier();
	void SetVertexBufferAndIndexBuffer(ComPtr<ID3D11Buffer> _vertexBuffer, ComPtr<ID3D11Buffer> _indexBuffer, unsigned int _numIndices);

public:
	void AddParticle(unsigned int _particleNum, CSParticleData& _particleData);
};

