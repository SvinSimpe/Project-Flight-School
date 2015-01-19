#ifndef IMAGE_H
#define IMAGE_H

#include "Entry.h"
#include "RenderManager.h"

class Image : public Entry
{
	// Member variables
	private:
		AssetID		m2DTexture;
	protected:
	public:

	// Member functions
	private:
	protected:
	public:
		virtual void	Render();
		virtual bool	Update( float deltaTime );
		virtual void	Initialize( std::string, UINT x, UINT y, UINT width, UINT height );
		void			Release();
						Image();
		virtual			~Image();
};
#endif