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

	widthHeight.x = 20.0f;
	widthHeight.y = 20.0f;

	topLeftCorner.x = (float)( Input::GetInstance()->mCurrentMousePos.x );
	topLeftCorner.y = (float)( Input::GetInstance()->mCurrentMousePos.y );

	RenderManager::GetInstance()->AddObject2dToList( mUpgradeWindow, topLeftCorner, widthHeight );

	mHealthButton.Render();
	mSpeedButton.Render();
	mMeleeButton.Render();
	mRangeButton.Render();

	mFont.WriteText( "Melee", mMeleeButton.button.GetPosition().x + mTextPos.x, mMeleeButton.button.GetPosition().y + mTextPos.y, 2.4f );
	mFont.WriteText( "Range", mRangeButton.button.GetPosition().x + mTextPos.x, mRangeButton.button.GetPosition().y + mTextPos.y, 2.4f );
	mFont.WriteText( "Speed", mSpeedButton.button.GetPosition().x + mTextPos.x, mSpeedButton.button.GetPosition().y + mTextPos.y, 2.4f );
	mFont.WriteText( "Health", mHealthButton.button.GetPosition().x + mTextPos.x, mHealthButton.button.GetPosition().y + mTextPos.y, 2.4f );

	mFont.WriteText( "+" + std::to_string( mMeleeButton.nrOfLevels ), mMeleeButton.button.GetPosition().x + mTextPos.x + 100.0f, mMeleeButton.button.GetPosition().y + mTextPos.y, 2.4f );
	mFont.WriteText( "+" + std::to_string( mRangeButton.nrOfLevels ), mRangeButton.button.GetPosition().x + mTextPos.x + 100.0f, mRangeButton.button.GetPosition().y + mTextPos.y, 2.4f );
	mFont.WriteText( "+" + std::to_string( mSpeedButton.nrOfLevels ), mSpeedButton.button.GetPosition().x + mTextPos.x + 100.0f, mSpeedButton.button.GetPosition().y + mTextPos.y, 2.4f );
	mFont.WriteText( "+" + std::to_string( mHealthButton.nrOfLevels ), mHealthButton.button.GetPosition().x + mTextPos.x + 100.0f, mHealthButton.button.GetPosition().y + mTextPos.y, 2.4f );
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

	XMFLOAT2 sizeBox		= XMFLOAT2( 200.0f, 208.0f );
	XMFLOAT2 meleeTopLeft	= XMFLOAT2( 142.0f, 651.0f );
	XMFLOAT2 rangeTopLeft	= XMFLOAT2( 332.0f, 649.0f );
	XMFLOAT2 speedTopLeft	= XMFLOAT2( 42.0f, 490.0f );
	XMFLOAT2 healthTopLeft	= XMFLOAT2( 234.0f, 485.0f );

	mHealthButton.button.Initialize( "../Content/Assets/GUI/HUD/Upgrade2.dds", healthTopLeft.x, healthTopLeft.y, sizeBox.x, sizeBox.y );
	mSpeedButton.button.Initialize( "../Content/Assets/GUI/HUD/Upgrade2.dds", speedTopLeft.x, speedTopLeft.y, sizeBox.x, sizeBox.y );
	mMeleeButton.button.Initialize( "../Content/Assets/GUI/HUD/Upgrade2.dds", meleeTopLeft.x, meleeTopLeft.y, sizeBox.x, sizeBox.y );
	mRangeButton.button.Initialize( "../Content/Assets/GUI/HUD/Upgrade2.dds", rangeTopLeft.x, rangeTopLeft.y, sizeBox.x, sizeBox.y );

	mTextPos = XMFLOAT2( 40.0f, 93.0f );

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