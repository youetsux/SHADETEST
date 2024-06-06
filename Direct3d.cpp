#include <d3dcompiler.h>
#include "Direct3D.h"
#include "StringToWideStringConverter.h"

using Microsoft::WRL::ComPtr;


//変数
namespace Direct3D
{
	ComPtr<ID3D11Device> pDevice;		//デバイス
	ComPtr<ID3D11DeviceContext> pContext;		//デバイスコンテキスト
	ComPtr<IDXGISwapChain> pSwapChain;		//スワップチェイン
	ComPtr<ID3D11RenderTargetView> pRenderTargetView;	//レンダーターゲットビュー
	ComPtr<ID3D11DepthStencilView> pDepthStencilView; //DepthStencilView

	ComPtr<ID3D11VertexShader> pVertexShader = nullptr;	//頂点シェーダー
	ComPtr<ID3D11PixelShader> pPixelShader = nullptr;		//ピクセルシェーダー
	ComPtr<ID3D11InputLayout> pVertexLayout = nullptr;	//頂点インプットレイアウト
	ComPtr<ID3D11RasterizerState> pRasterizerState = nullptr;	//ラスタライザー
	ComPtr<ID3D11BlendState> pBlendState = nullptr;
	
	::EffekseerRendererDX11::RendererRef gRenderer;
	::Effekseer::ManagerRef gManager;
	::Effekseer::EffectRef gEffect;
	::Effekseer::Backend::GraphicsDeviceRef gEFDev;

}

