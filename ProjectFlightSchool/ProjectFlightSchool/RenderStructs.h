#ifndef RENDERSTRUCTS_H
#define RENDERSTRUCTS_H

#include <DirectXMath.h>

#define MAX_AMOUNT_OF_OBJECT3D	5000
#define MAX_AMOUNT_OF_OBJECT2D	3000
#define MAX_AMOUNT_OF_ANIM3D	100
#define MAX_AMOUNT_OF_PLANES	200
#define MAX_AMOUNT_OF_BOXES		2000
#define MAX_AMOUNT_OF_NODEGRIDS	50
#define MAX_AMOUNT_OF_PARTICLES 200000
#define MAX_AMOUNT_OF_LINES		1000
#define MAX_AMOUNT_OF_CIRCLES	1000

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
	DirectX::XMFLOAT4	mColor;
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
	UINT				mParticleType;
	DirectX::XMFLOAT3	mWorldPosition;
	float				mAge;
	float				mTimeTillDeath;
	float				mRandomRotation;
};

struct NodeGridInfo
{
	StaticVertex*		mVertices;
	UINT				mNrOfVertices;
	AssetID				mBlendMap;
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
	DirectX::XMFLOAT3	min;
	DirectX::XMFLOAT3	max;
	DirectX::XMFLOAT4X4 world;
};

struct LineInfo
{
	DirectX::XMFLOAT3 start;
	DirectX::XMFLOAT3 end;
};

struct CircleInfo
{
	DirectX::XMFLOAT3 center;
	float radius;
	DirectX::XMFLOAT3 color;	
};

struct AnimationTrack
{
	UINT	mModelID;
	UINT	mCurrentAnimation;
	float	mCurrentAnimationTime;
	UINT	mNextAnimation;
	float	mNextAnimationTime;
	float	mInterpolation;
	bool	mBlendWithCurrent;
};

#endif