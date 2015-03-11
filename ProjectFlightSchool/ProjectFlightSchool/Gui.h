#ifndef GUI_H
#define GUI_H

#include "Radar.h"
#include "Text.h"
#include "Font.h"
#include "HealthBars.h"
#include "UpgradeShipWindow.h"
#include "UpgradePlayerWindow.h"
#include "InGameWindow.h"
#include "EndGameWindow.h"

#define MAX_REMOTE_PLAYERS 14

struct PlayerName
{
	DirectX::XMFLOAT3 mRemotePlayerPos;
	std::string mRemotePlayerName;
	int mRemotePlayerTeamID;
	int mRemotePlayerID;
	
};

struct GuiUpdate
{
	float deltaTime = 0.0f;;

	//Radar update info
	RADAR_UPDATE_INFO* mRadarObjects	= nullptr;
	UINT mNrOfObjects					= 0;
	DirectX::XMFLOAT3 mPlayerPos		= DirectX::XMFLOAT3( 0.0f, 0.0f, 0.0f );
	//------------------------------

	//Player names update info
	PlayerName* mPlayerNames	= nullptr;
	UINT mNrOfPlayerNames		= 0;
	int mPlayerTeamID			= 0;
	//------------------------------

	//Health bar update info
	float* mAlliesHP	= nullptr;
	int mNrOfAllies		= 0;
	float mShipHP		= 0.0f;
	//------------------------------

	//Player update info
	float	mPlayerHP;
	float	mPlayerXP;
	int		mLevel;
	//------------------------------

	//End game update info
	bool mEndGame = false;
	bool mWonGame = false;
	//------------------------------

};

class Gui
{
	private:
		UINT		mNrOfRemotePlayer;

		Radar*				mRadar;
		UpgradeShipWindow	mShipWindow;
		UpgradePlayerWindow	mPlayerWindow;
		InGameWindow		mInGameWindow;
		EndGameWindow		mEndGameWindow;
		bool				mEnergyCellsShowing;
		bool				mEndGame;

		HealthBar*	mHealtBar;

		Text		mPlayerNames[MAX_REMOTE_PLAYERS - 1]; //Don't need to store the local player's name

		Font		mFont;

		int			mPlayerHP;
		int			mPlayerXP;

		int			mExperience;

		Image		mHPXP;
		Image		mLevelUp;
		Image		mDeviderHPXP;

		int			mNeededEnergyCells;
		
	protected:
	public:
		std::list<IEventPtr>	gEventList;

	private:
	protected:
	public:
		void	SetTeamID( UINT teamID );
		void	ActivateUpgradeShipWindow();
		void	DeActivateUpgradeShipWindow();
		void	ActivateUpgradePlayerWindow();
		void	DeActivateUpgradePlayerWindow();
		void	ActivateEnergyCellsShowing();
		void	DeActivateEnergyCellsShowing();
		void	ActivateInGameWindow();
		void	DeActivateInGameWindow();
		HRESULT	Update( GuiUpdate guiUpdate );
		HRESULT	Render();
		HRESULT	Initialize( UINT neededEnergyCells );
		void	Reset();
		void	Release();
				Gui();
				~Gui();

		bool	UpgradeShipWindowIsActive();
		bool	UpgradePlayerWindowIsActive();
		bool	InGameWindowIsActive();
		bool	EnergyCellsActive();
};
#endif