#include "Camera.h"

bool Camera::Update()
{
	DirectX::XMVECTOR vecPos	= DirectX::XMLoadFloat4( &mPos );
	DirectX::XMVECTOR vecFocus	= DirectX::XMLoadFloat4( &mFocusPoint );
	DirectX::XMVECTOR vecUp		= DirectX::XMLoadFloat4( &mUp );

	if( !DirectX::XMVector3Equal( vecPos, DirectX::XMVectorZero() ) )
	{
		DirectX::XMStoreFloat4x4( &mViewMatrix, DirectX::XMMatrixLookAtLH( vecPos, vecFocus, vecUp ) );
	}

	return true;
}

void Camera::SetUp( DirectX::XMFLOAT4 up )
{
	mUp = up;
}

DirectX::XMMATRIX Camera::GetViewMatrix() const
{
	return DirectX::XMMatrixTranspose( DirectX::XMLoadFloat4x4( &mViewMatrix ) );
}

DirectX::XMMATRIX Camera::GetInverseViewMatrix() const
{
	return DirectX::XMMatrixInverse( nullptr, DirectX::XMLoadFloat4x4( &mViewMatrix ) );
}

DirectX::XMMATRIX Camera::GetInverseProjectionMatrix() const
{
	return  DirectX::XMMatrixInverse( nullptr, DirectX::XMLoadFloat4x4( &mProjMatrix ) );
}

DirectX::XMMATRIX Camera::GetProjMatrix() const
{
	return DirectX::XMMatrixTranspose( DirectX::XMLoadFloat4x4( &mProjMatrix ) );
}

DirectX::XMFLOAT4 Camera::GetPos() const
{
	return mPos;
}

DirectX::XMFLOAT4 Camera::GetUp()	const
{
	return mUp;
}

DirectX::XMFLOAT4 Camera::GetFocusPoint() const
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

void Camera::SetEyePosition( DirectX::XMFLOAT3 &eyePosition )
{
	mPos = DirectX::XMFLOAT4( eyePosition.x, eyePosition.y, eyePosition.z, 1.0f );
}

void Camera::SetFocus( DirectX::XMFLOAT3 &focusPoint )
{
	mFocusPoint = DirectX::XMFLOAT4( focusPoint.x, focusPoint.y, focusPoint.z, 1.0f );
}

void Camera::ZoomIn()
{
	mPos.y -= 0.5f;
	mPos.z += 0.5f;
}

void Camera::ZoomOut()
{
	mPos.y += 0.5f;
	mPos.z -= 0.5f;
}

void Camera::TurnLeft()
{
	DirectX::XMStoreFloat4( &mPos, DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat4( &mPos ), DirectX::XMMatrixRotationY( 0.5f ) ) );
}

void Camera::TurnRight()
{
	DirectX::XMStoreFloat4( &mPos, DirectX::XMVector3TransformCoord(DirectX::XMLoadFloat4( &mPos ), DirectX::XMMatrixRotationY( -0.5f ) ) );
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

	DirectX::XMVECTOR vecPos	= DirectX::XMLoadFloat4( &mPos );
	DirectX::XMVECTOR vecFocus	= DirectX::XMLoadFloat4( &mFocusPoint );
	DirectX::XMVECTOR vecUp		= DirectX::XMLoadFloat4( &mUp );

	DirectX::XMStoreFloat4x4( &mViewMatrix, DirectX::XMMatrixLookAtLH( vecPos, vecFocus, vecUp ) );
	DirectX::XMStoreFloat4x4( &mProjMatrix, DirectX::XMMatrixPerspectiveFovLH( mFoV, mAspectRatio, mNearZ, mFarZ ) );

	return S_OK;
}

HRESULT Camera::Initialize( CameraInfo* cameraInfo, bool orthoCam )
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

	DirectX::XMVECTOR vecPos	= DirectX::XMLoadFloat4( &mPos );
	DirectX::XMVECTOR vecFocus	= DirectX::XMLoadFloat4( &mFocusPoint );
	DirectX::XMVECTOR vecUp		= DirectX::XMLoadFloat4( &mUp );

	DirectX::XMStoreFloat4x4( &mViewMatrix, DirectX::XMMatrixLookAtLH( vecPos, vecFocus, vecUp ) );
	DirectX::XMStoreFloat4x4( &mProjMatrix, DirectX::XMMatrixOrthographicLH( mWidth, mHeight, mNearZ, mFarZ ) );

	return S_OK;
}

void Camera::Release()
{
}

Camera::Camera()
{
	mPos		= DirectX::XMFLOAT4(0,0,0,0);
	mUp			= DirectX::XMFLOAT4(0,0,0,0);
	mFocusPoint	= DirectX::XMFLOAT4(0,0,0,0);

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