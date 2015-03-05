#include "UpgradePlayerWindow.h"

void UpgradePlayerWindow::Activate()
{
	mIsActive = true;
}

void UpgradePlayerWindow::DeActivate()
{
	mIsActive = false;
}

void UpgradePlayerWindow::Update( float deltaTime )
{
	mSpeedButton.Update( deltaTime );
	mHealthButton.Update( deltaTime );
	mMeleeButton.Update( deltaTime );
	mRangeButton.Update( deltaTime );

	if( mSpeedButton.Pressed() )
	{
		if ( mSpeedButton.nrOfLevels < MAX_NR_OF_PLAYER_UPGRADES )
		{
			mSpeedButton.nrOfLevels++;
			IEventPtr E1( new Event_Upgrade_Player( 1, 0, 0, 0 ) );
			EventManager::GetInstance()->QueueEvent( E1 );
		}
	}
	else if( mHealthButton.Pressed() )
	{
		if( mHealthButton.nrOfLevels < MAX_NR_OF_PLAYER_UPGRADES )
		{
			mHealthButton.nrOfLevels++;
			IEventPtr E1( new Event_Upgrade_Player( 0, 1, 0, 0 ) );
			EventManager::GetInstance()->QueueEvent( E1 );
		}
	}
	else if ( mMeleeButton.Pressed() )
	{
		if( mMeleeButton.nrOfLevels < MAX_NR_OF_PLAYER_UPGRADES )
		{
			mMeleeButton.nrOfLevels++;
			IEventPtr E1( new Event_Upgrade_Player( 0, 0, 1, 0 ) );
			EventManager::GetInstance()->QueueEvent( E1 );
		}
	}
	else if( mRangeButton.Pressed() )
	{
		if( mRangeButton.nrOfLevels < MAX_NR_OF_PLAYER_UPGRADES )
		{
			mRangeButton.nrOfLevels++;
			IEventPtr E1( new Event_Upgrade_Player( 0, 0, 0, 1 ) );
			EventManager::GetInstance()->QueueEvent( E1 );
		}
	}
}

void UpgradePlayerWindow::Render()
{
	XMFLOAT2 topLeftCorner;
	XMFLOAT2 widthHeight;

	mHealthButton.Render();
	mSpeedButton.Render();
	mMeleeButton.Render();
	mRangeButton.Render();

	std::string textToWrite = "Melee +" + std::to_string( mMeleeButton.nrOfLevels );
	mFont.WriteText( textToWrite, mMeleeButton.button.GetPosition().x + 100.0f - mFont.GetMiddleXPoint( textToWrite, 2.4f ), mMeleeButton.button.GetPosition().y + 100.0f - 12.0f, 2.4f, COLOR_CYAN ); //Size of hexagon is 200, half is 100. Text scale i x10 pixels, 2.4 is 24 pixels, half is 12.
	textToWrite = "Range +" + std::to_string( mRangeButton.nrOfLevels );
	mFont.WriteText( textToWrite, mRangeButton.button.GetPosition().x + 100.0f - mFont.GetMiddleXPoint( textToWrite, 2.4f ), mRangeButton.button.GetPosition().y + 100.0f - 12.0f, 2.4f, COLOR_CYAN );
	textToWrite = "Speed +" + std::to_string( mSpeedButton.nrOfLevels );
	mFont.WriteText( textToWrite, mSpeedButton.button.GetPosition().x + 100.0f - mFont.GetMiddleXPoint( textToWrite, 2.4f ), mSpeedButton.button.GetPosition().y + 100.0f - 12.0f, 2.4f, COLOR_CYAN );
	textToWrite = "Health +" + std::to_string( mHealthButton.nrOfLevels );
	mFont.WriteText( textToWrite, mHealthButton.button.GetPosition().x + 100.0f - mFont.GetMiddleXPoint( textToWrite, 2.4f ), mHealthButton.button.GetPosition().y + 100.0f - 12.0f, 2.4f, COLOR_CYAN );
}

void UpgradePlayerWindow::Release()
{
	mHealthButton.button.Release();
	mSpeedButton.button.Release();
	mMeleeButton.button.Release();
	mRangeButton.button.Release();

	mFont.Release();
}

HRESULT UpgradePlayerWindow::Initialize()
{
	mIsActive	= false;

	HRESULT result;

	result = Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/GUI/HUD/shipUpgradeMenu.dds", mUpgradeWindow );

	mHealthButton.nrOfLevels	= 1;
	mSpeedButton.nrOfLevels		= 1;
	mMeleeButton.nrOfLevels		= 1;
	mRangeButton.nrOfLevels		= 1;

	XMFLOAT2 sizeBox		= XMFLOAT2( 200.0f, 200.0f );
	XMFLOAT2 meleeTopLeft	= XMFLOAT2( 103.0f, 710.0f );
	XMFLOAT2 rangeTopLeft	= XMFLOAT2( 309.0f, 710.0f );
	XMFLOAT2 speedTopLeft	= XMFLOAT2( 0.0f, 550.0f );
	XMFLOAT2 healthTopLeft	= XMFLOAT2( 206.0f, 550.0f );
	mHealthButton.button.Initialize( "../Content/Assets/GUI/HUD/hexagonSingle.dds", healthTopLeft.x, healthTopLeft.y, sizeBox.x, sizeBox.y );
	mSpeedButton.button.Initialize( "../Content/Assets/GUI/HUD/hexagonSingle.dds", speedTopLeft.x, speedTopLeft.y, sizeBox.x, sizeBox.y );
	mMeleeButton.button.Initialize( "../Content/Assets/GUI/HUD/hexagonSingle.dds", meleeTopLeft.x, meleeTopLeft.y, sizeBox.x, sizeBox.y );
	mRangeButton.button.Initialize( "../Content/Assets/GUI/HUD/hexagonSingle.dds", rangeTopLeft.x, rangeTopLeft.y, sizeBox.x, sizeBox.y );

	float offsetX = 34.0f;
	float offsetY = 78.0f;
	mHealthButton.background.Initialize( "../Content/Assets/GUI/HUD/upgradeBackGround.dds", healthTopLeft.x + offsetX, healthTopLeft.y + offsetY, 132.0f, 44.0f );
	mSpeedButton.background.Initialize( "../Content/Assets/GUI/HUD/upgradeBackGround.dds", speedTopLeft.x + offsetX, speedTopLeft.y + offsetY, 132.0f, 44.0f );
	mMeleeButton.background.Initialize( "../Content/Assets/GUI/HUD/upgradeBackGround.dds", meleeTopLeft.x + offsetX, meleeTopLeft.y + offsetY, 132.0f, 44.0f );
	mRangeButton.background.Initialize( "../Content/Assets/GUI/HUD/upgradeBackGround.dds", rangeTopLeft.x + offsetX, rangeTopLeft.y + offsetY, 132.0f, 44.0f );


	mFont.Initialize( "../Content/Assets/GUI/Fonts/final_font/" );

	return result;
}

UpgradePlayerWindow::UpgradePlayerWindow()
{

}

UpgradePlayerWindow::~UpgradePlayerWindow()
{

}

bool UpgradePlayerWindow::IsActive() const
{
	return mIsActive;
}