#ifndef ENTRY_H
#define ENTRY_H

#include "Input.h"

// Abstract base class for any square object in the game, such as buttons
#define CLICKCOOLDOWN 1.0f

class Entry
{
	// Member variables
	private:
	protected:
		POINT		mUpperLeft;
		POINT		mLowerRight;
		float		mWidth;
		float		mHeight;
		float		mCurrentCD;
	public:

	// Member functions
	private:
	protected:
		bool			MouseInbounds(); // Function used to check if the mouse is within the bounds of the box
	public:
		bool			LeftMousePressed();
		bool			RightMousePressed();
		virtual void	Render() = 0;
		virtual bool	Update( float deltaTime );
		virtual bool	Initialize( UINT x, UINT y, UINT width, UINT height );
		void			Release();
						Entry();
		virtual			~Entry();
};
#endif