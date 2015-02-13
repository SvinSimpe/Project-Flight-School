#include "HealthBars.h"

HRESULT	HealthBar::Update( int nrOfAllies, float alliesHP[], float shipHP )
{
	//////////////////////
	//Allies hp
	//////////////////////
	int nrOfHealthsToRender						= 0;
	float offSetForHealths						= 0.0f;
	mNrOfHealthAssetsToRender					= 0;
	DirectX::XMFLOAT2 lastAllyHealthBarOffset	= mAllyHealthOffset;
	HealthBars healthAsset;

	for ( int i = 0; i < nrOfAllies; i++ )
	{

		if( i > 0 )
		{
			lastAllyHealthBarOffset.y -= mSizeAllyHealthBar.y + 8;
		}
		
		healthAsset.mAssetID						= mAllyHealthBar;
		healthAsset.mTopLeftCorner					= lastAllyHealthBarOffset;
		healthAsset.mSize							= mSizeAllyHealthBar;
		mHealthBars[mNrOfHealthAssetsToRender++]	= healthAsset;
		
		nrOfHealthsToRender = (int)( mNrOfHealths * alliesHP[i] );

		if ( nrOfHealthsToRender > 0 )
		{
			healthAsset.mAssetID	= mAllyHealth;
			healthAsset.mSize		= mSizeAllyHealth;

			healthAsset.mTopLeftCorner.y = lastAllyHealthBarOffset.y + ( mSizeAllyHealth.y * 0.5f ) - mSpaceAllyHealthToBar - 1.5f;
			for ( int j = 0; j < nrOfHealthsToRender; j++ )
			{

				healthAsset.mTopLeftCorner.x = lastAllyHealthBarOffset.x - offSetForHealths - 14.0f + mSizeAllyHealthBar.x;
					
				mHealthBars[mNrOfHealthAssetsToRender++] = healthAsset;

				offSetForHealths += mSpaceAllyHealth + mSizeAllyHealth.x;
			}
		}

		offSetForHealths = 0.0f;
	}

	if ( nrOfAllies > 0 )
	{
		lastAllyHealthBarOffset.x					= lastAllyHealthBarOffset.x - mSpaceAllyHealthBar;
		lastAllyHealthBarOffset.y					= lastAllyHealthBarOffset.y - mSpaceAllyHealthBar;
		healthAsset.mAssetID						= mAllyHealthFrame;
		healthAsset.mTopLeftCorner					= lastAllyHealthBarOffset;
		healthAsset.mSize							= mSizeAllyHealthFrame;

		mHealthBars[mNrOfHealthAssetsToRender++]	= healthAsset;
	}

	///////////////////////
	//Ship Bar
	///////////////////////
	healthAsset.mAssetID						= mShipHealthBar;
	healthAsset.mTopLeftCorner					= mShipHealthOffset;
	healthAsset.mSize							= mSizeShipHealthBar;

	mHealthBars[mNrOfHealthAssetsToRender++]	= healthAsset;
	nrOfHealthsToRender							= (int)( mNrOfHealths * shipHP );
	offSetForHealths							= mShipHealthOffset.x + mEndShipHealth.x;

	if ( nrOfHealthsToRender > 0 )
	{
		healthAsset.mTopLeftCorner.y	= mStartShipHealth.y;
		healthAsset.mAssetID			= mShipHealth;
		healthAsset.mSize				= mSizeShipHealth;
		for ( int i = 0; i < nrOfHealthsToRender; i++ )
		{
			healthAsset.mTopLeftCorner.x				= offSetForHealths - mSizeShipHealth.x;
			mHealthBars[mNrOfHealthAssetsToRender++]	= healthAsset;

			offSetForHealths -= ( mSpaceShipHealth + mSizeShipHealthTop );
		}
	}

	return S_OK;
}

