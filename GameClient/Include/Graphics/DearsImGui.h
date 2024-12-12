#pragma once
#include <imgui.h>
#include <imgui_impl_dx11.h>
#include <imgui_impl_win32.h>
#include <imgui_internal.h> //����� ���� ������ ����ų� ImGui�� ������ �����ϴ� ��쿡 ���
#include "UIEnum.h"
#include "GraphicsResourceManager.h"

/// <summary>
/// �߰��ؾ� �� ��
/// ����ȭ
/// </summary>
class InputManager;

/// <summary>
/// ImGui�� ����ϱ� ���� ���� ��� Ŭ����
/// </summary>
class DearsImGui
{
public:
	DearsImGui(HWND _hWnd, ComPtr<ID3D11Device>& _pDevice, ComPtr<ID3D11DeviceContext>& m_pDeviceContext, int _endScreenWidth, int _endScreenHeight, GraphicsResourceManager* _pResourceManager);
	~DearsImGui();

	InputManager* m_pInputManager;

private:
	ComPtr<ID3D11Device> m_pDevice;					   // �׷��Ƚ� �������� ����̽��� �޾ƿ´�.
	GraphicsResourceManager* m_pResourceManager;	   // ���ҽ� �Ŵ����� �����͸� ����̽����� �޾ƿ´� -> �����̳ʿ� �����ϱ� ���� ȣ��
	int m_endScreenWidth;							   // ��ũ�� ���� ����
	int m_endScreenHeight;							   // ��ũ�� ���� ����
	ImGuiWindow* window;							   // ImGui�� ȭ���� ����Ű�� ����
	ImGuiIO* io;									   // â�� �ʱ�ȭ�ϰų� �����ϴµ� ���

	Vector2 t_imagePos = Vector2(150, 600);			   // ���� ����

public:
	/// �ʱ�ȭ�� �����ϴ� �κ� : ���� �� ���� (������� ����)
	void UILoadFonts(std::string _basePath, std::string _fileName, float _size, bool _isKorean);	// ��Ʈ�� �ε��Ѵ�. (��Ʈ ���� ���, ��Ʈ ������, �ѱ� ����)
	void UIBuildFonts();			// ��Ʈ�� �ε��� ��, ��Ʈ �ؽ�ó�� �ٽ� ����

	/// D3D ���� ���� �ֱ� (������� ����)
	void UIBeginRender(); // BeginRender ���� ȣ��
	void UISetting();		 // �׸� UI�� ����
	void UIRender();		 // ImGui ������

	///  D3D EndRender ���� �ֱ�
	void UIEndRender();	 // EndRender ���� ȣ��

	/// ĵ���� ����
	void UICanvasSet(Vector2 _posXY, Vector2 _sizeWH);	// ĵ���� ���� (���� ��ġxy, ���μ���)
	void UICanvasSetFin();										// ĵ���� ������ �������� ����
	void NewUISetWindow(Vector2 _posXY, Vector2 _sizeWH, const std::string _windowName); // ���ο� â�� ���� �� ��� (���� ��ġxy, ���μ���, ����â�� �̸�(�ߺ�x))
	void SetUICurrentWindow();							// ���� ������ ��������, �⺻ �׸��� �Լ��� �ٷ� ���� ȣ��

