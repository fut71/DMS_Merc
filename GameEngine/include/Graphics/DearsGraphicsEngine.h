#pragma once
#include <windows.h>
#include <wrl.h>
#include <directxtk/SimpleMath.h>
#include <map>
#include <string>

#include "Renderer.h"
#include "GraphicsResourceManager.h"
#include "DearsImGui.h"
#include "AnimationHelper.h"
#include "LightHelper.h"
#include "ModelBuffer.h"
#include "ParticleManager.h"
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector4;
using DirectX::SimpleMath::Quaternion;

///���� �ֻ������� �׷��Ƚ��� ������ ����ϴ� Ŭ���� 
class DearsGraphicsEngine
{
public:
	DearsGraphicsEngine(HWND _hWnd, int screenWidth, int screenHeight);
	~DearsGraphicsEngine();

	/// Window���� �Լ�
private:
	HWND m_hWnd;
	int m_screenWidth;
	int m_screenHeight;

	//����̽��� ����̽� ���ؽ�Ʈ�� �߾ӿ��� �����ϴ°� ����.
	ComPtr<ID3D11Device> m_pDevice;										//����̽�
	ComPtr<ID3D11DeviceContext> m_pDeviceContext;						//����̽� ���ؽ�Ʈ

public:
	///�������� ������ �ִ� �ڷᱸ��. 
	Renderer* mpRenderer;

	///UI ������ ���� 2D Ŭ����
	DearsImGui* m_pDearsImGui;

	///�׷��Ƚ� ���ҽ� �Ŵ���
	GraphicsResourceManager* m_pResourceManager;

	AnimationHelper* mpAnimationHelper;
	LightHelper* mpLightHelper;

	///ī�޶�, ���� ���ӿ����� �׷��Ƚ� ���� ��� �ƴ� ���̺귯���� ���� ������ �������� �д�.
	Camera* m_pTargetCamera;

	///��ƼŬ �Ŵ���
	ParticleManager* m_pParticleManager;
public:
	//DX�� �ʱ�ȭ�Ѵ�.
	void Initialize();

	//������Ʈ.
	void Update();
	//�������� �غ��Ѵ�.
	void BeginRender();
	//�������� ������.
	void EndRender();

	//������ �����Ѵ�.
	void Finalize();

	void RendParticle();

	// ȭ���� ���α��̸� �����´�.
	int GetScreenWidth() const;
	// ȭ���� ���α��̸� �����´�.
	int GetScreenHeight() const;
	//ȭ��� �����´�. -> Renderer�� ���°� ���� �� �ϴ�.
	float GetAspectRatio();

	//���� �߰�, ���� VB, IB, p, Model*, boneNameVec
	void AddModel(std::string _basePath, std::string _fileName);
	//�ִϸ��̼��� �߰�
	void AddAnimation(std::string _basePath, std::string _fileName);
	//3D�ؽ��ĸ� �߰�
	void Add3DTexture(std::string _basePath, std::string _fileName);
	//2D�ؽ��ĸ� �߰�
	void Add2DTexture(std::string _basePath, std::string _fileName);
	void AddDDSTexture(std::string _basePath, std::string _fileName);

	//���ؽ� ���۸� ������ �´�.
	ComPtr<ID3D11Buffer> Get_VertexBuffer(std::string _modelName);
	//�ε������۸� ������ �´�.
	ComPtr<ID3D11Buffer> Get_IndexBuffer(std::string _modelName);
	//�ε��� ���� ������ �´�.
	unsigned int Get_NumIndex(std::string _modelName);
	//���� ������ ������ �´�. �޽��� �̸��� �ƴ� ���ϸ��� �ִ´�.
	Model* Get_ModelInfo(std::string _modelName);
	//�ִϸ��̼� ������ ������ �´�.
	Animation* Get_Animation(std::string _animeName);
	//�ؽ��ĸ� ������ �´�.
	ComPtr<ID3D11ShaderResourceView> Get_Textures(std::string _textureName);
	//��Ʈ�� ������ �´�.
	ImFont* Get_Font(std::string _fontName);
	//
	int Get_TargetModelBoneIndex(std::string _modelName, std::string _boneName);

