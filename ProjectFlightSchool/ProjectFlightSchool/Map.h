#ifndef MAP_H
#define MAP_H

#include "MapSection.h"

class Map
{
	private:
		MapSection mMapSections[MAP_DIMX * MAP_DIMZ];
		UINT mVertexSpacing;
		
		UINT mMapId;
	protected:
	public:

	private:
		void InitializeVertices();
	protected:
	public:
		//HRESULT Update( float deltaTime );
		HRESULT Render( float deltaTime );
		void GenerateGrid();
		HRESULT Initialize( UINT vertexSpacing );
		void Release();
		Map();
		~Map();
};
#endif