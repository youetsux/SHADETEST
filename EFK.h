#pragma once
#include <string>
#include <DirectXMath.h>
#include <Effekseer.h>
#include <EffekseerRendererDX11.h>
#include <filesystem>

namespace fs = std::filesystem;

namespace EFK
{
	extern Effekseer::ManagerRef gManager;
	extern EffekseerRendererDX11::RendererRef gRenderer;
	void Init();
	void Release();
	void Update();
	void Draw();
	//extern Effekseer::ManagerRef gManager;
	extern std::map<fs::path, Effekseer::EffectRef> gEffectList;
	//extern EffekseerRendererDX11::RendererRef gRenderer;
	Effekseer::Handle Play(fs::path _path, float x, float y, float z);
	Effekseer::Handle Stop(Effekseer::Handle h);
	//Effekseer::Handle Play(std::wstring path, DirectX::XMFLOAT3 pos) { return Play(path.c_str(), pos.x, pos.y, pos.z); }
}