#include "LightManager.h"		

LightStructure* LightManager::GetLightStructure()
{
	for( int i = 0; i < NUM_POINTLIGHTS; i++ )
		mLightStructure.pointLights[i] = *mPointLights[i];
	return &mLightStructure;
}

HRESULT	LightManager::Initialize()
{
	for( int i = 0; i < NUM_POINTLIGHTS; i++ )
	{
		mPointLights[i] = new PointLight;
		mPointLights[i]->position		= DirectX::XMFLOAT4( (float)i * 5.0f, 15.0f, 0.0f, 0.0f );
		mPointLights[i]->colorAndRadius	= DirectX::XMFLOAT4( 0.6f, 0.4f, 0.4f, 10.0f ); 
	}

	return S_OK;
}

void LightManager::Release()
{
	for( int i = 0; i < NUM_POINTLIGHTS; i++ )
		SAFE_DELETE( mPointLights[i] );
}

LightManager::LightManager()
{
	for( int i = 0; i < NUM_POINTLIGHTS; i++ )
		mPointLights[i] = nullptr;
}

LightManager::~LightManager()
{

}