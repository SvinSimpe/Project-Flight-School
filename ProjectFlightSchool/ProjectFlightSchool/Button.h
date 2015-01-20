#ifndef BUTTON_H
#define BUTTON_H

#include "Image.h"
#include "RenderManager.h"

#define FRAME_AMOUNT 45
#define CLICKCOOLDOWN 1.0f

// Hard coded button used by the menus
class Button : public Image
{
	private:
		struct ActiveAsset
		{
			bool	active = false;
			AssetID asset;
		};

		const float SPIN_COOLDOWN = 0.04f;
		ActiveAsset mFrames[FRAME_AMOUNT];

		Image*		mTextImage;

		float		mSpinTimer;
		int			mFrameIndex;
	protected:
		float		mClickTimer;
	public:

	private:
		void			SpinCircle();
		bool			MouseInbounds();
	protected:
	public:
		bool			LeftMousePressed();
		bool			RightMousePressed();
		void			SetExitCooldown();
		virtual void	Render();
		virtual bool	Update( float deltaTime );
		void			Initialize( std::string textPath, float x, float y, float width, float height );
		void			Release();
						Button();
		virtual			~Button();
};
#endif