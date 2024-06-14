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

	Camera::Initialize({ 0,5,-5 }, { 0,0,0 });

	Quad* q;
	q = new Quad();
	q->Initialize();

	EFK::Init();


	//Direct3D::gManager = ::Effekseer::Manager::Create(8000);
	//// Setup effekseer modules
	//// Effekseer�̃��W���[�����Z�b�g�A�b�v����
	//Direct3D::gEFDev = ::EffekseerRendererDX11::CreateGraphicsDevice(Direct3D::pDevice.Get(), Direct3D::pContext.Get());
	//// Create a renderer of effects
	//// �G�t�F�N�g�̃����_���[�̍쐬
	//Direct3D::gRenderer = ::EffekseerRendererDX11::Renderer::Create(Direct3D::gEFDev, 8000);
	//
	//// Sprcify rendering modules
	//// �`�惂�W���[���̐ݒ�
	//Direct3D::gManager->SetSpriteRenderer(Direct3D::gRenderer->CreateSpriteRenderer());
	//Direct3D::gManager->SetRibbonRenderer(Direct3D::gRenderer->CreateRibbonRenderer());
	//Direct3D::gManager->SetRingRenderer(Direct3D::gRenderer->CreateRingRenderer());
	//Direct3D::gManager->SetTrackRenderer(Direct3D::gRenderer->CreateTrackRenderer());
	//Direct3D::gManager->SetModelRenderer(Direct3D::gRenderer->CreateModelRenderer());
	//// Specify a texture, model, curve and material loader
	//// It can be extended by yourself. It is loaded from a file on now.
	//// �e�N�X�`���A���f���A�J�[�u�A�}�e���A�����[�_�[�̐ݒ肷��B
	//// ���[�U�[���Ǝ��Ŋg���ł���B���݂̓t�@�C������ǂݍ���ł���B
	//Direct3D::gManager->SetTextureLoader(Direct3D::gRenderer->CreateTextureLoader());
	//Direct3D::gManager->SetModelLoader(Direct3D::gRenderer->CreateModelLoader());
	//Direct3D::gManager->SetMaterialLoader(Direct3D::gRenderer->CreateMaterialLoader());
	//Direct3D::gManager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());
	
	EFK::Play(u"magic.efk", 0, 0, 0);

	//Direct3D::gEffect = Effekseer::Effect::Create(Direct3D::gManager, (const EFK_CHAR *)L"magic.efk");



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
			//q->Draw();
			
			EFK::Update();


			EFK::Draw();
			//{
			//	// Specify a position of view
			//	// ���_�ʒu���m��
			//	auto viewerPosition = ::Effekseer::Vector3D(0.0f, 2.0f, 2.0f);
			//	// Specify a projection matrix
			//	// ���e�s���ݒ�
			//	::Effekseer::Matrix44 projectionMatrix;
			//	projectionMatrix.PerspectiveFovRH(90.0f / 180.0f * 3.14f, (float)winW/ (float)winH, 1.0f, 500.0f);
			//	
			//	// Specify a camera matrix
			//	// �J�����s���ݒ�
			//	::Effekseer::Matrix44 cameraMatrix;
			//	cameraMatrix.LookAtRH(viewerPosition, ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f), ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f));
			//	static int32_t time = 0;
			//	Effekseer::Handle efkHandle = 0;
			//	if (time % 120 == 0)
			//	{
			//		// Play an effect
			//		// �G�t�F�N�g�̍Đ�
			//		efkHandle = Direct3D::gManager->Play(Direct3D::gEffect, 0, 0, 0);
			//	}
			//	if (time % 120 == 119)
			//	{
			//		// Stop effects
			//		// �G�t�F�N�g�̒�~
			//		Direct3D::gManager->StopEffect(efkHandle);
			//	}
			//	// Move the effect
			//	// �G�t�F�N�g�̈ړ�
			//	Direct3D::gManager->AddLocation(efkHandle, ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f));
			//	// Set layer parameters
			//	// ���C���[�p�����[�^�̐ݒ�
			//	Effekseer::Manager::LayerParameter layerParameter;
			//	layerParameter.ViewerPosition = viewerPosition;
			//	Direct3D::gManager->SetLayerParameter(0, layerParameter);
			//	// Update the manager
			//	// �}�l�[�W���[�̍X�V
			//	Effekseer::Manager::UpdateParameter updateParameter;
			//	Direct3D::gManager->Update(updateParameter);
			//	
			//	// Update a time
			//	// ���Ԃ��X�V����
			//	Direct3D::gRenderer->SetTime(time / 60.0f);
			//	// Specify a projection matrix
			//	// ���e�s���ݒ�
			//	Direct3D::gRenderer->SetProjectionMatrix(projectionMatrix);
			//	// Specify a camera matrix
			//	// �J�����s���ݒ�
			//	Direct3D::gRenderer->SetCameraMatrix(cameraMatrix);
			//	// Begin to rendering effects
			//	// �G�t�F�N�g�̕`��J�n�������s���B
			//	Direct3D::gRenderer->BeginRendering();
			//	// Render effects
			//	// �G�t�F�N�g�̕`����s���B
			//	Effekseer::Manager::DrawParameter drawParameter;
			//	drawParameter.ZNear = 0.0f;
			//	drawParameter.ZFar = 1.0f;
			//	drawParameter.ViewProjectionMatrix = Direct3D::gRenderer->GetCameraProjectionMatrix();
			//	Direct3D::gManager->Draw(drawParameter);
			//	// Finish to rendering effects
			//	// �G�t�F�N�g�̕`��I���������s���B
			//	Direct3D::gRenderer->EndRendering();
			//	time++;
			//}

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