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

	void Render()
	{
		button.Render();
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
	XMFLOAT2			mTextPos;

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
	void	Release();
	HRESULT	Initialize();
	UpgradePlayerWindow();
	~UpgradePlayerWindow();

	bool	IsActive() const;
};

#endif // !UPGRADEPLAYERWINDOW_H