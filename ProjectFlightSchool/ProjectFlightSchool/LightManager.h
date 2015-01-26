#ifndef LIGHTMANAGER_H
#define LIGHTMANAGER_H

#include <Graphics.h>
#include <Windows.h>

class LightManager
{
	private:
		int				mNumActivePointLights;
		PointLight*		mPointLights[NUM_POINTLIGHTS];

		LightStructure	mLightStructure;

	protected:
	public:

	private:
	protected:
	public:
		LightStructure* GetLightStructure();
		HRESULT	Initialize();
		void	Release();
				LightManager();
		virtual ~LightManager();
};
#endif