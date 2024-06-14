#include "EFK.h"
#include "Direct3D.h"
#include <EffekseerRendererDX11.h>
#include <map>
#include <filesystem>
#include "Camera.h"


#ifdef _DEBUG
#pragma comment(lib, "EffekseerRendererDX11d.lib")
#pragma comment(lib, "Effekseerd.lib")
#else
#pragma comment(lib, "EffekseerRendererDX11.lib")
#pragma comment(lib, "Effekseer.lib")

#endif
namespace fs = std::filesystem;
using namespace Effekseer;

namespace EFK
{
	ManagerRef gManager;
	EffekseerRendererDX11::RendererRef gRenderer;
	std::map<fs::path, EffectRef> gEffectList;
	float gEFKTimer = 0;
	Matrix44 CnvMat(DirectX::XMFLOAT4X4 mat);
	DirectX::XMFLOAT4X4 GetProjMat(bool transpose);
	DirectX::XMFLOAT4X4 GetViewMat(bool transpose);
}


void EFK::Init()
{
	gManager = Manager::Create(8000);

	gEFKTimer = 0;

	::Effekseer::Backend::GraphicsDeviceRef grpDevice;
	grpDevice = EffekseerRendererDX11::CreateGraphicsDevice(Direct3D::pDevice.Get(), Direct3D::pContext.Get());
	gRenderer = EffekseerRendererDX11::Renderer::Create(grpDevice, 8000);

	gManager->SetCoordinateSystem(CoordinateSystem::LH);
	gManager->SetSpriteRenderer(gRenderer->CreateSpriteRenderer());
	gManager->SetRibbonRenderer(gRenderer->CreateRibbonRenderer());
	gManager->SetRingRenderer(gRenderer->CreateRingRenderer());
	gManager->SetTrackRenderer(gRenderer->CreateTrackRenderer());
	gManager->SetModelRenderer(gRenderer->CreateModelRenderer());

	// Specify a texture, model, curve and material loader
	// It can be extended by yourself. It is loaded from a file on now.
	// テクスチャ、モデル、カーブ、マテリアルローダーの設定する。
	// ユーザーが独自で拡張でき。現在はファイルから読み込んでいる。
	gManager->SetTextureLoader(gRenderer->CreateTextureLoader());
	gManager->SetModelLoader(gRenderer->CreateModelLoader());
	gManager->SetMaterialLoader(gRenderer->CreateMaterialLoader());
	gManager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());
}

void EFK::Release()
{
}

void EFK::Update()
{
	float tick = 1 / 60.0f;
	static int timer = 0;
	Effekseer::Handle efkHandle = 0;
	EFK::gRenderer->SetTime(gEFKTimer += tick);

	if (timer%120 == 0 )
	{
		Effekseer::EffectRef ref = gEffectList["mahoujin.efkefc"];
		efkHandle = gManager->Play(ref, 0, 0, 0);
	}
	if (timer % 120 == 119)
	{
		gManager->StopEffect(efkHandle);
	}
	

	Manager::UpdateParameter updateParameter;
	gManager->Update(updateParameter);
	
	timer++;
}

void EFK::Draw()
{

	gRenderer->SetProjectionMatrix(CnvMat(GetProjMat(true)));
	gRenderer->SetCameraMatrix(CnvMat(GetViewMat(true)));


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
}


Effekseer::Handle EFK::Play(fs::path _path, float x, float y, float z)
{
	
	if (!fs::exists(_path))
	{
		return -1;
	}

	if (EFK::gEffectList.count(_path) == 0)
	{
		EFK::gEffectList[_path] = Effect::Create(EFK::gManager, _path.u16string().c_str());
	}
	auto ret = EFK::gManager->Play(EFK::gEffectList[_path], x, y, z);
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

DirectX::XMFLOAT4X4 EFK::GetProjMat(bool transpose)
{

	DirectX::XMMATRIX mat = DirectX::XMMatrixPerspectiveFovLH(Camera::DefCam.fov_,
		Camera::DefCam.aspect_,
		Camera::DefCam.near_, Camera::DefCam.far_);
	if (transpose)
		mat = DirectX::XMMatrixTranspose(mat);
	DirectX::XMFLOAT4X4 fmat;
	DirectX::XMStoreFloat4x4(&fmat, mat);
	return fmat;
}


DirectX::XMFLOAT4X4  EFK::GetViewMat(bool transpose)
{
	DirectX::XMMATRIX mat = DirectX::XMMatrixLookAtLH(
		Camera::DefCam.position_, Camera::DefCam.target_, Camera::DefCam.upvector_
	);
	if (transpose)
		mat = DirectX::XMMatrixTranspose(mat);
	DirectX::XMFLOAT4X4 fmat;
	DirectX::XMStoreFloat4x4(&fmat, mat);
	return fmat;
}