	// ������ ������ ���� �Լ� -> ���� offsetMatrix�� ������ Matrix�� �޾ƿ´�.
	Matrix GetTargetBoneMatrix(std::string _targetModel, std::string _targetBoneName);
	// ���� ���� ��ġ�� �޾ƿ��� �Լ� -> ���� offsetMatrix�� �������� ���� ���� ��ġ���� �޾ƿ´�, �⺻ ������ = 1
	Matrix GetTargetBoneAboveMatrix(std::string _targetModel, std::string _targetBoneName, float _scale = 1.f);
	Matrix GetTargetBoneAboveMatrix(std::string _targetModel, int _index, float _scale = 1.f);

	AABB Get_AABB(std::string __targetModel);

	//���ҽ� �����̳ʾ��� Ư�� ���ؽ� ���۸� �����.
	void Erase_VertexBuffer(std::string _modelName);
	//���ҽ� �����̳ʾ��� Ư�� �ε��� ���۸� �����.
	void Erase_IndexBuffer(std::string _modelName);
	//���ҽ� �����̳ʾ��� Ư�� �ε���Num�� �����.
	void Erase_NumIndex(std::string _modelName);
	//���ҽ� �����̳ʾ��� Ư�� �������� �����.
	void Erase_ModelInfo(std::string _modelName);
	//���ҽ� �����̳ʾ��� Ư�� �ִϸ��̼� ������ �����.
	void Erase_Animation(std::string _animName);
	//���ҽ� �����̳� ���� Ư�� �ؽ��ĸ� �����.
	void Erase_Textures(std::string _textureName);
	//���ҽ� �����̳� ���� Ư�� ��Ʈ�� �����.
	void Erase_Font(std::string _fontName);

	//������Ʈ ���۸� �����Ѵ�. �ƿ� ���߿� �� �ؿ� �Լ��� ���ø����� �ٲ��� �����ְڴ�.. 
	ComPtr<ID3D11Buffer> CreateConstantBuffer(VSConstantBufferData& _VsConstantBufferData);
	ComPtr<ID3D11Buffer> CreateConstantBuffer(VSBoneConstantBufferData& _VsBoneConstantBufferData);
	ComPtr<ID3D11Buffer> CreateConstantBuffer(VSTargetBoneConstantBufferData& _PsConstantBufferData);
	ComPtr<ID3D11Buffer> CreateConstantBuffer(PSConstantBufferData& _PsConstantBufferData);
	ComPtr<ID3D11Buffer> CreateConstantBuffer(PSEdgeConstantBufferData& _pPSEdgeConstantBuffer);
	ComPtr<ID3D11Buffer> CreateConstantBuffer(VSShadowConstantBufferData& _VsShadowConstantBufferData);
	ComPtr<ID3D11Buffer> CreateConstantBuffer(PsShadowConstantBufferData& _VsShadowConstantBufferData);
	ComPtr<ID3D11Buffer> CreateConstantBuffer(CommonConstantBufferData& _CommonConstantBufferData);
	ComPtr<ID3D11Buffer> CreateConstantBuffer(VSInstantConstantBufferData& _VSInstantConstantBufferData);

	//Strucured Buffer�� �����Ѵ�. 
	ComPtr<ID3D11Buffer> CreateStructuredBuffer(CSParticleData& _TestCSParticleData, unsigned int _count);


	//���ڱ� �� ���������� �׳� ���ø����� §��.
	template <typename T>
	ComPtr<ID3D11Buffer> CreateConstantBuffer(T& _bufferData)
	{
		return RendererHelper::CreateConstantBuffer(m_pDevice, _bufferData);
	}

	// ���� ������Ʈ ���۸� ������Ʈ�Ѵ�.
	void UpdateCommonConstantBuffer(CommonConstantBufferData& _CommonBufferData);

	//������Ʈ���۸� ������Ʈ�Ѵ�.
	void UpdateConstantBuffer(ModelBuffer* _pModelBuffer, VSConstantBufferData& _VsConstantBufferData);
	//�� ������Ʈ ���۸� ������Ʈ�Ѵ�.
	void UpdateBoneConstantBuffer(ModelBuffer* _pModelBuffer, VSBoneConstantBufferData& _VsBoneConstantBufferData);
	
	//�� �ִϸ��̼��� ������ �ϸ� �� ������Ʈ ���۸� ������Ʈ�Ѵ�.-> Trangision ��� ��
	bool UpdateTransitionBoneConstantBuffer(ModelBuffer* _pModelBuffer, VSBoneConstantBufferData& _VsBoneConstantBufferData);
	//Ÿ�� �� ������Ʈ ���۸� ������Ʈ�Ѵ�.
	void UpdateTargetBoneConstantBuffer(ModelBuffer* _pModelBuffer, VSTargetBoneConstantBufferData& _VsTargetBoneConstantBufferData);


