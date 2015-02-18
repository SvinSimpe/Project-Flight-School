#include "MovingButton.h"

void MovingButton::SpinCircle()
{
	mFrames[mFrameIndex].active = false;

	mFrameIndex++;
	if( mFrameIndex >= FRAME_AMOUNT )
		mFrameIndex = 0;

	mFrames[mFrameIndex].active = true;

	mSpinTimer = 0.0f;
}

void MovingButton::Render()
{
	for( auto& it : mFrames )
	{
		if( it.active )
			RenderManager::GetInstance()->AddObject2dToList( it.asset, DirectX::XMFLOAT2( (float)mUpperLeft.x, (float)mUpperLeft.y ), DirectX::XMFLOAT2( mWidth, mHeight ) );
	}
	Button::Render();
}

bool MovingButton::Update( float deltaTime )
{
	Button::Update( deltaTime );

	mSpinTimer += deltaTime;
	if( mSpinTimer >= SPIN_COOLDOWN )
	{
		SpinCircle();
	}

	return true;
}

void MovingButton::Initialize( std::string imgPath, float x, float y, float width, float height )
{
	Button::Initialize( imgPath, x, y, width, height );

	std::string path = "../Content/Assets/Textures/Menu/Menu_Button/";

	for( int i = 0; i < FRAME_AMOUNT; i++ )
	{
		int index = i + 1;
		std::string itostring = std::to_string( index );
		Graphics::GetInstance()->LoadStatic2dAsset( path + "menuRing." + itostring + ".png", mFrames[i].asset );
	}
}

void MovingButton::Release()
{
	Button::Release();
}

MovingButton::MovingButton()
{
	mSpinTimer	= 0.0f;
	mFrameIndex	= 0;
}

MovingButton::~MovingButton()
{
}