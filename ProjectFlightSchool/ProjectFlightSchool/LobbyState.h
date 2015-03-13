#ifndef LOBBYSTATE_H
#define LOBBYSTATE_H

#include "BaseState.h"
#include "Font.h"
#include "Events.h"
#include "EventManager.h"
#include "MovingButton.h"
#include "Button.h"
#include "Client.h"
#include "LoadOutMenu.h"
#include "SoundBufferHandler.h"

struct LobbyPlayer
{
	UINT			ID = (UINT)-1;
	std::string		name = "";
	UINT			team = (UINT)-1; 
	XMFLOAT2		position = XMFLOAT2(0.0f, 0.0f);
	XMFLOAT2		size = XMFLOAT2( 328.0f, 64.0f );
	Button			button;
	bool			thisPlayer = false;
	bool			isReady = false;
};

class LobbyState : public BaseState
{
	// Class members
	private:
	protected:
		float						mTeamOneXPos;
		float						mTeamTwoXPos;
		std::vector<LobbyPlayer*>	mPlayers;
		Font						mFont;
		AssetID						mBackground;
		bool						mActive;
		MovingButton				mBackButton;
		Image						mReadyImg;
		Button						mCheckBox;
		LoadOutMenu					mLoadOutMenu;
		Button						mChooseWeaponButton;
		Image						mChooseWeaponText;
		UINT						mMyID;
		float						mGameCountdown;
		bool						mGameCountdownStarted;
		Image						mGameCountdownBack;
		bool						mTeamsLocked;

	public:

	// Class functions
	private:
		void	EventListener( IEventPtr newEvent );

	protected:
		void	StartGameCountdown();
		void	HandleInput();

	public:
		HRESULT Update( float deltaTime );
		HRESULT Render( float deltaTime );
		void	OnEnter();
		void	OnExit();
		void	Reset();

		HRESULT Initialize();
		void	Release();
				LobbyState();
				~LobbyState();
};

#endif