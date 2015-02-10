#ifndef GUI_H
#define GUI_H

#include "Radar.h"
#include "Text.h"
#include "Font.h"
#include "HealthBars.h"
#define MAX_REMOTE_PLAYERS 14

struct GuiUpdate
{
	//Radar update info
	DirectX::XMFLOAT3 mPlayerPos;
	RADAR_UPDATE_INFO *mRadarObjects;
	UINT mNrOfObjects;
	//------------------------------

	//Player names update info
	DirectX::XMFLOAT3 mRemotePlayerPos;
	std::string mRemotePlayerName;
	int mRemotePlayerTeamID;
	int mRemotePlayerID;
	int mPlayerTeamID;
	bool mUpdateRemotePlayerName;
	//------------------------------
};

class Gui
{


	private:
		Radar*		mRadar;
		HealthBar*	mHealtBar;

		Text	mPlayerNames[MAX_REMOTE_PLAYERS - 1]; //Don't need to store the local player's name

		Font					mFont;
		int						mScreenWidth;
		int						mScreenHeight;

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

		HRESULT	Update( GuiUpdate guiUpdate );
		HRESULT	Render( int nrOfAllies, float alliesHP[], float playerHP, float playerShield, float playerXp, float shipHP );
		HRESULT	Initialize();
		void	Release();
				Gui();
				~Gui();
};


#endif