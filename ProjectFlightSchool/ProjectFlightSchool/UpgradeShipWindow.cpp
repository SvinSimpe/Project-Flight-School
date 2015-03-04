#include "UpgradeShipWindow.h"

void UpgradeShipWindow::EventListener( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Server_Change_Ship_Levels::GUID )
	{
		std::shared_ptr<Event_Server_Change_Ship_Levels> data = std::static_pointer_cast<Event_Server_Change_Ship_Levels>( eventPtr );
		if( data->ID() == mTeam )
		{
			mForceFieldButtons.nrOfFilled	= data->ShieldLevelChange();
			mTurretButtons.nrOfFilled		= data->TurretLevelChange();
			mBuffButtons.nrOfFilled			= data->BuffLevelChange();
			mEngineButtons.nrOfFilled		= data->EngineLevelChange();
			mNrOfEnergyCells				= data->NrOfEnergyCells();
		}
	}
	else if ( eventPtr->GetEventType() == Event_Local_Joined::GUID ) // Add a remote player to the list when they connect
	{
		std::shared_ptr<Event_Local_Joined> data = std::static_pointer_cast<Event_Local_Joined>( eventPtr );
		mTeam = data->TeamID();
	}
	else if ( eventPtr->GetEventType() == Event_Client_Died::GUID ) // Add a remote player to the list when they connect
	{
		std::shared_ptr<Event_Client_Died> data = std::static_pointer_cast<Event_Client_Died>( eventPtr );
		mIsActive = false;
	}

}

void UpgradeShipWindow::Activate()
{
	mIsActive = true;
}

void UpgradeShipWindow::DeActivate()
{
	mIsActive = false;
}

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
			IEventPtr E1( new Event_Client_Change_Ship_Levels( mTeam, 0, 1, 0, 0 ) );
			gEventList.push_front( E1 );
		}
		else
		{
			IEventPtr E1( new Event_Client_Change_Ship_Levels( mTeam, 0, -1, 0, 0 ) );
			gEventList.push_front( E1 );
		}
	}
	else
	{
		int pressed = mTurretButtons.Pressed();
		if ( pressed != 0 )
		{
			if ( pressed == 1 )
			{
				IEventPtr E1( new Event_Client_Change_Ship_Levels( mTeam, 1, 0, 0, 0 ) );
				gEventList.push_front( E1 );
			}
			else
			{
				IEventPtr E1( new Event_Client_Change_Ship_Levels( mTeam, -1, 0, 0, 0 ) );
				gEventList.push_front( E1 );
			}
		}
		else
		{
			int pressed = mBuffButtons.Pressed();
			if ( pressed != 0 )
			{
				if ( pressed == 1 )
				{
					IEventPtr E1( new Event_Client_Change_Ship_Levels( mTeam, 0, 0, 1, 0 ) );
					gEventList.push_front( E1 );
				}
				else
				{
					IEventPtr E1( new Event_Client_Change_Ship_Levels( mTeam, 0, 0, -1, 0 ) );
					gEventList.push_front( E1 );
				}
			}
			else
			{
				int pressed = mEngineButtons.Pressed();
				if ( pressed != 0 )
				{
					if ( pressed == 1 )
					{
						IEventPtr E1( new Event_Client_Change_Ship_Levels( mTeam, 0, 0, 0, 1 ) );
						gEventList.push_front( E1 );
					}
					else
					{
						IEventPtr E1( new Event_Client_Change_Ship_Levels( mTeam, 0, 0, 0, -1 ) );
						gEventList.push_front( E1 );
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
	gEventList	= std::list<IEventPtr>();
	mIsActive	= false;
	mTeam		= -1;

	HRESULT result;

	result = Graphics::GetInstance()->LoadStatic2dAsset( "../Content/Assets/GUI/HUD/shipUpgradeMenu.dds", mUpgradeWindow );

	mSize.x = Input::GetInstance()->mScreenWidth * 0.45f;
	mSize.y = Input::GetInstance()->mScreenHeight * 0.26296296f;

	mTopLeftCorner.x = (float)( Input::GetInstance()->mScreenWidth / 2.0f ) - (float)( mSize.x / 2.0f );
	mTopLeftCorner.y = Input::GetInstance()->mScreenHeight - mSize.y;
	
	mTurretButtons.nrOfFilled		= 3;
	mForceFieldButtons.nrOfFilled	= 3;
	mBuffButtons.nrOfFilled			= 3;
	mEngineButtons.nrOfFilled		= 6;

	mTurretButtons.nrOfButtons		= MAX_NR_OF_UPGRADES;
	mForceFieldButtons.nrOfButtons	= MAX_NR_OF_UPGRADES;
	mBuffButtons.nrOfButtons		= MAX_NR_OF_UPGRADES;
	mEngineButtons.nrOfButtons		= MAX_NR_OF_UPGRADES * 2;

	mTurretButtons.buttons		= new Button[MAX_NR_OF_UPGRADES];
	mForceFieldButtons.buttons	= new Button[MAX_NR_OF_UPGRADES];
	mBuffButtons.buttons		= new Button[MAX_NR_OF_UPGRADES];
	mEngineButtons.buttons		= new Button[MAX_NR_OF_UPGRADES * 2];

	float sizeBox					= 48.0f;
	XMFLOAT2 turretTopLeft			= XMFLOAT2( 605.0f, 954.0f );
	XMFLOAT2 forceFieldTopLeft		= XMFLOAT2( 817.0f, 954.0f );
	XMFLOAT2 buffButtonTopLeft		= XMFLOAT2( 1014.0f, 954.0f );
	XMFLOAT2 engineButtonTopLeft	= XMFLOAT2( 1160.0f, 954.0f );

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

	EventManager::GetInstance()->AddListener( &UpgradeShipWindow::EventListener, this, Event_Local_Joined::GUID );
	EventManager::GetInstance()->AddListener( &UpgradeShipWindow::EventListener, this, Event_Server_Change_Ship_Levels::GUID ); 

	return result;
}

UpgradeShipWindow::UpgradeShipWindow()
{
	mNrOfEnergyCells = 0;
}

UpgradeShipWindow::~UpgradeShipWindow()
{

}

bool UpgradeShipWindow::IsActive() const
{
	return mIsActive;
}

int UpgradeShipWindow::GetNrOfEnergyCells() const
{
	return mNrOfEnergyCells;
}