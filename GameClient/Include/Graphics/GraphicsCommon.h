#pragma once
#include "RendererHelper.h"
#include "PipelineStateObject.h"
#include <vector>
using std::vector;

///�̰� ���߿� �ܺο��� �� ������ ������ �����ϱ� ���߿� ���Ƶ־��� const�� ���̵�, �Լ��� �����ϵ��� �����ϵ�.. �ؼ�.. 

namespace Dears {

	// ����: DirectX_Graphic-Samples �̴Ͽ���
	// https://github.com/microsoft/DirectX-Graphics-Samples/blob/master/MiniEngine/Core/GraphicsCommon.h

	namespace Graphics {

		// Samplers
		extern ComPtr<ID3D11SamplerState> linearWrapSS;
		extern ComPtr<ID3D11SamplerState> linearClampSS;
		extern ComPtr<ID3D11SamplerState> shadowPointSS;
		extern ComPtr<ID3D11SamplerState> shadowCompareSS;
		extern vector<ID3D11SamplerState*> sampleStates;

		// Rasterizer States
		extern ComPtr<ID3D11RasterizerState> solidRS;
		extern ComPtr<ID3D11RasterizerState> wireRS;

		// Depth Stencil States
		extern ComPtr<ID3D11DepthStencilState> drawDSS; // �Ϲ������� �׸���

		// Shaders
		extern ComPtr<ID3D11VertexShader> basicVS;
		extern ComPtr<ID3D11VertexShader> animeVS;
		extern ComPtr<ID3D11VertexShader> equimentVS;
		extern ComPtr<ID3D11VertexShader> skyboxVS;
		extern ComPtr<ID3D11VertexShader> depthOnlyVS;
		extern ComPtr<ID3D11VertexShader> depthAniOnlyVS;
		extern ComPtr<ID3D11VertexShader> depthEquipOnlyVS;
		extern ComPtr<ID3D11VertexShader> instanceBasicVS;

		extern ComPtr<ID3D11PixelShader> basicPS;
		extern ComPtr<ID3D11PixelShader> skyboxPS;

		//ComputeShader
		extern ComPtr<ID3D11ComputeShader> particleComputeShader;


		// Input Layouts
		extern ComPtr<ID3D11InputLayout> basicIL;
		extern ComPtr<ID3D11InputLayout> animeIL;
		extern ComPtr<ID3D11InputLayout> skyboxIL;
		extern ComPtr<ID3D11InputLayout> depthOnlyIL;
		extern ComPtr<ID3D11InputLayout> depthAniOnlyIL;
		extern ComPtr<ID3D11InputLayout> depthEquipOnlyIL;

		extern ComPtr<ID3D11InputLayout> instanceBasicIL;


		// Graphics Pipeline States
		extern PipelineStateObject BasicGeometryPSO;
		extern PipelineStateObject AnimeGeometryPSO;
		extern PipelineStateObject EquipmentGeometryPSO;
		extern PipelineStateObject DebugGeometryPSO;
		extern PipelineStateObject CubeMapGeometryPSO;
		extern PipelineStateObject depthOnlyPSO;
		extern PipelineStateObject depthAniOnlyPSO;
		extern PipelineStateObject depthEquipOnlyPSO;
		extern PipelineStateObject OpacityPSO;

		extern PipelineStateObject BasicInstancingPSO;

		extern PipelineStateObject PunchingPSO;
		extern PipelineStateObject EdgePSO;
		extern PipelineStateObject WaterPSO;
		
		extern PipelineStateObject TestPSO;
		extern PipelineStateObject ParticlePSO;


		// Blend States
		extern ComPtr<ID3D11BlendState> OpacityBS;

		void InitCommonStates(ComPtr<ID3D11Device>& device);

		// ���������� InitCommonStates()���� ���
		void InitSamplers(ComPtr<ID3D11Device>& device);
		void InitRasterizerStates(ComPtr<ID3D11Device>& device);
		void InitBlendStates(ComPtr<ID3D11Device>& device);
		void InitDepthStencilStates(ComPtr<ID3D11Device>& device);
		void InitPipelineStates(ComPtr<ID3D11Device>& device);
		void InitShaders(ComPtr<ID3D11Device>& device);

	} // namespace Graphics

} // namespace hlab
