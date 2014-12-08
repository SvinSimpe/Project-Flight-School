#include "Player.h"

void Player::HandleInput( float deltaTime )
{
	mLowerBody.direction = XMFLOAT3( 0.0f, 0.0f, 0.0f );

	if( Input::GetInstance()->mCurrentFrame.at( KEYS::KEYS_W ) && !Input::GetInstance()->mCurrentFrame.at( KEYS::KEYS_S ) )
		Move( XMFLOAT3( 0.0f, 0.0f, 1.0f ) );
	
	if( Input::GetInstance()->mCurrentFrame.at( KEYS::KEYS_A ) && !Input::GetInstance()->mCurrentFrame.at( KEYS::KEYS_D ) )
		Move( XMFLOAT3( -1.0f, 0.0f, 0.0f ) );

	if( Input::GetInstance()->mCurrentFrame.at( KEYS::KEYS_S ) && !Input::GetInstance()->mCurrentFrame.at( KEYS::KEYS_W ) )
		Move( XMFLOAT3( 0.0f, 0.0f, -1.0f ) );
	
	if( Input::GetInstance()->mCurrentFrame.at( KEYS::KEYS_D ) && !Input::GetInstance()->mCurrentFrame.at( KEYS::KEYS_A ) )
		Move( XMFLOAT3( 1.0f, 0.0f, 0.0f ) );
}

void Player::Move( XMFLOAT3 direction  )
{
	mLowerBody.direction.x += direction.x;
	mLowerBody.direction.z += direction.z;
	
	//Normalize direction vector
	if ( direction.x < 0.00001f && direction.z < 0.00001f )
	{
		mLowerBody.direction.x /= pow((pow(mLowerBody.direction.x, 2) + pow(mLowerBody.direction.y, 2) + pow(mLowerBody.direction.z, 2)), 0.5f);
		mLowerBody.direction.z /= pow((pow(mLowerBody.direction.x, 2) + pow(mLowerBody.direction.y, 2) + pow(mLowerBody.direction.z, 2)), 0.5f);
	}
}

void Player::LookAt( float rotation )
{
	
}

HRESULT Player::Update( float deltaTime )
{
	HandleInput( deltaTime );
	
	mUpperBody.position.x += mLowerBody.direction.x * mLowerBody.speed;
	mUpperBody.position.z += mLowerBody.direction.z * mLowerBody.speed;

	mLowerBody.position.x += mLowerBody.direction.x * mLowerBody.speed;
	mLowerBody.position.z += mLowerBody.direction.z * mLowerBody.speed;

	IEventPtr E1( new Event_Player_Moved( mLowerBody.position, mUpperBody.position ) );
	EventManager::GetInstance()->QueueEvent( E1 );

	return S_OK;
}

HRESULT Player::Render( float deltaTime )
{
	Graphics::GetInstance()->RenderStatic3dAsset( mUpperBody.playerModel, mUpperBody.position.x, mUpperBody.position.y, mUpperBody.position.z );
	Graphics::GetInstance()->RenderStatic3dAsset( mLowerBody.playerModel, mLowerBody.position.x, mLowerBody.position.y, mLowerBody.position.z );

	return S_OK;
}

HRESULT Player::Initialize()
{
	if( FAILED( Graphics::GetInstance()->LoadStatic3dAsset( "CUBE", mUpperBody.playerModel ) ) )
		OutputDebugString( L"\nERROR\n" );

	if( FAILED( Graphics::GetInstance()->LoadStatic3dAsset( "CUBE", mLowerBody.playerModel ) ) )
		OutputDebugString( L"\nERROR\n" );


	mUpperBody.position	= XMFLOAT3( 10.0f, 2.0f, 10.0f );
	mLowerBody.position	= XMFLOAT3( 10.0f, 1.0f, 10.0f );
	mLowerBody.speed	= 0.005f;

	return S_OK;
}

void Player::Release()
{}

Player::Player()
{
	mUpperBody.playerModel	= 0;
	mUpperBody.position		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mUpperBody.direction	= XMFLOAT3( 0.0f, 0.0f, 0.0f );

	mLowerBody.playerModel	= 0;
	mLowerBody.position		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mLowerBody.direction	= XMFLOAT3( 0.0f, 0.0f, 0.0f );
	mLowerBody.speed		= 0.0f;
}

Player::~Player()
{}