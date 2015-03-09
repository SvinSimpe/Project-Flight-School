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

	RenderManager::GetInstance()->AnimationUpdate( mAnimationTrack, deltaTime );

	return S_OK;
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
}

HRESULT EnergyCell::Initialize( DirectX::XMFLOAT3 position )
{
	HRESULT hr = S_OK;

	mPickedUp				= false;
	mPickUpRadius			= new BoundingCircle( 1.0f );
	mPickUpRadius->center	= position;

	mSecured				= false;

	AssetID model		= 0;
	AssetID skeleton	= 0;

	if( Graphics::GetInstance()->LoadSkeletonAsset( "../Content/Assets/PermanentAssets/Battery/", "energyCell.Skel", skeleton ) ) //Skeleton for legs
		OutputDebugString( L"\nERROR loading energy cell model\n" );

	if( FAILED( Graphics::GetInstance()->LoadAnimated3dAsset( "../Content/Assets/PermanentAssets/Battery/", "battery.apfs", skeleton, model ) ) )
		OutputDebugString( L"\nERROR loading energy cell model\n" ); 

	if( FAILED( Graphics::GetInstance()->LoadAnimationAsset( "../Content/Assets/PermanentAssets/Battery/", "batteryIdle.PaMan", mAnimation ) ) )
		OutputDebugString( L"\nERROR loading energy cell model\n" );

	RenderManager::GetInstance()->AnimationInitialize( mAnimationTrack, model, mAnimation );

	//if( FAILED( hr = Graphics::GetInstance()->LoadAnimated3dAsset( "../Content/Assets/PermanentAssets/Battery/", "battery.apfs", mAssetID ) ) )
	//{
	//	return hr;
	//}

	

	return hr;
}

void EnergyCell::Release()
{
	SAFE_DELETE( mPickUpRadius );
}

EnergyCell::EnergyCell()
{
	mAnimation		= (UINT)-1;
	mOwnerID		= (UINT)-1;
	mPickedUp		= false;
	mPickUpRadius	= nullptr;
	mHooverFactor	= 0.0f;
	mSecured		= false;
}

EnergyCell::~EnergyCell()
{

}
