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
	int						mNrOfHealths;
	XMFLOAT2				mSizeAllyHealth;
	XMFLOAT2				mSizeAllyHealthBar;
	XMFLOAT2				mSizeAllyHealthFrame;
	float					mSpaceAllyHealthBar;
	float					mSpaceAllyHealth;
	float					mSpaceAllyHealthBarToEdge;
	float					mSpaceAllyHealthToBar;

protected:

public:

private:

protected:

public:
	HRESULT	Update( DirectX::XMFLOAT3 playerPos, RADAR_UPDATE_INFO radarObjects[], UINT nrOfObjects );
	HRESULT	Render( int nrOfAllys, float allysHP[] );
	HRESULT	Initialize();
	void	Release();
			Gui();
			~Gui();

};


#endif