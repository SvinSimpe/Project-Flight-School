#include "UpgradeShipWindow.h"

void UpgradeShipWindow::Update( float deltaTime )
{
	mForceFieldButtons.Update( deltaTime );
	mTurretButtons.Update( deltaTime );
	mBuffButtons.Update( deltaTime );
	mEngineButtons.Update( deltaTime );

	int pressed = mForceFieldButtons.Pressed();
	if( pressed != 0 )
	{
		if( pressed == 1 )
		{
			MessageBox( NULL, L"Level up ForceField", L"Error", MB_OK );
		}
		else
		{
			MessageBox( NULL, L"Level down ForceField", L"Error", MB_OK );
		}
	}
	else
	{
		int pressed = mTurretButtons.Pressed();
		if ( pressed != 0 )
		{
			if ( pressed == 1 )
			{
				MessageBox( NULL, L"Level up Turret", L"Error", MB_OK );
			}
			else
			{
				MessageBox( NULL, L"Level down Turret", L"Error", MB_OK );
			}
		}
		else
		{
			int pressed = mBuffButtons.Pressed();
			if ( pressed != 0 )
			{
				if ( pressed == 1 )
				{
					MessageBox( NULL, L"Level up Buff", L"Error", MB_OK );
				}
				else
				{
					MessageBox( NULL, L"Level down Buff", L"Error", MB_OK );
				}
			}
			else
			{
				int pressed = mEngineButtons.Pressed();
				if ( pressed != 0 )
				{
					if ( pressed == 1 )
					{
						MessageBox( NULL, L"Level up Engine", L"Error", MB_OK );
					}
					else
					{
						MessageBox( NULL, L"Level down Engine", L"Error", MB_OK );
					}
				}
			}
		}
	}

}

void UpgradeShipWindow::Render()
{
	XMFLOAT2 topLeftCorner;
	XMFLOAT2 widthHeight;

	widthHeight.x = 20.0f;
	widthHeight.y = 20.0f;

	topLeftCorner.x = (float)( Input::GetInstance()->mCurrentMousePos.x );
	topLeftCorner.y = (float)( Input::GetInstance()->mCurrentMousePos.y );

	RenderManager::GetInstance()->AddObject2dToList( mUpgradeWindow, topLeftCorner, widthHeight );

	widthHeight.x = mSize.x;
	widthHeight.y = mSize.y;

	RenderManager::GetInstance()->AddObject2dToList( mUpgradeWindow, mTopLeftCorner, widthHeight );

	mTurretButtons.Render();
	mForceFieldButtons.Render();
	mBuffButtons.Render();
	mEngineButtons.Render();
}

void UpgradeShipWindow::Release()
{
	SAFE_DELETE_ARRAY( mTurretButtons.buttons );
	SAFE_DELETE_ARRAY( mForceFieldButtons.buttons );
	SAFE_DELETE_ARRAY( mBuffButtons.buttons );
	SAFE_DELETE_ARRAY( mEngineButtons.buttons );
}

HRESULT UpgradeShipWindow::Initialize()
{
	HRESULT result;

	result = Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/GUI/HUD/shipUpgradeMenu.dds", mUpgradeWindow );

	mSize.x = Input::GetInstance()->mScreenWidth * 0.45f;
	mSize.y = Input::GetInstance()->mScreenHeight * 0.26f;

	mTopLeftCorner.x = (float)( Input::GetInstance()->mScreenWidth / 2.0f ) - (float)( mSize.x / 2.0f );
	mTopLeftCorner.y = Input::GetInstance()->mScreenHeight - mSize.y;
	
	mTurretButtons.nrOfFilled		= 2;
	mForceFieldButtons.nrOfFilled	= 2;
	mBuffButtons.nrOfFilled			= 2;
	mEngineButtons.nrOfFilled		= 2;

	mTurretButtons.nrOfButtons		= MAX_NR_OF_UPGRADES;
	mForceFieldButtons.nrOfButtons	= MAX_NR_OF_UPGRADES;
	mBuffButtons.nrOfButtons		= MAX_NR_OF_UPGRADES;
	mEngineButtons.nrOfButtons		= MAX_NR_OF_UPGRADES * 2;

	mTurretButtons.buttons		= new Button[MAX_NR_OF_UPGRADES];
	mForceFieldButtons.buttons	= new Button[MAX_NR_OF_UPGRADES];
	mBuffButtons.buttons		= new Button[MAX_NR_OF_UPGRADES];
	mEngineButtons.buttons		= new Button[MAX_NR_OF_UPGRADES * 2];

	float sizeBox					= 48.0f;
	XMFLOAT2 turretTopLeft			= XMFLOAT2( 605.0f, 955.0f );
	XMFLOAT2 forceFieldTopLeft		= XMFLOAT2( 817.0f, 955.0f );
	XMFLOAT2 buffButtonTopLeft		= XMFLOAT2( 1014.0f, 955.0f );
	XMFLOAT2 engineButtonTopLeft	= XMFLOAT2( 1160.0f, 955.0f );

	XMFLOAT2 unused[MAX_NR_OF_UPGRADES] = { XMFLOAT2( 0.0f, 0.0f ), XMFLOAT2( 58.0f, 0.0f ), XMFLOAT2( 29.0f, 50.0f ) };


	for (int i = 0; i < MAX_NR_OF_UPGRADES; i++)
	{
		mTurretButtons.buttons[i].Initialize( "../Content/Assets/HUD/checkedCheckBox.dds", turretTopLeft.x + unused[i].x, turretTopLeft.y + unused[i].y, sizeBox, sizeBox );
		mForceFieldButtons.buttons[i].Initialize( "../Content/Assets/HUD/checkedCheckBox.dds", forceFieldTopLeft.x + unused[i].x, forceFieldTopLeft.y + unused[i].y, sizeBox, sizeBox );
		mBuffButtons.buttons[i].Initialize( "../Content/Assets/HUD/checkedCheckBox.dds", buffButtonTopLeft.x + unused[i].x, buffButtonTopLeft.y + unused[i].y, sizeBox, sizeBox );
	}

	XMFLOAT2 stillNotUsed[MAX_NR_OF_UPGRADES * 2] = { XMFLOAT2( 0.0f, 0.0f ), XMFLOAT2( 58.0f, 0.0f ), XMFLOAT2( 116.0f, 0.0f ), XMFLOAT2( 0.0f, 50.0f ), XMFLOAT2( 58.0f, 50.0f ), XMFLOAT2( 116.0f, 50.0f ) };
	for (int i = 0; i < MAX_NR_OF_UPGRADES * 2; i++)
	{
		mEngineButtons.buttons[i].Initialize( "../Content/Assets/HUD/checkedCheckBox.dds", engineButtonTopLeft.x + stillNotUsed[i].x, engineButtonTopLeft.y + stillNotUsed[i].y, sizeBox, sizeBox );
	}

	return result;
}

UpgradeShipWindow::UpgradeShipWindow()
{

}

UpgradeShipWindow::~UpgradeShipWindow()
{

}