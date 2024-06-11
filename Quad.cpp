#include "Quad.h"
#include "Camera.h"


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
	 0,  1,  2,     3,  2,  1,
	 4,  5,  6,     7,  6,  5,
	 8,  9, 10,    11, 10,  9,
	12, 13, 14,    15, 14, 13,
	16, 18, 17,    19, 18, 17,
	20, 21, 22,    23, 22, 21, };

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
	//XMVECTOR position = { 0, 5, -5, 0 };	//カメラの位置
	//XMVECTOR target = { 0, 0, 0, 0 };	//カメラの焦点
	//XMMATRIX view = XMMatrixLookAtLH(position, target, XMVectorSet(0, 1, 0, 0));	//ビュー行列
	//XMMATRIX proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, 800.0f / 600.0f, 0.1f, 100.0f);//射影行列

	static float angleRadians = 0;
	const auto DELTA = DirectX::XMConvertToRadians(0.1f);
	angleRadians += DELTA;
	// XMMATRIX型の値を作る！
	//auto m = DirectX::XMMatrixIdentity();
	auto m = DirectX::XMMatrixRotationY(angleRadians);

	// XMMATRIX型からXMFloat4x4型にStoreする
	//DirectX::XMStoreFloat4x4(&m_constant->Buffer.Model, m);

	CONSTANT_BUFFER cb;
	cb.matWVP = XMMatrixTranspose(m * Camera::GetViewMatrix() * Camera::GetProjectionMatrix());
	cb.isShadow = 0;

	D3D11_MAPPED_SUBRESOURCE pdata;
	Direct3D::pContext->Map(pConstantBuffer_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPUからのデータアクセスを止める
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));	// データを値を送る
	Direct3D::pContext->Unmap(pConstantBuffer_.Get(), 0);	//再開

	//頂点バッファ
	UINT stride = sizeof(VertexPositionColor);
	UINT offset = 0;
	Direct3D::pContext->IASetVertexBuffers(0, 1, pVertexBuffer_.GetAddressOf(), &stride, &offset);

	// インデックスバッファーをセット
	stride = sizeof(int);
	offset = 0;
	Direct3D::pContext->IASetIndexBuffer(pIndexBuffer_.Get(), DXGI_FORMAT_R32_UINT, 0);

	//コンスタントバッファ
	Direct3D::pContext->VSSetConstantBuffers(0, 1, pConstantBuffer_.GetAddressOf());	//頂点シェーダー用	
	Direct3D::pContext->PSSetConstantBuffers(0, 1, pConstantBuffer_.GetAddressOf());	//ピクセルシェーダー用

	Direct3D::pContext->DrawIndexed(36, 0, 0);
}

void Quad::DropShadow()
{
	//コンスタントバッファに渡す情報
	XMVECTOR position = { 0, 5, -5, 0 };	//カメラの位置
	XMVECTOR target = { 0, 0, 0, 0 };	//カメラの焦点
	XMMATRIX view = XMMatrixLookAtLH(position, target, XMVectorSet(0, 1, 0, 0));	//ビュー行列
	XMMATRIX proj = XMMatrixPerspectiveFovLH(XM_PIDIV4, 800.0f / 600.0f, 0.1f, 100.0f);//射影行列

	static float angleRadians = 0;
	const auto DELTA = DirectX::XMConvertToRadians(0.1f);
	angleRadians += DELTA;
	// XMMATRIX型の値を作る！
	//auto m = DirectX::XMMatrixIdentity();
	auto m = DirectX::XMMatrixRotationY(angleRadians);

	XMMATRIX shadow = XMMatrixShadow({ 0,1,0,2 }, { 0,15,0 });
	// XMMATRIX型からXMFloat4x4型にStoreする
	//DirectX::XMStoreFloat4x4(&m_constant->Buffer.Model, m);


	CONSTANT_BUFFER cb;
	cb.matWVP = XMMatrixTranspose(m * shadow * view * proj);
	cb.isShadow = 1;

	D3D11_MAPPED_SUBRESOURCE pdata;
	Direct3D::pContext->Map(pConstantBuffer_.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &pdata);	// GPUからのデータアクセスを止める
	memcpy_s(pdata.pData, pdata.RowPitch, (void*)(&cb), sizeof(cb));	// データを値を送る
	Direct3D::pContext->Unmap(pConstantBuffer_.Get(), 0);	//再開

	//頂点バッファ
	UINT stride = sizeof(VertexPositionColor);
	UINT offset = 0;
	Direct3D::pContext->IASetVertexBuffers(0, 1, pVertexBuffer_.GetAddressOf(), &stride, &offset);

	// インデックスバッファーをセット
	stride = sizeof(int);
	offset = 0;
	Direct3D::pContext->IASetIndexBuffer(pIndexBuffer_.Get(), DXGI_FORMAT_R32_UINT, 0);

	//コンスタントバッファ
	Direct3D::pContext->VSSetConstantBuffers(0, 1, pConstantBuffer_.GetAddressOf());	//頂点シェーダー用	
	Direct3D::pContext->PSSetConstantBuffers(0, 1, pConstantBuffer_.GetAddressOf());	//ピクセルシェーダー用

	Direct3D::pContext->DrawIndexed(36, 0, 0);
}

void Quad::Release()
{
}
