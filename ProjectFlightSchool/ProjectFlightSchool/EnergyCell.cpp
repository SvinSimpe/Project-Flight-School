#include "EnergyCell.h"

void EnergyCell::SetOwnerID( UINT id )
{
	if( !mPickedUp )
	{
		mOwnerID = id;
	}
}

void EnergyCell::SetPosition( DirectX::XMFLOAT3 position )
{
	mPickUpRadius->center = position;
}

void EnergyCell::SetPickedUp( bool pickedUp )
{
	mPickedUp = pickedUp;
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

HRESULT EnergyCell::Update( float deltaTime )
{
	return S_OK;
}

HRESULT EnergyCell::Render()
{
	RenderManager::GetInstance()->AddObject3dToList( CUBE_PLACEHOLDER, mPickUpRadius->center );

	return S_OK;
}

HRESULT EnergyCell::Initialize( DirectX::XMFLOAT3 position )
{
	HRESULT hr = S_OK;

	if( FAILED( hr = Graphics::GetInstance()->LoadStatic3dAsset( "TBD", "TBD", mAssetID ) ) )
	{
		return hr;
	}

	mPickedUp				= false;
	mPickUpRadius			= new BoundingCircle( 1.0f );
	mPickUpRadius->center	= position;

	return hr;
}

void EnergyCell::Release()
{
	SAFE_DELETE( mPickUpRadius );
}

EnergyCell::EnergyCell()
{
	mAssetID		= (UINT)-1;
	mOwnerID		= (UINT)-1;
	mPickedUp		= false;
	mPickUpRadius	= nullptr;
}

EnergyCell::~EnergyCell()
{

}
