#ifndef UPGRADEPLAYERWINDOW_H
#define UPGRADEPLAYERWINDOW_H

#include "Button.h"
#include "RenderManager.h"
#include "Font.h"
#include "Events.h"
#include "EventManager.h"

#define	MAX_NR_OF_PLAYER_UPGRADES 5

struct UpgradeButtonStruct
{
	Button		button;
	int			nrOfLevels	= 1;
	Image		background;

	bool Pressed()
	{
		if( button.LeftMousePressed() )
		{
			return true;
		}
		return false;
	}

	void Update( float deltaTime )
	{
		button.Update( deltaTime );
	}

	void Reset()
	{
		nrOfLevels = 1;
	}

	void Render()
	{
		button.Render();
		background.Render();
	}
};

class UpgradePlayerWindow
{
private:
	bool				mIsActive;
	Font				mFont;
	AssetID				mUpgradeWindow;
	UpgradeButtonStruct	mSpeedButton;
	UpgradeButtonStruct	mHealthButton;
	UpgradeButtonStruct	mMeleeButton;
	UpgradeButtonStruct	mRangeButton;

protected:
public:

private:
	void	HandleInput();
protected:
public:
	void	Activate();
	void	DeActivate();
	void	Update( float deltaTime );
	void	Render();
	void	Reset();
	void	Release();
	HRESULT	Initialize();
	UpgradePlayerWindow();
	~UpgradePlayerWindow();

	bool	IsActive() const;
};

#endif // !UPGRADEPLAYERWINDOW_H