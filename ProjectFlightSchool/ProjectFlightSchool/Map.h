#ifndef MAP_H
#define MAP_H


#include "MapSection.h"


class Map
{
	private:
		MapSection *mMapSections;
		std::vector<MapNodeInstance> nodes;
		float mVertexSpacing;
		
		UINT mMapId;
	protected:
	public:
		UINT mMapDim;
	private:
	protected:
	public:
		//HRESULT Update( float deltaTime );
		HRESULT Render( float deltaTime );
		void GenerateGrid();

		UINT GetMapDim() const;
		UINT GetMapWidth() const;
		UINT GetMapHeight() const;

		float GetMapHalfWidth() const;
		float GetMapHalfHeight() const;

		HRESULT Initialize( float vertexSpacing, UINT mapDim );
		void Release();
		Map();
		~Map();
};
#endif