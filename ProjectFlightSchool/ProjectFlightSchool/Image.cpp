#include "Image.h"

void Image::SetImage( AssetID texture )
{
	m2DTexture = texture;
}

AssetID Image::GetAssetID() const
{
	return m2DTexture;
}

void Image::Render()
{
	RenderManager::GetInstance()->AddObject2dToList( m2DTexture, DirectX::XMFLOAT2( (float)mUpperLeft.x, (float)mUpperLeft.y ), DirectX::XMFLOAT2( mWidth, mHeight ) );
}

void Image::Render( float x, float y, float w, float h )
{
	RenderManager::GetInstance()->AddObject2dToList( m2DTexture, DirectX::XMFLOAT2( x, y ), DirectX::XMFLOAT2( w, h ) );
}

bool Image::Update( float deltaTime )
{
	return true;
}

void Image::Initialize( std::string assetPath, float x, float y, float width, float height )
{
	Entry::Initialize( x, y, width, height );
	Graphics::GetInstance()->LoadStatic2dAsset( assetPath, m2DTexture );
}

void Image::Release()
{
	Entry::Release();
}

Image::Image() : Entry()
{
}

Image::~Image()
{
}

XMFLOAT2 Image::GetPosition() const
{
	return XMFLOAT2( (float)mUpperLeft.x, (float)mUpperLeft.y );
}
