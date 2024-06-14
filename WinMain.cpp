//インクルード
#include <Windows.h>
#include "Direct3D.h"
#include <string>
#include "StringToWideStringConverter.h"
#include "Quad.h"
#include "EFK.h"
#include "Camera.h"

//リンカ
#pragma comment(lib, "d3d11.lib")

#if _DEBUG
#pragma comment(lib, "Effekseerd.lib" )
#pragma comment(lib, "EffekseerRendererDX11d.lib" )
#else
#pragma comment(lib, "Effekseer.lib" )
#pragma comment(lib, "EffekseerRendererDX11.lib" )
#endif

using StrConv = StringConverter;

	//定数宣言
	const wchar_t* WIN_CLASS_NAME = L"SampleGame";  //ウィンドウクラス名
	//const wchar_t* APP_NAME = L"サンプルゲーム"; //アプリケーション名
	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 600;

//プロトタイプ宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//エントリーポイント
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	//ウィンドウクラス（設計図）を作成
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);             //この構造体のサイズ
	wc.hInstance = hInstance;                   //インスタンスハンドル
	wc.lpszClassName = WIN_CLASS_NAME;            //ウィンドウクラス名
	wc.lpfnWndProc = WndProc;                   //ウィンドウプロシージャ
	wc.style = CS_VREDRAW | CS_HREDRAW;         //スタイル（デフォルト）
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); //アイコン
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);   //小さいアイコン
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);   //マウスカーソル
	wc.lpszMenuName = NULL;                     //メニュー（なし）
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //背景（白）
	RegisterClassEx(&wc); //クラスを登録

	//ウィンドウサイズの計算
