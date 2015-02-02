#ifndef IMAGE_H
#define IMAGE_H

#include "Entry.h"
#include "RenderManager.h"

class Image : public Entry
{
	// Member variables
	private:
	protected:
		AssetID			m2DTexture;
	public:

	// Member functions
	private:
	protected:
	public:
		virtual void	Render();
		virtual bool	Update( float deltaTime );
		virtual void	Initialize( std::string assetPath, float x, float y, float width, float height );
		void			Release();
						Image();
		virtual			~Image();
};
#endif