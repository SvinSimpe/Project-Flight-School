#include "Radar.h"

#pragma region Public functions

void Radar::AddObjectToList( DirectX::XMFLOAT3 position, UINT type )
{
	ObjectOnRadar object;
	object.mTopLeftCorner.x	= ( position.x );
	object.mTopLeftCorner.y = ( position.y );
	object.mWidthHeight.x	= 12.0f;
	object.mWidthHeight.y	= 12.0f;

	if( type == 0 )
	{
		object.mAssetId = mEnemyAssetID;
	}

	mObjects[mNrOfObjects++] = object;
}

HRESULT	Radar::Update( DirectX::XMFLOAT3 playerPos, DirectX::XMFLOAT3 radarObjectPos )
{
	mNrOfObjects = 1;
	float vecLength;
	DirectX::XMStoreFloat( &vecLength, DirectX::XMVector3Length( ( DirectX::XMLoadFloat3( &radarObjectPos ) - DirectX::XMLoadFloat3( &playerPos ) ) ) );
	
	if( vecLength <= mRadius )
	{
		radarObjectPos.x = ( mOffsetX + ( ( mRadarDimXY * 0.5f ) - HALF_WIDTH ) ) + ( radarObjectPos.x - playerPos.x );
		radarObjectPos.y = -radarObjectPos.z + playerPos.z + ( mRadarDimXY * 0.5f );
		AddObjectToList( radarObjectPos, 0 );
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
	mRadius			= (float)( sqrt( pow( (float)Input::GetInstance()->mScreenWidth, 2 ) + pow( (float)Input::GetInstance()->mScreenHeight, 2 ) ) * 0.04f);
	/*mScaleX			= Input::GetInstance()->mScreenWidth * 0.2f;
	mScaleY			= Input::GetInstance()->mScreenHeight * 0.2f;*/
	//mOffsetX		= Input::GetInstance()->mScreenWidth * 0.845f;
	//mOffsetY		= -20.0f;
	//mRadarDimXY		= mRadius * 7.0f;
	mOffsetX		= Input::GetInstance()->mScreenWidth - 384;
	mOffsetY		= 0;
	mRadarDimXY		= 384;
	Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/Textures/burger.png", mEnemyAssetID );
	Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/Radar/radar2.dds", mRadarAssetID );
	mObjects[0].mAssetId			= mRadarAssetID;
	mObjects[0].mTopLeftCorner.x	= mOffsetX;
	mObjects[0].mTopLeftCorner.y	= mOffsetY;
	mObjects[0].mWidthHeight.x		= mRadarDimXY;
	mObjects[0].mWidthHeight.y		= mRadarDimXY;

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