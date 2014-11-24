#ifndef BASESTATE_H
#define BASESTATE_H

#include <Windows.h>

enum StateType{ STATE_TYPE_START_MENU, STATE_TYPE_PLAY };

class BaseState
{
	// Class members
	private:
	protected:
		StateType		mStateType;
	public:

	// Class functions
	private:
	protected:
	public:
		virtual HRESULT Update( float deltaTime )	= 0;
		virtual HRESULT Render()					= 0;
		virtual HRESULT Initialize()				= 0;
		virtual void	OnEnter()					= 0;
		virtual void	OnExit()					= 0;
		virtual void	Reset()						= 0;
		virtual void	Release()					= 0;
		StateType		GetStateType();		
						BaseState();
		virtual			~BaseState();
};

#endif