//初期化
HRESULT Direct3D::Initialize(int winW, int winH, HWND hWnd)
{
	HRESULT hr = S_OK;
	///////////////////////////いろいろ準備するための設定///////////////////////////////
	//いろいろな設定項目をまとめた構造体
	DXGI_SWAP_CHAIN_DESC scDesc;

	//とりあえず全部0
	ZeroMemory(&scDesc, sizeof(scDesc));

	//描画先のフォーマット
	scDesc.BufferDesc.Width = winW;		//画面幅
	scDesc.BufferDesc.Height = winH;	//画面高さ
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// 何色使えるか

	//FPS（1/60秒に1回）
	scDesc.BufferDesc.RefreshRate.Numerator = 60;
	scDesc.BufferDesc.RefreshRate.Denominator = 1;

	//その他
	scDesc.Windowed = TRUE;			//ウィンドウモードかフルスクリーンか
	scDesc.OutputWindow = hWnd;		//ウィンドウハンドル
	scDesc.BufferCount = 1;			//バックバッファの枚数
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//バックバッファの使い道＝画面に描画するために
	scDesc.SampleDesc.Count = 1;		//MSAA（アンチエイリアス）の設定
	scDesc.SampleDesc.Quality = 0;		//　〃
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;


	////////////////上記設定をもとにデバイス、コンテキスト、スワップチェインを作成////////////////////////
	D3D_FEATURE_LEVEL level;
	hr = D3D11CreateDeviceAndSwapChain(
		nullptr,				// どのビデオアダプタを使用するか？既定ならばnullptrで
		D3D_DRIVER_TYPE_HARDWARE,		// ドライバのタイプを渡す。ふつうはHARDWARE
		nullptr,				// 上記をD3D_DRIVER_TYPE_SOFTWAREに設定しないかぎりnullptr
		0,					// 何らかのフラグを指定する。（デバッグ時はD3D11_CREATE_DEVICE_DEBUG？）
		nullptr,				// デバイス、コンテキストのレベルを設定。nullptrにしとけばOK
		0,					// 上の引数でレベルを何個指定したか
		D3D11_SDK_VERSION,			// SDKのバージョン。必ずこの値
		&scDesc,				// 上でいろいろ設定した構造体
		pSwapChain.GetAddressOf(),				// 無事完成したSwapChainのアドレスが返ってくる
		pDevice.GetAddressOf(),				// 無事完成したDeviceアドレスが返ってくる
		&level,					// 無事完成したDevice、Contextのレベルが返ってくる
		pContext.GetAddressOf());				// 無事完成したContextのアドレスが返ってくる


	if (FAILED(hr))
	{
		//エラー処理
		MessageBox(NULL, L"デバイス、コンテキスト、スワップチェインの作成に失敗しました", L"エラー", MB_OK);
		return hr;
	}

	///////////////////////////レンダーターゲットビュー作成///////////////////////////////
	//スワップチェーンからバックバッファを取得（バックバッファ ＝ レンダーターゲット）
	ComPtr<ID3D11Texture2D> pBackBuffer;
	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"バックバッファの取得に失敗しました", L"エラー", MB_OK);
		//エラー処理
		return hr;
	}


	//レンダーターゲットビューを作成
	hr = pDevice->CreateRenderTargetView(pBackBuffer.Get(), NULL, pRenderTargetView.GetAddressOf());
	if (FAILED(hr))
	{
		MessageBox(NULL, L"レンダーターゲットビューの作成に失敗しました", L"エラー", MB_OK);
		//エラー処理
		return hr;
	}
	//一時的にバックバッファを取得しただけなので解放
	//pBackBuffer->Release();

	//デプスステンシルビュー
	ComPtr<ID3D11Texture2D> m_pDepthStencilTexture;
	D3D11_TEXTURE2D_DESC txDesc;
	ZeroMemory(&txDesc, sizeof(txDesc));
	txDesc.Width = winW;
	txDesc.Height = winH;
	txDesc.MipLevels = 1;
	txDesc.ArraySize = 1;
	txDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	txDesc.SampleDesc.Count = 1;
	txDesc.SampleDesc.Quality = 0;
	txDesc.SampleDesc = scDesc.SampleDesc;
	txDesc.Usage = D3D11_USAGE_DEFAULT;
	txDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	txDesc.CPUAccessFlags = 0;
	txDesc.MiscFlags = 0;
	hr = pDevice->CreateTexture2D(&txDesc, NULL, m_pDepthStencilTexture.GetAddressOf());
	if (FAILED(hr))
		return hr;

	D3D11_DEPTH_STENCIL_VIEW_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(dsDesc));
	dsDesc.Format = txDesc.Format;
	dsDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;
	dsDesc.Texture2D.MipSlice = 0;
	hr = pDevice->CreateDepthStencilView(m_pDepthStencilTexture.Get(), &dsDesc, pDepthStencilView.GetAddressOf());
	if (FAILED(hr))
		return hr;
	pContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), pDepthStencilView.Get());



	//// RenderTarget0へのAlphaブレンド描画設定
	//D3D11_BLEND_DESC BlendState;
	//ZeroMemory(&BlendState, sizeof(D3D11_BLEND_DESC));
	//BlendState.AlphaToCoverageEnable = FALSE;
	//BlendState.IndependentBlendEnable = FALSE;
	//BlendState.RenderTarget[0].BlendEnable = TRUE;
	//BlendState.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	//BlendState.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	//BlendState.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	//BlendState.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	//BlendState.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	//BlendState.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	//BlendState.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	//hr = pDevice->CreateBlendState(&BlendState, pBlendState.GetAddressOf());
	//if (FAILED(hr)) {
	//	MessageBox(NULL, L"ブレンドステートが生成できません", L"エラー", MB_OK);
	//	return hr;
	//}

	//FLOAT blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
	//pContext->OMSetBlendState(pBlendState.Get(), blendFactor, 0xffffffff);


	///////////////////////////ビューポート（描画範囲）設定///////////////////////////////
