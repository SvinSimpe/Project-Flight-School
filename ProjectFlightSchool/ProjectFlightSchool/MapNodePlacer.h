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
		//std::vector<MapNodeInstance*> mNodes;
		int mNrOfNodes;

	protected:
	public:
		static MapNodePlacer* instance;
	private:
		NODE_RETURN_CODE CanPlace( int x, int y, MapNodeInstance* newNode, MapNodeInstance*** buildMap );
	protected:
	public:

		static MapNodePlacer*			GetInstance();
	//	std::vector<MapNodeInstance*>	GetAllNodes() const;
		void							BuildMap( MapNodeInstance***& buildMap );
		UINT							GetNrOfNodes() const;
		void							Reset();
		HRESULT							Initialize( Map* map );
		void							Release();
										MapNodePlacer();
		virtual							~MapNodePlacer();
};
#endif