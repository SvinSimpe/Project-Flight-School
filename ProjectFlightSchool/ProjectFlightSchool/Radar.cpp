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
				radarObjects[i].mRadarObjectPos.x = mRadarShipOffsetX + ( radarObjects[i].mRadarObjectPos.x - playerPos.x ) * mRadarShipTranslationX;
				radarObjects[i].mRadarObjectPos.y = mRadarShipOffsetY - radarObjects[i].mRadarObjectPos.z + playerPos.z * mRadarShipTranslationY;
				
				AddObjectToList( radarObjects[i].mRadarObjectPos, radarObjects[i].mType );
			}
		}
		else
		{
			if( vecLength <= mRadius )
			{
				radarObjects[i].mRadarObjectPos.x = mRadarObjectOffsetX + ( radarObjects[i].mRadarObjectPos.x - playerPos.x ) * mRadarObjectTranslationX;
				radarObjects[i].mRadarObjectPos.y = mRadarObjectOffsetY - radarObjects[i].mRadarObjectPos.z + playerPos.z * mRadarObjectTranslationY;

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

	//Full image max pixel width and height == 384
	//Radius from the center == 97 pixels
	
	mNrOfObjects		= 1;
	float radarDimXY	= Input::GetInstance()->mScreenWidth * BASE_SCALE_VALUE;
	float radarRadius	= radarDimXY * 0.092f;
	float radarCenter	= radarDimXY * 0.5f;
	mRadius				= 48.0f;

	Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/PlaceHolderTextures/diffuse.png", mEnemyAssetID );
	Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/PlaceHolderTextures/diffuse.png", mObjectAssetID );
	Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/PlaceHolderTextures/diffuse.png", mFriendlyAssetID );
	Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/PlaceHolderTextures/diffuse.png", mFriendlyShipAssetID );
	Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/PlaceHolderTextures/diffuse.png", mHostileShipAssetID );
	Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/Radar/radar2.dds", mRadarAssetID );
	mObjects[0].mAssetId			= mRadarAssetID;
	mObjects[0].mTopLeftCorner.x	= Input::GetInstance()->mScreenWidth - radarDimXY;
	mObjects[0].mTopLeftCorner.y	= 0;
	mObjects[0].mWidthHeight.x		= radarDimXY;
	mObjects[0].mWidthHeight.y		= radarDimXY;

	mRadarObjWidth					= radarDimXY * 0.055f;
	mRadarObjHeight					= radarDimXY * 0.055f;
	float radarObjHalfWidth			= mRadarObjWidth * 0.5f;
	float radarObjHalfHeight		= mRadarObjHeight * 0.5f;

	mRadarShipHeight				= radarDimXY * 0.069f; 
	mRadarShipWidth					= radarDimXY * 0.069f; 
	float radarShipHalfHeight		= mRadarShipHeight * 0.5f;	
	float radarShipHalfWidth		= mRadarShipWidth * 0.5f; 

	mRadarShipOffsetX				= ( ( Input::GetInstance()->mScreenWidth - radarDimXY ) + ( ( radarDimXY * 0.5f ) - radarShipHalfWidth - ( mRadarShipWidth * 0.2f ) ) );
	mRadarShipOffsetY				= ( ( radarDimXY * 0.5f ) - radarShipHalfHeight - ( mRadarShipWidth * 0.3f ) );
	mRadarShipTranslationX			= ( ( radarDimXY / radarRadius ) * BASE_SCALE_VALUE ) * ( radarDimXY / ( BASE_VALUE_FOR_RESOLUTION_SCALING ) );
	mRadarShipTranslationY			= ( ( ( radarDimXY / radarRadius ) * BASE_SCALE_VALUE ) * ( radarDimXY / ( BASE_VALUE_FOR_RESOLUTION_SCALING ) ) );

	mRadarObjectOffsetX				= ( ( Input::GetInstance()->mScreenWidth - radarDimXY ) + ( ( radarDimXY * 0.5f ) - radarObjHalfWidth - ( mRadarObjWidth * 0.2f ) ) );
	mRadarObjectOffsetY				= ( ( radarDimXY * 0.5f ) - radarObjHalfWidth - ( mRadarObjWidth * 0.1f ) );
	mRadarObjectTranslationX		= ( ( radarDimXY / radarRadius ) * BASE_SCALE_VALUE ) * ( radarDimXY / ( BASE_VALUE_FOR_RESOLUTION_SCALING ) );
	mRadarObjectTranslationY		= ( ( ( radarDimXY / radarRadius ) * BASE_SCALE_VALUE ) * ( radarDimXY / ( BASE_VALUE_FOR_RESOLUTION_SCALING ) ) );

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