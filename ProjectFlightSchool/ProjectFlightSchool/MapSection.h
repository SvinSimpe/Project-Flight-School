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
		//HRESULT Update( float deltaTime );
		void Render( float deltaTime );
		static void SetUpIndices();
		void SetUpVertices();
		HRESULT Initialize( UINT sectionID );
		void Release();
		MapSection();
		~MapSection();
};
#endif
