#ifndef MAPSECTION_H
#define MAPSECTION_H
#include <Graphics.h>
#include "MapConstants.h"
//#include "MapQuad.h"



class MapSection
{
	private:
		StaticVertex mVertices[VERTEX_COUNT];
		UINT mSectionID;
		UINT mAssetID;


	protected:
	public:
		static UINT INDICES[INDEX_COUNT];
	private:

	protected:
	public:
		void Render( float deltaTime );
		static void SetUpIndices();
		HRESULT Initialize( UINT sectionID,  float vertexSpacing, UINT mapDim );
		void Release();
		MapSection();
		~MapSection();
};
#endif