	//������Ʈ���۸� ������Ʈ�Ѵ�.
	void UpdateConstantBuffer(ModelBuffer* _pModelBuffer, PSConstantBufferData& _PsConstantBufferData);

	//�ܰ����� �׸��µ� ���õ� VSconstantBuffer�� ������Ʈ�Ѵ�.
	void UpdateVSEdgeConstantBuffer(ModelBuffer* _pModelBuffer, VSEdgeConstantBufferData& _pPSEdgeConstantBuffer);

	//�ܰ����� �׸��µ� ���õ� PSconstantBuffer�� ������Ʈ�Ѵ�.
	void UpdatePSEdgeConstantBuffer(ModelBuffer* _pModelBuffer, PSEdgeConstantBufferData& _pPSEdgeConstantBuffer);

	//���� ǥ���Ҷ� �ʿ��� VSWaterConstnatBuffer�� ������Ʈ�Ѵ�.
	void UpdateVSWaterConstantBuffer(ModelBuffer* _pModelBuffer, VSWaterConstantBufferData& _pPSEdgeConstantBuffer);

	void UpdateShadowConstantBuffer(ModelBuffer* _pModelBuffer, VSShadowConstantBufferData& _VsShadowConstantBufferData);
	void UpdateShadowConstantBuffer(ModelBuffer* _pModelBuffer, PsShadowConstantBufferData& _VsShadowConstantBufferData);

	//ť����� �����Ѵ�.
	void Set_CubeMap(std::string diffuseTextureName, std::string specularTextureName);
	
	//PipelineState�� �����Ѵ�.
	void SetPipelineState(PipelineStateObject& _pso);
	void Rend_AnimateModel(ModelBuffer* _modelBuffer);
	void Rend_Model(ModelBuffer* _modelBuffer);
	void Rend_EquipmentModel(ModelBuffer* _modelBuffer);
	void SetOpacityFactor(float blendFactor[4]);							//������ �����Ѵ�.
	void Rend_OpacitiyModel(ModelBuffer* _modelBuffer);						//������ ���� ������Ʈ�� �����Ѵ�.

	void Rend_EdgeModel(ModelBuffer* _modelBuffer);							//�ܰ����� �մ� ������Ʈ�� �����Ѵ�.
	void Rend_Water(ModelBuffer* _modelBuffer);


	void RenderDepthMap(ModelBuffer* _modelbuffer);
	void RenderAniDepthMap(ModelBuffer* _modelbuffer);
	void RenderEquipDepthMap(ModelBuffer* _modelbuffer);

	void Rend_InstancedModels(ModelBuffer* _modelbuffers);
	void Rend_BillBoard(ModelBuffer* _modelbuffers);

	void Rend_DebugBox(Vector3 _size, Vector3 _rotation, Vector3 _transpose);
	void Rend_DebugBox(Matrix _size, Matrix _rotation, Matrix _transpose);
	void Rend_DebugBox(Matrix _size, Matrix _rotation, Matrix _transpose, Matrix _tempMatrix);
	void Rend_DebugBox(AABB& _AABB, Matrix Scale, Matrix _rotation, Matrix _tempMatrix = Matrix());

	void Rend_DebugSphere(Vector3 _size, Vector3 _rotation, Vector3 _transpose);
	void Rend_DebugCapsule(Vector3 _size, Vector3 _rotation, Vector3 _transpose);

	void Rend_CubeMap(ModelBuffer* _modelBuffer);
	//�������� �ʿ��� ī�޶� �����Ѵ�.
	void SetCamera(Camera* _pTargetCamera);

	ModelBuffer* Debug_ModelBuffer;		///*�ӽ�*



	VSConstantBufferData m_VSConstantBufferData;
	PSConstantBufferData m_PSConstantBufferData;
	VSBoneConstantBufferData m_VSBoneConstantBufferData;

	/// UI �۾� -------------------------------------------------------------------------------------------------

	// [[[�ʱ�ȭ�� ����]]]
	// [�⺻ ���, ���� �̸�, ��Ʈ ������, �ѱ��ΰ���?] ��Ʈ�� �߰� (�����̸��� ��Ʈ ID�� �ڵ� ����)
	void AddFont(std::string _basePath, std::string _fileName, float _size, bool _isKorean);

	// ��Ʈ�� ��� �߰������� ���� �ϷḦ ȣ����
	void FontSetFinish();

	// [[[�������� ����]]]
	// ������ ����
	void UIBegineRender();

