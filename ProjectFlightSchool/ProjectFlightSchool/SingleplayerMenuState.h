#ifndef SINGLEPLAYERMENUSTATE_H
#define SINGLEPLAYERMENUSTATE_H

#include "BaseMenuState.h"

class SingleplayerMenuState : public BaseMenuState
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
		SingleplayerMenuState();
		~SingleplayerMenuState();
};
#endif