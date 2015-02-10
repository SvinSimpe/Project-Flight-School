#include "Gui.h"

HRESULT Gui::Update( DirectX::XMFLOAT3 playerPos, RADAR_UPDATE_INFO radarObjects[], UINT nrOfObjects, DirectX::XMFLOAT3 remotePlayerPos, std::string remotePlayerName, int remotePlayerTeamID, int remotePlayerID, int playerTeamID, bool updateRemotePlayerName )
{
	HRESULT result = S_OK;
	
	if( updateRemotePlayerName )
	{
		//Updating remote player names
		DirectX::XMMATRIX view, proj;
		Graphics::GetInstance()->GetViewMatrix( view );
		Graphics::GetInstance()->GetProjectionMatrix( proj );
		view = DirectX::XMMatrixTranspose( view );
		proj = DirectX::XMMatrixTranspose( proj );

	
		DirectX::XMFLOAT4 textPos = XMFLOAT4( remotePlayerPos.x, remotePlayerPos.y + 3.25f, remotePlayerPos.z, 1.0f );
		DirectX::XMStoreFloat4( &textPos, DirectX::XMVector4Transform( DirectX::XMLoadFloat4( &textPos ), DirectX::XMMatrixMultiply( view, proj ) ) );

		textPos.x /= textPos.w;
		textPos.y /= textPos.w;

		textPos.x = ( textPos.x + 1.0f ) * ( 0.5f * Input::GetInstance()->mScreenWidth );
		textPos.y = ( 1.0f - textPos.y ) * ( 0.5f * Input::GetInstance()->mScreenHeight );
		
		if( remotePlayerTeamID == playerTeamID )
		{
			mPlayerNames[remotePlayerID].SetText( remotePlayerName, COLOR_CYAN );
		}
		else
		{
			mPlayerNames[remotePlayerID].SetText( remotePlayerName, COLOR_RED );
		}

		mPlayerNames[remotePlayerID].SetPosition( textPos.x - mPlayerNames[remotePlayerID].GetMiddleXPoint( mPlayerNames[remotePlayerID].GetText(), mPlayerNames[remotePlayerID].GetScale() ), textPos.y );
		//-------------------------------------------------------------
	}

	result = mRadar->Update( playerPos, radarObjects, nrOfObjects );
	return result;

}

HRESULT Gui::Render( int nrOfAllies, float alliesHP[], float playerHP, float playerShield, float playerXp, float shipHP )
{

	mRadar->Render();

	for( int i = 0; i < MAX_REMOTE_PLAYERS - 1; i++ )
	{
		mPlayerNames[i].Render();
	}

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

		nrOfHealthsToRender = (int)( 20.0f * alliesHP[i] );

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
	int renderHealth = (int)( playerHP * 100 );
	std::string renderText;
	if ( renderHealth >= 0 )
	{
		renderText = std::to_string( renderHealth );
		renderText += "%";
		mFont.WriteText( renderText, 95.0f, 896.0f, 2.9f );
	}

	int renderShield = (int)( playerShield * 100 );
	if ( renderShield >= 0 )
	{
		mFont.WriteText( "Shield", 68.0f, 945.0f, 2.9f );
		renderText = std::to_string( renderShield );
		renderText += "%";
		mFont.WriteText( renderText, 95.0f, 983.0f, 2.9f );
	}

	int renderXp = (int)( playerXp * 100 );
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
		topLeft.x	= topLeft.x + mTopLeftCompWithPlayerHealthXP.x;
		topLeft.y	= topLeft.y - mTopLeftCompWithPlayerHealthXP.y;
		widthHeight	= mSizeLevelUp;
		RenderManager::GetInstance()->AddObject2dToList( mLevelUp, topLeft, widthHeight );

		int renderLevelUp = (int)playerXp;
		renderText = "!";
		renderText += std::to_string( renderLevelUp );
		mFont.WriteText( renderText, (topLeft.x + 75.0f ), ( topLeft.y + 66.0f ), 4.8f );
	}

	///////////////////////
	//Ship Bar
	///////////////////////
	topLeft.x							= ( mScreenWidth / 2 ) - ( mSizeShipHealthBar.x / 2 );
	topLeft.y							= 0.0f;
	XMFLOAT2 shipHealthBarTopLeft		= topLeft;
	RenderManager::GetInstance()->AddObject2dToList( mShipHealthBar, topLeft, mSizeShipHealthBar );

	nrOfHealthsToRender = (int)( 20.0f * shipHP );
	offSetForHealths	= shipHealthBarTopLeft.x + mEndShipHealth.x;

	if ( nrOfHealthsToRender > 0 )
	{
		for ( int i = 0; i < nrOfHealthsToRender; i++ )
		{
			topLeft.x	= offSetForHealths - mSizeShipHealth.x;
			topLeft.y	= mStartShipHealth.y;
			widthHeight	= mSizeShipHealth;
			RenderManager::GetInstance()->AddObject2dToList( mShipHealth, topLeft, widthHeight );
			offSetForHealths -= ( mSpaceShipHealth + mSizeShipHealthTop );
		}
	}

	return result;
}

HRESULT Gui::Initialize()
{

	for( int i = 0; i < MAX_REMOTE_PLAYERS - 1; i++ )
	{
		mPlayerNames[i].Initialize( "../Content/Assets/Fonts/final_font/", "", 0, 0, 1.5f );
	}

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
	mSizeLevelUp						= XMFLOAT2( (float)( mSizePlayerHealthXP.x / 2.16f ), (float)( mSizePlayerHealthXP.y / 1.4f ) );
	mTopLeftCompWithPlayerHealthXP		= XMFLOAT2( (float)( mSizePlayerHealthXP.x / 3.14f ), (float)( mSizeLevelUp.y / 1.45f ) );
	mSizeShipHealthBar					= XMFLOAT2( 986.0f, 113.0f );
	mSpaceShipHealth					= 3.0f;
	mStartShipHealth.x					= (float)( mSizeShipHealthBar.x / 3.94f );
	mStartShipHealth.y					= (float)( mSizeShipHealthBar.y / 12.20f );
	mEndShipHealth.x					= (float)( mSizeShipHealthBar.x / 1.08f );
	mEndShipHealth.y					= (float)( mSizeShipHealthBar.y / 1.55f );
	mSizeShipHealth.x					= (float)( mSizeShipHealthBar.x / 10.3f );
	mSizeShipHealth.y					= ( mEndShipHealth.y - mStartShipHealth.y );
	mSizeShipHealthTop					= ( ( mEndShipHealth.x - mStartShipHealth.x - ( ( mNrOfHealths - 1 ) * mSpaceShipHealth ) ) / mNrOfHealths );

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
	result = Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/HUD/playerHealth+XP2.dds", mPlayerBar );
	if( FAILED( result ) )
	{
		return result;
	}
	result = Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/HUD/levelUp.dds", mLevelUp );
	if( FAILED( result ) )
	{
		return result;
	}
	result = Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/HUD/shipHealth.dds", mShipHealth );
	if( FAILED( result ) )
	{
		return result;
	}
	result = Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/HUD/shipHealthBar.dds", mShipHealthBar );
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
	mRadar = nullptr;
}

Gui::~Gui()
{

}