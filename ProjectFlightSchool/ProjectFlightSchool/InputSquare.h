#ifndef INPUTSQUARE_H
#define INPUTSQUARE_H

#include "Button.h"
#include "Font.h"
#include "Image.h"

#define KEYCOOLDOWN 0.12f;

class InputSquare : public Button
{
	private:
		Image*		mTitle;
		Font*		mText;
		std::string mCurrentText;
		bool		mActive;
		float		mKeyTimer;

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
		void		Initialize( std::string text, std::string imgName, float x, float y, float width, float height );
		void		Release();
					InputSquare();
		virtual		~InputSquare();
};
#endif