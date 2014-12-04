#include "Player.h"

void Player::HandleInput( float deltaTime )
{
	if( Input::GetInstance()->mCurrentFrame.at( KEYS::KEYS_W ) )
		Move( 0.005, XMFLOAT2( -0.5f, 0.5f ) );
	
	if( Input::GetInstance()->mCurrentFrame.at( KEYS::KEYS_A ) )
		Move( -0.005, XMFLOAT2( 0.5f, 0.5f ) );

	if( Input::GetInstance()->mCurrentFrame.at( KEYS::KEYS_S ) )
		Move( 0.005, XMFLOAT2( 0.5f, -0.5f ) );

	if( Input::GetInstance()->mCurrentFrame.at( KEYS::KEYS_D ) )
		Move( -0.005, XMFLOAT2( -0.5f, -0.5f ) );
}

void Player::Move( float speed, XMFLOAT2 direction  )
{
	mUpperBody.position.x += direction.x * speed;
	mUpperBody.position.z += direction.y * speed;

	mLowerBody.position.x += direction.x * speed;
	mLowerBody.position.z += direction.y * speed;
}

void Player::LookAt( float rotation )
{
	
}

HRESULT Player::Update( float deltaTime )
{
	HandleInput( deltaTime );
	
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

	mLowerBody.speed = 1.0f;

	return S_OK;
}

void Player::Release()
{}

Player::Player()
{
	mUpperBody.playerModel	= 0;
	mUpperBody.direction.x	= 0.0f;
	mUpperBody.direction.y	= 0.0f;
	mUpperBody.position.x	= 10.0f;
	mUpperBody.position.y	= 2.0f;
	mUpperBody.position.z	= 10.0f;

	mLowerBody.playerModel	= 0;
	mLowerBody.position.x	= 10.0f;
	mLowerBody.position.y	= 1;
	mLowerBody.position.z	= 10.0f;	
	mLowerBody.speed		= 0.0f;
}

Player::~Player()
{}