#ifndef MAP_H
#define MAP_H
#include "MapSection.h"
#include "BoundingGeometry.h"
#define MAX_NODES 1000

static enum MapSize
{
	LARGE = 1200,
	MEDIUM = 600,
	SMALL = 300
};

class Map
{
	private:
		MapSection *mMapSection;
		MapNodeInstance* mNodes[MAX_NODES];
		float mVertexSpacing;
		UINT mNrOfNodes;
		UINT mMapId;
	protected:
	public:
		UINT mMapDim;
	private:
	protected:
	public:
		HRESULT Render( float deltaTime, Player* player );
		void GenerateGrid();

		UINT GetMapDim() const;
		UINT GetMapWidth() const;
		UINT GetMapHeight() const;

		UINT GetMapHalfWidth() const;
		UINT GetMapHalfHeight() const;

		UINT GetNrOfNodes() const;

		HRESULT Initialize( UINT mapDim );
		void Release();
		Map();
		~Map();
};
#endif