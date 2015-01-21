#ifndef BASEMENUSTATE_H
#define BASEMENUSTATE_H

#include "BaseState.h"
#include "Input.h"
#include "EventManager.h"
#include "Events.h"
#include "Button.h"
#include "Font.h"

class BaseMenuState : public BaseState
{
	private:
	protected:
		AssetID					mBackground;
		std::vector<Button*>	mButtons;
		Font					mText;

	public:

	private:
	protected:
		virtual void	HandleInput()	= 0;
	public:
		virtual HRESULT Update( float deltaTime );
		virtual HRESULT Render();
		virtual void	OnEnter()		= 0;
		virtual void	OnExit();
		virtual void	Reset()			= 0;
		virtual HRESULT Initialize();
		virtual void	Release();
						BaseMenuState();
		virtual			~BaseMenuState();
};
#endif