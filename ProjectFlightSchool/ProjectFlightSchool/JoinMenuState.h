#ifndef JOINMENUSTATE_H
#define JOINMENUSTATE_H

#include "BaseMenuState.h"
#include "InputSquare.h"

class JoinMenuState : public BaseMenuState
{
	private:
		static const int
			JOIN			= 0,
			BACK			= 1,
			MIKAEL			= 2,
			BUTTON_AMOUNT	= 3;

		InputSquare	mIPBox;
		InputSquare	mPortBox;
		InputSquare	mNameBox;

	protected:
	public:

	private:
		void	HandleInput();
	protected:
	public:
		HRESULT Update( float deltaTime );
		HRESULT Render( float deltaTime );
		void	OnEnter();
		void	OnExit();
		void	Reset();
		HRESULT Initialize();
		void	Release();
				JoinMenuState();
		virtual ~JoinMenuState();
};
#endif