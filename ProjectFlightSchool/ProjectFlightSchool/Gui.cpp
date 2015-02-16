#include "Gui.h"

void Gui::ActivateUpgradeShipWindow()
{
	mShipWindow.Activate();
}

void Gui::DeActivateUpgradeShipWindow()
{
	mShipWindow.DeActivate();
}

void Gui::ActivateUpgradePlayerWindow()
{
	mPlayerWindow.Activate();
}

void Gui::DeActivateUpgradePlayerWindow()
{
	mPlayerWindow.DeActivate();
}

HRESULT Gui::Update( GuiUpdate guiUpdate )
{
	HRESULT result = S_OK;

	mNrOfRemotePlayer = guiUpdate.mNrOfPlayerNames;

	//Updating remote player names
	DirectX::XMMATRIX view, proj;
	Graphics::GetInstance()->GetViewMatrix( view );
	Graphics::GetInstance()->GetProjectionMatrix( proj );
	view = DirectX::XMMatrixTranspose( view );
	proj = DirectX::XMMatrixTranspose( proj );

	for( UINT i = 0; i < guiUpdate.mNrOfPlayerNames; i++ )
	{
		DirectX::XMFLOAT4 textPos = XMFLOAT4( guiUpdate.mPlayerNames[i].mRemotePlayerPos.x, guiUpdate.mPlayerNames[i].mRemotePlayerPos.y + 3.25f, guiUpdate.mPlayerNames[i].mRemotePlayerPos.z, 1.0f );
		DirectX::XMStoreFloat4( &textPos, DirectX::XMVector4Transform( DirectX::XMLoadFloat4( &textPos ), DirectX::XMMatrixMultiply( view, proj ) ) );

		textPos.x /= textPos.w;
		textPos.y /= textPos.w;

		textPos.x = ( textPos.x + 1.0f ) * ( 0.5f * Input::GetInstance()->mScreenWidth );
		textPos.y = ( 1.0f - textPos.y ) * ( 0.5f * Input::GetInstance()->mScreenHeight );
		
		if( guiUpdate.mPlayerNames[i].mRemotePlayerTeamID == guiUpdate.mPlayerTeamID )
		{
			mPlayerNames[guiUpdate.mPlayerNames[i].mRemotePlayerID].SetText( guiUpdate.mPlayerNames[i].mRemotePlayerName, COLOR_CYAN );
		}
		else
		{
			mPlayerNames[guiUpdate.mPlayerNames[i].mRemotePlayerID].SetText( guiUpdate.mPlayerNames[i].mRemotePlayerName, COLOR_RED );
		}

		mPlayerNames[guiUpdate.mPlayerNames[i].mRemotePlayerID].SetPosition( textPos.x - mPlayerNames[guiUpdate.mPlayerNames[i].mRemotePlayerID].GetMiddleXPoint( mPlayerNames[guiUpdate.mPlayerNames[i].mRemotePlayerID].GetText(), mPlayerNames[guiUpdate.mPlayerNames[i].mRemotePlayerID].GetScale() ), textPos.y );
	}
	
	//-------------------------------------------------------------


	result = mRadar->Update( guiUpdate.mPlayerPos, guiUpdate.mRadarObjects, guiUpdate.mNrOfObjects );

	mHealtBar->Update( guiUpdate.mNrOfAllies, guiUpdate.mAlliesHP, guiUpdate.mShipHP );

	mPlayerHP		= (int)( guiUpdate.mPlayerHP * 100 );
	mPlayerXP		= (int)( guiUpdate.mPlayerXP * 100 );
	mPlayerShield	= (int)( guiUpdate.mPlayerShield * 100 );
	mExperience		= guiUpdate.mPlayerXP;

	if ( mShipWindow.IsActive() )
	{
		mShipWindow.Update( guiUpdate.deltaTime );
	}

	if ( mPlayerWindow.IsActive() )
	{
		mPlayerWindow.Update( guiUpdate.deltaTime );
	}

	return result;

}

