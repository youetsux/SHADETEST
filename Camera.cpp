#include "Camera.h"


namespace Camera
{
    CameraSetting DefCam;
	XMMATRIX viewMatrix_;	//ビュー行列
	XMMATRIX projMatrix_;	//プロジェクション行列
}


void Camera::Initialize()
{
	DefCam.position_ = XMVectorSet(0, 3, -10, 0);	//カメラの位置
	DefCam.target_ = XMVectorSet(0, 0, 0, 0);	//カメラの焦点
	DefCam.upvector_ = XMVectorSet(0, 1, 0, 0);
	DefCam.fov_ = XM_PIDIV4;
	DefCam.near_ = 0.1f;
	DefCam.far_ = 100.0f;
	DefCam.aspect_ = (FLOAT)800 / (FLOAT)600;
	//プロジェクション行列
	projMatrix_ = XMMatrixPerspectiveFovLH(DefCam.fov_, DefCam.aspect_,DefCam.near_, DefCam.far_);
}

void Camera::Initialize(XMVECTOR _position, XMVECTOR _target)
{
	DefCam.position_ = _position;	//カメラの位置
	DefCam.target_ = _target;	//カメラの焦点
	DefCam.upvector_ = XMVectorSet(0, 1, 0, 0);
	DefCam.fov_ = XM_PIDIV4;
	DefCam.near_ = 0.1f;
	DefCam.far_ = 100.0f;
	DefCam.aspect_ = (FLOAT)800 / (FLOAT)600;

	//プロジェクション行列
	projMatrix_ = XMMatrixPerspectiveFovLH(DefCam.fov_, DefCam.aspect_, DefCam.near_, DefCam.far_);
}

void Camera::Update()
{
	//ビュー行列の作成
	viewMatrix_ = XMMatrixLookAtLH(DefCam.position_, DefCam.target_, DefCam.upvector_);
}

void Camera::SetPosition(XMVECTOR position)
{
	DefCam.position_ = position;
}

void Camera::SetTarget(XMVECTOR target)
{
	DefCam.target_ = target;
}

XMMATRIX Camera::GetViewMatrix()
{
	return viewMatrix_;
}

XMMATRIX Camera::GetProjectionMatrix()
{
	return projMatrix_;
}

XMVECTOR Camera::GetViewPosition()
{
	return DefCam.position_;
}

XMVECTOR Camera::GetTarget()
{
	return DefCam.target_;
}

XMVECTOR Camera::GetUpVector()
{
	return DefCam.upvector_;
}
