#pragma once
#include <string>
#include <DirectXMath.h>
#include <Effekseer.h>




namespace EFK
{
	void Init();
	void Release();
	void Update();
	void Draw();
	Effekseer::Handle Play(std::wstring path, float x, float y, float z);
	Effekseer::Handle Stop(Effekseer::Handle h);
	Effekseer::Handle Play(std::wstring path, DirectX::XMFLOAT3 pos) { return Play(path.c_str(), pos.x, pos.y, pos.z); }
}