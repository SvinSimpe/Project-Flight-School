#ifndef CAMERA_H
#define CAMERA_H

#include "CameraInfo.h"
#include <d3d11.h>



class Camera
{
	private:
		DirectX::XMFLOAT4	mPos;
		DirectX::XMFLOAT4	mUp;
		DirectX::XMFLOAT4	mFocusPoint;
		DirectX::XMFLOAT4X4	mViewMatrix;
		DirectX::XMFLOAT4X4	mProjMatrix;

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
	
		void SetUp( DirectX::XMFLOAT4 up );

		DirectX::XMMATRIX GetViewMatrix()				const;
		DirectX::XMMATRIX GetProjMatrix()				const;	
		DirectX::XMMATRIX GetInverseViewMatrix()		const;
		DirectX::XMMATRIX GetInverseProjectionMatrix()	const;

		DirectX::XMFLOAT4	GetPos()		const;
		DirectX::XMFLOAT4	GetUp()			const;
		DirectX::XMFLOAT4	GetFocusPoint() const;

		float GetNearZ()			const;
		float GetFarZ()				const;
		float GetFoV()				const;
		float GetAspectRatio()		const;
		float GetWidth()			const;
		float GetHeight()			const;

		void SetEyePosition( DirectX::XMFLOAT3 &eyePosition );
		void SetFocus( DirectX::XMFLOAT3 &focusPoint );

		void ZoomIn();
		void ZoomOut();
		void TurnLeft();
		void TurnRight();
		

		//Must be called before GetViewMatrix, GetProjMatrix
		HRESULT Initialize( CameraInfo* cameraInfo );
		HRESULT Initialize( CameraInfo* cameraInfo, bool orthoCam );
		void Release();

		Camera();
		~Camera();

};
#endif