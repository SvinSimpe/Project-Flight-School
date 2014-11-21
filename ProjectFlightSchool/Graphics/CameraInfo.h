#ifndef CAMERAINFO_H
#define CAMERAINFO_H

#include <DirectXMath.h>

struct CameraInfo
{
	DirectX::XMFLOAT4	eyePos;
	DirectX::XMFLOAT4	focusPoint;
	DirectX::XMFLOAT4	up;
	float				foVY;
	float				width;
	float				height;
	float				nearZ;
	float				farZ;
};
#endif