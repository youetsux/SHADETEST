//�C���N���[�h
#include <Windows.h>
#include "Direct3D.h"
#include <string>
#include "StringToWideStringConverter.h"
#include "Quad.h"
#include "EFK.h"
#include "Camera.h"

//�����J
#pragma comment(lib, "d3d11.lib")

#if _DEBUG
#pragma comment(lib, "Effekseerd.lib" )
#pragma comment(lib, "EffekseerRendererDX11d.lib" )
#else
#pragma comment(lib, "Effekseer.lib" )
#pragma comment(lib, "EffekseerRendererDX11.lib" )
#endif

using StrConv = StringConverter;

	//�萔�錾
	const wchar_t* WIN_CLASS_NAME = L"SampleGame";  //�E�B���h�E�N���X��
	//const wchar_t* APP_NAME = L"�T���v���Q�[��"; //�A�v���P�[�V������
	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 600;

//�v���g�^�C�v�錾
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

//�G���g���[�|�C���g
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
	_set_abort_behavior(0, _WRITE_ABORT_MSG);
	//�E�B���h�E�N���X�i�݌v�}�j���쐬
	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);             //���̍\���̂̃T�C�Y
	wc.hInstance = hInstance;                   //�C���X�^���X�n���h��
	wc.lpszClassName = WIN_CLASS_NAME;            //�E�B���h�E�N���X��
	wc.lpfnWndProc = WndProc;                   //�E�B���h�E�v���V�[�W��
	wc.style = CS_VREDRAW | CS_HREDRAW;         //�X�^�C���i�f�t�H���g�j
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); //�A�C�R��
	wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);   //�������A�C�R��
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);   //�}�E�X�J�[�\��
	wc.lpszMenuName = NULL;                     //���j���[�i�Ȃ��j
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //�w�i�i���j
	RegisterClassEx(&wc); //�N���X��o�^

	//�E�B���h�E�T�C�Y�̌v�Z
//�i�\���̈��WINDOW_WIDTHxWINDOW_HEIGHT�Ɏw�肷�邽�߂̌v�Z�j
	RECT winRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, FALSE);
	int winW = winRect.right - winRect.left;     //�E�B���h�E��
	int winH = winRect.bottom - winRect.top;     //�E�B���h�E����

	std::wstring appName = L"�X�[�p�[���Q�[��";
	//LPCWSTR APP_NAME = StringConverter::StringToWide(appName);

	//�E�B���h�E���쐬
	HWND hWnd = CreateWindow(
		WIN_CLASS_NAME,         //�E�B���h�E�N���X��
		appName.c_str(),				//�^�C�g���o�[�ɕ\��������e
		WS_OVERLAPPEDWINDOW, //�X�^�C���i���ʂ̃E�B���h�E�j
		CW_USEDEFAULT,       //�\���ʒu���i���܂����j
		CW_USEDEFAULT,       //�\���ʒu��i���܂����j
		winW,        //�E�B���h�E��
		winH,       //�E�B���h�E����
		NULL,                //�e�E�C���h�E�i�Ȃ��j
		NULL,                //���j���[�i�Ȃ��j
		hInstance,           //�C���X�^���X
		NULL                 //�p�����[�^�i�Ȃ��j
	);
	CoInitializeEx(nullptr, 0);
	//�E�B���h�E��\��
	ShowWindow(hWnd, nCmdShow);

	
	//Direct3D������
	Direct3D::Initialize(winW, winH, hWnd);

	Camera::Initialize({ 0, 2, 3}, { 0,0,0 });
	//Camera::Initialize();

	Quad* q;
	q = new Quad();
	q->Initialize();

	EFK::Init();

	EFK::Play(u"mahoujin.efkefc", 0, 0, 0);
	//���b�Z�[�W���[�v�i�����N����̂�҂j
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT)
	{
		//���b�Z�[�W����
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//���b�Z�[�W�Ȃ�
		else
		{
			//�Q�[���̏���
			Direct3D::BeginDraw();

			//�����Ɏ��O�̕`�揈����ǉ����Ă���
			//q->DropShadow();
			q->Draw();
			
			EFK::Update();

			EFK::Draw();


			//�`�揈��
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

//�E�B���h�E�v���V�[�W���i�������������ɂ�΂��֐��j
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);  //�v���O�����I��
		return 0;
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}