#ifndef GUI_H
#define GUI_H

#include "Radar.h"

class Gui
{

private:
	Radar*					mRadar;
	int						mScreenWidth;
	int						mScreenHeight;
	AssetID					mAllyHealth;
	AssetID					mAllyHealthBar;
	AssetID					mAllyHealthFrame;
	AssetID					mPlayerBar;

protected:

public:

private:

protected:

public:
	HRESULT	Update( DirectX::XMFLOAT3 playerPos, RADAR_UPDATE_INFO radarObjects[], UINT nrOfObjects );
	HRESULT	Render();
	HRESULT	Initialize();
	void	Release();
			Gui();
			~Gui();

};


#endif