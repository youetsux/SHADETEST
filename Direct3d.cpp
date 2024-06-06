#include <d3dcompiler.h>
#include "Direct3D.h"
#include "StringToWideStringConverter.h"

using Microsoft::WRL::ComPtr;


//�ϐ�
namespace Direct3D
{
	ComPtr<ID3D11Device> pDevice;		//�f�o�C�X
	ComPtr<ID3D11DeviceContext> pContext;		//�f�o�C�X�R���e�L�X�g
	ComPtr<IDXGISwapChain> pSwapChain;		//�X���b�v�`�F�C��
	ComPtr<ID3D11RenderTargetView> pRenderTargetView;	//�����_�[�^�[�Q�b�g�r���[
	ComPtr<ID3D11DepthStencilView> pDepthStencilView; //DepthStencilView

	ComPtr<ID3D11VertexShader> pVertexShader = nullptr;	//���_�V�F�[�_�[
	ComPtr<ID3D11PixelShader> pPixelShader = nullptr;		//�s�N�Z���V�F�[�_�[
	ComPtr<ID3D11InputLayout> pVertexLayout = nullptr;	//���_�C���v�b�g���C�A�E�g
	ComPtr<ID3D11RasterizerState> pRasterizerState = nullptr;	//���X�^���C�U�[
	ComPtr<ID3D11BlendState> pBlendState = nullptr;
	
	::EffekseerRendererDX11::RendererRef gRenderer;
	::Effekseer::ManagerRef gManager;
	::Effekseer::EffectRef gEffect;
	::Effekseer::Backend::GraphicsDeviceRef gEFDev;

}

