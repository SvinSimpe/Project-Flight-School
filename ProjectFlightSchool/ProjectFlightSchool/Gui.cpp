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

void Gui::ActivateEnergyCellsShowing()
{
	mEnergyCellsShowing = true;
}

void Gui::DeActivateEnergyCellsShowing()
{
	mEnergyCellsShowing = false;
}

void Gui::ActivateInGameWindow()
{
	mInGameWindow.Activate();
}

void Gui::DeActivateInGameWindow()
{
	mInGameWindow.DeActivate();
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
	mExperience		= guiUpdate.mLevel;

	if ( mShipWindow.IsActive() )
	{
		mShipWindow.Update( guiUpdate.deltaTime );
	}

	if ( mPlayerWindow.IsActive() )
	{
		mPlayerWindow.Update( guiUpdate.deltaTime );
	}

	if ( mInGameWindow.IsActive() )
	{
		mInGameWindow.Update( guiUpdate.deltaTime );
	}

	while( !mShipWindow.gEventList.empty() )
	{
		gEventList.push_front( mShipWindow.gEventList.back() );
		mShipWindow.gEventList.pop_back();
	}

	if( guiUpdate.mEndGame )
	{
		mEndGameWindow.Update( guiUpdate.deltaTime, guiUpdate.mWonGame );
		mEndGame = guiUpdate.mEndGame;
	}

	return result;

}

HRESULT Gui::Render()
{
	HRESULT result = S_OK;

	if( !mEndGame )
	{
		for( UINT i = 0; i < mNrOfRemotePlayer; i++ )
		{
			mPlayerNames[i].Render();
		}

		mHealtBar->Render();

		result = mRadar->Render();

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
		if( mExperience >= 1 && !mPlayerWindow.IsActive() )
		{

			RenderManager::GetInstance()->AddObject2dToList( mLevelUp, mTopLeftLevelUp, mSizeLevelUp );

			renderText = "+";
			renderText += std::to_string( (int)mExperience );
			mFont.WriteText( renderText, (mTopLeftLevelUp.x + 75.0f ), ( mTopLeftLevelUp.y + 66.0f ), 4.8f );
		}

		if ( mShipWindow.IsActive() )
		{
			mShipWindow.Render();
		}

		if ( mPlayerWindow.IsActive() )
		{
			mPlayerWindow.Render();
		}

		if ( mInGameWindow.IsActive() )
		{
			mInGameWindow.Render();
		}

		renderText = std::to_string( mShipWindow.GetNrOfEnergyCells() ) + " of " + std::to_string( mNeededEnergyCells ) + " energy cells";
		mFont.WriteText( renderText, 1680.0f, 280.0f, 2.0f );
	}
	else
	{
		mEndGameWindow.Render();
	}
	

	return result;
}

HRESULT Gui::Initialize( UINT neededEnergyCells )
{
	mNrOfRemotePlayer	= 0;

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

	mExperience		= 0;

	result = Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/GUI/HUD/hexagonDouble.dds", mPlayerBar );
	if( FAILED( result ) )
	{
		return result;
	}

	result = Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/GUI/HUD/hexagonSingle.dds", mLevelUp );
	if( FAILED( result ) )
	{
		return result;
	}

	mSizePlayerHealthXP					= XMFLOAT2( 480.0f, 284.0f );
	mPlayerHealthXPTopLeftCorner		= XMFLOAT2( 0.0f, ( Input::GetInstance()->mScreenHeight - mSizePlayerHealthXP.y ) );
	mSizeLevelUp						= XMFLOAT2( 200.0f, 208.0f );
	mTopLeftLevelUp						= XMFLOAT2( 149.0f, 641.0f );

	mShipWindow.Initialize();
	mPlayerWindow.Initialize();
	mInGameWindow.Initialize();
	mEndGameWindow.Initialize();

	mEndGame			= false;
	mNeededEnergyCells	= neededEnergyCells;

	return result;
}

void Gui::Reset()
{
	Release();
	Initialize( 6 );
	mEndGameWindow.Reset();
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
	mEndGameWindow.Release();
}

Gui::Gui()
{
	mNrOfRemotePlayer	= 0;
	mRadar				= nullptr;
	mHealtBar			= nullptr;
	mEnergyCellsShowing	= true;
	mNeededEnergyCells	= 0;
	mEndGame			= false;
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

bool Gui::InGameWindowIsActive()
{
	return mInGameWindow.IsActive();
}

bool Gui::EnergyCellsActive()
{
	return mEnergyCellsShowing;
}