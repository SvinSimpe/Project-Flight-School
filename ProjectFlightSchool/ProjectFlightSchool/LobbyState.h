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
	MovingButton	button;
};

class LobbyState : public BaseState
{
	// Class members
	private:
		std::vector<LobbyPlayer*>	mPlayers;
		Font						mFont;
		MovingButton				mStartButton;

	protected:
	public:

	// Class functions
	private:
		void	EventListener( IEventPtr newEvent );
		void	HandleInput();

	protected:
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