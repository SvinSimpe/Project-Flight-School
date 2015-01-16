#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include "Button.h"
#include "Text.h"

// Hard coded button used by the menus
class MenuButton : public Entry
{
	private:
		struct ActiveAsset
		{
			bool	active = false;
			AssetID asset;
		};

		const float SPIN_COOLDOWN = 0.09f;
		ActiveAsset mInner[3];
		ActiveAsset mMiddle[3];
		ActiveAsset mOuter[3];

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
					MenuButton();
		virtual		~MenuButton();
};
#endif