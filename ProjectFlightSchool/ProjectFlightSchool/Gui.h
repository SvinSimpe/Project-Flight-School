#ifndef GUI_H
#define GUI_H

#include "Radar.h"
#include "Font.h"

class Gui
{

private:
	Font					mFont;
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
	XMFLOAT2				mSizePlayerHealthXP;
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
	HRESULT	Render( int nrOfAllies, float alliesHP[], float playerHP );
	HRESULT	Initialize();
	void	Release();
			Gui();
			~Gui();

};


#endif