#ifndef OPTIONSMENUSTATE_H
#define OPTIONSMENUSTATE_H

#include "BaseMenuState.h"
#include "SoundBufferHandler.h"

class OptionsMenuState : public BaseMenuState
{
	private:
		static const int
			HELP			= 0,
			FULLSCREEN		= 1,
			SOUND			= 2,
			BACK			= 3,
			BUTTON_AMOUNT	= 4;
		bool	mInOptions;
		Button	mInOptionsBackground;

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
				OptionsMenuState();
		virtual ~OptionsMenuState();
};
#endif