#ifndef RADAR_H
#define RADARA_H

#include <Graphics.h>
#include "RenderManager.h"
#include "Input.h"

static const float HEIGHT		= 12.0f;
static const float WIDTH		= 12.0f;
static const float HALF_WIDTH	= WIDTH * 0.5f;
static const float HALF_HEIGHT	= HEIGHT * 0.5f;
const float RADIUS				= 36.0f;  

struct ObjectOnRadar
{
	UINT mAssetId;
	DirectX::XMFLOAT2 mTopLeftCorner;
	DirectX::XMFLOAT2 mWidthHeight;
};

class Radar
{
	private:
		UINT			mRadarAssetID;
		UINT			mEnemyAssetID;
		UINT			mObjectAssetID;
		UINT			mFriendlyAssetID;
		UINT			mNrOfObjects;
		ObjectOnRadar	mObjects[20];
		float			mRadius;
		float			mRadarDimXY;
		float			mScaleX;
		float			mScaleY;
		float			mOffsetX;
		float			mOffsetY;
	protected:
	public:

	private:
	protected:
	public:
		void	AddObjectToList( DirectX::XMFLOAT3 position, UINT type );
		HRESULT	Update( DirectX::XMFLOAT3 playerPos, DirectX::XMFLOAT3 enemyPos );
		HRESULT	Render();
		HRESULT	Initialize();
		void	Release();
				Radar();
		virtual ~Radar();
};

#endif