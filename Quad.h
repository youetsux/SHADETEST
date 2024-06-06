#pragma once
#include "Direct3D.h"
#include <DirectXMath.h>
using namespace DirectX;

typedef struct _vertexPositionColor
{
	XMFLOAT3 pos;
	XMFLOAT4 color;
} VertexPositionColor;

//コンスタントバッファー
struct CONSTANT_BUFFER
{
	XMMATRIX	matWVP;
	int			isShadow;
};

class Quad
{
	ComPtr<ID3D11Buffer> pVertexBuffer_;	//頂点バッファ
	ComPtr<ID3D11Buffer> pIndexBuffer_;    //インデックスバッファ
	ComPtr<ID3D11Buffer> pConstantBuffer_;	//コンスタントバッファ
public:
	Quad();
	~Quad();
	void Initialize();
	void Draw();
	void DropShadow();
	void Release();
};

