#ifndef LOBBYOWNERSTATE_H
#define LOBBYOWNERSTATE_H

#include "LobbyState.h"

class LobbyOwnerState : public LobbyState
{
	// Class members
	private:
		MovingButton	mStartButton;

	protected:
	public:

	// Class functions
	private:
		void	HandleInput();

	protected:
	public:
		HRESULT Update( float deltaTime );
		HRESULT Render();
		void	OnExit();

		HRESULT Initialize();
		void	Release();
				LobbyOwnerState();
				~LobbyOwnerState();
};

#endif