#include "EFK.h"
#include "Direct3D.h"
#include <EffekseerRendererDX11.h>
#include <map>


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
	std::map<const char16_t*, EffectRef> gEffectList;
	float gEFKTimer = 0;
	Matrix44 CnvMat(DirectX::XMMATRIX mat);
}


void Efk::Init()
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

void Efk::Release()
{
}

void Efk::Update()
{
	float tick = 1 / 60.0f;
	EFK::gRenderer->SetTime(EFK::gEFKTimer += tick);
	Manager::UpdateParameter updateParameter;
	EFK::gManager->Update(updateParameter);
}

void Efk::Draw()
{
	g_efkRender->SetProjectionMatrix(GetMat(CameraBase::GetPrimary()->GetProj()));
	g_efkRender->SetCameraMatrix(GetMat(CameraBase::GetPrimary()->GetView()));

	g_efkRender->BeginRendering();

	// Render effects
	// エフェクトの描画を行う。
	Effekseer::Manager::DrawParameter drawParameter;
	drawParameter.ZNear = 0.0f;
	drawParameter.ZFar = 1;
	drawParameter.ViewProjectionMatrix = g_efkRender->GetCameraProjectionMatrix();
	g_efkManager->Draw(drawParameter);

	// Finish to rendering effects
	// エフェクトの描画終了処理を行う。
	g_efkRender->EndRendering();
}

Effekseer::Handle Efk::Play(const char16_t* path, float x, float y, float z)
{
	return Effekseer::Handle();
}

Effekseer::Handle Efk::Stop(Effekseer::Handle h)
{
	return Effekseer::Handle();
}


Matrix44 EFK::CnvMat(DirectX::XMMATRIX mat)
{
	return Matrix44();
}