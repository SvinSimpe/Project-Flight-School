#include "Button.h"

void Image::Render()
{
	Graphics::GetInstance()->Render2dAsset( m2DTexture, (float)mUpperLeft.x, (float)mUpperLeft.y, mWidth, mHeight );
}

bool Image::Update( float deltaTime )
{
	return true;
}

void Image::Initialize( std::string assetPath, UINT x, UINT y, UINT width, UINT height )
{
	Entry::Initialize(x, y, width, height);
	Graphics::GetInstance()->LoadStatic2dAsset( assetPath, m2DTexture );
}

void Image::Release()
{
}

Image::Image() : Entry()
{
	mCurrentCD = 0.0f;
}

Image::~Image()
{
}
