#ifndef CREATEMENUSTATE_H
#define CREATEMENUSTATE_H

#include "BaseMenuState.h"

class CreateMenuState : public BaseMenuState
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
		CreateMenuState();
		~CreateMenuState();
};
#endif