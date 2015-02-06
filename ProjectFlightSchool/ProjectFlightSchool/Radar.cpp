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
			if( vecLength <= mRadius - mRadarShipHalfWidth )
			{

				radarObjects[i].mRadarObjectPos.x = ( mOffsetX + ( ( mRadarDimXY * 0.5f ) - mRadarShipHalfWidth ) ) + ( radarObjects[i].mRadarObjectPos.x - playerPos.x );
				radarObjects[i].mRadarObjectPos.y = -radarObjects[i].mRadarObjectPos.z + playerPos.z + ( mRadarDimXY * 0.5f ) - mRadarShipHalfHeight;

				AddObjectToList( radarObjects[i].mRadarObjectPos, radarObjects[i].mType );
			}
		}
		else
		{
			if( vecLength <= mRadius - mRadarObjHalfWidth )
			{

				radarObjects[i].mRadarObjectPos.x = ( mOffsetX + ( ( mRadarDimXY * 0.5f ) - mRadarObjHalfWidth ) ) + ( radarObjects[i].mRadarObjectPos.x - playerPos.x );
				radarObjects[i].mRadarObjectPos.y = -radarObjects[i].mRadarObjectPos.z + playerPos.z + ( mRadarDimXY * 0.5f ) - mRadarObjHalfHeight;

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


	//Full image max pixel width and height == 384
	//Radius from the center == 97 pixels
	mRadarDimXY = Input::GetInstance()->mScreenWidth * 0.15f;
	mRadius = mRadarDimXY * 0.25f;
	mRadarCenter = mRadarDimXY * 0.5f;

	mOffsetX		= Input::GetInstance()->mScreenWidth - mRadarDimXY;
	mOffsetY		= 0;

	Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/PlaceHolderTextures/diffuse.png", mEnemyAssetID );
	Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/PlaceHolderTextures/diffuse.png", mObjectAssetID );
	Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/PlaceHolderTextures/diffuse.png", mFriendlyAssetID );
	Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/PlaceHolderTextures/diffuse.png", mFriendlyShipAssetID );
	Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/PlaceHolderTextures/diffuse.png", mHostileShipAssetID );
	Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/Radar/radar2.dds", mRadarAssetID );
	mObjects[0].mAssetId			= mRadarAssetID;
	mObjects[0].mTopLeftCorner.x	= mOffsetX;
	mObjects[0].mTopLeftCorner.y	= mOffsetY;
	mObjects[0].mWidthHeight.x		= mRadarDimXY;
	mObjects[0].mWidthHeight.y		= mRadarDimXY;

	mRadarObjWidth		= Input::GetInstance()->mScreenWidth * 0.00625f;
	mRadarObjHeight		= Input::GetInstance()->mScreenWidth * 0.00625f;
	mRadarObjHalfWidth	= mRadarObjWidth * 0.5f;
	mRadarObjHalfHeight	= mRadarObjHeight * 0.5f;

	mRadarShipHeight		= Input::GetInstance()->mScreenWidth * 0.009375f;		
	mRadarShipWidth			= Input::GetInstance()->mScreenWidth * 0.009375f;		
	mRadarShipHalfHeight	= mRadarShipHeight * 0.5f;	
	mRadarShipHalfWidth		= mRadarShipWidth * 0.5f;

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