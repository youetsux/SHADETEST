#pragma once
#include <wrl\client.h>
#include <d3d11.h>

//�����J
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

using Microsoft::WRL::ComPtr;

namespace Direct3D

{
	//������
	HRESULT Initialize(int winW, int winH, HWND hWnd);
	//�V�F�[�_�[����
	HRESULT InitShader();
	//�`��J�n
	void BeginDraw();
	//�`��I��
	void EndDraw();
	//���
	void Release();

	extern ComPtr <ID3D11Device> pDevice;
	extern ComPtr<ID3D11DeviceContext> pContext;

};