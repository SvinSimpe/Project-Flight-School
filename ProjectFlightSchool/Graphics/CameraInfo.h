#ifndef CAMERAINFO_H
#define CAMERAINFO_H

struct CameraInfo
{
	XMFLOAT4	eyePos;
	XMFLOAT4	focusPoint;
	XMFLOAT4	up;
	float		foVY;
	float		width;
	float		height;
	float		nearZ;
	float		farZ;
};
#endif