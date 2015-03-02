#ifndef BUTTON_H
#define BUTTON_H

#include "Image.h"
#include "RenderManager.h"

#define CLICKCOOLDOWN 1.0f

// Hard coded button used by the menus
class Button : public Image
{
	private:
	protected:
		float		mClickTimer;

	public:

	private:
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

		XMFLOAT2		GetPosition() const;
		void			SetPosition( XMFLOAT2 pos, XMFLOAT2 size );
};
#endif