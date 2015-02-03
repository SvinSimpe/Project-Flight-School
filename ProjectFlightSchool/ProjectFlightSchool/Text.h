#ifndef TEXT_H
#define TEXT_H

#include "Font.h"

class Text : public Font
{
	private:
		float mX;
		float mY;
		float mWidth;
		float mHeight;
		float mScale;
		std::string mText;
	protected:
	public:

	private:
	protected:
	public:
		void	Render();
		void	SetText( std::string text );
		void	SetPosition( float x, float y );
		void	Initialize( std::string fontPath, std::string text, float x, float y, float scale );
		void	Release();
				Text();
		virtual ~Text();
};
#endif
