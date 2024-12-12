#pragma once
#include <wrl/client.h>
#include <d3d11.h>

using Microsoft::WRL::ComPtr;

/*
ID3D11Texture2D ��ü�� �ؽ�ó�� ���� �����͸� GPU�� �����ϴ� �ݸ�,
ID3D11ShaderResourceView�� �� �ؽ�ó �����͸� ���̴� ���α׷����� ����� �� �ֵ��� �ϴ� �������̽� ����
*/
struct Texture
{
	//2D �ؽ��ĸ� ��Ÿ����.
	ComPtr<ID3D11Texture2D> mpTexture;

	//�ؽ��ĸ� ���̴����� ����� �� �ֵ��� �ϴ� �並 ��Ÿ����. �̸� ���� ���ؽ� ���̴��� �ȼ� ���̴����� �ش� �ؽ��ĸ� ���� �� �ִ�.
	ComPtr<ID3D11ShaderResourceView> mpTextureResourceView;
};
