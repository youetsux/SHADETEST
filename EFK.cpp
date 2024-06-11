#include "EFK.h"
#include "Direct3D.h"
#include <EffekseerRendererDX11.h>
#include <map>
#include "Camera.h"


#ifdef _DEBUG
#pragma comment(lib, "EffekseerRendererDX11d.lib")
#pragma comment(lib, "Effekseerd.lib")
#else
#pragma comment(lib, "EffekseerRendererDX11.lib")
#pragma comment(lib, "Effekseer.lib")

#endif

using namespace Effekseer;

namespace EFK
{
	ManagerRef gManager;
	EffekseerRendererDX11::RendererRef gRenderer;
	std::map<std::wstring, EffectRef> gEffectList;
	float gEFKTimer = 0;
	Matrix44 CnvMat(DirectX::XMFLOAT4X4 mat);
	DirectX::XMMATRIX GetProjMat(bool transpose);
	DirectX::XMMATRIX GetViewMat(bool transpose);
}


void EFK::Init()
{
	EFK::gManager = Manager::Create(1024);
	auto grpDevice = EffekseerRendererDX11::CreateGraphicsDevice(Direct3D::pDevice.Get(), Direct3D::pContext.Get());
	EFK::gRenderer = EffekseerRendererDX11::Renderer::Create(grpDevice, 1024);
	EFK::gManager->SetSpriteRenderer(EFK::gRenderer->CreateSpriteRenderer());
	EFK::gManager->SetRibbonRenderer(EFK::gRenderer->CreateRibbonRenderer());
	EFK::gManager->SetRingRenderer(EFK::gRenderer->CreateRingRenderer());
	EFK::gManager->SetTrackRenderer(EFK::gRenderer->CreateTrackRenderer());
	EFK::gManager->SetModelRenderer(EFK::gRenderer->CreateModelRenderer());

	// Specify a texture, model, curve and material loader
	// It can be extended by yourself. It is loaded from a file on now.
	// テクスチャ、モデル、カーブ、マテリアルローダーの設定する。
	// ユーザーが独自で拡張できる。現在はファイルから読み込んでいる。
	EFK::gManager->SetTextureLoader(EFK::gRenderer->CreateTextureLoader());
	EFK::gManager->SetModelLoader(EFK::gRenderer->CreateModelLoader());
	EFK::gManager->SetMaterialLoader(EFK::gRenderer->CreateMaterialLoader());
	EFK::gManager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());
}

void EFK::Release()
{
}

void EFK::Update()
{
	float tick = 1 / 60.0f;
	EFK::gRenderer->SetTime(EFK::gEFKTimer += tick);
	Manager::UpdateParameter updateParameter;
	EFK::gManager->Update(updateParameter);
}

void EFK::Draw()
{
	XMFLOAT4X4 proj;
	XMStoreFloat4x4(&proj, Camera::GetProjectionMatrix());
	XMFLOAT4X4 view;
	XMStoreFloat4x4(&view, Camera::GetViewMatrix());

	gRenderer->SetProjectionMatrix(CnvMat(proj));
	gRenderer->SetCameraMatrix(CnvMat(view));

	gRenderer->BeginRendering();

	// Render effects
	// エフェクトの描画を行う。
	Effekseer::Manager::DrawParameter drawParameter;
	drawParameter.ZNear = 0.0f;
	drawParameter.ZFar = 1;
	drawParameter.ViewProjectionMatrix = gRenderer->GetCameraProjectionMatrix();
	gManager->Draw(drawParameter);

	// Finish to rendering effects
	// エフェクトの描画終了処理を行う。
	gRenderer->EndRendering();

	//auto viewerPosition = ::Effekseer::Vector3D(0.0f, 2.0f, 2.0f);
	//Effekseer::Matrix44 projectionMatrix;
	//projectionMatrix.PerspectiveFovRH(90.0f / 180.0f * 3.14f, (float)800 / (float)600, 1.0f, 500.0f);
	//// Specify a camera matrix
	//// カメラ行列を設定
	//Effekseer::Matrix44 cameraMatrix;
	//cameraMatrix.LookAtRH(viewerPosition, ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f), ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f));
	//gRenderer->SetProjectionMatrix(projectionMatrix);
	//gRenderer->SetCameraMatrix(cameraMatrix);
	//gRenderer->BeginRendering();
	//// Render effects
	//// エフェクトの描画を行う。
	//Effekseer::Manager::DrawParameter drawParameter;
	//drawParameter.ZNear = 0.0f;
	//drawParameter.ZFar = 1;
	//drawParameter.ViewProjectionMatrix = gRenderer->GetCameraProjectionMatrix();
	//gManager->Draw(drawParameter);
	//// Finish to rendering effects
	//// エフェクトの描画終了処理を行う。
	//gRenderer->EndRendering();
}

Effekseer::Handle EFK::Play(std::wstring path, float x, float y, float z)
{
	if (EFK::gEffectList.count(path) == 0)
	{
		EFK::gEffectList[path] = Effect::Create(EFK::gManager, (const EFK_CHAR *)path.c_str());
	}
	auto ret = EFK::gManager->Play(EFK::gEffectList[path], x, y, z);
	return ret;
}

Effekseer::Handle EFK::Stop(Effekseer::Handle h)
{
	gManager->StopEffect(h);
	return -1;
}


Matrix44 EFK::CnvMat(DirectX::XMFLOAT4X4 mat)
{
	Matrix44 out;

	
	out.Values[0][0] = mat._11;
	out.Values[1][0] = mat._12;
	out.Values[2][0] = mat._13;
	out.Values[3][0] = mat._14;
	out.Values[0][1] = mat._21;
	out.Values[1][1] = mat._22;
	out.Values[2][1] = mat._23;
	out.Values[3][1] = mat._24;
	out.Values[0][2] = mat._31;
	out.Values[1][2] = mat._32;
	out.Values[2][2] = mat._33;
	out.Values[3][2] = mat._34;
	out.Values[0][3] = mat._41;
	out.Values[1][3] = mat._42;
	out.Values[2][3] = mat._43;
	out.Values[3][3] = mat._44;
	return out;
}

DirectX::XMMATRIX EFK::GetProjMat(bool transpose)
{
	DirectX::XMMATRIX mat = Camera::GetProjectionMatrix();
	if (transpose)
		mat = DirectX::XMMatrixTranspose(mat);
	return mat;
}

DirectX::XMMATRIX EFK::GetViewMat(bool transpose)
{
	
	DirectX::XMMATRIX mat = Camera::GetViewMatrix();
	if (transpose)
		mat = DirectX::XMMatrixTranspose(mat);
	return mat;
}
