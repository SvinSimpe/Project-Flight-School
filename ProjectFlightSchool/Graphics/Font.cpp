#include "Font.h"


float Font::GetMiddleXPoint( std::string toWrite, float scale ) 
{
	float xOffset = 0.0f;
	float yOffset = 0.0f;
	float yLower  = 20.0f * scale;
	for( unsigned int i = 0; i < toWrite.size(); i++ )
	{
		char l = toWrite[i];

		if( l > 64 && l < 91 )
			l = tolower( l );

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
	
		xOffset += ( mCharacters[l].w + X_OFFSET ) * scale;
	}

	return ( xOffset * 0.5f );
}

void Font::WriteText( std::string toWrite, float x, float y, float scale, XMFLOAT4 color )
{
	float xOffset = 0.0f;
	float yOffset = 0.0f;
	float yLower  = 20.0f * scale/LETTER_SCALE;
	
	for( unsigned int i = 0; i < toWrite.size(); i++ )
	{
		char l = toWrite[i];

		if( l > 64 && l < 91 )
			l = tolower( l );

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


		RenderManager::GetInstance()->AddObject2dToList( mCharacters[l].asset, DirectX::XMFLOAT2( (x + xOffset), (y + yOffset) ), DirectX::XMFLOAT2( (mCharacters[l].w * scale), (mCharacters[l].h * scale) ), color );

		xOffset += ( mCharacters[l].w + X_OFFSET ) * scale;
	}
}

void Font::Initialize( std::string path )
{
	path = "../Content/Assets/GUI/Fonts/final_font/";
	//Letter UPPER_CASE[] =
	//{
	//	Letter( 'A', 49.0f/LETTER_SCALE ),
	//	Letter( 'B', 50.0f/LETTER_SCALE ),
	//	Letter( 'C', 49.0f/LETTER_SCALE ),
	//	Letter( 'D', 50.0f/LETTER_SCALE ),
	//	Letter( 'E', 49.0f/LETTER_SCALE ),
	//	Letter( 'F', 49.0f/LETTER_SCALE ),
	//	Letter( 'G', 50.0f/LETTER_SCALE ),
	//	Letter( 'H', 50.0f/LETTER_SCALE ),
	//	Letter( 'I', 50.0f/LETTER_SCALE ),
	//	Letter( 'J', 40.0f/LETTER_SCALE ),
	//	Letter( 'K', 49.0f/LETTER_SCALE ),
	//	Letter( 'L', 49.0f/LETTER_SCALE ),
	//	Letter( 'M', 50.0f/LETTER_SCALE ),
	//	Letter( 'N', 50.0f/LETTER_SCALE ),
	//	Letter( 'O', 50.0f/LETTER_SCALE ),
	//	Letter( 'P', 50.0f/LETTER_SCALE ),
	//	Letter( 'Q', 50.0f/LETTER_SCALE ),
	//	Letter( 'R', 50.0f/LETTER_SCALE ),
	//	Letter( 'S', 50.0f/LETTER_SCALE ),
	//	Letter( 'T', 50.0f/LETTER_SCALE ),
	//	Letter( 'U', 50.0f/LETTER_SCALE ),
	//	Letter( 'V', 50.0f/LETTER_SCALE ),
	//	Letter( 'W', 50.0f/LETTER_SCALE ),
	//	Letter( 'X', 50.0f/LETTER_SCALE ),
	//	Letter( 'Y', 50.0f/LETTER_SCALE ),
	//	Letter( 'Z', 50.0f/LETTER_SCALE ) // Is actually ?
	//};

	//for( auto& it : UPPER_CASE )
	//{
	//	std::string fullPath = path + "Upper case/" + it.name + ".dds";
	//	Graphics::GetInstance()->LoadStatic2dAsset( fullPath, it.asset );
	//	mCharacters[it.name] = it;
	//}

	Letter LOWER_CASE[] =
	{
		Letter( 'a', 48.0f/LETTER_SCALE ),
		Letter( 'b', 48.0f/LETTER_SCALE ),
		Letter( 'c', 48.0f/LETTER_SCALE ),
		Letter( 'd', 48.0f/LETTER_SCALE ),
		Letter( 'e', 48.0f/LETTER_SCALE ),
		Letter( 'f', 48.0f/LETTER_SCALE ),
		Letter( 'g', 48.0f/LETTER_SCALE ),
		Letter( 'h', 48.0f/LETTER_SCALE ),
		Letter( 'i', 16.0f/LETTER_SCALE ),
		Letter( 'j', 36.0f/LETTER_SCALE ),
		Letter( 'k', 48.0f/LETTER_SCALE ),
		Letter( 'l', 36.0f/LETTER_SCALE ),
		Letter( 'm', 56.0f/LETTER_SCALE ),
		Letter( 'n', 40.0f/LETTER_SCALE ),
		Letter( 'o', 40.0f/LETTER_SCALE ),
		Letter( 'p', 40.0f/LETTER_SCALE ),
		Letter( 'q', 44.0f/LETTER_SCALE ),
		Letter( 'r', 40.0f/LETTER_SCALE ),
		Letter( 's', 40.0f/LETTER_SCALE ),
		Letter( 't', 40.0f/LETTER_SCALE ),
		Letter( 'u', 40.0f/LETTER_SCALE ),
		Letter( 'v', 44.0f/LETTER_SCALE ),
		Letter( 'w', 48.0f/LETTER_SCALE ),
		Letter( 'x', 40.0f/LETTER_SCALE ),
		Letter( 'y', 32.0f/LETTER_SCALE ),
		Letter( 'z', 32.0f/LETTER_SCALE ),
	};

	for( auto& it : LOWER_CASE )
	{
		std::string fullPath = path + "Lower case/" + it.name + ".dds";
		Graphics::GetInstance()->LoadStatic2dAsset( fullPath, it.asset );
		mCharacters[it.name] = it;
	}

	Letter NUMBERS[] =
	{
		Letter( '0', 32.0f/LETTER_SCALE ),
		Letter( '1', 16.0f/LETTER_SCALE ),
		Letter( '2', 32.0f/LETTER_SCALE ),
		Letter( '3', 32.0f/LETTER_SCALE ),
		Letter( '4', 36.0f/LETTER_SCALE ),
		Letter( '5', 32.0f/LETTER_SCALE ),
		Letter( '6', 32.0f/LETTER_SCALE ),
		Letter( '7', 32.0f/LETTER_SCALE ),
		Letter( '8', 32.0f/LETTER_SCALE ),
		Letter( '9', 32.0f/LETTER_SCALE )
	};

	for( auto& it : NUMBERS )
	{
		std::string fullPath = path + "Numbers/" + it.name + ".dds";
		Graphics::GetInstance()->LoadStatic2dAsset( fullPath, it.asset );
		mCharacters[it.name] = it;
	}

	Letter SYMBOLS[] =
	{
		Letter( '_', 8.0f/LETTER_SCALE ),
		Letter( '!', 12.0f/LETTER_SCALE ),
		Letter( '~', 32.0f/LETTER_SCALE ),
		Letter( '%', 32.0f/LETTER_SCALE ),
	};

	for( auto& it : SYMBOLS )
	{
		std::string fullPath = path + "Symbols/" + it.name + ".dds";
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