#pragma once

#include <d3d11.h>

//�����J
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")


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

	extern ID3D11Device* pDevice;
	extern ID3D11DeviceContext* pContext;

};