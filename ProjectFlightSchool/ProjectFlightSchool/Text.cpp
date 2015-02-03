#include "Text.h"

void Text::Render()
{
	Font::WriteText( mText, mX, mY, mScale );
}

void Text::SetText( std::string text )
{
	mText = text;
}

void Text::SetPosition( float x, float y )
{
	mX = x;
	mY = y;
}

void Text::Initialize( std::string fontPath, std::string text, float x, float y, float scale )
{
	Font::Initialize( fontPath );
	mX		= x;
	mY		= y;
	mScale	= scale;
	mText	= text;
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
