#ifndef UPGRADESHIPWINDOW_H
#define UPGRADESHIPWINDOW_H

#include "Button.h"
#include "Font.h"
#include "RenderManager.h"

#define	MAX_NR_OF_UPGRADES 3

struct ButtonStruct
{
	Button*		buttons;
	XMFLOAT2	topLeftCorner;
};

class UpgradeShipWindow
{
private:
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
	void	Render();
	void	Release();
	HRESULT	Initialize();
	UpgradeShipWindow();
	~UpgradeShipWindow();

};

#endif // !UPGRADESHIPWINDOW_H