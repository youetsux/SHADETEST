#pragma once
#include <wrl\client.h>
#include <d3d11.h>

//リンカ
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

using Microsoft::WRL::ComPtr;

namespace Direct3D

{
	//初期化
	HRESULT Initialize(int winW, int winH, HWND hWnd);
	//シェーダー準備
	HRESULT InitShader();
	//描画開始
	void BeginDraw();
	//描画終了
	void EndDraw();
	//解放
	void Release();

	extern ComPtr <ID3D11Device> pDevice;
	extern ComPtr<ID3D11DeviceContext> pContext;

};