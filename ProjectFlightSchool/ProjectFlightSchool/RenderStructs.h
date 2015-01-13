#ifndef RENDERSTRUCTS_H
#define RENDERSTRUCTS_H

#include <DirectXMath.h>


struct Object3dInfo
{
	UINT				mAssetId;
	DirectX::XMFLOAT4X4	mWorld;
};

struct Anim3dInfo
{
	UINT				mModelId;
	UINT				mAnimId;
	float*				mAnimTime;
};

struct Object2dInfo
{
	UINT				mAssetId;
	DirectX::XMFLOAT2	mTopLeftCorner;
	DirectX::XMFLOAT2	mWidthHeight;

};

struct PlaneInfo
{
	UINT mAssetId;
	DirectX::XMFLOAT3 mTopTriangle;
	DirectX::XMFLOAT3 mBottomTriangle;
};


#endif