#ifndef LOBBYOWNERSTATE_H
#define LOBBYOWNERSTATE_H

#include "LobbyState.h"

class LobbyOwnerState : public LobbyState
{
	// Class members
	private:
		static const int
			ALL_READY		= 0,
			FULL_GAME		= 1,
			WARNING_AMOUNT	= 2;

		struct activatedString
		{
			string text = "";
			float timer = -1.0f;
		};

		MovingButton	mStartButton;
		const float		MAX_TEXT_TIME = 5.0f;
		float			mCurrentTextTime;
		activatedString	mWarningTexts[WARNING_AMOUNT];
		UINT			mMaxPlayers;

	protected:
	public:

	// Class functions
	private:
		void	EventListener( IEventPtr eventPtr );
		void	HandleInput();

	protected:
	public:
		HRESULT Update( float deltaTime );
		HRESULT Render( float deltaTime );
		void	OnExit();

		HRESULT Initialize();
		void	Release();
				LobbyOwnerState();
				~LobbyOwnerState();
};

#endif