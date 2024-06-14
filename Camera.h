#pragma once

#include "Direct3D.h"
#include <DirectXMath.h>

using namespace DirectX;

struct CameraSetting
{
	//変数
	XMVECTOR position_;	//カメラの位置（視点）
	XMVECTOR target_;	//見る位置（焦点）
	XMVECTOR upvector_; //上向きのベクトル
	float	 fov_;
	float	 near_;
	float	 far_;
	float    aspect_;
};

//-----------------------------------------------------------
//カメラ
//-----------------------------------------------------------
namespace Camera
{
	//初期化（プロジェクション行列作成）
	void Initialize();
	void Initialize(XMVECTOR _position, XMVECTOR _target); //オーバーロード


	//更新（ビュー行列作成）
	void Update();

	//視点（カメラの位置）を設定
	void SetPosition(XMVECTOR position);

	//焦点（見る位置）を設定
	void SetTarget(XMVECTOR target);

	//ビュー行列を取得
	XMMATRIX GetViewMatrix();

	//プロジェクション行列を取得
	XMMATRIX GetProjectionMatrix();

	XMVECTOR GetViewPosition();
	XMVECTOR GetTarget();
	XMVECTOR GetUpVector();
	extern  CameraSetting DefCam;
};