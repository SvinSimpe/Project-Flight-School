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
		MapNodeInstance*** mBuildMap;
		float mVertexSpacing;
		UINT mNrOfNodes;
		UINT mMapId;

		std::vector<NavTriangle> mNavData;

	protected:
	public:
		UINT mMapDim;
	private:
	protected:
	public:
		HRESULT Render( float deltaTime, Player* player );
		void GenerateGrid();
		void OnLoadLevel( IEventPtr E1 );
		std::vector<DirectX::XMFLOAT2> GetPath( XMFLOAT3 start, XMFLOAT3 goal );

		UINT GetMapDim() const;
		UINT GetMapWidth() const;
		UINT GetMapHeight() const;

		UINT GetMapHalfWidth() const;
		UINT GetMapHalfHeight() const;

		UINT GetNrOfNodes() const;

		MapNodeInstance*** GetNodeMap() const;
		MapNodeInstance* GetNodeInstance( int x, int z );

		HRESULT Initialize( UINT mapDim );
		void Release();
		Map();
		~Map();
};
#endif