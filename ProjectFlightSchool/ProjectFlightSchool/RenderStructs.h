#ifndef RENDERSTRUCTS_H
#define RENDERSTRUCTS_H

#include <DirectXMath.h>

#define MAX_AMOUNT_OF_OBJECT3D	5000
#define MAX_AMOUNT_OF_OBJECT2D	5000
#define MAX_AMOUNT_OF_ANIM3D	100
#define MAX_AMOUNT_OF_PLANES	2000
#define MAX_AMOUNT_OF_BOXES		2000
#define MAX_AMOUNT_OF_NODEGRIDS	500

struct Object3dInfo
{
	UINT				mAssetId;
	DirectX::XMFLOAT4X4	mWorld;
};

struct Anim3dInfo
{
	UINT				mModelId;
	DirectX::XMFLOAT4X4	mWorld;
	DirectX::XMFLOAT4X4	mBoneTransforms[NUM_SUPPORTED_JOINTS];
};

struct Object2dInfo
{
	UINT				mAssetId;
	DirectX::XMFLOAT2	mTopLeftCorner;
	DirectX::XMFLOAT2	mWidthHeight;

};

struct BillboardInfo
{
	UINT				mAssetId;
	DirectX::XMFLOAT3	mWorldPosition;
	float				mWidth;
	float				mHeight;
};

struct ParticleInfo
{
	UINT				mAssetId;
	UINT				mOffsetToNextParticleType;
	DirectX::XMFLOAT3	mWorldPosition;
	float				mLifeTime;
};

struct NodeGridInfo
{
	StaticVertex*		mVertices;
	UINT				mNrOfVertices;
	DirectX::XMFLOAT4X4	mWorld;

};

struct PlaneInfo
{
	UINT				mAssetId;
	DirectX::XMFLOAT3	mTopTriangle;
	DirectX::XMFLOAT3	mBottomTriangle;
};

struct BoxInfo
{
	DirectX::XMFLOAT3 min;
	DirectX::XMFLOAT3 max;
};

struct RenderLists
{
	Object3dInfo*	object3d;
	UINT			sizeOfObject3dList;
	Anim3dInfo&		anim3d;
	UINT			sizeOfAnim3dList;
	Object2dInfo*	object2d;
	UINT			sizeOfObject2dList;
	PlaneInfo*		plane;
	UINT			sizeOfPlaneList;
};

struct AnimationTrack
{
	UINT	mModelID;
	UINT	mCurrentAnimation;
	float	mCurrentAnimationTime;
	UINT	mNextAnimation;
	float	mNextAnimationTime;
	float	mInterpolation;
};

#endif