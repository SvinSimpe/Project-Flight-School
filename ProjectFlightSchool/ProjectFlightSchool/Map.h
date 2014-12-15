#ifndef MAP_H
#define MAP_H

#include "MapSection.h"

class Map
{
	private:
		MapSection *mMapSections;
		float mVertexSpacing;
		
		UINT mMapId;
	protected:
	public:
		UINT mVerticesPerAxis;

		UINT mSectionDim;

		UINT mMapDim;

	private:
		void InitializeVertices();
	protected:
	public:
		//HRESULT Update( float deltaTime );
		HRESULT Render( float deltaTime );
		void GenerateGrid();
		HRESULT Initialize( float vertexSpacing, UINT mapDim );
		void Release();
		Map();
		~Map();
};
#endif