	/// �⺻ �׸���
	// �ؽ�Ʈ
	void UIDrawText(Vector2 _posXY, const std::string _text, Vector4 _rgba = Vector4(0.0f, 0.0f, 0.0f, 1.0f));				// �ؽ�Ʈ�� �׸��� �Լ� (���� ��ġxy, ���� ���� ����) 
	// ���ڰ� �ִ� �ؽ�Ʈ
	template<typename ...Args>
	void UIDrawTextWithNum(Vector2 _posXY, const std::string _formatText, Vector4 _rgba, Args&&... args);
	// �簢��
	void UIDrawRect(Vector2 _posXY, Vector2 _sizeWH, Vector4 _rgba, float _rounding = 0.0f, float _thickness = 1.0F);				// �簢���� �׵θ��� �׸��� �Լ� (���� ��ġxy, ���μ���, RGBA, �𼭸� �ձ� ����, �β�)
	void UIDrawRectFilled(Vector2 _posXY, Vector2 _sizeWH, Vector4 _rgba, float _rounding = 0.0f);							// ä���� �簢���� �׸��� �Լ� (���� ��ġxy, ���μ���, RGBA, �𼭸� �ձ� ����)
	void UIDrawRectwithBorder(Vector2 _posXY, Vector2 _sizeWH, Vector4 _rgba, float _rounding = 0.0f, float _thickness = 1.0F);	// �簢���� �׵θ��� �׸��� �Լ�(���� ��ġxy, ���μ���, RGBA, �𼭸� �ձ� ����, �β�)
	void UIFreeRect(Vector2 _posXY1, Vector2 _posXY2, Vector2 _posXY3, Vector2 _posXY4, Vector4 _rgba, float _thickness);	// ������ �簢���� �׵θ��� �׸��� �Լ� (point �ð�������� 4��, RGBA, �β�)
	void UIFreeRectFilled(Vector2 _posXY1, Vector2 _posXY2, Vector2 _posXY3, Vector2 _posXY4, Vector4 _rgba);				// ä���� ������ �簢���� �׸��� �Լ� (point �ð�������� 4��, RGBA)
	void UIFreeRectwithBorder(Vector2 _posXY1, Vector2 _posXY2, Vector2 _posXY3, Vector2 _posXY4, Vector4 _rgba, float _thickness, Vector4 _borderRgba); // ������ �簢���� �׵θ��� �׸��� �Լ� (point �ð�������� 4��, RGBA, �β�, �β� RGBA)
	// ��
	void UIDrawLine(Vector2 _sPosXY, Vector2 _ePosXY, Vector4 _rgba);		// ���� �׸��� �Լ� (������ ��ġxy, ���� ��ġxy, RGBA)
	// ��
	void UIDrawCircle(Vector2 _posXY, float _radius, Vector4 _rgba);		// ���� �׸��� �Լ� (���� ��ġxy, ������, RGBA)
	
	/// �̹��� �׸���
	void UIDrawImage(Vector2 _posXY, Vector2 _sizeWH, ComPtr<ID3D11ShaderResourceView> _srv, Vector4 _rgba = Vector4(1.0f, 1.0f, 1.0f, 1.0f)); // �̹����� �׸��� ���� �⺻���� �Լ� (���� ��ġxy, ���μ���, �ؽ�ó, �ؽ�ó �߰� ����)

	/// ���
	bool Hovering();			// ���콺�� ���� ���̵� ������Ʈ ���� �����ϴ��� Ȯ��
	bool Dragging();			// ���콺�� ���� �巡�� ������ Ȯ��
	void ResetDragDelta();		// �巡�� ���� ��ġ���� ����

	/// ���� ���̵� �߰��� ����
	// ImGui���� ��Ʈ�� ����ϴ� ����� �Ϲ����� �޸� �Ҵ�� �ٸ���.
	// ImFont ��ü�� ImGuiIO�� ��Ʈ ���ÿ� ����Ǹ�, ������ �Ҵ��̳� ������ �ʿ䰡 ����.
	void UIDrawImageStart();	// ������Ʈ ���� ���̵� �߰�
	void UIDrawImageFin();			// ������Ʈ ���� ���̵� ����
	void UIStartFontID(ImFont* _imFont);	// ��Ʈ ���̵� �߰�
	void UIEndFontID();			// ��Ʈ ���̵� ����

	/// C++20���� ������ UTF-8 ���ڿ��� char8_t ���Կ� ���� �ڷ������� �ٲ�� ������ �̸� �ٽ� ���ڿ��� ��ȯ�ϴ� �Լ�
	std::string ConvertUTF8String(const std::u8string& _text); // u8 �ؽ�Ʈ�� String ������ ��ȯ

private:
	void Drag(Vector2& _posXY); 
};

template<typename ...Args>
void DearsImGui::UIDrawTextWithNum(Vector2 _posXY, const std::string _formatText, Vector4 _rgba, Args&&... args)
{
	char buffer[256]; // ����� ū ���۸� Ȯ�� -> ���� ��ȹ���� �����ϴ� �ϸ� 512�� �ٲ� ��
	sprintf_s(buffer, sizeof(buffer), _formatText.c_str(), std::forward<Args>(args)...);

	UIDrawText(_posXY, std::string(buffer)/*fullText*/, _rgba);
}

