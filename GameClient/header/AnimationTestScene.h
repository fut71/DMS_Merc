#pragma once
#include "Scene.h"
#include "AllyFSM.h"

class AnimationTestScene : public Scene
{
public:
	/// <summary>
	/// ������ �⺻������ ���Ǵ� ��ü���� �����ڿ��� �������ݴϴ�.
	/// </summary>
	AnimationTestScene(entt::registry& _registry, const std::string& _name
		, EventManager* _pEventManager, RenderManager* _pRenderManager
		, PhysicsManager* _pPhysicsManager, InputManager* _pInpueManager
		, WorldManager* _worldManager, UIManager* _UIManager, EntityManager* _pEntityManager
		, ResourceManager* _pResourceManager);

	/// <summary>
	/// ���� �ʱ�ȭ�մϴ�.
	/// </summary>
	/// <returns>__super::Initialize() ��ȯ</returns>
	virtual bool Initialize() override;

	/// <summary>
	/// ������ �ð� �������� ���� ������Ʈ�� �����մϴ�.
	/// </summary>
	/// <param name="_dTime">������ ��ŸŸ��</param>
	virtual void FixedUpdate(float _dTime) override;

	/// <summary>
	/// �� ������ ������Ʈ�� �����մϴ�.
	/// </summary>
	/// <param name="_dTime">������ �ð�</param>
	virtual void Update(float _dTime) override;

	/// <summary>
	/// ��ó�� ������Ʈ�� �����մϴ�.
	/// </summary>
	/// <param name="_dTime">������ �ð�</param>
	virtual void LateUpdate(float _dTime) override;

	/// <summary>
	/// ���� �����ϰ� �����մϴ�.
	/// </summary>
	virtual void Finalize() override;

	/// <summary>
	/// ���� ���ҽ��� �߰��մϴ�.
	/// </summary>
	virtual void AddResource();

	Vector3 ScreenToNDC(int mouseX, int mouseY, float screenWidth, float screenHeight);
	Vector4 NDCToClip(Vector3 _NDCCoords);
	Vector4 ClipToEye(Vector4 clipCoords, Matrix invProj);
	Vector3 EyeToWorld(Vector4 eyeCoords, Matrix invView);
	Vector3 ScreenToWorldRay();

	// ��ü�� ���콺 ��ġ���� ���� �Ÿ���ŭ ������ ��ġ�� �̵�
	Vector3 GetTargetPosition(Vector3 rayWorldDirection, Vector3 cameraPosition, Vector3 pointOffset, float cameraDistance);

private:
	std::vector<std::shared_ptr<Entity>> m_pCoins;
	float mCoolTime = 0.0f;
	float interpolationTime = 0.0f;
	float animationPlaytime = 0.0f;
};