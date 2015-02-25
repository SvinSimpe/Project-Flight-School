#ifndef BASESTATE_H
#define BASESTATE_H

#include "Client.h"
#include "Graphics.h"
#include "RenderManager.h"

// State-tracking constants
const int START_MENU_STATE		= 0;
const int JOIN_MENU_STATE		= 1;
const int MULTI_MENU_STATE		= 2;
const int OPTIONS_MENU_STATE	= 3;
const int PLAY_STATE			= 4;
const int LOBBY_STATE			= 5;
const int NR_OF_STATES			= 6;

class BaseState
{
	// Class members
	private:
	protected:
		int				mStateType;
	public:

	// Class functions
	private:
	protected:
	public:
		virtual HRESULT Update( float deltaTime )	= 0;
		virtual HRESULT Render()					= 0;
		virtual void	OnEnter()					= 0;
		virtual void	OnExit()					= 0;
		virtual void	Reset()						= 0;
		int				GetStateType();	

		virtual HRESULT Initialize()				= 0;
		virtual void	Release()					= 0;
						BaseState();
		virtual			~BaseState();
};

#endif