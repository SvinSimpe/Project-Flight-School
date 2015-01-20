#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "StartMenuState.h"
#include "CreateMenuState.h"
#include "JoinMenuState.h"
#include "MultiplayerMenuState.h"
#include "OptionsMenuState.h"
#include "SingleplayerMenuState.h"
#include "PlayState.h"

class StateMachine
{
	// Class members
	private:
		BaseState**			mStates;
		int					mCurrentState;

	protected:
	public:

	// Class functions
	private:
		void	EventListener( IEventPtr newEvent );
	protected:
	public:
		HRESULT Update( float deltaTime );
		HRESULT Render();
		HRESULT ChangeState( const int NEW_STATE );
		void	ResetState( const int STATE );

		HRESULT Initialize();
		void	Release();
				StateMachine();
				~StateMachine();
};

#endif