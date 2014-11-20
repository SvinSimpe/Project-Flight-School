#include "Camera.h"

bool Camera::Update()
{
	XMVECTOR vecPos		= XMLoadFloat4( &mPos );
	XMVECTOR vecFocus	= XMLoadFloat4( &mFocusPoint );
	XMVECTOR vecUp		= XMLoadFloat4( &mUp );

	XMStoreFloat4x4( &mViewMatrix, XMMatrixLookAtLH( vecPos, vecFocus, vecUp ) );

	return true;
}

bool Camera::Initialize( XMFLOAT4 eyePos, XMFLOAT4 focusPoint, XMFLOAT4 up , float foVY, float width, float height, float nearZ, float farZ )
{
	this->mPos			= eyePos;
	this->mFocusPoint	= focusPoint;
	this->mUp			= up;

	this->mFoV			= foVY;
	this->mNearZ		= nearZ;
	this->mFarZ			= farZ;
	this->mWidth		= width;
	this->mHeight		= height;
	this->mAspectRatio	= mWidth/mHeight;

	XMVECTOR vecPos		= XMLoadFloat4( &mPos );
	XMVECTOR vecFocus	= XMLoadFloat4( &mFocusPoint );
	XMVECTOR vecUp		= XMLoadFloat4( &mUp );

	XMStoreFloat4x4( &mViewMatrix, XMMatrixLookAtLH( vecPos, vecFocus, vecUp ) );
	XMStoreFloat4x4( &mProjMatrix, XMMatrixPerspectiveFovLH( mFoV, mAspectRatio, mNearZ, mFarZ ) );

	return true;
}

void Camera::Release()
{
}

Camera::Camera()
{
	mPos		= XMFLOAT4(0,0,0,0);
	mUp			= XMFLOAT4(0,1,0,0);
	mFocusPoint	= XMFLOAT4(0,0,0,0);

	this->mFoV			= 0;
	this->mNearZ		= 0;
	this->mFarZ			= 0;
	this->mWidth		= 0;
	this->mHeight		= 0;
	this->mAspectRatio	= 0;
}

Camera::~Camera()
{
}

void Camera::SetFocus(XMFLOAT4 focusPoint)
{
	mFocusPoint = focusPoint;
}

void Camera::SetUp(XMFLOAT4 up)
{
	mUp = up;
}

void Camera::SetPos(XMFLOAT4 pos)
{
	mPos = pos;
}

XMMATRIX Camera::GetViewMatrix() const
{
	return XMLoadFloat4x4( &mViewMatrix );
}

XMMATRIX Camera::GetProjMatrix() const
{
	return XMLoadFloat4x4( &mProjMatrix );
}


