#include "MenuButton.h"

void MenuButton::SpinCircle()
{
	mInner[mCurrentIndex].active = false;
	mMiddle[mCurrentIndex].active = false;
	mOuter[mCurrentIndex].active = false;

	mCurrentIndex++;
	if(mCurrentIndex >= 3)
		mCurrentIndex = 0;

	mInner[mCurrentIndex].active = true;
	mMiddle[mCurrentIndex].active = true;
	mOuter[mCurrentIndex].active = true;

	mSpinTimer = 0.0f;
}

void MenuButton::Render()
{
	for( auto& it : mInner )
	{
		if(it.active)
			RenderManager::GetInstance()->AddObject2dToList( it.asset, DirectX::XMFLOAT2( (float)mUpperLeft.x, (float)mUpperLeft.y ), DirectX::XMFLOAT2( mWidth, mHeight ) );
	}

	for( auto& it : mMiddle )
	{
		if( it.active )
			RenderManager::GetInstance()->AddObject2dToList( it.asset, DirectX::XMFLOAT2( (float)mUpperLeft.x, (float)mUpperLeft.y ), DirectX::XMFLOAT2( mWidth, mHeight ) );
	}

	for( auto& it : mOuter )
	{
		if( it.active )
			RenderManager::GetInstance()->AddObject2dToList( it.asset, DirectX::XMFLOAT2( (float)mUpperLeft.x, (float)mUpperLeft.y ), DirectX::XMFLOAT2( mWidth, mHeight ) );
	}
}

bool MenuButton::Update( float deltaTime )
{
	Entry::Update( deltaTime );
	mSpinTimer += deltaTime;

	if( mSpinTimer >= SPIN_COOLDOWN )
	{
		SpinCircle();
	}

	return true;
}

bool MenuButton::Initialize( UINT x, UINT y, UINT width, UINT height )
{
	Entry::Initialize( x, y, width, height );

	std::string path = "../Content/Assets/Textures/Menu/Menu_Button/";

	Graphics::GetInstance()->LoadStatic2dAsset( path + "Inner/inner.png", mInner[0].asset );
	Graphics::GetInstance()->LoadStatic2dAsset( path + "Inner/innerRot120.png", mInner[1].asset );
	Graphics::GetInstance()->LoadStatic2dAsset( path + "Inner/innerRot240.png", mInner[2].asset );

	Graphics::GetInstance()->LoadStatic2dAsset( path + "Middle/middle.png", mMiddle[0].asset );
	Graphics::GetInstance()->LoadStatic2dAsset( path + "Middle/middleRot120.png", mMiddle[1].asset );
	Graphics::GetInstance()->LoadStatic2dAsset( path + "Middle/middleRot240.png", mMiddle[2].asset );

	Graphics::GetInstance()->LoadStatic2dAsset( path + "Outer/outer.png", mOuter[0].asset );
	Graphics::GetInstance()->LoadStatic2dAsset( path + "Outer/outerRot120.png", mOuter[1].asset );
	Graphics::GetInstance()->LoadStatic2dAsset( path + "Outer/outerRot240.png", mOuter[2].asset );

	return true;
}

MenuButton::MenuButton()
{
	mSpinTimer		= 0.0f;
	mCurrentIndex	= 0;
}

MenuButton::~MenuButton()
{
}
