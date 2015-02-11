#include "LightManager.h"		

void LightManager::EventListener( IEventPtr newEvent )
{
	if ( newEvent->GetEventType() == Event_Add_Point_Light::GUID )
	{
		std::shared_ptr<Event_Add_Point_Light> data = std::static_pointer_cast<Event_Add_Point_Light>( newEvent );
		if( mNumActivePointLights < MAX_NUM_POINTLIGHTS )
		{
			PointLight* cmp	= (PointLight*)data->Light();
			bool adder		= true;
			for( int i = 0; i < mNumActivePointLights; i++ )
			{
				if (mPointLights[i] == cmp)
				{
					adder = false;
					break;
				}
			}
			if (adder)
			{
				mPointLights[mNumActivePointLights] = cmp;
				mNumActivePointLights++;
			}
		}
	}
	else if ( newEvent->GetEventType() == Event_Remove_Point_Light::GUID )
	{
		std::shared_ptr<Event_Remove_Point_Light> data = std::static_pointer_cast<Event_Remove_Point_Light>( newEvent );
		for( int i = 0; i < MAX_NUM_POINTLIGHTS; i++ )
		{
			void* cmp = data->Light();
			if( cmp != nullptr && mPointLights[i] == cmp )
			{
				mNumActivePointLights--;
				mPointLights[i]						= mPointLights[mNumActivePointLights];
				mPointLights[mNumActivePointLights] = nullptr;
				break;
			}
		}
	}
}

int LightManager::GetNumActivePointLights()
{
	return mNumActivePointLights;
}

LightStructure* LightManager::GetLightStructure()
{
	for( int i = 0; i < mNumActivePointLights; i++ )
		mLightStructure.pointLights[i] = *mPointLights[i];
	return &mLightStructure;
}

HRESULT	LightManager::Initialize()
{
	//for( int i = 0; i < NUM_POINTLIGHTS; i++ )
	//{
	//	mPointLights[i] = new PointLight;
	//	mPointLights[i]->position		= DirectX::XMFLOAT4( (float)i * 5.0f, 15.0f, 0.0f, 0.0f );
	//	mPointLights[i]->colorAndRadius	= DirectX::XMFLOAT4( 0.6f, 0.4f, 0.4f, 3.0f ); 
	//}

	//mNumActivePointLights = 8;

	EventManager::GetInstance()->AddListener( &LightManager::EventListener, this, Event_Add_Point_Light::GUID );
	EventManager::GetInstance()->AddListener( &LightManager::EventListener, this, Event_Remove_Point_Light::GUID );

	return S_OK;
}

void LightManager::Release()
{
}

LightManager::LightManager()
{
	mNumActivePointLights = 0;
	for( int i = 0; i < MAX_NUM_POINTLIGHTS; i++ )
		mPointLights[i] = nullptr;
}

LightManager::~LightManager()
{

}