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
		void			SetPos( float x, float y );
		void			SetDims( float w, float h );
		void			SetImage( AssetID texture );
		AssetID			GetAssetID() const;
		virtual void	Render();
		void			Render( float x, float y, float w, float h );
		virtual bool	Update( float deltaTime );
		virtual void	Initialize( std::string assetPath, float x, float y, float width, float height );
		void			Release();
						Image();
		virtual			~Image();

		XMFLOAT2		GetPosition() const;
};
#endif