#ifndef ENTRY_H
#define ENTRY_H

#include "Input.h"

class Entry
{
	// Member variables
	private:
		POINT mUpperLeft;
		POINT mLowerRight;
	protected:
	public:

	// Member functions
	private:
		bool IsPressed();
	protected:
	public:
		void Render();
		bool Update( float deltaTime );
		bool Initialize( UINT x, UINT y, UINT width, UINT height );
		void Release();
		Entry();
		~Entry();
};
#endif