HRESULT Gui::Render()
{

	for( UINT i = 0; i < mNrOfRemotePlayer; i++ )
	{
		mPlayerNames[i].Render();
	}

	mHealtBar->Render();

	HRESULT result = mRadar->Render();

	///////////////////
	//Player hp, shield & xp
	///////////////////
	RenderManager::GetInstance()->AddObject2dToList( mPlayerBar, mPlayerHealthXPTopLeftCorner, mSizePlayerHealthXP );
	std::string renderText;

	mFont.WriteText( "Hp", 106.0f, 858.0f, 2.9f );
	renderText = std::to_string( mPlayerHP );
	renderText += "%";
	mFont.WriteText( renderText, 95.0f, 896.0f, 2.9f );

	mFont.WriteText( "Shield", 68.0f, 945.0f, 2.9f );
	renderText = std::to_string( mPlayerShield );
	renderText += "%";
	mFont.WriteText( renderText, 95.0f, 983.0f, 2.9f );

	mFont.WriteText( "Xp", 316.0f, 871.0f, 3.8f );
	renderText = std::to_string( mPlayerXP );
	renderText += "%";
	mFont.WriteText( renderText, 297.0f, 930.0f, 3.8f );


	////////////////
	//Level up
	////////////////
	if( mExperience >= 1 )
	{

		RenderManager::GetInstance()->AddObject2dToList( mLevelUp, mTopLeftCompWithPlayerHealthXP, mSizeLevelUp );

		renderText = "!";
		renderText += std::to_string( (int)mExperience );
		mFont.WriteText( renderText, (mTopLeftCompWithPlayerHealthXP.x + 75.0f ), ( mTopLeftCompWithPlayerHealthXP.y + 66.0f ), 4.8f );
	}

	if ( mShipWindow.IsActive() )
	{
		mShipWindow.Render();
	}

	if ( mPlayerWindow.IsActive() )
	{
		mPlayerWindow.Render();
	}

	return result;
}

HRESULT Gui::Initialize()
{
	mNrOfRemotePlayer = 0;

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

	for( int i = 0; i < MAX_REMOTE_PLAYERS - 1; i++ )
	{
		mPlayerNames[i].Initialize( "../Content/Assets/GUI/Fonts/final_font/", "", 0, 0, 1.5f );
	}

	mFont.Initialize( "../Content/Assets/GUI/Fonts/final_font/" );

	mPlayerHP		= 0;
	mPlayerXP		= 0;
	mPlayerShield	= 0;

	mExperience		= 0.0f;

	result = Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/GUI/HUD/playerHealth+XP2.dds", mPlayerBar );
	if( FAILED( result ) )
	{
		return result;
	}

	result = Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/GUI/HUD/levelUp.dds", mLevelUp );
	if( FAILED( result ) )
	{
		return result;
	}

	mSizePlayerHealthXP					= XMFLOAT2( 440.0f, 280.0f );
	mPlayerHealthXPTopLeftCorner		= XMFLOAT2( 0.0f, ( Input::GetInstance()->mScreenHeight - mSizePlayerHealthXP.y ) );
	mSizeLevelUp						= XMFLOAT2( (float)( mSizePlayerHealthXP.x / 2.16f ), (float)( mSizePlayerHealthXP.y / 1.4f ) );
	mTopLeftCompWithPlayerHealthXP		= XMFLOAT2( (float)( mSizePlayerHealthXP.x / 3.14f ), ( mPlayerHealthXPTopLeftCorner.y - (float)( mSizeLevelUp.y / 1.45f ) ) );

	mShipWindow.Initialize();
	mPlayerWindow.Initialize();

	return result;
}

void Gui::Release()
{
	mRadar->Release();
	SAFE_DELETE( mRadar );

	mHealtBar->Release();
	SAFE_DELETE( mHealtBar );
	
	for( int i = 0; i < MAX_REMOTE_PLAYERS - 1; i++ )
	{
		mPlayerNames[i].Release();
	}

	mFont.Release();

	mShipWindow.Release();
	mPlayerWindow.Release();
}

Gui::Gui()
{
	mNrOfRemotePlayer	= 0;
	mRadar				= nullptr;
	mHealtBar			= nullptr;
}

Gui::~Gui()
{

}

bool Gui::UpgradeShipWindowIsActive()
{
	return mShipWindow.IsActive();
}

bool Gui::UpgradePlayerWindowIsActive()
{
	return mPlayerWindow.IsActive();
}