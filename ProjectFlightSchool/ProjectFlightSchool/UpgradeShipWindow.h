#ifndef UPGRADESHIPWINDOW_H
#define UPGRADESHIPWINDOW_H

#include "Input.h"
#include "Button.h"
#include "Font.h"
#include "RenderManager.h"

#define	MAX_NR_OF_UPGRADES 3

class UpgradeShipWindow
{
private:
	AssetID			mUpgradeWindow;

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