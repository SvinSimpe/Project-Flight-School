#ifndef FONT_H
#define FONT_H

#include <unordered_map>
#include "Graphics.h"

#define SPACE_WIDTH		15.0f
#define TAB_WIDTH		(SPACE_WIDTH * 8)
#define LETTER_HEIGHT	55.0f

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
	void		WriteText( std::string toWrite, float x, float y, float scale );
	void		Initialize( std::string path );
	void		Release();
				Font();
	virtual		~Font();
};
#endif