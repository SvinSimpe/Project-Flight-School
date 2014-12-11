#ifndef MAP_H
#define MAP_H

#include "MapSection.h"


//--------------------------------TEST VALUES------------------------------------
static const UINT MAP_DIMX		= 10;
static const UINT MAP_DIMZ		= 10;
static const UINT MAP_WIDTH		= MAP_DIMX * SECTION_DIMX;
static const UINT MAP_HEIGHT	= MAP_DIMZ * SECTION_DIMZ;
static const UINT VERTEX_COUNT	= ( MAP_WIDTH ) * ( MAP_HEIGHT );
//--------------------------------------------------------------------------------

class Map
{
	private:
		MapSection mMapSections[MAP_DIMX * MAP_DIMZ];
		StaticVertex mVertices[VERTEX_COUNT];
		UINT mVertexSpacing;
		
		UINT mMapId;
	protected:
	public:

	private:
		void InitializeVertices();
	protected:
	public:
		//HRESULT Update( float deltaTime );
		HRESULT Initialize( UINT vertexSpacing );
		void Release();
		Map();
		~Map();
};
#endif