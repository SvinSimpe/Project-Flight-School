#ifndef BUTTON_H
#define BUTTON_H

#include "Entry.h"

#define CLICKCOOLDOWN 1.0f

class Button : public Entry
{
	// Member variables
	private:
		float		mCurrentCD;
		AssetID		m2DTexture;
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
		void		Initialize(const char* assetPath, UINT x, UINT y, UINT width, UINT height );
		void		Release();
					Button();
		virtual		~Button();
};
#endif