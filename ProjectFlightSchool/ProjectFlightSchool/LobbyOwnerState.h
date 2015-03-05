#ifndef LOBBYOWNERSTATE_H
#define LOBBYOWNERSTATE_H

#include "LobbyState.h"

class LobbyOwnerState : public LobbyState
{
	// Class members
	private:
		static const int
			EVEN_TEAMS		= 0,
			ALL_READY		= 1,
			FULL_GAME		= 2,
			WARNING_AMOUNT	= 3;

		struct activatedString
		{
			string text = "";
			float timer = 0.0f;
		};

		MovingButton	mStartButton;
		const float		MAX_TEXT_TIME = 5.0f;
		float			mCurrentTextTime;
		activatedString	mWarningTexts[WARNING_AMOUNT];

	protected:
	public:

	// Class functions
	private:
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