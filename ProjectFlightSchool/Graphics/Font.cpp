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
		else if(l == '.')
		{
			l = '_';
		}
		else if( l == '?' || mCharacters.find(l) == mCharacters.end() )
		{
			l = '~';
		}
		
		RenderManager::GetInstance()->AddObject2dToList( mCharacters[l].asset, DirectX::XMFLOAT2( (x + xOffset), (y + yOffset) ), DirectX::XMFLOAT2( (mCharacters[l].w * scale), (mCharacters[l].h * scale) ) );
	
		xOffset += (mCharacters[l].w + X_OFFSET) * scale;
	}
}

void Font::Initialize( std::string path )
{
	path = "../Content/Assets/Fonts/final_font/";
	Letter UPPER_CASE[] =
	{
		Letter( 'A', 49.0f ),
		Letter( 'B', 50.0f ),
		Letter( 'C', 49.0f ),
		Letter( 'D', 50.0f ),
		Letter( 'E', 49.0f ),
		Letter( 'F', 49.0f ),
		Letter( 'G', 50.0f ),
		Letter( 'H', 50.0f ),
		Letter( 'I', 50.0f ),
		Letter( 'J', 40.0f ),
		Letter( 'K', 49.0f ),
		Letter( 'L', 49.0f ),
		Letter( 'M', 50.0f ),
		Letter( 'N', 50.0f ),
		Letter( 'O', 50.0f ),
		Letter( 'P', 50.0f ),
		Letter( 'Q', 50.0f ),
		Letter( 'R', 50.0f ),
		Letter( 'S', 50.0f ),
		Letter( 'T', 50.0f ),
		Letter( 'U', 50.0f ),
		Letter( 'V', 50.0f ),
		Letter( 'W', 50.0f ),
		Letter( 'X', 50.0f ),
		Letter( 'Y', 50.0f ),
		Letter( 'Z', 50.0f ) // Is actually ?
	};

	for( auto& it : UPPER_CASE )
	{
		std::string fullPath = path + "Upper case/" + it.name + ".png";
		Graphics::GetInstance()->LoadStatic2dAsset( fullPath, it.asset );
		mCharacters[it.name] = it;
	}

	Letter LOWER_CASE[] =
	{
		Letter( 'a', 48.0f ),
		Letter( 'b', 48.0f ),
		Letter( 'c', 48.0f ),
		Letter( 'd', 48.0f ),
		Letter( 'e', 48.0f ),
		Letter( 'f', 43.0f ),
		Letter( 'g', 48.0f ),
		Letter( 'h', 48.0f ),
		Letter( 'i', 38.0f ),
		Letter( 'j', 38.0f ),
		Letter( 'k', 48.0f ),
		Letter( 'l', 38.0f ),
		Letter( 'm', 48.0f ),
		Letter( 'n', 48.0f ),
		Letter( 'o', 48.0f ),
		Letter( 'p', 48.0f ),
		Letter( 'q', 48.0f ),
		Letter( 'r', 48.0f ),
		Letter( 's', 48.0f ),
		Letter( 't', 48.0f ),
		Letter( 'u', 48.0f ),
		Letter( 'v', 48.0f ),
		Letter( 'w', 48.0f ),
		Letter( 'x', 48.0f ),
		Letter( 'y', 48.0f ),
		Letter( 'z', 48.0f ),
	};

	for( auto& it : LOWER_CASE )
	{
		std::string fullPath = path + "Lower case/" + it.name + ".png";
		Graphics::GetInstance()->LoadStatic2dAsset( fullPath, it.asset );
		mCharacters[it.name] = it;
	}

	Letter NUMBERS[] =
	{
		Letter( '0', 50.0f ),
		Letter( '1', 50.0f ),
		Letter( '2', 50.0f ),
		Letter( '3', 50.0f ),
		Letter( '4', 45.0f ),
		Letter( '5', 50.0f ),
		Letter( '6', 50.0f ),
		Letter( '7', 50.0f ),
		Letter( '8', 50.0f ),
		Letter( '9', 50.0f )
	};

	for( auto& it : NUMBERS )
	{
		std::string fullPath = path + "Numbers/" + it.name + ".png";
		Graphics::GetInstance()->LoadStatic2dAsset( fullPath, it.asset );
		mCharacters[it.name] = it;
	}

	Letter SYMBOLS[] =
	{
		Letter( '_', 20.0f ),
		Letter( '!', 13.0f ),
		Letter( '~', 50.0f ),
	};

	for( auto& it : SYMBOLS )
	{
		std::string fullPath = path + "Symbols/" + it.name + ".png";
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