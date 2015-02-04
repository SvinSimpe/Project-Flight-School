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
	XMFLOAT2				mSizeAllyHealth;
	float					mSpaceAllyHealth;
	float					mSpaceAllyHealthToBar;
	int						mNrOfHealths;

	AssetID					mAllyHealthBar;
	XMFLOAT2				mSizeAllyHealthBar;
	float					mSpaceAllyHealthBar;
	float					mSpaceAllyHealthBarToEdge;

	AssetID					mAllyHealthFrame;
	XMFLOAT2				mSizeAllyHealthFrame;

	AssetID					mPlayerBar;
	XMFLOAT2				mSizePlayerHealthXP;

	AssetID					mLevelUp;
	XMFLOAT2				mSizeLevelUp;
	XMFLOAT2				mTopLeftCompWithPlayerHealthXP;

	AssetID					mShipHealth;
	XMFLOAT2				mSizeShipHealth;
	float					mSizeShipHealthTop;
	float					mSpaceShipHealth;
	XMFLOAT2				mStartShipHealth;
	XMFLOAT2				mEndShipHealth;

	AssetID					mShipHealthBar;
	XMFLOAT2				mSizeShipHealthBar;

protected:

public:

private:

protected:

public:
	HRESULT	Update( DirectX::XMFLOAT3 playerPos, RADAR_UPDATE_INFO radarObjects[], UINT nrOfObjects );
	HRESULT	Render( int nrOfAllies, float alliesHP[], float playerHP, float playerShield, float playerXp, float shipHP );
	HRESULT	Initialize();
	void	Release();
			Gui();
			~Gui();

};


#endif