#include "Font.h"

void Font::WriteText( std::string toWrite, float x, float y, float scale )
{
	float xOffset = 0.0f;
	float yOffset = 0.0f;
	for( unsigned int i = 0; i < toWrite.size(); i++ )
	{
		char l = toWrite[i];
		if( l == ' ' ) // space
		{
			xOffset += SPACE_WIDTH * scale;
			continue;
		}
		else if( l == 10 ) // '\n'
		{
			xOffset = 0.0f;
			yOffset += LETTER_HEIGHT * scale;
			continue;
		}
		else if( l == 9 ) // '\t'
		{
			xOffset += TAB_WIDTH * scale;
			continue;
		}
		else if( l == '?' || mCharacters.find(l) == mCharacters.end() )
		{
			l = '~';
		}
		
		RenderManager::GetInstance()->AddObject2dToList( mCharacters[l].asset, DirectX::XMFLOAT2( (x + xOffset), (y + yOffset) ), DirectX::XMFLOAT2( (mCharacters[l].w * scale), (mCharacters[l].h * scale) ) );
	
		xOffset += mCharacters[l].w * scale;
	}
}

void Font::Initialize( std::string path )
{
	Letter AVAILABLE_LETTERS[] =
	{
		Letter( 'A', 24.0f ),
		Letter( 'B', 24.0f ),
		Letter( 'C', 22.0f ),
		Letter( 'D', 25.0f ),
		Letter( 'E', 26.0f ),
		Letter( 'F', 25.0f ),
		Letter( 'G', 25.0f ),
		Letter( 'H', 28.0f ),
		Letter( 'I', 13.0f ),
		Letter( 'J', 29.0f ),
		Letter( 'K', 25.0f ),
		Letter( 'L', 22.0f ),
		Letter( 'M', 34.0f ),
		Letter( 'N', 30.0f ),
		Letter( 'O', 26.0f ),
		Letter( 'P', 26.0f ),
		Letter( 'Q', 26.0f ),
		Letter( 'R', 25.0f ),
		Letter( 'S', 24.0f ),
		Letter( 'T', 29.0f ),
		Letter( 'U', 28.0f ),
		Letter( 'V', 27.0f ),
		Letter( 'W', 35.0f ),
		Letter( 'X', 27.0f ),
		Letter( 'Y', 28.0f ),
		Letter( 'Z', 30.0f ),
		Letter( '!', 15.0f ),
		Letter( '#', 26.0f ),
		Letter( '@', 34.0f ),
		Letter( '~', 22.0f ), // Is actually ?
	};

	for( auto& it : AVAILABLE_LETTERS )
	{
		std::string fullPath = path + it.name + ".png";
		Graphics::GetInstance()->LoadStatic2dAsset( fullPath, it.asset );
		mCharacters[it.name] = it;
	}
}

void Font::Release()
{
	mCharacters.clear();
}

Font::Font()
{
	mCharacters = std::unordered_map<char, Letter>();
}

Font::~Font()
{
}