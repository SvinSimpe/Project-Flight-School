#ifndef MAPSECTION_H
#define MAPSECTION_H
#include <Graphics.h>
#include "MapConstants.h"
//#include "MapQuad.h"



class MapSection
{
	private:

		UINT mIndices[INDEX_COUNT];
		StaticVertex mVertices[VERTEX_COUNT];
		UINT mSectionID;
		UINT mAssetID;

	protected:
	public:

	private:

	protected:
	public:
		//HRESULT Update( float deltaTime );
		void Render( float deltaTime );
		void SetUpIndices();
		void SetUpVertices();
		UINT& GetIndices();
		HRESULT Initialize( UINT sectionID );
		void Release();
		MapSection();
		~MapSection();
};
#endif
