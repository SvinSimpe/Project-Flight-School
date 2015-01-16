#ifndef BASEMENUSTATE_H
#define BASEMENUSTATE_H

#include "BaseState.h"
#include "Input.h"
#include "EventManager.h"
#include "Events.h"
#include "Image.h"
#include "MenuButton.h"

class BaseMenuState : public BaseState
{
	private:
	protected:
		AssetID			mBackground;
	public:

	private:
	protected:
		virtual void	HandleInput() = 0;
	public:
		virtual HRESULT Update( float deltaTime );
		virtual HRESULT Render();
		virtual HRESULT Initialize();
		virtual void	Release();
						BaseMenuState();
		virtual			~BaseMenuState();
};
#endif