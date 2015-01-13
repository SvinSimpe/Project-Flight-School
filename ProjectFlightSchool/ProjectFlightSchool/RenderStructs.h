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


#endif