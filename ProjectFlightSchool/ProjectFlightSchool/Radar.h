#ifndef RADAR_H
#define RADARA_H

#include <Graphics.h>
#include "RenderManager.h"
#include "Input.h"


//Full image pixel width and height == 384 pixels
//Radius from the center of the image == 176 pixels
//One tile == 24 ingame units
//Ingame radius == 48 ingame units

#define MAX_RADAR_OBJECTS 100
#define BASE_SCALE_VALUE 0.15f
#define BASE_RESOLUTION_VALUE 1920.0f	//Using the width of the screen as the base for the calculations of the radar
#define BASE_VALUE_FOR_RESOLUTION_SCALING ( BASE_RESOLUTION_VALUE * BASE_SCALE_VALUE )

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
		UINT			mObjectiveArrowID;
		UINT			mNrOfObjects;
		ObjectOnRadar	mObjects[MAX_RADAR_OBJECTS];
		float			mRadius;
		
		float			mRadarObjWidth;	
		float			mRadarObjHeight;
		float			mRadarShipWidth;	
		float			mRadarShipHeight;	

		float			mRadarShipOffsetX;
		float			mRadarShipOffsetY;		
		float			mRadarShipTranslationX;
		float			mRadarShipTranslationY;

		float			mRadarObjectOffsetX;		
		float			mRadarObjectOffsetY;		
		float			mRadarObjectTranslationX;
		float			mRadarObjectTranslationY;


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