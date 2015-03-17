#include "Gui.h"

void Gui::SetTeamID( UINT teamID )
{
	mShipWindow.SetTeamID( teamID );
}

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

bool Gui::CheckWin()
{
	return mShipWindow.GetNrOfEnergyCells() == mNeededEnergyCells;
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
		//Player hp & xp
		///////////////////
		mHPXP.Render();
		std::string renderText;

		renderText = "HP " + std::to_string( mPlayerHP ) + "%";
		mFont.WriteText( renderText, 100.0f - mFont.GetMiddleXPoint( renderText, 3.0 ), mHPXP.GetPosition().y + 100.0f - 25.0f - 15.0f, 3.0f, COLOR_CYAN );//100 is half the size of the hexagon, 25 is a offset from the middle and 15 is half the size of the text (3.0*10/2)

		renderText = "XP " + std::to_string( mPlayerXP ) + "%";
		mFont.WriteText( renderText, 100.0f - mFont.GetMiddleXPoint( renderText, 3.0 ), mHPXP.GetPosition().y + 100.0f + 25.0f - 15.0f, 3.0f, COLOR_CYAN );
		mDeviderHPXP.Render();

		////////////////
		//Level up
		////////////////
		if( mExperience >= 1 )
		{

			mLevelUp.Render();

			renderText = "+";
			renderText += std::to_string( (int)mExperience );
			mFont.WriteText( renderText, mLevelUp.GetPosition().x + 100.0f - mFont.GetMiddleXPoint( renderText, 4.8f ), ( mLevelUp.GetPosition().y + 100.0f - 24.0f ), 4.8f, COLOR_CYAN ); //100 is half of the hexagon, 24 is half of the text height.
		}

		if ( mShipWindow.IsActive() )
		{
			mShipWindow.Render();
			renderText = std::to_string( mShipWindow.GetUnusedCells() );
			mFont.WriteText( renderText, ( (float)Input::GetInstance()->mScreenWidth * 0.64f ) - mFont.GetMiddleXPoint( renderText, 5.0f ), mHPXP.GetPosition().y + 100.0f + 25.0f - 15.0f, 5.0f, COLOR_CYAN );
		}

		if ( mPlayerWindow.IsActive() )
		{
			mPlayerWindow.Render();
		}

		if ( mInGameWindow.IsActive() )
		{
			mInGameWindow.Render();
		}
		else
		{
			renderText = std::to_string( mShipWindow.GetNrOfEnergyCells() ) + " of " + std::to_string( mNeededEnergyCells ) + " energy cells";
			mFont.WriteText( renderText, 1680.0f, 280.0f, 2.0f, COLOR_CYAN );

			renderText = std::to_string( mShipWindow.GetOtherShipNrOfEnergyCells() ) + " of " + std::to_string( mNeededEnergyCells ) + " energy cells";
			mFont.WriteText( renderText, 1680.0f, 305.0f, 2.0f, COLOR_RED );
		}
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

	mExperience		= 0;

	mHPXP.Initialize( "../Content/Assets/GUI/HUD/hexagonSingle.dds", 0.0f, Input::GetInstance()->mScreenHeight - 208.0f, 200.0f, 200.0f );
	mLevelUp.Initialize( "../Content/Assets/GUI/HUD/hexagonSingle.dds", 205.0f, Input::GetInstance()->mScreenHeight - 208.0f, 200.0f, 200.0f );
	mDeviderHPXP.Initialize( "../Content/Assets/GUI/HUD/HP_XPGlowDivider.dds", mHPXP.GetPosition().x + 100.0f - 70.0f, mHPXP.GetPosition().y + 100.0f - 10.0f, 140.0f, 20.0f );
	
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
	mHPXP.Release();
	mLevelUp.Release();
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