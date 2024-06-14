#pragma once

#include "Direct3D.h"
#include <DirectXMath.h>

using namespace DirectX;

struct CameraSetting
{
	//�ϐ�
	XMVECTOR position_;	//�J�����̈ʒu�i���_�j
	XMVECTOR target_;	//����ʒu�i�œ_�j
	XMVECTOR upvector_; //������̃x�N�g��
	float	 fov_;
	float	 near_;
	float	 far_;
	float    aspect_;
};

//-----------------------------------------------------------
//�J����
//-----------------------------------------------------------
namespace Camera
{
	//�������i�v���W�F�N�V�����s��쐬�j
	void Initialize();
	void Initialize(XMVECTOR _position, XMVECTOR _target); //�I�[�o�[���[�h


	//�X�V�i�r���[�s��쐬�j
	void Update();

	//���_�i�J�����̈ʒu�j��ݒ�
	void SetPosition(XMVECTOR position);

	//�œ_�i����ʒu�j��ݒ�
	void SetTarget(XMVECTOR target);

	//�r���[�s����擾
	XMMATRIX GetViewMatrix();

	//�v���W�F�N�V�����s����擾
	XMMATRIX GetProjectionMatrix();

	XMVECTOR GetViewPosition();
	XMVECTOR GetTarget();
	XMVECTOR GetUpVector();
	extern  CameraSetting DefCam;
};