HRESULT	HealthBar::Render()
{
	for( UINT i = 0; i < mNrOfHealthAssetsToRender; i++ )
	{
		RenderManager::GetInstance()->AddObject2dToList( mHealthBars[i].mAssetID, mHealthBars[i].mTopLeftCorner, mHealthBars[i].mSize );
	}

	return S_OK;
}

HRESULT	HealthBar::Initialize()
{
	mNrOfHealthAssetsToRender			= 0;

	mAllyHealthOffset.x					= Input::GetInstance()->mScreenWidth - 27.0f - 215.0f;
	mAllyHealthOffset.y					= Input::GetInstance()->mScreenHeight - 27.0f - 40.0f;

	mNrOfHealths						= 20;
	mSpaceAllyHealthToBar				= 6.0f;
	mSpaceAllyHealth					= 2.0f;
	mSpaceAllyHealthBar					= 19.0f;
	mSpaceAllyHealthBarToEdge			= 27.0f;
	mSizeAllyHealthBar					= DirectX::XMFLOAT2( 215.0f, 40.0f );
	mSizeAllyHealth						= DirectX::XMFLOAT2( ( ( mSizeAllyHealthBar.x - ( mSpaceAllyHealthToBar * 2 ) - ( ( mNrOfHealths ) * mSpaceAllyHealth ) ) / mNrOfHealths ), ( mSizeAllyHealthBar.y - ( mSpaceAllyHealthToBar * 2 ) ) );
	mSizeAllyHealthFrame				= DirectX::XMFLOAT2( 110.0f, 110.0f );


	mSizeShipHealthBar					= XMFLOAT2( 986.0f, 113.0f );
	mSpaceShipHealth					= 3.0f;
	mStartShipHealth.x					= (float)( mSizeShipHealthBar.x / 3.94f );
	mStartShipHealth.y					= (float)( mSizeShipHealthBar.y / 12.20f );
	mEndShipHealth.x					= (float)( mSizeShipHealthBar.x / 1.08f );
	mEndShipHealth.y					= (float)( mSizeShipHealthBar.y / 1.55f );
	mSizeShipHealth.x					= (float)( mSizeShipHealthBar.x / 10.3f );
	mSizeShipHealth.y					= ( mEndShipHealth.y - mStartShipHealth.y );
	mSizeShipHealthTop					= ( ( mEndShipHealth.x - mStartShipHealth.x - ( ( 20 - 1 ) * mSpaceShipHealth ) ) / 20 );

	mShipHealthOffset					= DirectX::XMFLOAT2( ( Input::GetInstance()->mScreenWidth * 0.5f ) - ( mSizeShipHealthBar.x * 0.5f ), 0.0f );
	HRESULT hr; 
	if( FAILED( hr = Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/GUI/HUD/allyhealth.dds", mAllyHealth ) ) )
	{
		return hr;
	}

	if( FAILED( hr = Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/GUI/HUD/allyHealthBar.dds", mAllyHealthBar ) ) )
	{
		return hr;
	}

	if( FAILED( hr = Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/GUI/HUD/allyHealthFrame.dds", mAllyHealthFrame ) ) )
	{
		return hr;
	}

	if( FAILED( hr = Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/GUI/HUD/shipHealth.dds", mShipHealth ) ) )
	{
		return hr;
	}
	
	if( FAILED( hr = Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/GUI/HUD/shipHealthBar.dds", mShipHealthBar ) ) )
	{
		return hr;
	}


	for( UINT i = 0; i < MAX_AMOUNT_OF_HEALTBAR_ASSETS; i++ )
	{
		mHealthBars[i].mAssetID			= (UINT)-1;
		mHealthBars[i].mTopLeftCorner	= DirectX::XMFLOAT2( 0.0f, 0.0f );
		mHealthBars[i].mSize			= DirectX::XMFLOAT2( 0.0f, 0.0f );
	}

	return hr;
}

void HealthBar::Release()
{

}

HealthBar::HealthBar()
{

}

HealthBar::~HealthBar()
{

}
