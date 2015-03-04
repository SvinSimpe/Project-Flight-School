#ifndef STARTMENUSTATE_H
#define STARTMENUSTATE_H

#include "BaseMenuState.h"
#include "InputSquare.h"
#include "SoundBufferHandler.h"

class StartMenuState : public BaseMenuState
{
	// Class members
	private:
		static const int
			CREATE			= 0,
			JOIN			= 1,
			OPTIONS			= 2,
			EXIT			= 3,
			BUTTON_AMOUNT	= 4;

		int	mStreamSoundAsset;

	protected:
	public:

	// Class functions
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
				StartMenuState();
		virtual	~StartMenuState();
};

#endif