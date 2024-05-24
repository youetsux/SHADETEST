#include "Quad.h"



Quad::Quad()
{
}

Quad::~Quad()
{
}

void Quad::Initialize()
{
	// 頂点情報
	VertexPositionColor vertices[] =
	{
		{{ -0.5f,  0.5f, -0.5f }, {1,   0,   0, 1}},
		{{  0.5f,  0.5f, -0.5f }, {1,   0,   0, 1}},
		{{ -0.5f, -0.5f, -0.5f }, {1,   0,   0, 1}},
		{{  0.5f, -0.5f, -0.5f }, {1,   0,   0, 1}},

		{{ -0.5f,  0.5f,  0.5f }, {0,   1,   0, 1}},
		{{ -0.5f, -0.5f,  0.5f }, {0,   1,   0, 1}},
		{{  0.5f,  0.5f,  0.5f }, {0,   1,   0, 1}},
		{{  0.5f, -0.5f,  0.5f }, {0,   1,   0, 1}},

		{{ -0.5f,  0.5f,  0.5f }, {0,   0,   1, 1}},
		{{ -0.5f,  0.5f, -0.5f }, {0,   0,   1, 1}},
		{{ -0.5f, -0.5f,  0.5f }, {0,   0,   1, 1}},
		{{ -0.5f, -0.5f, -0.5f }, {0,   0,   1, 1}},

		{{  0.5f,  0.5f,  0.5f }, {1,   1,   0, 1}},
		{{  0.5f, -0.5f,  0.5f }, {1,   1,   0, 1}},
		{{  0.5f,  0.5f, -0.5f }, {1,   1,   0, 1}},
		{{  0.5f, -0.5f, -0.5f }, {1,   1,   0, 1}},

		{{ -0.5f,  0.5f,  0.5f }, {1,   0,   1, 1}},
		{{  0.5f,  0.5f,  0.5f }, {1,   0,   1, 1}},
		{{ -0.5f,  0.5f, -0.5f }, {1,   0,   1, 1}},
		{{  0.5f,  0.5f, -0.5f }, {1,   0,   1, 1}},

		{{ -0.5f, -0.5f,  0.5f },{0,   1,   1, 1}},
		{{ -0.5f, -0.5f, -0.5f },{0,   1,   1, 1}},
		{{  0.5f, -0.5f,  0.5f },{0,   1,   1, 1}},
		{{  0.5f, -0.5f, -0.5f },{0,   1,   1, 1}}
	};

	//インデックス情報
	int index[] = {
	 0,  2,  1,     3,  1,  2,
	 4,  6,  5,     7,  5,  6,
	 8,  10, 9,    11, 9,  10,
	12, 14, 13,    15, 13, 14,
	16, 18, 17,    19, 17, 18,
	20, 22, 21,    23, 21, 22, };

	// 頂点データ用バッファの設定
	D3D11_BUFFER_DESC bd_vertex;
	bd_vertex.ByteWidth = sizeof(vertices);
	bd_vertex.Usage = D3D11_USAGE_DEFAULT;
	bd_vertex.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd_vertex.CPUAccessFlags = 0;
	bd_vertex.MiscFlags = 0;
	bd_vertex.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA data_vertex;
	data_vertex.pSysMem = vertices;
	Direct3D::pDevice->CreateBuffer(&bd_vertex, &data_vertex, &pVertexBuffer_);

	// インデックスバッファを生成する
	D3D11_BUFFER_DESC   bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(index);
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = index;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;
	Direct3D::pDevice->CreateBuffer(&bd, &InitData, &pIndexBuffer_);


	//コンスタントバッファ作成
	D3D11_BUFFER_DESC cb;
	cb.ByteWidth = sizeof(CONSTANT_BUFFER);
	cb.Usage = D3D11_USAGE_DYNAMIC;
	cb.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb.MiscFlags = 0;
	cb.StructureByteStride = 0;

	// コンスタントバッファの作成
	Direct3D::pDevice->CreateBuffer(&cb, nullptr, &pConstantBuffer_);
}

void Quad::Draw()
{
	//コンスタントバッファに渡す情報
	XMVECTOR position = { 3, 3, -5, 0 };	//カメラの位置
	XMVECTOR target = { 0, 0, 0, 0 };	//カメラの焦点
	XMMATRIX view = XMMatrixLookAtLH(position, target, XMVectorSet(0, 1, 0, 0));	//ビュー行列
	XMMATRIX proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, 800.0f / 600.0f, 0.1f, 100.0f);//射影行列

	CONSTANT_BUFFER cb;
	cb.matWVP = XMMatrixTranspose(view * proj);

	D3D11_MAPPED_SUBRESOURCE pdata;
	Direct3D::pContext->Map(pConstantBuffer_, 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPUからのデータアクセスを止める
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));	// データを値を送る
	Direct3D::pContext->Unmap(pConstantBuffer_, 0);	//再開

	//頂点バッファ
	UINT stride = sizeof(VertexPositionColor);
	UINT offset = 0;
	Direct3D::pContext->IASetVertexBuffers(0, 1, &pVertexBuffer_, &stride, &offset);

	// インデックスバッファーをセット
	stride = sizeof(int);
	offset = 0;
	Direct3D::pContext->IASetIndexBuffer(pIndexBuffer_, DXGI_FORMAT_R32_UINT, 0);

	//コンスタントバッファ
	Direct3D::pContext->VSSetConstantBuffers(0, 1, &pConstantBuffer_);	//頂点シェーダー用	
	Direct3D::pContext->PSSetConstantBuffers(0, 1, &pConstantBuffer_);	//ピクセルシェーダー用

	Direct3D::pContext->DrawIndexed(24, 0, 0);
}

void Quad::Release()
{
}
