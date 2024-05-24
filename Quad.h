#pragma once
#include "Direct3D.h"
#include <DirectXMath.h>
using namespace DirectX;

typedef struct _vertexPositionColor
{
	XMFLOAT3 pos;
	XMFLOAT4 color;
} VertexPositionColor;

//�R���X�^���g�o�b�t�@�[
struct CONSTANT_BUFFER
{
	XMMATRIX	matWVP;
};

class Quad
{
	ID3D11Buffer* pVertexBuffer_;	//���_�o�b�t�@
	ID3D11Buffer* pIndexBuffer_;    //�C���f�b�N�X�o�b�t�@
	ID3D11Buffer* pConstantBuffer_;	//�R���X�^���g�o�b�t�@
public:
	Quad();
	~Quad();
	void Initialize();
	void Draw();
	void Release();
};

