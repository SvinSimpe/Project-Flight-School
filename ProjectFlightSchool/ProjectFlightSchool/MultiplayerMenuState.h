#ifndef MULTIPLAYERMENUSTATE_H
#define MULTIPLAYERMENUSTATE_H

#include "BaseMenuState.h"

class MultiplayerMenuState : public BaseMenuState
{
	private:
	protected:
	public:

	private:
		void	HandleInput();
	protected:
	public:
		HRESULT Update( float deltaTime );
		HRESULT Render();
		void	OnEnter();
		void	OnExit();
		void	Reset();
		HRESULT Initialize();
		void	Release();
		MultiplayerMenuState();
		~MultiplayerMenuState();
};
#endif