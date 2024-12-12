#pragma once
#include "Component.h"
#include "GraphicsDefine.h"

/// <summary>
/// ī�޶� ������ ���Ǵ� ī�޶� ������Ʈ�Դϴ�.
/// <para>
/// CameraComponent(Screen Width, Screen Height, Speed, Position, ViewDir, UpDir, Axes, Proj);
/// </para>
/// </summary>

enum class cameraEnum
{
	WorldCamera,
	LightCamera,
};

struct CameraComponent
	: public Component
{
public:
	CameraComponent(std::shared_ptr<Entity> _owner, int _width, int _height, float _speed, const Vector3& _eyePos
		, const Vector3& _viewDir, const Vector3& _viewUp, const Vector3& _proj, unsigned int _cameraEnum);
	~CameraComponent();

	std::shared_ptr<Component> Clone() const override
	{
		return std::make_shared<CameraComponent>(*this);
	}
	void setViewDir(Vector3 _viewDir);						   //#Sung ������Ʈ�ȿ� �����ϴ� �Լ��� �����.
	void setEyePos(Vector3 _eyePos);
	void setLightCameraIndex(unsigned int _lightIndex);
	unsigned int m_cameraEnum;
public:
	Camera* m_pCamera;
	unsigned int m_lightIndex;
	bool m_isPerspective = true;
	static uint8_t m_numCamera;

};