#ifndef MAPNODEPLACER_H
#define MAPNODEPLACER_H
#include "Map.h"
enum NODE_RETURN_CODE
{
	OCCUPIED,
	NOFIT,
	PLACED
};
class MapNodePlacer
{
	private:
		Map* mMap;
		MapNodeInstance*** mBuildMap;
		//std::vector<MapNodeInstance*> mNodes;
		int mNrOfNodes;

	protected:
	public:
		static MapNodePlacer* instance;
	private:
		NODE_RETURN_CODE CanPlace( int x, int y, MapNodeInstance* newNode );
	protected:
	public:

		static MapNodePlacer*			GetInstance();
	//	std::vector<MapNodeInstance*>	GetAllNodes() const;
		MapNodeInstance*				GetNodeInstance( int x, int z );
		void							BuildMap( MapNodeInstance** map );
		UINT							GetNrOfNodes() const;
		void							Reset();
		HRESULT							Initialize( Map* map );
		void							Release();
										MapNodePlacer();
		virtual							~MapNodePlacer();
};
#endif