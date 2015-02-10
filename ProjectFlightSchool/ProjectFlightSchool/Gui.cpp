#include "Gui.h"

HRESULT Gui::Update( GuiUpdate guiUpdate )
{
	HRESULT result = S_OK;
	
	if( guiUpdate.mUpdateRemotePlayerName )
	{
		//Updating remote player names
		DirectX::XMMATRIX view, proj;
		Graphics::GetInstance()->GetViewMatrix( view );
		Graphics::GetInstance()->GetProjectionMatrix( proj );
		view = DirectX::XMMatrixTranspose( view );
		proj = DirectX::XMMatrixTranspose( proj );

		
		DirectX::XMFLOAT4 textPos = XMFLOAT4( guiUpdate.mRemotePlayerPos.x, guiUpdate.mRemotePlayerPos.y + 3.25f, guiUpdate.mRemotePlayerPos.z, 1.0f );
		DirectX::XMStoreFloat4( &textPos, DirectX::XMVector4Transform( DirectX::XMLoadFloat4( &textPos ), DirectX::XMMatrixMultiply( view, proj ) ) );

		textPos.x /= textPos.w;
		textPos.y /= textPos.w;

		textPos.x = ( textPos.x + 1.0f ) * ( 0.5f * Input::GetInstance()->mScreenWidth );
		textPos.y = ( 1.0f - textPos.y ) * ( 0.5f * Input::GetInstance()->mScreenHeight );
		
		if( guiUpdate.mRemotePlayerTeamID == guiUpdate.mPlayerTeamID )
		{
			mPlayerNames[guiUpdate.mRemotePlayerID].SetText( guiUpdate.mRemotePlayerName, COLOR_CYAN );
		}
		else
		{
			mPlayerNames[guiUpdate.mRemotePlayerID].SetText( guiUpdate.mRemotePlayerName, COLOR_RED );
		}

		mPlayerNames[guiUpdate.mRemotePlayerID].SetPosition( textPos.x - mPlayerNames[guiUpdate.mRemotePlayerID].GetMiddleXPoint( mPlayerNames[guiUpdate.mRemotePlayerID].GetText(), mPlayerNames[guiUpdate.mRemotePlayerID].GetScale() ), textPos.y );
		//-------------------------------------------------------------
	}
	
	result = mRadar->Update( guiUpdate.mPlayerPos, guiUpdate.mRadarObjects, guiUpdate.mNrOfObjects );
	return result;

}

HRESULT Gui::Render( int nrOfAllies, float alliesHP[], float playerHP, float playerShield, float playerXp, float shipHP )
{

	for( int i = 0; i < MAX_REMOTE_PLAYERS - 1; i++ )
	{
		mPlayerNames[i].Render();
	}

	HRESULT result = mRadar->Render();
	XMFLOAT2 topLeft;
	XMFLOAT2 widthHeight;

	mHealtBar->Update( nrOfAllies, alliesHP, playerHP, playerShield, playerXp, shipHP );
	mHealtBar->Render();
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
	/*topLeft.x							= ( mScreenWidth * 0.5f ) - ( mSizeShipHealthBar.x * 0.5f );
	topLeft.y							= 0.0f;
	XMFLOAT2 shipHealthBarTopLeft		= topLeft;
	RenderManager::GetInstance()->AddObject2dToList( mShipHealthBar, topLeft, mSizeShipHealthBar );

	nrOfHealthsToRender = (int)( mNrOfHealths * shipHP );
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
	}*/

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

	mHealtBar = new HealthBar();
	if( FAILED( result = mHealtBar->Initialize() ) )
	{
		return result;
	}

	mScreenHeight						= Input::GetInstance()->mScreenHeight;
	mScreenWidth						= Input::GetInstance()->mScreenWidth;
	
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
	mSizeShipHealthTop					= ( ( mEndShipHealth.x - mStartShipHealth.x - ( ( 20 - 1 ) * mSpaceShipHealth ) ) / 20 );


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