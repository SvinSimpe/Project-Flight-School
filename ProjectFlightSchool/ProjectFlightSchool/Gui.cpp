#include "Gui.h"

HRESULT Gui::Update( DirectX::XMFLOAT3 playerPos, RADAR_UPDATE_INFO radarObjects[], UINT nrOfObjects )
{
	HRESULT result = mRadar->Update( playerPos, radarObjects, nrOfObjects );
	return result;
}

HRESULT Gui::Render( int nrOfAllies, float alliesHP[], float playerHP, float playerShield, float playerXp )
{
	HRESULT result = mRadar->Render();
	XMFLOAT2 topLeft;
	XMFLOAT2 widthHeight;


	//////////////////////
	//Allies hp
	//////////////////////
	XMFLOAT2	latestAllyHealthTopLeft;
	XMFLOAT2	latestAllyHealthBottomRight;
	float		offSetForHealths = mSpaceAllyHealthToBar;
	int			nrOfHealthsToRender;
	for ( int i = 0; i < nrOfAllies; i++ )
	{
		topLeft.x						= mScreenWidth - mSpaceAllyHealthBarToEdge - mSizeAllyHealthBar.x;
		topLeft.y						= mScreenHeight - mSpaceAllyHealthBarToEdge - mSizeAllyHealthBar.y;
		widthHeight						= mSizeAllyHealthBar;
		latestAllyHealthTopLeft			= topLeft;
		latestAllyHealthBottomRight.x	= topLeft.x + mSizeAllyHealthBar.x;
		latestAllyHealthBottomRight.y	= topLeft.y + mSizeAllyHealthBar.y;
		RenderManager::GetInstance()->AddObject2dToList( mAllyHealthBar, topLeft, widthHeight );

		nrOfHealthsToRender = ( 20.0f * alliesHP[i] );

		if ( nrOfHealthsToRender > 0 )
		{
			for ( int j = 0; j < nrOfHealthsToRender; j++ )
			{

				topLeft.x = latestAllyHealthBottomRight.x - offSetForHealths - mSizeAllyHealth.x;
				topLeft.y = latestAllyHealthBottomRight.y - mSpaceAllyHealthToBar - mSizeAllyHealth.y;
				widthHeight = mSizeAllyHealth;
				RenderManager::GetInstance()->AddObject2dToList( mAllyHealth, topLeft, widthHeight );
				offSetForHealths += mSpaceAllyHealth + mSizeAllyHealth.x;
			}
		}
	}

	if ( nrOfAllies > 0 )
	{
		topLeft.x = latestAllyHealthTopLeft.x - mSpaceAllyHealthBar - 4.0f;
		topLeft.y = latestAllyHealthTopLeft.y - mSpaceAllyHealthBar - 4.0f;
		widthHeight = mSizeAllyHealthFrame;
		RenderManager::GetInstance()->AddObject2dToList( mAllyHealthFrame, topLeft, widthHeight );
	}

	///////////////////
	//Player hp, shield & xp
	///////////////////
	topLeft.x	= 0.0f;
	topLeft.y	= mScreenHeight - mSizePlayerHealthXP.y;
	widthHeight	= mSizePlayerHealthXP;
	RenderManager::GetInstance()->AddObject2dToList( mPlayerBar, topLeft, widthHeight );
	
	mFont.WriteText( "Hp", 106.0f, 858.0f, 2.9f );
	int renderHealth = playerHP * 100;
	std::string renderText;
	if ( renderHealth >= 0 )
	{
		renderText = std::to_string( renderHealth );
		renderText += "%";
		mFont.WriteText( renderText, 95.0f, 896.0f, 2.9f );
	}

	int renderShield = playerShield * 100;
	if ( renderShield >= 0 )
	{
		mFont.WriteText( "Shield", 68.0f, 945.0f, 2.9f );
		renderText = std::to_string( renderShield );
		renderText += "%";
		mFont.WriteText( renderText, 95.0f, 983.0f, 2.9f );
	}

	int renderXp = playerXp * 100;
	if ( renderXp >= 0 )
	{
		mFont.WriteText( "Xp", 316.0f, 871.0f, 3.8f );
		renderText = std::to_string( renderXp );
		renderText += "%";
		mFont.WriteText( renderText, 297.0f, 930.0f, 3.8f );
	}

	////////////////
	//Level up
	////////////////
	if( playerXp >= 1 )
	{
		topLeft.x	= 140.0f;
		topLeft.y	= mScreenHeight - 218 - mSizeLevelUp.y;
		widthHeight	= mSizeLevelUp;
		RenderManager::GetInstance()->AddObject2dToList( mLevelUp, topLeft, widthHeight );

		int renderLevelUp = playerXp;
		renderText = "!";
		renderText += std::to_string( renderLevelUp );
		mFont.WriteText( renderText, (topLeft.x + 75.0f ), ( topLeft.y + 66.0f ), 4.8f );
	}

	return result;
}

HRESULT Gui::Initialize()
{
	mFont.Initialize( "../Content/Assets/Fonts/final_font/" );
	mRadar			= new Radar();
	HRESULT result	= mRadar->Initialize();
	if( FAILED( result ) )
	{
		return result;
	}
	mScreenHeight						= Input::GetInstance()->mScreenHeight;
	mScreenWidth						= Input::GetInstance()->mScreenWidth;
	mNrOfHealths						= 20;
	mSpaceAllyHealthToBar				= 6.0f;
	mSpaceAllyHealth					= 2.0f;
	mSpaceAllyHealthBar					= 15.0f;
	mSpaceAllyHealthBarToEdge			= 27.0f;
	mSizeAllyHealthBar					= XMFLOAT2( 215.0f, 40.0f );
	mSizeAllyHealth						= XMFLOAT2( ( ( mSizeAllyHealthBar.x - ( mSpaceAllyHealthToBar * 2 ) - ( ( mNrOfHealths ) * mSpaceAllyHealth ) ) / mNrOfHealths ), ( mSizeAllyHealthBar.y - ( mSpaceAllyHealthToBar * 2 ) ) );
	mSizeAllyHealthFrame				= XMFLOAT2( 110.0f, 110.0f );
	mSizePlayerHealthXP					= XMFLOAT2( 440.0f, 280.0f );
	mSizeLevelUp						= XMFLOAT2( 204.0f, 200.0f );

	result = Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/HUD/allyhealth.dds", mAllyHealth );
	if( FAILED( result ) )
	{
		return result;
	}
	result = Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/HUD/allyHealthBar.dds", mAllyHealthBar );
	if( FAILED( result ) )
	{
		return result;
	}
	result = Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/HUD/allyHealthFrame.dds", mAllyHealthFrame );
	if( FAILED( result ) )
	{
		return result;
	}
	result = Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/HUD/playerHealth+XP.dds", mPlayerBar );
	if( FAILED( result ) )
	{
		return result;
	}
	result = Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/HUD/levelUp.dds", mLevelUp );
	if( FAILED( result ) )
	{
		return result;
	}

	return result;
}

void Gui::Release()
{
	mRadar->Release();
	SAFE_DELETE( mRadar );
}

Gui::Gui()
{
	mRadar				= nullptr;
}

Gui::~Gui()
{

}