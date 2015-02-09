#include "Radar.h"

#pragma region Public functions

void Radar::AddObjectToList( DirectX::XMFLOAT3 position, UINT type )
{
	if( mNrOfObjects < MAX_RADAR_OBJECTS )
	{
		ObjectOnRadar object;
		object.mTopLeftCorner.x	= ( position.x );
		object.mTopLeftCorner.y = ( position.y );

		if( type == RADAR_TYPE::HOSTILE )
		{
			object.mAssetId			= mEnemyAssetID;
			object.mWidthHeight.x	= mRadarObjHeight;
			object.mWidthHeight.y	= mRadarObjWidth;
		}
		else if( type == RADAR_TYPE::FRIENDLY )
		{
			object.mAssetId			= mFriendlyAssetID;
			object.mWidthHeight.x	= mRadarObjHeight;
			object.mWidthHeight.y	= mRadarObjWidth;
		}
		else if( type == RADAR_TYPE::OBJECTIVE )
		{
			object.mAssetId			= mObjectAssetID;
			object.mWidthHeight.x	= mRadarObjHeight;
			object.mWidthHeight.y	= mRadarObjWidth;
		}
		else if( type == RADAR_TYPE::SHIP_FRIENDLY )
		{
			object.mAssetId			= mFriendlyShipAssetID;
			object.mWidthHeight.x	= mRadarShipHeight;
			object.mWidthHeight.y	= mRadarShipWidth;
		}
		else if( type == RADAR_TYPE::SHIP_HOSTILE )
		{
			object.mAssetId			= mHostileShipAssetID;
			object.mWidthHeight.x	= mRadarShipHeight;
			object.mWidthHeight.y	= mRadarShipWidth;
		}
	
		mObjects[mNrOfObjects++] = object;
	}
}

HRESULT	Radar::Update( DirectX::XMFLOAT3 playerPos, RADAR_UPDATE_INFO radarObjects[], UINT nrOfObjects )
{
	mNrOfObjects = 1;
	float vecLength;

	for( UINT i = 0; i < nrOfObjects; i++ )
	{
		DirectX::XMStoreFloat( &vecLength, DirectX::XMVector3Length( ( DirectX::XMLoadFloat3( &radarObjects[i].mRadarObjectPos ) - DirectX::XMLoadFloat3( &playerPos ) ) ) );
	
		if( radarObjects[i].mType == RADAR_TYPE::SHIP_FRIENDLY || radarObjects[i].mType == RADAR_TYPE::SHIP_HOSTILE )
		{
			if( vecLength <= mRadius )
			{
				radarObjects[i].mRadarObjectPos.x = ( mOffsetX + ( ( mRadarDimXY * 0.5f ) - mRadarShipHalfWidth - ( mRadarShipWidth * 0.2f ) ) ) + ( radarObjects[i].mRadarObjectPos.x - playerPos.x ) * ( ( mRadarDimXY / mRadarRadius ) * BASE_SCALE_VALUE ) * ( mRadarDimXY / ( BASE_VALUE_FOR_RESOLUTION_SCALING ) );
				radarObjects[i].mRadarObjectPos.y = ( ( mRadarDimXY * 0.5f ) - mRadarShipHalfHeight - ( mRadarShipWidth * 0.3f ) ) - radarObjects[i].mRadarObjectPos.z + playerPos.z * ( ( ( mRadarDimXY / mRadarRadius ) * BASE_SCALE_VALUE ) * ( mRadarDimXY / ( BASE_VALUE_FOR_RESOLUTION_SCALING ) ) );
				
				AddObjectToList( radarObjects[i].mRadarObjectPos, radarObjects[i].mType );
			}
		}
		else
		{
			if( vecLength <= mRadius )
			{

				//radarObjects[i].mRadarObjectPos.x = ( mOffsetX + ( ( mRadarDimXY * 0.5f ) - mRadarObjHalfWidth ) ) + ( radarObjects[i].mRadarObjectPos.x - playerPos.x );
				//radarObjects[i].mRadarObjectPos.y = -radarObjects[i].mRadarObjectPos.z + playerPos.z + ( mRadarDimXY * 0.5f ) - mRadarObjHalfHeight;

				radarObjects[i].mRadarObjectPos.x = ( mOffsetX + ( ( mRadarDimXY * 0.5f ) - mRadarObjHalfWidth - ( mRadarObjWidth * 0.2f ) ) ) + ( radarObjects[i].mRadarObjectPos.x - playerPos.x ) * ( ( mRadarDimXY / mRadarRadius ) * BASE_SCALE_VALUE ) * ( mRadarDimXY / ( BASE_VALUE_FOR_RESOLUTION_SCALING ) );
				radarObjects[i].mRadarObjectPos.y = ( ( mRadarDimXY * 0.5f ) - mRadarObjHalfWidth - ( mRadarObjWidth * 0.1f ) ) - radarObjects[i].mRadarObjectPos.z + playerPos.z * ( ( ( mRadarDimXY / mRadarRadius ) * BASE_SCALE_VALUE ) * ( mRadarDimXY / ( BASE_VALUE_FOR_RESOLUTION_SCALING ) ) );
				

				AddObjectToList( radarObjects[i].mRadarObjectPos, radarObjects[i].mType );
			}
		}
	}
	return S_OK;
}

HRESULT	Radar::Render()
{
	for( UINT i = 0; i < mNrOfObjects; i++ )
	{
		RenderManager::GetInstance()->AddObject2dToList( mObjects[i].mAssetId, mObjects[i].mTopLeftCorner, mObjects[i].mWidthHeight );
	}

	return S_OK;
}

HRESULT	Radar::Initialize()
{
	
	mNrOfObjects	= 1;
	mRadarDimXY		= Input::GetInstance()->mScreenWidth * 0.15f;
	mRadarRadius	= mRadarDimXY * 0.092f;
	mRadarCenter	= mRadarDimXY * 0.5f;
	mRadius			= 48.0f;

	mOffsetX		= Input::GetInstance()->mScreenWidth - mRadarDimXY;
	mOffsetY		= 0;

	Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/PlaceHolderTextures/diffuse.png", mEnemyAssetID );
	Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/PlaceHolderTextures/diffuse.png", mObjectAssetID );
	Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/PlaceHolderTextures/diffuse.png", mFriendlyAssetID );
	Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/Radar/radarEnemyShipTest.dds", mFriendlyShipAssetID );
	Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/PlaceHolderTextures/diffuse.png", mHostileShipAssetID );
	Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/Radar/radar2.dds", mRadarAssetID );
	
	mObjects[0].mAssetId			= mRadarAssetID;
	mObjects[0].mTopLeftCorner.x	= mOffsetX;
	mObjects[0].mTopLeftCorner.y	= mOffsetY;
	mObjects[0].mWidthHeight.x		= mRadarDimXY;
	mObjects[0].mWidthHeight.y		= mRadarDimXY;

	mRadarObjWidth					= mRadarDimXY * 0.055f;
	mRadarObjHeight					= mRadarDimXY * 0.055f;
	mRadarObjHalfWidth				= mRadarObjWidth * 0.5f;
	mRadarObjHalfHeight				= mRadarObjHeight * 0.5f;

	mRadarShipHeight				= mRadarDimXY * 0.069f; 
	mRadarShipWidth					= mRadarDimXY * 0.069f; 
	mRadarShipHalfHeight			= mRadarShipHeight * 0.5f;	
	mRadarShipHalfWidth				= mRadarShipWidth * 0.5f; 

	return S_OK;
}

void Radar::Release()
{

}

Radar::Radar()
{

}

Radar::~Radar()
{

}

#pragma endregion