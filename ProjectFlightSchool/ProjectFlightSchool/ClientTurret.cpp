#include "ClientTurret.h"

void ClientTurret::ServerUpdateTurret( IEventPtr eventPtr )
{
	if( eventPtr->GetEventType() == Event_Server_Update_Turret::GUID )
	{
		std::shared_ptr<Event_Server_Update_Turret> data = std::static_pointer_cast<Event_Server_Update_Turret>( eventPtr );
		if( data->ID() == mID )
		{
			if( mTurretHead != nullptr )
			{
				mTurretHead->rot = data->Rotation();
				mRot = mTurretHead->rot;
			}
		}
	}
}

void ClientTurret::Render( float deltaTime, DirectX::XMFLOAT4X4 parentWorld )
{
	float yaw = atan2f( mTurretHead->rot.z, mTurretHead->rot.x );
	float roll	= -XMVectorGetX( 
					XMVector3AngleBetweenVectors( 
					XMLoadFloat3( &XMFLOAT3( mTurretHead->rot.x, 0.0f, mTurretHead->rot.z ) ),
					XMLoadFloat3( &XMFLOAT3( mTurretHead->rot.x, mTurretHead->rot.y, mTurretHead->rot.z ) ) ) );

	RenderManager::GetInstance()->AddObject3dToList( mAssetID, mPos, XMFLOAT3( 0.0f, -yaw, 0.0f ) );
	RenderManager::GetInstance()->AddObject3dToList( mTurretHead->model, mTurretHead->pos, XMFLOAT3( 0.0f, -yaw, roll ) );
}

void ClientTurret::Update( float deltaTime )
{
	// Maybe there should be some code here? How should I know!?
}

void ClientTurret::Reset( UINT id, UINT team, XMFLOAT3 pos, XMFLOAT4 rot, XMFLOAT3 scale )
{
	ServerTurret::Reset( id, team, pos, rot, scale );
}

void ClientTurret::Initialize( UINT id, UINT team, XMFLOAT3 pos, XMFLOAT4 rot, XMFLOAT3 scale, AssetID assetID )
{
	ServerTurret::Initialize( id, team, pos, rot, scale, assetID );

	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/PermanentAssets/Turret/", "turretBase2.pfs", mAssetID );
	Graphics::GetInstance()->LoadStatic3dAsset( "../Content/Assets/PermanentAssets/Turret/", "turret.pfs", mTurretHead->model );

	EventManager::GetInstance()->AddListener( &ClientTurret::ServerUpdateTurret, this, Event_Server_Update_Turret::GUID );
}

void ClientTurret::Release()
{
	ServerTurret::Release();
}

ClientTurret::ClientTurret() : ServerTurret()
{
}

ClientTurret::~ClientTurret()
{
}