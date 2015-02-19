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
	RenderManager::GetInstance()->AddObject3dToList( mAssetID, mPickUpRadius->center );

	return S_OK;
}

void EnergyCell::Reset()
{
	mAssetID		= (UINT)-1;
	mOwnerID		= (UINT)-1;
	mPickedUp		= false;
	SAFE_DELETE( mPickUpRadius );
	mPickUpRadius	= nullptr;
}

HRESULT EnergyCell::Initialize( DirectX::XMFLOAT3 position )
{
	HRESULT hr = S_OK;

	mPickedUp				= false;
	mPickUpRadius			= new BoundingCircle( 1.0f );
	mPickUpRadius->center	= position;

	if( FAILED( hr = Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/PermanentAssets/Battery/", "battery.pfs", mAssetID ) ) )
	{
		return hr;
	}

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
