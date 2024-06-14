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
	_set_abort_behavior(0, _WRITE_ABORT_MSG);
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

	Camera::Initialize({ 0, 2, 3}, { 0,0,0 });
	//Camera::Initialize();

	Quad* q;
	q = new Quad();
	q->Initialize();

	EFK::Init();

	EFK::Play(u"mahoujin.efkefc", 0, 0, 0);
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
			q->Draw();
			
			EFK::Update();

			EFK::Draw();


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