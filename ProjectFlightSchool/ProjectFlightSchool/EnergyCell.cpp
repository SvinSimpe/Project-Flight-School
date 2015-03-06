#include "EnergyCell.h"

void EnergyCell::SetOwnerID( UINT id )
{
	mOwnerID = id;
}

void EnergyCell::SetPosition( DirectX::XMFLOAT3 position )
{
	mPickUpRadius->center = position;
}

void EnergyCell::SetPickedUp( bool pickedUp )
{
	mPickedUp = pickedUp;
}

void EnergyCell::SetSecured( bool secured )
{
	mSecured = secured;
}

UINT EnergyCell::GetOwnerID() const 
{
	return mOwnerID;
}

bool EnergyCell::GetPickedUp() const
{
	return mPickedUp;
}

DirectX::XMFLOAT3 EnergyCell::GetPosition() const
{
	return mPickUpRadius->center;
}

BoundingCircle*	EnergyCell::GetPickUpRadius() const
{
	return mPickUpRadius;
}

bool EnergyCell::GetSecured() const
{
	return mSecured;
}

HRESULT EnergyCell::Update( float deltaTime )
{
	mHooverFactor += deltaTime * 1.0f;
	mPickUpRadius->center.y = sinf( mHooverFactor ) + 1.0f;

	UpdateLight( deltaTime );

	RenderManager::GetInstance()->AnimationUpdate( mAnimationTrack, deltaTime );

	return S_OK;
}

void EnergyCell::UpdateLight( float deltaTime )
{
	if( !mPickedUp )
	{
		if ( !isLightActive )
		{
			mEnergyPointLight->positionAndIntensity.x = mPickUpRadius->center.x;
			mEnergyPointLight->positionAndIntensity.y = mPickUpRadius->center.y;
			mEnergyPointLight->positionAndIntensity.z = mPickUpRadius->center.z;
			mEnergyPointLight->positionAndIntensity.w = 1.0f;

			IEventPtr reg( new Event_Add_Point_Light( mEnergyPointLight ) );
			EventManager::GetInstance()->QueueEvent( reg );
			isLightActive = true;
		}		
	}

	else if( mPickedUp )
	{
		if ( isLightActive )
		{
			IEventPtr reg( new Event_Remove_Point_Light( mEnergyPointLight ) );
			EventManager::GetInstance()->QueueEvent( reg );
			isLightActive = false;
		}
	}
}
HRESULT EnergyCell::Render()
{
	RenderManager::GetInstance()->AddAnim3dToList( mAnimationTrack, ANIMATION_PLAY_LOOPED, mPickUpRadius->center );

	return S_OK;
}

void EnergyCell::Reset()
{
	mOwnerID		= (UINT)-1;
	mPickedUp		= false;
	mSecured		= false;
	isLightActive	= false;
}

HRESULT EnergyCell::Initialize( DirectX::XMFLOAT3 position )
{
	HRESULT hr = S_OK;

	mPickedUp				= false;
	mPickUpRadius			= new BoundingCircle( 1.0f );
	mPickUpRadius->center	= position;

	mEnergyPointLight						= new PointLight;
	mEnergyPointLight->colorAndRadius		= XMFLOAT4( 2.0f, 2.0f, 4.0f, 1.0f );
	isLightActive		= false;

	mSecured			= false;

	AssetID model		= 0;
	AssetID skeleton	= 0;

	if( Graphics::GetInstance()->LoadSkeletonAsset( "../Content/Assets/PermanentAssets/Battery/", "energyCell.Skel", skeleton ) ) //Skeleton for legs
		OutputDebugString( L"\nERROR loading energy cell model\n" );

	if( FAILED( Graphics::GetInstance()->LoadAnimated3dAsset( "../Content/Assets/PermanentAssets/Battery/", "battery.apfs", skeleton, model ) ) )
		OutputDebugString( L"\nERROR loading energy cell model\n" ); 

	if( FAILED( Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/PermanentAssets/Battery/", "batteryIdle.PaMan", mAnimation ) ) )
		OutputDebugString( L"\nERROR loading energy cell model\n" );

	RenderManager::GetInstance()->AnimationInitialize( mAnimationTrack, model, mAnimation );

	return hr;
}

void EnergyCell::Release()
{
	SAFE_DELETE( mPickUpRadius );
	SAFE_DELETE( mEnergyPointLight );
}

EnergyCell::EnergyCell()
{
	mAnimation		= (UINT)-1;
	mOwnerID		= (UINT)-1;
	mPickedUp		= false;
	mPickUpRadius	= nullptr;
	mHooverFactor	= 0.0f;
	mSecured		= false;
	mEnergyPointLight	= nullptr;
}

EnergyCell::~EnergyCell()
{

}
