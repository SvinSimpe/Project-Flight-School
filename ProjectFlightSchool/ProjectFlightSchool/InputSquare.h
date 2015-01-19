#ifndef INPUTSQUARE_H
#define INPUTSQUARE_H

#include "Entry.h"
#include "Font.h"
#include "Image.h"

#define KEYCOOLDOWN 0.12f;

class InputSquare : public Entry
{
	private:
		Image		mInputImage;
		Font		mText;
		std::string mCurrentText;
		bool		mActive;
		float		mKeyCD;

	protected:
	public:
	
	private:
		void		Add(std::string letter);
		void		Pop();
	protected:
	public:
		bool		IsActive();
		void		SwitchActive( bool b );
		std::string GetText();
		void		HandleInput();
		void		Render();
		bool		Update( float deltaTime );
		bool		Initialize( std::string text, UINT x, UINT y, UINT width, UINT height );
		void		Release();
					InputSquare();
		virtual		~InputSquare();
};
#endif