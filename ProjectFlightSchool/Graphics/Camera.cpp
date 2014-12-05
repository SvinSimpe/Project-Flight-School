#include "Camera.h"

bool Camera::Update()
{
	XMVECTOR vecPos		= XMLoadFloat4( &mPos );
	XMVECTOR vecFocus	= XMLoadFloat4( &mFocusPoint );
	XMVECTOR vecUp		= XMLoadFloat4( &mUp );

	XMStoreFloat4x4( &mViewMatrix, XMMatrixLookAtLH( vecPos, vecFocus, vecUp ) );

	return true;
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
	return XMMatrixTranspose( XMLoadFloat4x4( &mViewMatrix ) );
}

XMMATRIX Camera::GetProjMatrix() const
{
	return XMMatrixTranspose( XMLoadFloat4x4( &mProjMatrix ) );
}

XMFLOAT4	Camera::GetPos() const
{
	return mPos;
}

XMFLOAT4	Camera::GetUp()	const
{
	return mUp;
}

XMFLOAT4	Camera::GetFocusPoint() const
{
	return mFocusPoint;
}

float Camera::GetNearZ() const
{
	return mNearZ;
}

float Camera::GetFarZ()	const
{
	return mFarZ;
}

float Camera::GetFoV() const
{
	return mFoV;
}

float Camera::GetAspectRatio() const
{
	return mAspectRatio;
}

float Camera::GetWidth() const
{
	return mWidth;
}

float Camera::GetHeight() const
{
	return mHeight;
}

HRESULT Camera::Initialize( CameraInfo* cameraInfo )
{
	mPos			= cameraInfo->eyePos;
	mFocusPoint		= cameraInfo->focusPoint;
	mUp				= cameraInfo->up;

	mFoV			= cameraInfo->foVY;
	mNearZ			= cameraInfo->nearZ;
	mFarZ			= cameraInfo->farZ;
	mWidth			= cameraInfo->width;
	mHeight			= cameraInfo->height;
	mAspectRatio	= mWidth/mHeight;

	XMVECTOR vecPos		= XMLoadFloat4( &mPos );
	XMVECTOR vecFocus	= XMLoadFloat4( &mFocusPoint );
	XMVECTOR vecUp		= XMLoadFloat4( &mUp );

	XMStoreFloat4x4( &mViewMatrix, XMMatrixLookAtLH( vecPos, vecFocus, vecUp ) );
	XMStoreFloat4x4( &mProjMatrix, XMMatrixPerspectiveFovLH( mFoV, mAspectRatio, mNearZ, mFarZ ) );

	return S_OK;
}

void Camera::Release()
{
}

Camera::Camera()
{
	mPos		= XMFLOAT4(0,0,0,0);
	mUp			= XMFLOAT4(0,0,0,0);
	mFocusPoint	= XMFLOAT4(0,0,0,0);

	mFoV			= 0;
	mNearZ			= 0;
	mFarZ			= 0;
	mWidth			= 0;
	mHeight			= 0;
	mAspectRatio	= 0;
}

Camera::~Camera()
{
}