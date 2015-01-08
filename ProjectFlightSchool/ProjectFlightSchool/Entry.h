#ifndef ENTRY_H
#define ENTRY_H

#include "Input.h"

#define CLICKCOOLDOWN 1.0f

class Entry
{
	// Member variables
	private:
		POINT		mUpperLeft;
		POINT		mLowerRight;
		float		mCurrentCD;
	protected:
	public:

	// Member functions
	private:
	protected:
	public:
		bool		LeftMousePressed();
		bool		RightMousePressed();
		void		Render();
		bool		Update( float deltaTime );
		bool		Initialize( UINT x, UINT y, UINT width, UINT height );
		void		Release();
					Entry();
		virtual		~Entry();
};
#endif