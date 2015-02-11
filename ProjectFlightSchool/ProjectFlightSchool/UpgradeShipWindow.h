#ifndef UPGRADESHIPWINDOW_H
#define UPGRADESHIPWINDOW_H

#include "Input.h"
#include "Button.h"
#include "Font.h"
#include "RenderManager.h"

#define	MAX_NR_OF_UPGRADES 3

struct UpgradeButton
{
	Button* emptyButton		= nullptr;
	Button*	filledButton	= nullptr;	
	bool	filled			= false;
};

class UpgradeShipWindow
{
private:
	UpgradeButton	mTurretButtons[MAX_NR_OF_UPGRADES];
	UpgradeButton	mForceFieldButtons[MAX_NR_OF_UPGRADES];
	UpgradeButton	mBuffButtons[MAX_NR_OF_UPGRADES];
	UpgradeButton	mEngineButtons[MAX_NR_OF_UPGRADES * 2];
	Font			mFont;
	AssetID			mUpgradeWindow;

protected:
public:

private:
	void	HandleInput();
protected:
public:
	HRESULT Update( float deltaTime );
	void	Render();
	void	Release();
	HRESULT	Initialize();
	UpgradeShipWindow();
	~UpgradeShipWindow();

};

#endif // !UPGRADESHIPWINDOW_H