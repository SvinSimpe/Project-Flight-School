#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "PlayState.h"
#include "StartMenuState.h"

// StateMachine constants
const int START_MENU_STATE	= 0;
const int PLAY_STATE		= 1;
const int NR_OF_STATES		= 2;

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