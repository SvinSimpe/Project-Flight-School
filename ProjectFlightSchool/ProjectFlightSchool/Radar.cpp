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
		else if( type == RADAR_TYPE::PICKED_UP )
		{
			object.mAssetId			= mPickedUpID;
			object.mWidthHeight.x	= mRadarObjHeight;
			object.mWidthHeight.y	= mRadarObjWidth;
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
				radarObjects[i].mRadarObjectPos.y = mRadarShipOffsetY - ( radarObjects[i].mRadarObjectPos.z - playerPos.z ) * mRadarShipTranslationY;
	
				AddObjectToList( radarObjects[i].mRadarObjectPos, radarObjects[i].mType );
			}
			else
			{
				DirectX::XMFLOAT3 normPos;

				DirectX::XMStoreFloat3( &normPos, DirectX::XMVector3Normalize( ( DirectX::XMLoadFloat3( &radarObjects[i].mRadarObjectPos ) - DirectX::XMLoadFloat3( &playerPos ) ) ) );

				DirectX::XMStoreFloat3( &normPos, DirectX::XMLoadFloat3( &normPos ) );

				radarObjects[i].mRadarObjectPos.x = mRadarShipOffsetX + ( normPos.x * mRadius ) * mRadarShipTranslationX;
				radarObjects[i].mRadarObjectPos.y = mRadarShipOffsetY - ( normPos.z * mRadius ) * mRadarShipTranslationY;
				
				//CHANGE THIS WHEN WE HAVE THE ACTUALL ARROW
				AddObjectToList( radarObjects[i].mRadarObjectPos, radarObjects[i].mType );
				//AddObjectToList( radarObjects[i].mRadarObjectPos, mObjectiveArrowID );
			}
		}
		else if( radarObjects[i].mType == RADAR_TYPE::OBJECTIVE || radarObjects[i].mType == RADAR_TYPE::PICKED_UP )
		{
			if( vecLength <= mRadius )
			{
				radarObjects[i].mRadarObjectPos.x = mRadarObjectOffsetX + ( radarObjects[i].mRadarObjectPos.x - playerPos.x ) * mRadarObjectTranslationX;
				radarObjects[i].mRadarObjectPos.y = mRadarObjectOffsetY - ( radarObjects[i].mRadarObjectPos.z - playerPos.z ) * mRadarObjectTranslationY;
				
				AddObjectToList( radarObjects[i].mRadarObjectPos, radarObjects[i].mType );
			}
			else
			{
				DirectX::XMFLOAT3 normPos;

				DirectX::XMStoreFloat3( &normPos, DirectX::XMVector3Normalize( ( DirectX::XMLoadFloat3( &radarObjects[i].mRadarObjectPos ) - DirectX::XMLoadFloat3( &playerPos ) ) ) );

				DirectX::XMStoreFloat3( &normPos, DirectX::XMLoadFloat3( &normPos ) );

				radarObjects[i].mRadarObjectPos.x = mRadarObjectOffsetX + ( normPos.x * mRadius ) * mRadarObjectTranslationX;
				radarObjects[i].mRadarObjectPos.y = mRadarObjectOffsetY - ( normPos.z * mRadius ) * mRadarObjectTranslationY;
				
				//CHANGE THIS WHEN WE HAVE THE ACTUALL ARROW
				AddObjectToList( radarObjects[i].mRadarObjectPos, radarObjects[i].mType );
				//AddObjectToList( radarObjects[i].mRadarObjectPos, mObjectiveArrowID );
			}
		}
		else
		{
			if( vecLength <= mRadius )
			{
				radarObjects[i].mRadarObjectPos.x = mRadarObjectOffsetX + ( radarObjects[i].mRadarObjectPos.x - playerPos.x ) * mRadarObjectTranslationX;
				radarObjects[i].mRadarObjectPos.y = mRadarObjectOffsetY - ( radarObjects[i].mRadarObjectPos.z - playerPos.z ) * mRadarObjectTranslationY;
				
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
	
	mNrOfObjects		= 1;
	float radarDimXY	= Input::GetInstance()->mScreenWidth * BASE_SCALE_VALUE;
	float radarDimY		= Input::GetInstance()->mScreenHeight * BASE_SCALE_VALUE;
	float radarRadius	= radarDimXY * 0.092f;
	float radarCenter	= radarDimXY * 0.5f;
	mRadius				= 48.0f;

	Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/GUI/Radar/radarEnemy.dds", mEnemyAssetID );
	Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/GUI/Radar/radarBattery.dds", mObjectAssetID );
	Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/GUI/Radar/radarFriendly.dds", mFriendlyAssetID );
	Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/GUI/Radar/radarShip.dds", mFriendlyShipAssetID );
	Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/GUI/Radar/radarEnemyShipTest.dds", mHostileShipAssetID );
	Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/GUI/Radar/radar2.dds", mRadarAssetID );
	Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/GUI/Radar/radar2.dds", mObjectiveArrowID );

	Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/GUI/Radar/pickedBattery.dds", mPickedUpID );

	mObjects[0].mAssetId			= mRadarAssetID;
	mObjects[0].mTopLeftCorner.x	= Input::GetInstance()->mScreenWidth - radarDimXY;
	mObjects[0].mTopLeftCorner.y	= 0;
	mObjects[0].mWidthHeight.x		= radarDimXY;
	mObjects[0].mWidthHeight.y		= radarDimXY;

	mRadarObjWidth					= radarDimXY * 0.055f;
	mRadarObjHeight					= mRadarObjWidth;
	float radarObjHalfWidth			= mRadarObjWidth * 0.5f;
	float radarObjHalfHeight		= mRadarObjHeight * 0.5f;

	mRadarShipHeight				= radarDimXY * 0.069f; 
	mRadarShipWidth					= mRadarShipHeight;
	float radarShipHalfHeight		= mRadarShipHeight * 0.5f;	
	float radarShipHalfWidth		= mRadarShipWidth * 0.5f; 

	mRadarShipOffsetX				= ( ( Input::GetInstance()->mScreenWidth - radarDimXY ) + ( ( radarDimXY * 0.5f ) - radarShipHalfWidth - ( mRadarShipWidth * 0.1f ) ) );
	mRadarShipOffsetY				= ( ( radarDimXY * 0.5f ) - radarShipHalfHeight - ( mRadarShipHeight * 0.1f ) );
	mRadarShipTranslationX			= ( ( radarDimXY / radarRadius ) * BASE_SCALE_VALUE ) * ( radarDimXY / ( BASE_VALUE_FOR_RESOLUTION_SCALING ) );
	mRadarShipTranslationY			= mRadarShipTranslationX;

	mRadarObjectOffsetX				= ( ( Input::GetInstance()->mScreenWidth - radarDimXY ) + ( ( radarDimXY * 0.5f ) - radarObjHalfWidth - ( mRadarObjWidth * 0.1f ) ) );
	mRadarObjectOffsetY				= ( radarDimXY * 0.5f ) - radarObjHalfHeight - ( mRadarObjHeight * 0.1f );
	mRadarObjectTranslationX		= ( ( radarDimXY / radarRadius ) * BASE_SCALE_VALUE ) * ( radarDimXY / ( BASE_VALUE_FOR_RESOLUTION_SCALING ) );
	mRadarObjectTranslationY		= mRadarObjectTranslationX;

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