#ifndef INPUTSQUARE_H
#define INPUTSQUARE_H

#include "Entry.h"
#include "Font.h"
#include "Image.h"

#define KEYCOOLDOWN 0.12f;

class InputSquare : public Entry
{
	private:
		Image		mInputImage[2];
		Font		mText;
		std::string mCurrentText;
		bool		mActive;
		float		mKeyCD;

	protected:
	public:
	
	private:
		void		Add( std::string letter );
		void		Pop();
	protected:
	public:
		bool		IsActive();
		void		SwitchActive( bool b );
		std::string GetText();
		void		HandleInput();
		void		Render();
		bool		Update( float deltaTime );
		bool		Initialize( std::string text, std::string imgName, float x, float y, float width, float height );
		void		Release();
					InputSquare();
		virtual		~InputSquare();
};
#endif