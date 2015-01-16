#include "MenuButton.h"

void MenuButton::SpinCircle()
{
	mFrames[mCurrentIndex].active = false;

	mCurrentIndex++;
	if(mCurrentIndex >= 3)
		mCurrentIndex = 0;

	mFrames[mCurrentIndex].active = true;

	mSpinTimer = 0.0f;
}

void MenuButton::Render()
{
	for( auto& it : mFrames )
	{
		if(it.active)
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

	for( int i = 0; i < FRAME_AMOUNT; i++ )
	{
		int index = i + 1;
		std::string itostring = std::to_string(index);
		Graphics::GetInstance()->LoadStatic2dAsset( path + "menuRing." + itostring + ".png", mFrames[i].asset );
	}

	return true;
}

void MenuButton::Release()
{
}

MenuButton::MenuButton()
{
	mSpinTimer		= 0.0f;
	mCurrentIndex	= 0;
}

MenuButton::~MenuButton()
{
}
