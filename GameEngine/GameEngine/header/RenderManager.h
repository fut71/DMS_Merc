#pragma once
#include "IManager.h"
#include "AnimationManager.h"
#include "Graphics/DearsGraphicsEngine.h"

class WorldManager;
class TimeManager;
class ResourceManager;
class Camera;
class Entity;
class LightComponent;

/// <summary>
/// ���� �������� �������� ����ϴ� Ŭ�����Դϴ�.
/// �׷��� ������ �������̽��� ������ �����ؼ� ����� �����Դϴ�.
/// 2024.04.22 _Doyo;
/// </summary>
class RenderManager
	: public IManager
{
public:
	RenderManager() = delete;
	RenderManager(entt::registry& _registry, std::shared_ptr<DearsGraphicsEngine> _pGraphicsEngine
		, WorldManager* _pWorldManager, TimeManager* _pTimeManager);
	~RenderManager();

	/// <summary>
	/// ������ �ý����� �ʱ�ȭ�մϴ�.
	/// </summary>
	/// <returns>�ʱ�ȭ ���н� false ��ȯ</returns>
	virtual bool Initialize() override;

	/// <summary>
	/// �� ��ȯ�� ���� �ʱ�ȭ�մϴ�.
	/// </summary>
	/// <returns></returns>
	bool InitializeScene();

	/// <summary>
	/// ���� ����� �����ϴµ� �� �ʿ���.
	/// </summary>
	/// <param name="diffuseTextureName">diffuse_texture Key��</param>
	/// <param name="specularTextureName">specular_texture Key��</param>
	void SetCubeMap(std::string diffuseTextureName, std::string specularTextureName);

	/// <summary>
	/// ��Ÿ�ӿ� ��ƼƼ �߰� �� �ʱ�ȭ�� �������� �� �ִ�.
	/// </summary>
	/// <param name="entity">�ʱ�ȭ�� ������ ��ƼƼ</param>
	/// <returns></returns>
	bool InitailizeEntity(std::shared_ptr<Entity> entity);

	/// <summary>
	/// ������ �ð� �������� ������Ʈ�� �����մϴ�.
	/// </summary>
	/// <param name="_dTime"></param>
	virtual void FixedUpdate(float _dTime) override {}

	/// <summary>
	/// �� ������ ������Ʈ�� �����մϴ�.
	/// </summary>
	/// <param name="_dTime"></param>
	virtual void Update(float _dTime) override;

	void UpdateEntityTexture(std::shared_ptr<Entity> _entity);

	/// <summary>
	/// ��ó�� ������Ʈ�� �����մϴ�.
	/// </summary>
	/// <param name="_dTime"></param>
	virtual void LateUpdate(float _dTime) override;

	/// <summary>
	/// ���� �������� �����մϴ�.
	/// </summary>
	/// <param name="_dTime"></param>
	void Render(float _dTime);

	void BeginRender();

	/// <summary>
	/// �������� ������.
	/// </summary>
	/// <param name="_dTime"></param>
	void EndRender();

	/// <summary>
	/// ������ �ý����� �����ϰ� �����մϴ�.
	/// </summary>
	virtual void Finalize() override;

	/// <summary>
	/// ������ ��带 �����մϴ�.
	/// </summary>
	/// <param name="_model">�� ���</param>
	/// <param name="_geometry">�ݶ��̴� ���</param>
	/// <param name="_shadow">�׸��� ���</param>
	void SetRenderMode(bool _model, bool _geometry, bool _shadow);

	/// <summary>
	/// ���� ī�޶� �����մϴ�.
	/// </summary>
	/// <param name="_index">������ ī�޶��� �ε���</param>
	void ChangeCamera(const uint8_t _index);

	/// <summary> ��ũ�� ���θ� �����´�.</summary> <returns>int</returns>
	int GetScreenWidth();

	/// <summary> ��ũ�� ���̸� �����´�.</summary> <returns>int</returns>
	int GetScreenHeight();


	/// <summary> ���� �߰��Ѵ�.</summary>
	/// <param name="_basePath"> �⺻��� </param> <param name="_fileName">���� �̸�</param>
	void AddModel(std::string _basePath, std::string _fileName);

	// ���� �����ϴ� �ſ� ���ؼ��� �����翡�� ����� ä�� ��

	/// <summary> �ִϸ��̼��� �߰��Ѵ�. </summary>
	/// <param name="_basePath"> �⺻��� </param> <param name="_fileName">���� �̸�</param>
	void AddAnimation(std::string _basePath, std::string _fileName);

	/// <summary> �ִϸ��̼� ������ �����´�. </summary>
	/// <param name="_animeName">�ִϸ��̼� �̸�</param> <returns>Animation*</returns>
	Animation* Get_Animation(std::string _animeName);

	/// <summary> �ִϸ��̼��� �����Ѵ�. </summary>
	/// <param name="_animName"> ���� �̸�(�ִϸ��̼� �̸�)</param>
	void Erase_Animation(std::string _animName);

	/// <summary> 3D �ؽ�ó�� �߰��Ѵ�.</summary>
	/// <param name="_basePath"> �⺻��� </param> <param name="_fileName">���� �̸�</param>
	void Add3DTexture(std::string _basePath, std::string _fileName);

	void AddDDSTexture(std::string _basePath, std::string _fileName);

	/// <summary> 3D �ؽ�ó�� �����Ѵ�. </summary>
	/// <param name="_animName"> ���� �̸�(�ؽ�ó �̸�)</param>
	void Erase_Textures(std::string _textureName);


	/// <summary> ���� Ư�� ���� �����´� -> ��� ������ �� ����Ѵ�. </summary>
	/// <param name="_modelName">�� �̸�</param> <param name="_boneName">�� �̸�</param> <returns>int</returns>
	int Get_TargetModelBoneIndex(std::string _modelName, std::string _boneName);

	// ���� ���� ��ġ�� �޾ƿ��� �Լ� -> ���� offsetMatrix�� �������� ���� ���� ��ġ���� �޾ƿ´�, �⺻ ������ = 1

	/// <summary>���� ���� ��ġ�� �޾ƿ��� �Լ� -> ���� offsetMatrix�� �������� ���� ���� ��ġ���� �޾ƿ´� </summary>
	/// <param name="_targetModel">�� �̸�</param> <param name="_targetBoneName">�� �̸�</param> <param name="_scale">������ = 1.0f</param>
	/// <returns>Matrix</returns>
	Matrix GetTargetBoneAboveMatrix(std::string _targetModel, std::string _targetBoneName, float _scale = 1.0f);

	/// <summary> �浹 üũ�� ���� AABB�� �������� �Լ� </summary>
	/// <param name="__targetModel">�� �̸�</param> <returns>AABB</returns>
	AABB Get_AABB(std::string __targetModel);

	// ����Ʈ ���� -------------------------------------------------------------------------------

	/// <summary> ���ϴ� Light�� ������ �����, PSConstantBufferData�� �����ش�.</summary>
	/// <param name="_psBufferData">PSConstantBufferData</param> <param name="_num">����</param>
	void LightInitialize(CommonConstantBufferData* _ccBufferData, UINT _num);

	/// <summary> Directional Light ���� </summary>
	/// <param name="_psBufferData">PSConstantBufferData</param> <param name="_index">�ε���</param> <param name="_strength">����</param> 
	/// <param name="_dir">����</param> <param name="_color">��</param>
	void SetDirLight(CommonConstantBufferData* _ccBufferData, UINT _index, float _strength, Vector3 _dir, Vector3 _color = Vector3(1.0f, 1.0f, 1.0f));

	/// <summary> ���� ������Ʈ ������ ���� ������ ���� ��� </summary>
	/// <param name="_psBufferData">PSConstantBufferData</param>
	void PrintLightInfo(CommonConstantBufferData* _ccBufferData);

	void CameraSetPerspective();
	void CameraSetOrthographic(float _scale);

	void SetOpacityFactor(const Vector4& _value);

	void AddParticle(uint16_t _num, CSParticleData& _particleData) const;

	std::shared_ptr<DearsGraphicsEngine> m_pGraphicsEngine; // �׷��Ƚ� ���� ��ü

private:
	// ������ ���� �Լ� �и�
	void RenderShadow(LightComponent* _light);
	void RenderModel(LightComponent* _light);
	void RenderGeometry(LightComponent* _light);

	// ������Ʈ �Լ��� �и�
	void UpdateLight();
	void UpdateModel(float _dTime);
	void UpdateCubeMap();
	void UpdatePSCB();
	void UpdateTexture();

private:
	// ������ ��� ����
	bool m_renderModel = true;
	bool m_renderGeometry = true;
	bool m_renderShadow = true;

	entt::registry& m_registry;
	WorldManager* m_pWorldManager;
	TimeManager* m_pTimeManager;

	Camera* m_pWorldCamera;									//#sung �ƿ� ����ī�޶�� ����Ʈ ī�޶��� �����͸� ������ ������ �Ͽ���.
	Camera* m_pLightCamera[MAX_LIGHTS];
	AnimationManager* mpAnimationManager;

	std::unique_ptr<entt::observer> m_pLightObserver;
	std::unique_ptr<entt::observer> m_pMeshObserver;
	std::unique_ptr<entt::observer> m_pTexture3DObserver;
	std::unique_ptr<entt::observer> m_pTexture2DObserver;

	float m_opacityFactor[4] = { 1.f,1.f,1.f,1.f };

};