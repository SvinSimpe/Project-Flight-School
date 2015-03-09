#ifndef FONT_H
#define FONT_H

#include <unordered_map>
#include "Graphics.h"
#include "..\ProjectFlightSchool\RenderManager.h"

#define LETTER_SCALE	7.8f
#define X_OFFSET		3.0f/LETTER_SCALE
#define SPACE_WIDTH		15.0f/LETTER_SCALE
#define TAB_WIDTH		(SPACE_WIDTH * 8)
#define LETTER_HEIGHT	78.0f/LETTER_SCALE

#define COLOR_BLACK	XMFLOAT4( 0.0f, 0.0f, 0.0f, 1.0f )
#define COLOR_RED	XMFLOAT4( 1.0f, 0.0f, 0.0f, 1.0f )
#define COLOR_CYAN	XMFLOAT4( 0.0f, 0.94f, 1.0f, 1.0f )
#define COLOR_ORANGE XMFLOAT4( 1.0f, 0.74f, 0.0f, 1.0f )
#define COLOR_DARK_ORANGE XMFLOAT4( 1.0f, 0.38f, 0.0f, 1.0f )

class Font
{
	private:
		struct Letter
		{
			AssetID asset;
			char name;
			float w;
			float h = LETTER_HEIGHT;
			Letter()
			{
			}
			Letter( char _name, float _w )
			{
				name = _name;
				w = _w;
			}
		};

		std::unordered_map<char, Letter> mCharacters;

	protected:
	public:

	private:
	protected:
	public:
		float			GetMiddleXPoint( std::string toWrite, float scale );
		void			WriteText( std::string toWrite, float x, float y, float scale, XMFLOAT4 color = XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f ) );
		virtual void	Initialize( std::string path );
		virtual void	Release();
						Font();
		virtual			~Font();
};
#endif