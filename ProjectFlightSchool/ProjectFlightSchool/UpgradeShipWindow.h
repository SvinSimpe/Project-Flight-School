#ifndef UPGRADESHIPWINDOW_H
#define UPGRADESHIPWINDOW_H

#include "Button.h"
#include "RenderManager.h"
#include "Events.h"
#include "EventManager.h"

#define	MAX_NR_OF_UPGRADES 3

struct ButtonStruct
{
	Button*		buttons;
	int			nrOfButtons;
	int			nrOfFilled;

	int Pressed()
	{
		int result = 0;
		for( int i = 0; i < nrOfButtons; i++ )
		{
			if( buttons[i].LeftMousePressed() )
			{
				if( i < nrOfFilled )
				{
					result = -1;
				}
				else
				{
					result = 1;
				}
			}
		}
		return result;
	}

	void Update( float deltaTime )
	{
		for( int i = 0; i < nrOfButtons; i++ )
		{
			buttons[i].Update( deltaTime );
		}
	}

	void Render()
	{
		for( int i = 0; i < nrOfFilled; i++ )
		{
			buttons[i].Render();
		}
	}
};

class UpgradeShipWindow
{
private:
	int				mTeam;
	bool			mIsActive;
	AssetID			mUpgradeWindow;
	XMFLOAT2		mTopLeftCorner;
	XMFLOAT2		mSize;
	ButtonStruct	mTurretButtons;
	ButtonStruct	mForceFieldButtons;
	ButtonStruct	mBuffButtons;
	ButtonStruct	mEngineButtons;

protected:
public:

private:
	void	HandleInput();
protected:
public:
	void	EventListener( IEventPtr eventPtr );
	void	Activate();
	void	DeActivate();
	void	Update( float deltaTime );
	void	Render();
	void	Release();
	HRESULT	Initialize();
	UpgradeShipWindow();
	~UpgradeShipWindow();

	bool	IsActive() const;
};

#endif // !UPGRADESHIPWINDOW_H