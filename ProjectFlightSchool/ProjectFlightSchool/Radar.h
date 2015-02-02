#ifndef RADAR_H
#define RADARA_H

#include <Graphics.h>
#include "RenderManager.h"
#include "Input.h"

#define MAX_RADAR_OBJECTS 20

enum RADAR_TYPE
{
	SHIP_FRIENDLY,
	SHIP_HOSTILE,
	FRIENDLY,
	HOSTILE,
	OBJECTIVE
};

struct RADAR_UPDATE_INFO
{
	XMFLOAT3	mRadarObjectPos;
	UINT		mType;
};

static const float HEIGHT			= 12.0f;
static const float WIDTH			= 12.0f;
static const float HALF_WIDTH		= WIDTH * 0.5f;
static const float HALF_HEIGHT		= HEIGHT * 0.5f;

static const float SHIP_HEIGHT		= 18.0f;
static const float SHIP_WIDTH		= 18.0f;
static const float SHIP_HALF_HEIGHT	= SHIP_HEIGHT * 0.5f;
static const float SHIP_HALF_WIDTH	= SHIP_WIDTH * 0.5f;

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
		UINT			mFriendlyShipAssetID;
		UINT			mHostileShipAssetID;
		UINT			mNrOfObjects;
		ObjectOnRadar	mObjects[MAX_RADAR_OBJECTS];
		float			mRadius;
		float			mRadarDimXY;
		float			mRadarCenter;
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
		HRESULT	Update( DirectX::XMFLOAT3 playerPos, RADAR_UPDATE_INFO radarObjects[], UINT nrOfObjects );
		HRESULT	Render();
		HRESULT	Initialize();
		void	Release();
				Radar();
		virtual ~Radar();
};

#endif