#ifndef LIGHTMANAGER_H
#define LIGHTMANAGER_H

#include <Graphics.h>
#include <Windows.h>
#include "Events.h"
#include "EventManager.h"

class LightManager
{
	private:
		int				mNumActivePointLights;
		PointLight*		mPointLights[MAX_NUM_POINTLIGHTS];

		LightStructure	mLightStructure;

	protected:
	public:

	private:
	protected:
	public:
		void			EventListener( IEventPtr newEvent );
		int				GetNumActivePointLights();
		LightStructure* GetLightStructure();
		HRESULT			Initialize();
		void			Release();
						LightManager();
		virtual			~LightManager();
};
#endif