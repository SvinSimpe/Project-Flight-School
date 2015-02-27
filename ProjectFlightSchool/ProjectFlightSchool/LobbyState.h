#ifndef LOBBYSTATE_H
#define LOBBYSTATE_H

#include "BaseState.h"
#include "Font.h"
#include "Events.h"
#include "EventManager.h"
#include "MovingButton.h"
#include "Button.h"
#include "Client.h"

struct LobbyPlayer
{
	int				ID;
	std::string		name;
	int				team;
	XMFLOAT2		position;
	XMFLOAT2		size = XMFLOAT2( 328.0f, 64.0f );
	Button			button;
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

	public:

	// Class functions
	private:
		void	EventListener( IEventPtr newEvent );

	protected:
		void	HandleInput();
	public:
		HRESULT Update( float deltaTime );
		HRESULT Render();
		void	OnEnter();
		void	OnExit();
		void	Reset();

		HRESULT Initialize();
		void	Release();
				LobbyState();
				~LobbyState();
};

#endif