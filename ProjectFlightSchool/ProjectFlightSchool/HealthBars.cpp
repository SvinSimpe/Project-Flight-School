#include "HealthBars.h"

HRESULT	HealthBar::Update( int nrOfAllies, float alliesHP[], float playerHP, float playerShield, float playerXp, float shipHP )
{
	//////////////////////
	//Allies hp
	//////////////////////
	mNrOfHealthAssetsToRender = 0;
	DirectX::XMFLOAT2 lastAllyHealthBarOffset = mAllyHealthOffset;
	DirectX::XMFLOAT2 allyHealthOffset;
	HealthBars healthAsset;
	for ( int i = 0; i < nrOfAllies; i++ )
	{

		if( i > 0 )
		{
			lastAllyHealthBarOffset.y -= mSizeAllyHealthBar.y + 8;
		}
		
		healthAsset.mAssetID = mAllyHealthBar;
		healthAsset.mTopLeftCorner = lastAllyHealthBarOffset;
		healthAsset.mSize = mSizeAllyHealthBar;
		mHealthBars[mNrOfHealthAssetsToRender++] = healthAsset;
		
		int nrOfHealthsToRender = (int)( mNrOfHealths * alliesHP[i] );

		if ( nrOfHealthsToRender > 0 )
		{
			healthAsset.mAssetID	= mAllyHealth;
			healthAsset.mSize		= mSizeAllyHealth;

			float offSetForHealths = 0.0f;
			allyHealthOffset.y = lastAllyHealthBarOffset.y + ( mSizeAllyHealth.y * 0.5f ) - mSpaceAllyHealthToBar - 1.5f;
			for ( int j = 0; j < nrOfHealthsToRender; j++ )
			{

				allyHealthOffset.x			= lastAllyHealthBarOffset.x - offSetForHealths - 14.0f + mSizeAllyHealthBar.x;
				healthAsset.mTopLeftCorner	= allyHealthOffset;

				mHealthBars[mNrOfHealthAssetsToRender++] = healthAsset;

				offSetForHealths += mSpaceAllyHealth + mSizeAllyHealth.x;
			}
		}
	}

	if ( nrOfAllies > 0 )
	{
		lastAllyHealthBarOffset.x	= lastAllyHealthBarOffset.x - mSpaceAllyHealthBar;
		lastAllyHealthBarOffset.y	= lastAllyHealthBarOffset.y - mSpaceAllyHealthBar;
		healthAsset.mAssetID		= mAllyHealthFrame;
		healthAsset.mTopLeftCorner	= lastAllyHealthBarOffset;
		healthAsset.mSize			= mSizeAllyHealthFrame;

		mHealthBars[mNrOfHealthAssetsToRender++] = healthAsset;
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


	HRESULT hr; 
	if( FAILED( hr = Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/HUD/allyhealth.dds", mAllyHealth ) ) )
	{
		return hr;
	}

	if( FAILED( hr = Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/HUD/allyHealthBar.dds", mAllyHealthBar ) ) )
	{
		return hr;
	}

	if( FAILED( hr = Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/HUD/allyHealthFrame.dds", mAllyHealthFrame ) ) )
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
