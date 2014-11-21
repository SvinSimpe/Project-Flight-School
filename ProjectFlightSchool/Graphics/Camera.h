#ifndef CAMERA_H
#define CAMERA_H

#include "CameraInfo.h"

using namespace DirectX;

class Camera
{
	private:
		XMFLOAT4	mPos;
		XMFLOAT4	mUp;
		XMFLOAT4	mFocusPoint;
		XMFLOAT4X4	mViewMatrix;
		XMFLOAT4X4	mProjMatrix;

		float mNearZ;
		float mFarZ;
		float mFoV;
		float mAspectRatio;
		float mWidth;
		float mHeight;

	protected:
	public:

	private:
	protected:
	public:
		bool Update();
	
		void SetFocus( XMFLOAT4 focusPoint );
		void SetUp( XMFLOAT4 up );
		void SetPos( XMFLOAT4 pos );

		XMMATRIX GetViewMatrix() const;
		XMMATRIX GetProjMatrix() const;

		//Must be called before GetViewMatrix, GetProjMatrix
		bool Initialize( CameraInfo* cameraInfo );
		void Release();

		Camera();
		~Camera();

};
#endif