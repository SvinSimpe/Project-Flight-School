#include "Text.h"

float Text::GetScale() const
{
	return mScale;
}

std::string Text::GetText() const
{
	return mText;
}

void Text::Render()
{
	Font::WriteText( mText, mX, mY, mScale, mColor );
}

void Text::SetText( std::string text, DirectX::XMFLOAT4 color )
{
	mText	= text;
	mColor	= color;
}

void Text::SetPosition( float x, float y )
{
	mX = x;
	mY = y;
}

void Text::Initialize( std::string fontPath, std::string text, float x, float y, float scale, DirectX::XMFLOAT4 color )
{
	Font::Initialize( fontPath );
	mX		= x;
	mY		= y;
	mScale	= scale;
	mText	= text;
	mColor  = color;
}

void Text::Release()
{
}

Text::Text() : Font()
{
}


Text::~Text()
{
}
