#ifndef MOVINGBUTTON_H
#define MOVINGBUTTON_H

#include "Button.h"
#include "RenderManager.h"

#define FRAME_AMOUNT 45

// Hard coded button used by the menus
class MovingButton : public Button
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
		int			mFrameIndex;
	protected:
	public:

	private:
		void			SpinCircle();
	protected:
	public:
		virtual void	Render();
		virtual bool	Update( float deltaTime );
		void			Initialize( std::string textPath, float x, float y, float width, float height );
		void			Release();
						MovingButton();
		virtual			~MovingButton();
};
#endif