//レンダリング結果を表示する範囲
	D3D11_VIEWPORT vp;
	vp.Width = (float)winW;	//幅
	vp.Height = (float)winH;//高さ
	vp.MinDepth = 0.0f;	//手前
	vp.MaxDepth = 1.0f;	//奥
	vp.TopLeftX = 0;	//左
	vp.TopLeftY = 0;	//上
	pContext->RSSetViewports(1, &vp);

	//データを画面に描画するための一通りの設定（パイプライン）
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);  // データの入力種類を指定
	//pContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), nullptr);            // 描画先を設定
	

	

	//シェーダー準備
	hr = InitShader();
	if (FAILED(hr))
	{
		return hr;
	}
	return hr;
}

//シェーダー準備
HRESULT Direct3D::InitShader()
{
	HRESULT hr = S_OK;
	// 頂点シェーダの作成（コンパイル）
	ID3DBlob* pCompileVS = nullptr;
	D3DCompileFromFile(L"simple3D.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &pVertexShader);
	if (FAILED(hr))
	{
		//エラー処理
		hr = MessageBox(NULL, L"頂点シェーダーの作成に失敗しました", L"エラー", MB_OK);
		//解放処理
		return hr;
	}

	//頂点インプットレイアウト
	D3D11_INPUT_ELEMENT_DESC layout[] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	hr = pDevice->CreateInputLayout(layout, ARRAYSIZE(layout), pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &pVertexLayout);
	if (FAILED(hr))
	{
		//エラー処理
		MessageBox(NULL, L"頂点インプットレイアウトの作成に失敗しました", L"エラー", MB_OK);
		//解放処理
		return hr;
	}


	pCompileVS->Release();

	// ピクセルシェーダの作成（コンパイル）
	ID3DBlob* pCompilePS = nullptr;
	D3DCompileFromFile(L"simple3D.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	hr = pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, &pPixelShader);
	if (FAILED(hr))
	{
		//エラー処理
		MessageBox(NULL, L"ピクセルシェーダの作成に失敗しました", L"エラー", MB_OK);
		return hr;
	}
	
	pCompilePS->Release();

	//ラスタライザ作成
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_NONE;  //多角形の裏側は描画しない（カリング）
	rdc.FillMode = D3D11_FILL_SOLID; //多角形の内部を塗りつぶす
	rdc.FrontCounterClockwise = FALSE; //反時計回りを表にするかどうか（がfalseなので時計回りが表）
	hr = pDevice->CreateRasterizerState(&rdc, pRasterizerState.GetAddressOf());
	if (FAILED(hr))
	{
		//エラー処理
		MessageBox(NULL, L"ラスタライザの作成に失敗しました", L"エラー", MB_OK);
		return hr;
	}


	//それぞれをデバイスコンテキストにセット
	pContext->VSSetShader(pVertexShader.Get(), NULL, 0);	//頂点シェーダー
	pContext->PSSetShader(pPixelShader.Get(), NULL, 0);	//ピクセルシェーダー
	pContext->IASetInputLayout(pVertexLayout.Get());	//頂点インプットレイアウト

	pContext->RSSetState(pRasterizerState.Get());		//ラスタライザー

	return hr;
}

//描画開始
void Direct3D::BeginDraw()
{
	//背景の色
	float clearColor[4] = { 255/255.0f, 250/255.0f, 205/255.0f, 1.0f };//R,G,B,A

	//画面をクリア
	pContext->ClearRenderTargetView(pRenderTargetView.Get(), clearColor);

	//pContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), pDepthStencilView.Get());
	//pContext->ClearRenderTargetView(pRenderTargetView.Get(), clearColor);

	pContext->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}



//描画終了

void Direct3D::EndDraw()
{
	//スワップ（バックバッファを表に表示する）
	pSwapChain->Present(1, 0);
}



//解放処理
void Direct3D::Release()
{
	//解放処理

	//pRasterizerState->Release();
	//pVertexLayout->Release();
	//pPixelShader->Release();
	//pVertexShader->Release();
	//pRenderTargetView->Release();
	//pSwapChain->Release();
	//pContext->Release();
	//pDevice->Release();
}