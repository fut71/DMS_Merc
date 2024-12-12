#pragma once
#include "IManager.h"
#include "Entity.h"
#include "MeshRenderer.h"
#include "InputManager.h"
#include "Graphics/DearsGraphicsEngine.h"
#include "GameInfo.h"
#include "Button.h"
/// <summary>
/// UI ��ŷ���� �����մϴ�.
/// </summary>
class UIManager
	: public IManager
{
public:
	UIManager() = delete;
	UIManager(entt::registry& _registry, InputManager* _pInputManager, std::shared_ptr<DearsGraphicsEngine> _pGraphicsEngine
		, GameInfo* _info)
		: mRegistry(_registry), mpInputManager(_pInputManager), mpGraphicsEngine(_pGraphicsEngine)
		, mIsAlreadyFontLoaded(false), maxNumLayer(1), mViewMatrix(Matrix()), mProjMatrix(Matrix()), mpInfo(_info) {}

	~UIManager() = default;

	virtual bool Initialize();

	virtual void FixedUpdate(float _fixedDTime);

	// �ؽ�ó�� �ٲ�� �ٲ㲸�ش�.
	virtual void Update(float _dTime);

	virtual void LateUpdate(float _dTime);

	// UI�� �����Ѵ�.
	void Render(float _dTime);

	/// <summary>
	/// �߰��� UI�� ���� ��� ������Ʈ�� �����ϰ�, Texture�� ��Ʈ ���ҽ��� �����մϴ�.
	/// </summary>
	virtual void Finalize();

	/// ��Ʈ �� �ؽ�Ʈ ----------------------

	// [[[�ʱ�ȭ�� ����]]]
	// [�⺻ ���, ���� �̸�, ��Ʈ ������, �ѱ��ΰ���?] ���ҽ� �����̳ʿ� ��Ʈ�� �߰� (�����̸��� ��Ʈ ID�� �ڵ� ����)
	void AddFont(std::string _basePath, std::string _fileName, float _size, bool _isKorean);

	// [���� ����] ��Ʈ�� ��� �߰������� ���� �ϷḦ ȣ����
	void FontSetFinish();

	// �ؽ�Ʈ�� 3D ��ǥ�� �� ��� m_hasDepth�� true�� �����ϰ�, m_position�� � ���� �־ ���õ˴ϴ�. m_worldPosition�� ���� ä���ּ���.

	// �ؽ�Ʈ�� �߰��Ѵ�.
	void AddText(std::shared_ptr<Entity> _entity, std::u8string _text, std::string _fontName, Vector2 _pos, int _layer = 0, bool _isVisible = true, Vector4 _rgba = Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	// int ���ڰ� �ϳ� �����ϴ� �Լ��Դϴ�. u8"������ : %d" -> ������Ʈ���� ���� m_num1�� �����մϴ�.
	void AddTextwithInt(std::shared_ptr<Entity> _entity, std::u8string _text, std::string _fontName, Vector2 _pos, int _num, int _layer = 0, bool _isVisible = true, Vector4 _rgba = Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	// int ���ڰ� �� �� �����ϴ� �Լ��Դϴ�. u8"xz��ǥ: %d, %d" -> ������Ʈ���� ���� m_num1, m_num2�� �����մϴ�.
	void AddTextwithInt2(std::shared_ptr<Entity> _entity, std::u8string _text, std::string _fontName, Vector2 _pos, int _num1, int _num2, int _layer = 0, bool _isVisible = true, Vector4 _rgba = Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	// float ���ڰ� �ϳ� �����ϴ� �Լ��Դϴ�. u8"����� : %f" -> ������Ʈ���� ���� m_num3�� �����մϴ�.
	void AddTextwithFloat(std::shared_ptr<Entity> _entity, std::u8string _text, std::string _fontName, Vector2 _pos, float _num, int _layer = 0, bool _isVisible = true, Vector4 _rgba = Vector4(0.0f, 0.0f, 0.0f, 1.0f));
	// float ���ڰ� �� �� �����ϴ� �Լ��Դϴ�. u8"���콺 x��ǥ: %.2f \n���콺 y��ǥ: %.2f" -> ������Ʈ���� ���� m_num3, m_num4�� �����մϴ�.
	void AddTextwithFloat2(std::shared_ptr<Entity> _entity, std::u8string _text, std::string _fontName, Vector2 _pos, float _num1, float _num2, int _layer = 0, bool _isVisible = true, Vector4 _rgba = Vector4(0.0f, 0.0f, 0.0f, 1.0f));

	// �ؽ�Ʈ�� �����Ѵ�.
	void RemoveText(std::shared_ptr<Entity> _entity);

	// ���ҽ� �����̳� ���� ��Ʈ�� �����Ѵ�.
	void EraseFont(std::string _fontName);

	/// 2D �ؽ�ó ----------------------

	// [���� ����]���ҽ� �����̳ʿ� 2D �ؽ�ó�� �߰��Ѵ�.
	void AddTexture2D(std::string _basePath, std::string _fileName);

	// UI�� �߰��Ѵ�.
	void AddUI(std::shared_ptr<Entity> _entity, std::string _fileName, Vector2 _pos, Vector2 _size, int _layer = 0, Vector4 _rgba = Vector4(1.0f, 1.0f, 1.0f, 1.0f), bool _isVisible = true);

	// UI�� �����Ѵ�.
	void RemoveUI(std::shared_ptr<Entity> _entity);

	//���ҽ� �����̳� ���� Ư�� �ؽ��ĸ� �����.
	void Erase_Textures(std::string _textureName);

	// ������Ʈ �ÿ� �ʱ�ȭ�� �־��� ���̾�� ���� ���� �ִ´ٸ� �� �Լ��� ����ؼ� �ִ� layerNum�� ������ ��
	void SetMaxNumLayer(int _num);

	/// ��ư ----------------------

	// Press UI �� Text, ������ �߰��Ѵ�.
	void AddButtonPressUI(std::shared_ptr<Entity> _entity, std::string _UIImange, Vector2 _pressedPosition = Vector2(), Vector2 _pressedSize = Vector2());
	void AddButtonPressText(std::shared_ptr<Entity> _entity, std::string _font, std::u8string _text, Vector2 _pressedPosition = Vector2());
	void AddButtonPressColor(std::shared_ptr<Entity> _entity, Vector4 _PressedRGBA);

	// Hover UI �� Text, ������ �߰��Ѵ�.
	void AddButtonHoveredUI(std::shared_ptr<Entity> _entity, std::string _UIImange, Vector2 _hoveringPosition = Vector2(), Vector2 _hoveringSize = Vector2());
	void AddButtonHoveredText(std::shared_ptr<Entity> _entity, std::string _font, std::u8string _text, Vector2 _hoveringPosition = Vector2());
	void AddButtonHoveredColor(std::shared_ptr<Entity> _entity, Vector4 _HoveredRGBA);

	// Press�� Hover ������ �߰��Ѵ�.
	void AddButtonAllColor(std::shared_ptr<Entity> _entity, Vector4 _PressedRGBA, Vector4 _HoveredRGBA);
	// Press�� Hover UI�� �߰��Ѵ�.[press�̹���, ȣ���� �̹���, press��ġ�� ������, ȣ���� ��ġ�� ������]
	void AddButtonAllUI(std::shared_ptr<Entity> _entity, std::string _pressedUIImange, std::string _hoveringUIImange, Vector2 _pressedPosition = Vector2(), Vector2 _pressedSize = Vector2()
		, Vector2 _hoveringPosition = Vector2(), Vector2 _hoveringSize = Vector2());

	ButtonState GetButtonState(std::shared_ptr<Entity> _entity);

	// ��ư�� �����Ѵ�.
	void RemoveButton(std::shared_ptr<Entity> _entity);

	// �簢�� ----------------------

	/// <summary>�簢���� �߰��Ѵ�.</summary>
	/// <param name="_entity">��ƼƼ</param> <param name="_posXY">��ġ</param> <param name="_sizeWH">������</param>
	/// <param name="_layer">���̾�</param> <param name="_rgba">����</param> <param name="_thickness">�β� = 0.0f</param> <param name="_BorderRgba">�׵θ��� = Vector4()</param>
	void AddRect(std::shared_ptr<Entity> _entity, Vector2 _posXY, Vector2 _sizeWH, int _layer, Vector4 _rgba, float _thickness = 0.0f, Vector4 _BorderRgba = Vector4());

	/// <summary>���簢���� �����Ѵ�.</summary> <param name="_entity">��ƼƼ</param>
	void RemoveRect(std::shared_ptr<Entity> _entity);

	// Ÿ��  ----------------------
	// ���� �β��� ���� �ɷθ� �׽�Ʈ ���� ����
	/// <summary> Ÿ���� �߰��Ѵ�. ��, point�� �ð�������� �߰��Ѵ�.</summary>
	/// <param name="_entity">��ƼƼ</param> <param name="_posXY">��ġ</param> <param name="_sizeWH">������</param>
	/// <param name="_layer">���̾�</param> <param name="_rgba">����</param> <param name="_thickness">�β� = 0.0f</param> <param name="_BorderRgba">�׵θ��� = Vector4()</param>
	void AddTile(std::shared_ptr<Entity> _entity, Vector2 _posXY, Vector2 _sizeWH, int _layer, Vector4 _rgba, float _thickness = 0.0f, Vector4 _BorderRgba = Vector4());

	// Ÿ���� �߰��� �� �����Ѵ�.
	void AddTileMap(std::shared_ptr<Entity> _entity, Vector2 _center, int _col, int _row);
	// ü�¹� -------------------------

	/// <summary>2D ü�¹ٸ� �߰��Ѵ�.</summary>
	/// <param name="_entity">��ƼƼ</param> <param name="_posXY">��ġ</param> <param name="_sizeWH">������</param> <param name="_layer">���̾�</param> 
		/// <param name="_backgroundRgba">����</param> <param name="_foregroundColor">ü�»�</param> <param name="_healthPercentage">ü�º��� = 1.0f</param>
	void AddHPBar(std::shared_ptr<Entity> _entity, Vector2 _posXY, Vector2 _sizeWH, int _layer,
		Vector4 _backgroundRgba, Vector4 _foregroundColor, float _healthPercentage = 1.0f);

	/// <summary>3D ü�¹ٸ� �߰��Ѵ�.</summary>
	/// <param name="_entity">��ƼƼ</param> <param name="_offset">���� ��ġ�κ����� offset</param> <param name="_sizeWH">������</param> <param name="_layer">���̾�</param> 
	/// <param name="_backgroundRgba">����</param> <param name="_foregroundColor">ü�»�</param> <param name="_healthPercentage">ü�º��� = 1.0f</param>
	void Add3DHPBar(std::shared_ptr<Entity> _entity, Vector3 _offset, Vector2 _sizeWH,
		int _layer, Vector4 _backgroundRgba, Vector4 _foregroundColor, float _healthPercentage = 1.0f);

	/// <summary> ü�¹ٸ� �����Ѵ�. </summary> <param name="_entity">��ƼƼ</param>
	void RemoveHPBar(std::shared_ptr<Entity> _entity);

	// �޼��� �ڽ� --------------------

	/// <summary> 2D �������� ���̴� �޼��� �ڽ��� �߰��Ѵ�.(�ؽ�Ʈ�� Vector4(1.0f)�� �⺻������ ��������) </summary>
	/// <param name="_entity">��ƼƼ</param> <param name="_fileName">img �����̸�</param> <param name="_texturePos">img ��ġ</param> <param name="_size">img ������</param> 
	/// <param name="_imgRgba">img RGBA(1.0f)</param> <param name="_text">�ؽ�Ʈ</param> <param name="_textPos">�ؽ�Ʈ ��ġ</param> <param name="_textScale">text ũ��</param>
	/// <param name="_fontName">��Ʈ�̸�</param> <param name="_textRgba">�ؽ�Ʈ rgba</param> <param name="_layer">���̾� = 0</param> <param name="_isVisible">���� ���� = true</param>
	void AddMessageBox2D(std::shared_ptr<Entity> _entity, std::string _imgFileName, Vector2 _imgPos, Vector2 _size, Vector4 _imgRgba
		, std::u8string _text, Vector2 _textPos, float _textScale, std::string _fontName, Vector4 _textRgba, int _layer = 0, bool _isVisible = true);

	/// <summary> 3D �������� ���̴� �޼��� �ڽ��� �߰��Ѵ�.(�ؽ�Ʈ�� Vector4(1.0f)�� �⺻������ ��������) </summary> 
	/// <param name="_entity">��ƼƼ</param> <param name="_imgFileName">img �����̸�</param> <param name="_imgOffset">img ������</param> <param name="_size">img ������</param> 
	/// <param name="_imgRgba">img RGBA(1.0f)</param> <param name="_text">�ؽ�Ʈ</param> <param name="_textOffset">text ������</param> <param name="_textScale">text ũ��</param>
	/// <param name="_fontName">��Ʈ �̸�</param> <param name="_textRgba">text RGBA</param> <param name="_worldPos">���� pos</param> <param name="_worldPosOffset">���� ������xy</param>
	///  <param name="_layer">���̾� = 0</param> <param name="_isVisible">���� ���� = true</param>
	void AddMessageBox3D(std::shared_ptr<Entity> _entity, std::string _imgFileName, Vector2 _imgOffset, Vector2 _size, Vector4 _imgRgba
		, std::u8string _text, Vector2 _textOffset, float _textScale, std::string _fontName, Vector4 _textRgba, Vector3 _worldPos, Vector2 _worldPosOffset, int _layer = 0, bool _isVisible = true);

	/// <summary> �޼��� �ڽ��� �����Ѵ�. </summary> <param name="_entity">��ƼƼ</param>
	void RemoveMessageBox2D(std::shared_ptr<Entity> _entity);
	void RemoveMessageBox3D(std::shared_ptr<Entity> _entity);

	void SetRenderDebugInfo(bool _fps, bool _camera);

private:
	entt::registry& mRegistry;
	std::shared_ptr<DearsGraphicsEngine> mpGraphicsEngine; // �׷��Ƚ� ���� ��ü
	InputManager* mpInputManager;
	bool mIsAlreadyFontLoaded; // �̹� ��Ʈ�� �߰��ƴ��� Ȯ���ؼ�, �߰� �ϷḦ ���� �÷���

	int maxNumLayer;  // ���� ���� �ִ� ���̾� ��ȣ
	Matrix mViewMatrix; // ī�޶��� �����
	Matrix mProjMatrix; // ī�޶��� �������� ���

	GameInfo* mpInfo;

	bool m_rendFPS = true;
	bool m_rendCameraInfo = true;

private:

	// ī�޶��� ����İ� �������� ����� �޶����� ���� ������Ʈ �Ѵ�.
	void UpdateViewNProjection(Camera* camera);

	// ���� ��ǥ�� NDC ��ǥ�� ��ȯ
	Vector2 ConvertWorldtoNDC(Vector3 _worldPos, const Matrix& viewMatrix, const Matrix& projMatrix);

	// 3D�� ��ǥ�� NDC ��ǥ�� �ű��
	Vector2 ConvertNDCtoScreeen(Vector2 _NDCPos);

	// ��ư�� ���� ���� �̹����� ������� ��, �̹����� ������Ʈ�Ѵ�.
	void UpdateButton(Texture2D* _pTexture2D, Button* _pButton, Text* _pText);

	// ��ư�� ���¸� üũ�Ѵ�.
	void CheckButtonState(Button* _pButton, bool _isHovered);

	void RenderUI(int _layerOrder);
	void RenderText(int _layerOrder);

	// ���簢���� �׸���.
	void RenderRectFilled(int _layerOrder);

	// ü�¹ٸ� �׸���.
	void RenderHealthBar(int _layerOrder);

	// Ÿ���� �׸���.
	void RenderTile(int _layerOrder);

	// �޼��� �ڽ��� ������Ʈ�Ѵ�.
	void UpdateMessageBox(int _layerOrder);

	// ����� ������ ����մϴ�.
	void RenderDebugInfo();
};