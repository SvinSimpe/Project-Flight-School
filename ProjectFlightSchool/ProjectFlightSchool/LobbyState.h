#ifndef LOBBYSTATE_H
#define LOBBYSTATE_H

#include "BaseState.h"
#include "Font.h"
#include "Events.h"
#include "EventManager.h"

struct LobbyPlayer
{
	int			ID;
	std::string	name;
	int			team;
};

class LobbyState : public BaseState
{
	// Class members
	private:
		std::vector<LobbyPlayer>	mPlayers;
		Font						mFont;

	protected:
	public:

	// Class functions
	private:
		void	EventListener( IEventPtr newEvent );

	protected:
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