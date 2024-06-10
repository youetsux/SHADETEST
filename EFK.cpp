#include "EFK.h"
#include <EffekseerRendererDX11.h>

#include "Direct3D.h"
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
	std::map<std::wstring, EffectRef> gEffectList;
	float gEFKTimer = 0;
	Matrix44 CnvMat(DirectX::XMFLOAT4X4 mat);
}


void EFK::Init()
{
	gManager = Manager::Create(1024);
	auto grpDevice = EffekseerRendererDX11::CreateGraphicsDevice(Direct3D::pDevice.Get(), Direct3D::pContext.Get());
	gRenderer = EffekseerRendererDX11::Renderer::Create(grpDevice, 1024);
	gManager->SetSpriteRenderer(gRenderer->CreateSpriteRenderer());
	gManager->SetRibbonRenderer(gRenderer->CreateRibbonRenderer());
	gManager->SetRingRenderer(gRenderer->CreateRingRenderer());
	gManager->SetTrackRenderer(gRenderer->CreateTrackRenderer());
	gManager->SetModelRenderer(gRenderer->CreateModelRenderer());

	// Specify a texture, model, curve and material loader
	// It can be extended by yourself. It is loaded from a file on now.
	// �e�N�X�`���A���f���A�J�[�u�A�}�e���A�����[�_�[�̐ݒ肷��B
	// ���[�U�[���Ǝ��Ŋg���ł���B���݂̓t�@�C������ǂݍ���ł���B
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
	gRenderer->SetTime(gEFKTimer += tick);
	
	Manager::UpdateParameter updateParameter;
	
	gManager->Update(updateParameter);
}

void EFK::Draw()
{
	auto viewerPosition = ::Effekseer::Vector3D(0.0f, 2.0f, 2.0f);
	Effekseer::Matrix44 projectionMatrix;
	projectionMatrix.PerspectiveFovRH(90.0f / 180.0f * 3.14f, (float)800 / (float)600, 0.1f, 500.0f);

	// Specify a camera matrix
	// �J�����s���ݒ�
	Effekseer::Matrix44 cameraMatrix;
	cameraMatrix.LookAtRH(viewerPosition, ::Effekseer::Vector3D(0.0f, 0.0f, 0.0f), ::Effekseer::Vector3D(0.0f, 1.0f, 0.0f));
	gRenderer->SetProjectionMatrix(projectionMatrix);
	gRenderer->SetCameraMatrix(cameraMatrix);
	
	Effekseer::RenderMode mode;
	
	gRenderer->SetRenderMode()

	gRenderer->BeginRendering();

	// Render effects
// �G�t�F�N�g�̕`����s���B
	Effekseer::Manager::DrawParameter drawParameter;
	drawParameter.ZNear = 0.0f;
	drawParameter.ZFar = 1;
	drawParameter.ViewProjectionMatrix = gRenderer->GetCameraProjectionMatrix();
	gManager->Draw(drawParameter);

	// Finish to rendering effects
	// �G�t�F�N�g�̕`��I���������s���B
	gRenderer->EndRendering();
}

Effekseer::Handle EFK::Play(std::wstring path, float x, float y, float z)
{
	if (gEffectList.count(path) == 0)
	{
		gEffectList[path] = Effect::Create(gManager, (const EFK_CHAR *)path.c_str());
	}
	auto ret = gManager->Play(gEffectList[path], x, y, z);
	auto setting = gManager->GetSetting();
	
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