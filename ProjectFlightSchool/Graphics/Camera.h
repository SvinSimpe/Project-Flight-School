#ifndef CAMERA_H
#define CAMERA_H

#include <DirectXMath.h>

using namespace DirectX;

class Camera
{
private:
	XMFLOAT4 mPos;
	XMFLOAT4 mUp;
	XMFLOAT4 mFocusPoint;
	XMFLOAT4X4 mViewMatrix;
	XMFLOAT4X4 mProjMatrix;

	float mNearZ;
	float mFarZ;
	float mFoV;
	float mAspectRatio;
	float mWidth;
	float mHeight;

public:
	bool Update();
	
	//Must be called before GetViewMatrix, GetProjMatrix
	bool Initialize( XMFLOAT4 eyePos, XMFLOAT4 focusPoint, XMFLOAT4 up , float foVY, float width, float height, float nearZ, float farZ );
	void Release();

	Camera();
	~Camera();
	
	void SetFocus( XMFLOAT4 focusPoint );
	void SetUp( XMFLOAT4 up );
	void SetPos( XMFLOAT4 pos );

	XMMATRIX GetViewMatrix() const;
	XMMATRIX GetProjMatrix() const;

};
#endif