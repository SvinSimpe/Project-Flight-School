#ifndef STARTMENUSTATE_H
#define STARTMENUSTATE_H

#include "BaseState.h"
#include "Input.h"
#include "EventManager.h"
#include "Events.h"
#include "Entry.h"

class StartMenuState : public BaseState
{
	// Class members
	private:
		Entry			mStartServerClient;	// The square used to start a server and client connection, upper left part of the screen
		Entry			mStartClient;		// The square used to start a client connection, lower right part of the screen
	protected:
	public:

	// Class functions
	private:
		void			HandleInput();
	protected:
	public:
		virtual HRESULT Update( float deltaTime );
		virtual HRESULT Render();
		virtual void	OnEnter();
		virtual void	OnExit();
		virtual void	Reset();

		virtual HRESULT Initialize();
		virtual void	Release();
						StartMenuState();
		virtual			~StartMenuState();
};

#endif