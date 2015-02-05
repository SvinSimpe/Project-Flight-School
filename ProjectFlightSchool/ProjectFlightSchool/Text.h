#ifndef TEXT_H
#define TEXT_H

#include "Font.h"

class Text : public Font
{
	private:
		float				mX;
		float				mY;
		float				mWidth;
		float				mHeight;
		float				mScale;
		DirectX::XMFLOAT4	mColor;
		std::string			mText;
	protected:
	public:

	private:
	protected:
	public:
		float		GetScale() const;
		std::string GetText() const;
		void		Render();
		void		SetText( std::string text, DirectX::XMFLOAT4 color );
		void		SetPosition( float x, float y );
		void		Initialize( std::string fontPath, std::string text, float x, float y, float scale, DirectX::XMFLOAT4 color = DirectX::XMFLOAT4( 1.0f, 1.0f, 1.0f, 1.0f ) );
		void		Release();
					Text();
		virtual		~Text();
};
#endif
