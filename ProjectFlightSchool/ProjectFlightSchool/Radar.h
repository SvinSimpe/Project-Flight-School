#ifndef RADAR_H
#define RADARA_H

#include <Graphics.h>
#include "RenderManager.h"
#include "Input.h"

#define MAX_RADAR_OBJECTS 100

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

		float			mRadarObjHeight;		
		float			mRadarObjWidth;			
		float			mRadarObjHalfWidth;		
		float			mRadarObjHalfHeight;	

		float			mRadarShipHeight;			
		float			mRadarShipWidth;			
		float			mRadarShipHalfHeight;		
		float			mRadarShipHalfWidth;		

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