//������
HRESULT Direct3D::Initialize(int winW, int winH, HWND hWnd)
{
	HRESULT hr = S_OK;
	///////////////////////////���낢�돀�����邽�߂̐ݒ�///////////////////////////////
	//���낢��Ȑݒ荀�ڂ��܂Ƃ߂��\����
	DXGI_SWAP_CHAIN_DESC scDesc;

	//�Ƃ肠�����S��0
	ZeroMemory(&scDesc, sizeof(scDesc));

	//�`���̃t�H�[�}�b�g
	scDesc.BufferDesc.Width = winW;		//��ʕ�
	scDesc.BufferDesc.Height = winH;	//��ʍ���
	scDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// ���F�g���邩

	//FPS�i1/60�b��1��j
	scDesc.BufferDesc.RefreshRate.Numerator = 60;
	scDesc.BufferDesc.RefreshRate.Denominator = 1;

	//���̑�
	scDesc.Windowed = TRUE;			//�E�B���h�E���[�h���t���X�N���[����
	scDesc.OutputWindow = hWnd;		//�E�B���h�E�n���h��
	scDesc.BufferCount = 1;			//�o�b�N�o�b�t�@�̖���
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	//�o�b�N�o�b�t�@�̎g��������ʂɕ`�悷�邽�߂�
	scDesc.SampleDesc.Count = 1;		//MSAA�i�A���`�G�C���A�X�j�̐ݒ�
	scDesc.SampleDesc.Quality = 0;		//�@�V
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	scDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	scDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	scDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;


	////////////////��L�ݒ�����ƂɃf�o�C�X�A�R���e�L�X�g�A�X���b�v�`�F�C�����쐬////////////////////////
	D3D_FEATURE_LEVEL level;
	hr = D3D11CreateDeviceAndSwapChain(
		nullptr,				// �ǂ̃r�f�I�A�_�v�^���g�p���邩�H����Ȃ��nullptr��
		D3D_DRIVER_TYPE_HARDWARE,		// �h���C�o�̃^�C�v��n���B�ӂ���HARDWARE
		nullptr,				// ��L��D3D_DRIVER_TYPE_SOFTWARE�ɐݒ肵�Ȃ�������nullptr
		0,					// ���炩�̃t���O���w�肷��B�i�f�o�b�O����D3D11_CREATE_DEVICE_DEBUG�H�j
		nullptr,				// �f�o�C�X�A�R���e�L�X�g�̃��x����ݒ�Bnullptr�ɂ��Ƃ���OK
		0,					// ��̈����Ń��x�������w�肵����
		D3D11_SDK_VERSION,			// SDK�̃o�[�W�����B�K�����̒l
		&scDesc,				// ��ł��낢��ݒ肵���\����
		pSwapChain.GetAddressOf(),				// ������������SwapChain�̃A�h���X���Ԃ��Ă���
		pDevice.GetAddressOf(),				// ������������Device�A�h���X���Ԃ��Ă���
		&level,					// ������������Device�AContext�̃��x�����Ԃ��Ă���
		pContext.GetAddressOf());				// ������������Context�̃A�h���X���Ԃ��Ă���


	if (FAILED(hr))
	{
		//�G���[����
		MessageBox(NULL, L"�f�o�C�X�A�R���e�L�X�g�A�X���b�v�`�F�C���̍쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
		return hr;
	}

	///////////////////////////�����_�[�^�[�Q�b�g�r���[�쐬///////////////////////////////
	//�X���b�v�`�F�[������o�b�N�o�b�t�@���擾�i�o�b�N�o�b�t�@ �� �����_�[�^�[�Q�b�g�j
	ComPtr<ID3D11Texture2D> pBackBuffer;
	hr = pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
	{
		MessageBox(NULL, L"�o�b�N�o�b�t�@�̎擾�Ɏ��s���܂���", L"�G���[", MB_OK);
		//�G���[����
		return hr;
	}


	//�����_�[�^�[�Q�b�g�r���[���쐬
	hr = pDevice->CreateRenderTargetView(pBackBuffer.Get(), NULL, pRenderTargetView.GetAddressOf());
	if (FAILED(hr))
	{
		MessageBox(NULL, L"�����_�[�^�[�Q�b�g�r���[�̍쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
		//�G���[����
		return hr;
	}
	//�ꎞ�I�Ƀo�b�N�o�b�t�@���擾���������Ȃ̂ŉ��
	//pBackBuffer->Release();

	//�f�v�X�X�e���V���r���[
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



	//// RenderTarget0�ւ�Alpha�u�����h�`��ݒ�
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
	//	MessageBox(NULL, L"�u�����h�X�e�[�g�������ł��܂���", L"�G���[", MB_OK);
	//	return hr;
	//}

	//FLOAT blendFactor[4] = { D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO, D3D11_BLEND_ZERO };
	//pContext->OMSetBlendState(pBlendState.Get(), blendFactor, 0xffffffff);


	///////////////////////////�r���[�|�[�g�i�`��͈́j�ݒ�///////////////////////////////
//�����_�����O���ʂ�\������͈�
	D3D11_VIEWPORT vp;
	vp.Width = (float)winW;	//��
	vp.Height = (float)winH;//����
	vp.MinDepth = 0.0f;	//��O
	vp.MaxDepth = 1.0f;	//��
	vp.TopLeftX = 0;	//��
	vp.TopLeftY = 0;	//��
	pContext->RSSetViewports(1, &vp);

	//�f�[�^����ʂɕ`�悷�邽�߂̈�ʂ�̐ݒ�i�p�C�v���C���j
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);  // �f�[�^�̓��͎�ނ��w��
	//pContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), nullptr);            // �`����ݒ�
	

	

	//�V�F�[�_�[����
	hr = InitShader();
	if (FAILED(hr))
	{
		return hr;
	}
	return hr;
}

//�V�F�[�_�[����
HRESULT Direct3D::InitShader()
{
	HRESULT hr = S_OK;
	// ���_�V�F�[�_�̍쐬�i�R���p�C���j
	ID3DBlob* pCompileVS = nullptr;
	D3DCompileFromFile(L"simple3D.hlsl", nullptr, nullptr, "VS", "vs_5_0", NULL, 0, &pCompileVS, NULL);
	hr = pDevice->CreateVertexShader(pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), NULL, &pVertexShader);
	if (FAILED(hr))
	{
		//�G���[����
		hr = MessageBox(NULL, L"���_�V�F�[�_�[�̍쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
		//�������
		return hr;
	}

	//���_�C���v�b�g���C�A�E�g
	D3D11_INPUT_ELEMENT_DESC layout[] = 
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	hr = pDevice->CreateInputLayout(layout, ARRAYSIZE(layout), pCompileVS->GetBufferPointer(), pCompileVS->GetBufferSize(), &pVertexLayout);
	if (FAILED(hr))
	{
		//�G���[����
		MessageBox(NULL, L"���_�C���v�b�g���C�A�E�g�̍쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
		//�������
		return hr;
	}


	pCompileVS->Release();

	// �s�N�Z���V�F�[�_�̍쐬�i�R���p�C���j
	ID3DBlob* pCompilePS = nullptr;
	D3DCompileFromFile(L"simple3D.hlsl", nullptr, nullptr, "PS", "ps_5_0", NULL, 0, &pCompilePS, NULL);
	hr = pDevice->CreatePixelShader(pCompilePS->GetBufferPointer(), pCompilePS->GetBufferSize(), NULL, &pPixelShader);
	if (FAILED(hr))
	{
		//�G���[����
		MessageBox(NULL, L"�s�N�Z���V�F�[�_�̍쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
		return hr;
	}
	
	pCompilePS->Release();

	//���X�^���C�U�쐬
	D3D11_RASTERIZER_DESC rdc = {};
	rdc.CullMode = D3D11_CULL_NONE;  //���p�`�̗����͕`�悵�Ȃ��i�J�����O�j
	rdc.FillMode = D3D11_FILL_SOLID; //���p�`�̓�����h��Ԃ�
	rdc.FrontCounterClockwise = FALSE; //�����v����\�ɂ��邩�ǂ����i��false�Ȃ̂Ŏ��v��肪�\�j
	hr = pDevice->CreateRasterizerState(&rdc, pRasterizerState.GetAddressOf());
	if (FAILED(hr))
	{
		//�G���[����
		MessageBox(NULL, L"���X�^���C�U�̍쐬�Ɏ��s���܂���", L"�G���[", MB_OK);
		return hr;
	}


	//���ꂼ����f�o�C�X�R���e�L�X�g�ɃZ�b�g
	pContext->VSSetShader(pVertexShader.Get(), NULL, 0);	//���_�V�F�[�_�[
	pContext->PSSetShader(pPixelShader.Get(), NULL, 0);	//�s�N�Z���V�F�[�_�[
	pContext->IASetInputLayout(pVertexLayout.Get());	//���_�C���v�b�g���C�A�E�g

	pContext->RSSetState(pRasterizerState.Get());		//���X�^���C�U�[

	return hr;
}

//�`��J�n
void Direct3D::BeginDraw()
{
	//�w�i�̐F
	float clearColor[4] = { 255/255.0f, 250/255.0f, 205/255.0f, 1.0f };//R,G,B,A

	//��ʂ��N���A
	pContext->ClearRenderTargetView(pRenderTargetView.Get(), clearColor);

	//pContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), pDepthStencilView.Get());
	//pContext->ClearRenderTargetView(pRenderTargetView.Get(), clearColor);

	pContext->ClearDepthStencilView(pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}



//�`��I��

void Direct3D::EndDraw()
{
	//�X���b�v�i�o�b�N�o�b�t�@��\�ɕ\������j
	pSwapChain->Present(1, 0);
}



//�������
void Direct3D::Release()
{
	//�������

	//pRasterizerState->Release();
	//pVertexLayout->Release();
	//pPixelShader->Release();
	//pVertexShader->Release();
	//pRenderTargetView->Release();
	//pSwapChain->Release();
	//pContext->Release();
	//pDevice->Release();
}