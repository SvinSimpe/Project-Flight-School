#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include "Entry.h"
#include "RenderManager.h"

#define FRAME_AMOUNT 45

// Hard coded button used by the menus
class MenuButton : public Entry
{
	private:
		struct ActiveAsset
		{
			bool	active = false;
			AssetID asset;
		};

		const float SPIN_COOLDOWN = 0.04f;
		ActiveAsset mFrames[FRAME_AMOUNT];

		float		mSpinTimer;
		int			mCurrentIndex;
	protected:
	public:

	private:
		void		SpinCircle();
	protected:
	public:
		void		Render();
		bool		Update( float deltaTime );
		bool		Initialize( UINT x, UINT y, UINT width, UINT height );
		void		Release();
					MenuButton();
		virtual		~MenuButton();
};
#endif