	// ----- �̹��� �� ��Ʈ -----------------------------------

	// [���� ��ġxy, ���μ���] ĵ���� ���� 
	void UICanvasSet(Vector2 _posXY, Vector2 _sizeWH = Vector2());

	// �̹����� �׸��� ������ ������ ����
	void UIDrawImageStart();

	// [���� ��ġxy, ���μ���, UI�̸�, �ؽ�ó �̸�] �̹����� �׸���
	void UIDrawImage(Vector2 _posXY, Vector2 _sizeWH, std::string _textureName, Vector4 _rgba = Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	// �� �̻� �߰��Ǵ� �̹����� ������ ����
	void UIDrawImageFin();

	// [font ���ҽ� �̸�] ������ ��Ʈ�� ����� ������ ����
	void UIStartFontID(std::string _fontName);

	// [��ġxy, ��� ����, RGBA��] �ؽ�Ʈ�� �׸��� �Լ�	//C++
	void UIDrawText(Vector2 _pos, std::u8string _text, Vector4 _rgba = Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	// [��ġxy, ��� ����, RGBA��, �μ���...] ���ϴ� ���ڸ� ������ �ؽ�Ʈ�� �׸��� �Լ�	//C++
	template<typename ...Args>
	void UIDrawTextWithNum(Vector2 _posXY, const std::u8string _formatText, Vector4 _rgba = Vector4(0.0f, 0.0f, 0.0f, 1.0f), Args&& ...args);

	// �������� ������ ��Ʈ�� �� ������� ���� ������ ����
	void UIFinFontID();

	// ----- �⺻ ���� �׸��� -----------------------------------

	// ���� ������ ��������, �⺻ �׸��� �Լ����� �ٷ� ���� ȣ��
	void SetUICurrentWindow();

	// [��ġxy, ���μ���, RGBA] �簢���� �׵θ��� �׸��� �Լ� 
	void UIDrawRect(Vector2 _posXY, Vector2 _sizeWH, Vector4 _rgba, float _rounding = 0.0f, float _thickness = 1.0f);

	// [���� ��ġxy, ���μ���, RGBA, �𼭸� �ձ� ����] ä���� �簢���� �׸��� �Լ�
	void UIDrawRectFilled(Vector2 _posXY, Vector2 _sizeWH, Vector4 _rgba, float _rounding = 0.0f);							

	// [���� ��ġxy, ���μ���, RGBA, �𼭸� �ձ� ����, �β�] �簢���� �׵θ��� �׸��� �Լ�
	void UIDrawRectwithBorder(Vector2 _posXY, Vector2 _sizeWH, Vector4 _rgba, float _rounding = 0.0f, float _thickness = 1.0f);
	
	// [point �ð�������� 4��, RGBA, �β�] ������ �簢���� �׵θ��� �׸��� �Լ�
	void UIFreeRect(Vector2 _posXY1, Vector2 _posXY2, Vector2 _posXY3, Vector2 _posXY4, Vector4 _rgba, float _thickness);
	
	// [point �ð�������� 4��, RGBA] ä���� ������ �簢���� �׸��� �Լ�
	void UIFreeRectFilled(Vector2 _posXY1, Vector2 _posXY2, Vector2 _posXY3, Vector2 _posXY4, Vector4 _rgba);
	
	// [point �ð�������� 4��, RGBA, �β�] ������ �簢���� �׵θ��� �׸��� �Լ�
	void UIFreeRectwithBorder(Vector2 _posXY1, Vector2 _posXY2, Vector2 _posXY3, Vector2 _posXY4, Vector4 _rgba, float _thickness, Vector4 _borderRgba);

	// [������ ��ġxy, ���� ��ġxy, RGBA] ���� �׸��� �Լ� 
	void UIDrawLine(Vector2 _sPosXY, Vector2 _ePosXY, Vector4 _rgba);

	// [���� ��ġxy, ������, RGBA] ���� �׸��� �Լ� 
	void UIDrawCir(Vector2 _posXY, float _radius, Vector4 _rgba);
	// -----------------------------------------------------

	void RenderImGui();

	void EndRenderImGui();

	/// ����Ʈ �۾� -------------------------------------------------------------------------------------------------
	// [PS���۱���, ������� ���� ����] ���ϴ� Light�� ������ �����, PSConstantBufferData�� �����ش�.
	void LightInitialize(CommonConstantBufferData* _psBufferData, UINT _num);

	// [PS���۱���, ������� ���� ����] PS ������ Max Light�� ���� ����
	void ChangeLightMaxNum(CommonConstantBufferData* _psBufferData, UINT _num);

	// [PS���۱���] Light Update, PS ���� ������Ʈ ���� ȣ��
	void LightUpdate(CommonConstantBufferData* _psBufferData);

	// [PS���۱���] ���� ������Ʈ ������ ���� ������ ���� ���
	void PrintLightInfo(CommonConstantBufferData* _psBufferData);

	// ����Ʈ ���� ���� ��ü ���� [PS���۱���, ����, ����, �� ���� �϶��� ����, �� ���� �϶��� ��, ����, ��ġ, spotPower]
	void SetLightSettingAll(CommonConstantBufferData* _psBufferData, UINT _index, LightEnum _lightType, float _strength, float _fallOffStart,
							float _fallOffEnd, Vector3 _dir, Vector3 _pos, float _spotPower, Vector3 _color = Vector3(1.0f, 1.0f, 1.0f));
	// Directional Light ����
	void SetDirLight(CommonConstantBufferData* _psBufferData, UINT _index, float _strength, Vector3 _dir, Vector3 _color = Vector3(1.0f, 1.0f, 1.0f));

	// Point Light ����
	void SetPointLight(CommonConstantBufferData* _psBufferData, UINT _index, float _strength, float _fallOffStart,
		float _fallOffEnd, Vector3 _pos, Vector3 _color = Vector3(1.0f, 1.0f, 1.0f));

	// Spot Light ����
	void SetSpotLight(CommonConstantBufferData* _psBufferData, UINT _index, float _strength, float _fallOffStart,
		float _fallOffEnd, Vector3 _dir, Vector3 _pos, float _spotPower, Vector3 _color = Vector3(1.0f, 1.0f, 1.0f));

	// ���� �ε����� ����Ʈ�� NONE���� �ٲߴϴ�. -> Light ���� ����
	void SetLightOff(CommonConstantBufferData* _psBufferData, UINT _index);

	// [PS���۱���, index, ���� ����(0~1)] ���� ���⸦ ����, 0~1�� ���� �ִ´�.
	void SetLightStrength(CommonConstantBufferData* _psBufferData, UINT _index, float _strength);
	// [PS���۱���, index, ���� ����] ���� ������ ����
	void SetLightType(CommonConstantBufferData* _psBufferData, UINT _index, LightEnum _lightType);
	// [PS���۱���, index, ���� ����] ���� ���� ����, _dir�� ���� ���ͷ� ���� ��.
	void SetLightDir(CommonConstantBufferData* _psBufferData, UINT _index, Vector3 _dir);
	// [PS���۱���, index, ���� ������ �پ��� ������ ���� �Ÿ�] ���� ���� ���� : 5���Ϸ� ������ ������ �� ����
	void SetLightFallOffStart(CommonConstantBufferData* _psBufferData, UINT _index, float _distance);
	// [PS���۱���, index, �� ������ ��] ���� ���� ���� : 5���Ϸ� ������ ������ �� ����
	void SetLightFallOffEnd(CommonConstantBufferData* _psBufferData, UINT _index, float _length);
	// [PS���۱���, index, ���� ��ġ] ���� ��ġ�� ����
	void SetLightPos(CommonConstantBufferData* _psBufferData, UINT _index, Vector3 _pos);
	// [PS���۱���, index, ���� spotPower(0~100)] ���� spotPower�� ���� : Ŭ���� ���� �����
	void SetLightSpotPower(CommonConstantBufferData* _psBufferData, UINT _index, float _power);
	// [PS���۱���, index, ���� ����] ���� ���� ����
	void SetLightColor(CommonConstantBufferData* _psBufferData, UINT _index, Vector3 _rgb);

	// [PS���۱���, ������ ��, ����� �ε���] Ư�� �ε����� �� �����ϱ�
	void CopyLight(CommonConstantBufferData* _psBufferData, UINT _copy, UINT _origin);

	Matrix CreateShadowViewMatrix(const Light& light);
	Matrix CreateShadowProjectionMatrix(const Light& light, float nearPlane, float farPlane, float fieldOfView, float aspectRatio);
};

template<typename ...Args>
void DearsGraphicsEngine::UIDrawTextWithNum(Vector2 _posXY, const std::u8string _formatText, Vector4 _rgba, Args&& ...args)
{
	m_pDearsImGui->UIDrawTextWithNum(_posXY, m_pDearsImGui->ConvertUTF8String(_formatText), _rgba, std::forward<Args>(args)...);
}