//（表示領域をWINDOW_WIDTHxWINDOW_HEIGHTに指定するための計算）
	RECT winRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, FALSE);
	int winW = winRect.right - winRect.left;     //ウィンドウ幅
	int winH = winRect.bottom - winRect.top;     //ウィンドウ高さ

	std::wstring appName = L"スーパー俺ゲーム";
	//LPCWSTR APP_NAME = StringConverter::StringToWide(appName);

	//ウィンドウを作成
	HWND hWnd = CreateWindow(
		WIN_CLASS_NAME,         //ウィンドウクラス名
		appName.c_str(),				//タイトルバーに表示する内容
		WS_OVERLAPPEDWINDOW, //スタイル（普通のウィンドウ）
		CW_USEDEFAULT,       //表示位置左（おまかせ）
		CW_USEDEFAULT,       //表示位置上（おまかせ）
		winW,        //ウィンドウ幅
		winH,       //ウィンドウ高さ
		NULL,                //親ウインドウ（なし）
		NULL,                //メニュー（なし）
		hInstance,           //インスタンス
		NULL                 //パラメータ（なし）
	);
	CoInitializeEx(nullptr, 0);
	//ウィンドウを表示
	ShowWindow(hWnd, nCmdShow);

	
	//Direct3D初期化
	Direct3D::Initialize(winW, winH, hWnd);

	Camera::Initialize({ 0,5,-5 }, { 0,0,0 });

	Quad* q;
	q = new Quad();
	q->Initialize();

	EFK::Init();


	//Direct3D::gManager = ::Effekseer::Manager::Create(8000);
	//// Setup effekseer modules
	//// Effekseerのモジュールをセットアップする
	//Direct3D::gEFDev = ::EffekseerRendererDX11::CreateGraphicsDevice(Direct3D::pDevice.Get(), Direct3D::pContext.Get());
	//// Create a renderer of effects
	//// エフェクトのレンダラーの作成
	//Direct3D::gRenderer = ::EffekseerRendererDX11::Renderer::Create(Direct3D::gEFDev, 8000);
	//
	//// Sprcify rendering modules
	//// 描画モジュールの設定
	//Direct3D::gManager->SetSpriteRenderer(Direct3D::gRenderer->CreateSpriteRenderer());
	//Direct3D::gManager->SetRibbonRenderer(Direct3D::gRenderer->CreateRibbonRenderer());
	//Direct3D::gManager->SetRingRenderer(Direct3D::gRenderer->CreateRingRenderer());
	//Direct3D::gManager->SetTrackRenderer(Direct3D::gRenderer->CreateTrackRenderer());
	//Direct3D::gManager->SetModelRenderer(Direct3D::gRenderer->CreateModelRenderer());
	//// Specify a texture, model, curve and material loader
	//// It can be extended by yourself. It is loaded from a file on now.
	//// テクスチャ、モデル、カーブ、マテリアルローダーの設定する。
	//// ユーザーが独自で拡張できる。現在はファイルから読み込んでいる。
	//Direct3D::gManager->SetTextureLoader(Direct3D::gRenderer->CreateTextureLoader());
	//Direct3D::gManager->SetModelLoader(Direct3D::gRenderer->CreateModelLoader());
	//Direct3D::gManager->SetMaterialLoader(Direct3D::gRenderer->CreateMaterialLoader());
	//Direct3D::gManager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());
	
	EFK::Play(u"magic.efk", 0, 0, 0);

	//Direct3D::gEffect = Effekseer::Effect::Create(Direct3D::gManager, (const EFK_CHAR *)L"magic.efk");



	//メッセージループ（何か起きるのを待つ）
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		//メッセージあり
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//メッセージなし
		else
		{
			//ゲームの処理
			Direct3D::BeginDraw();

			//ここに自前の描画処理を追加していく
			//q->DropShadow();
			//q->Draw();
			
			EFK::Update();


			EFK::Draw();
			//{
			//	// Specify a position of view
			//	// 視点位置を確定
			//	auto viewerPosition = ::Effekseer::Vector3D(0.0f, 2.0f, 2.0f);
			//	// Specify a projection matrix
			//	// 投影行列を設定
			//	::Effekseer::Matrix44 projectionMatrix;
			//	projectionMatrix.PerspectiveFovRH(90.0f / 180.0f * 3.14f, (float)winW/ (float)winH, 1.0f, 500.0f);
			//	
			//	// Specify a camera matrix
			//	// カメラ行列を設定
			//	::Effekseer::Matrix44 cameraMatrix;
			//	cameraMatrix.LookAtRH(viewerPosition, ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f), ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f));
			//	static int32_t time = 0;
			//	Effekseer::Handle efkHandle = 0;
			//	if (time % 120 == 0)
			//	{
			//		// Play an effect
			//		// エフェクトの再生
			//		efkHandle = Direct3D::gManager->Play(Direct3D::gEffect, 0, 0, 0);
			//	}
			//	if (time % 120 == 119)
			//	{
			//		// Stop effects
			//		// エフェクトの停止
			//		Direct3D::gManager->StopEffect(efkHandle);
			//	}
			//	// Move the effect
			//	// エフェクトの移動
			//	Direct3D::gManager->AddLocation(efkHandle, ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f));
			//	// Set layer parameters
			//	// レイヤーパラメータの設定
			//	Effekseer::Manager::LayerParameter layerParameter;
			//	layerParameter.ViewerPosition = viewerPosition;
			//	Direct3D::gManager->SetLayerParameter(0, layerParameter);
			//	// Update the manager
			//	// マネージャーの更新
			//	Effekseer::Manager::UpdateParameter updateParameter;
			//	Direct3D::gManager->Update(updateParameter);
			//	
			//	// Update a time
			//	// 時間を更新する
			//	Direct3D::gRenderer->SetTime(time / 60.0f);
			//	// Specify a projection matrix
			//	// 投影行列を設定
			//	Direct3D::gRenderer->SetProjectionMatrix(projectionMatrix);
			//	// Specify a camera matrix
			//	// カメラ行列を設定
			//	Direct3D::gRenderer->SetCameraMatrix(cameraMatrix);
			//	// Begin to rendering effects
			//	// エフェクトの描画開始処理を行う。
			//	Direct3D::gRenderer->BeginRendering();
			//	// Render effects
			//	// エフェクトの描画を行う。
			//	Effekseer::Manager::DrawParameter drawParameter;
			//	drawParameter.ZNear = 0.0f;
			//	drawParameter.ZFar = 1.0f;
			//	drawParameter.ViewProjectionMatrix = Direct3D::gRenderer->GetCameraProjectionMatrix();
			//	Direct3D::gManager->Draw(drawParameter);
			//	// Finish to rendering effects
			//	// エフェクトの描画終了処理を行う。
			//	Direct3D::gRenderer->EndRendering();
			//	time++;
			//}

			//描画処理
			Direct3D::EndDraw();
		}
		
	}

	Direct3D::Release();
	
	//Direct3D::gEffect->Release();
	////Direct3D::gManager->Release();
	//Direct3D::gRenderer->Release();
	//Direct3D::gEFDev->Release();

	return 0;
}

//ウィンドウプロシージャ（何かあった時によばれる関数）
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);  //プログラム終了
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}