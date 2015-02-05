#include "Font.h"

void Font::WriteText( std::string toWrite, float x, float y, float scale )
{
	float xOffset = 0.0f;
	float yOffset = 0.0f;
	float yLower  = 20.0f * scale/LETTER_SCALE;
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
		else if( l == '.' )
		{
			l = '_';
		}
		else if( l == '?' || mCharacters.find(l) == mCharacters.end() )
		{
			l = '~';
		}
		else if( l == '%' || mCharacters.find(l) == mCharacters.end() )
		{
			l = '%';
		}

		if( l == 'g' || l == 'j' || l == 'y' || l == 'q' || l == 'p')
		{
			RenderManager::GetInstance()->AddObject2dToList( mCharacters[l].asset, DirectX::XMFLOAT2( (x + xOffset), (y + yLower + yOffset) ), DirectX::XMFLOAT2( (mCharacters[l].w * scale), (mCharacters[l].h * scale) ) );
		}
		else
		{
			RenderManager::GetInstance()->AddObject2dToList( mCharacters[l].asset, DirectX::XMFLOAT2( (x + xOffset), (y + yOffset) ), DirectX::XMFLOAT2( (mCharacters[l].w * scale), (mCharacters[l].h * scale) ) );
		}
	
		xOffset += ( mCharacters[l].w + X_OFFSET ) * scale;
	}
}

void Font::Initialize( std::string path )
{
	path = "../Content/Assets/Fonts/final_font/";
	Letter UPPER_CASE[] =
	{
		Letter( 'A', 49.0f/LETTER_SCALE ),
		Letter( 'B', 50.0f/LETTER_SCALE ),
		Letter( 'C', 49.0f/LETTER_SCALE ),
		Letter( 'D', 50.0f/LETTER_SCALE ),
		Letter( 'E', 49.0f/LETTER_SCALE ),
		Letter( 'F', 49.0f/LETTER_SCALE ),
		Letter( 'G', 50.0f/LETTER_SCALE ),
		Letter( 'H', 50.0f/LETTER_SCALE ),
		Letter( 'I', 50.0f/LETTER_SCALE ),
		Letter( 'J', 40.0f/LETTER_SCALE ),
		Letter( 'K', 49.0f/LETTER_SCALE ),
		Letter( 'L', 49.0f/LETTER_SCALE ),
		Letter( 'M', 50.0f/LETTER_SCALE ),
		Letter( 'N', 50.0f/LETTER_SCALE ),
		Letter( 'O', 50.0f/LETTER_SCALE ),
		Letter( 'P', 50.0f/LETTER_SCALE ),
		Letter( 'Q', 50.0f/LETTER_SCALE ),
		Letter( 'R', 50.0f/LETTER_SCALE ),
		Letter( 'S', 50.0f/LETTER_SCALE ),
		Letter( 'T', 50.0f/LETTER_SCALE ),
		Letter( 'U', 50.0f/LETTER_SCALE ),
		Letter( 'V', 50.0f/LETTER_SCALE ),
		Letter( 'W', 50.0f/LETTER_SCALE ),
		Letter( 'X', 50.0f/LETTER_SCALE ),
		Letter( 'Y', 50.0f/LETTER_SCALE ),
		Letter( 'Z', 50.0f/LETTER_SCALE ) // Is actually ?
	};

	for( auto& it : UPPER_CASE )
	{
		std::string fullPath = path + "Upper case/" + it.name + ".png";
		Graphics::GetInstance()->LoadStatic2dAsset( fullPath, it.asset );
		mCharacters[it.name] = it;
	}

	Letter LOWER_CASE[] =
	{
		Letter( 'a', 48.0f/LETTER_SCALE ),
		Letter( 'b', 48.0f/LETTER_SCALE ),
		Letter( 'c', 48.0f/LETTER_SCALE ),
		Letter( 'd', 48.0f/LETTER_SCALE ),
		Letter( 'e', 48.0f/LETTER_SCALE ),
		Letter( 'f', 43.0f/LETTER_SCALE ),
		Letter( 'g', 48.0f/LETTER_SCALE ),
		Letter( 'h', 48.0f/LETTER_SCALE ),
		Letter( 'i', 38.0f/LETTER_SCALE ),
		Letter( 'j', 38.0f/LETTER_SCALE ),
		Letter( 'k', 48.0f/LETTER_SCALE ),
		Letter( 'l', 38.0f/LETTER_SCALE ),
		Letter( 'm', 48.0f/LETTER_SCALE ),
		Letter( 'n', 48.0f/LETTER_SCALE ),
		Letter( 'o', 48.0f/LETTER_SCALE ),
		Letter( 'p', 48.0f/LETTER_SCALE ),
		Letter( 'q', 48.0f/LETTER_SCALE ),
		Letter( 'r', 48.0f/LETTER_SCALE ),
		Letter( 's', 48.0f/LETTER_SCALE ),
		Letter( 't', 48.0f/LETTER_SCALE ),
		Letter( 'u', 48.0f/LETTER_SCALE ),
		Letter( 'v', 48.0f/LETTER_SCALE ),
		Letter( 'w', 48.0f/LETTER_SCALE ),
		Letter( 'x', 48.0f/LETTER_SCALE ),
		Letter( 'y', 48.0f/LETTER_SCALE ),
		Letter( 'z', 48.0f/LETTER_SCALE ),
	};

	for( auto& it : LOWER_CASE )
	{
		std::string fullPath = path + "Lower case/" + it.name + ".png";
		Graphics::GetInstance()->LoadStatic2dAsset( fullPath, it.asset );
		mCharacters[it.name] = it;
	}

	Letter NUMBERS[] =
	{
		Letter( '0', 50.0f/LETTER_SCALE ),
		Letter( '1', 50.0f/LETTER_SCALE ),
		Letter( '2', 50.0f/LETTER_SCALE ),
		Letter( '3', 50.0f/LETTER_SCALE ),
		Letter( '4', 45.0f/LETTER_SCALE ),
		Letter( '5', 50.0f/LETTER_SCALE ),
		Letter( '6', 50.0f/LETTER_SCALE ),
		Letter( '7', 50.0f/LETTER_SCALE ),
		Letter( '8', 50.0f/LETTER_SCALE ),
		Letter( '9', 50.0f/LETTER_SCALE )
	};

	for( auto& it : NUMBERS )
	{
		std::string fullPath = path + "Numbers/" + it.name + ".png";
		Graphics::GetInstance()->LoadStatic2dAsset( fullPath, it.asset );
		mCharacters[it.name] = it;
	}

	Letter SYMBOLS[] =
	{
		Letter( '_', 20.0f/LETTER_SCALE ),
		Letter( '!', 13.0f/LETTER_SCALE ),
		Letter( '~', 50.0f/LETTER_SCALE ),
		Letter( '%', 50.0f/LETTER_SCALE ),
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