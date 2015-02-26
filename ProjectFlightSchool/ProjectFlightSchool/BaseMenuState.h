#ifndef BASEMENUSTATE_H
#define BASEMENUSTATE_H

#include "BaseState.h"
#include "Input.h"
#include "Events.h"
#include "MovingButton.h"
#include "Font.h"
#include "InputSquare.h"

class BaseMenuState : public BaseState
{
	private:
	protected:
		AssetID						mBackground;
		std::vector<MovingButton*>	mButtons;
		Font						mText;

	public:

	private:
	protected:
		virtual void	HandleInput()				= 0;
	public:
		virtual HRESULT Update( float deltaTime );
		virtual HRESULT Render();
		virtual void	OnEnter();
		virtual void	OnExit();
		virtual void	Reset()						= 0;
		virtual HRESULT Initialize();
		virtual void	Release();
						BaseMenuState();
		virtual			~BaseMenuState();
};
#endif