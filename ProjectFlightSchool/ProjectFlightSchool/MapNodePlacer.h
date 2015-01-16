#ifndef MAPNODEPLACER_H
#define MAPNODEPLACER_H
#include "Map.h"
#define MAX_NODES 1000
class MapNodePlacer
{
	private:
		Map* mMap;
		//MapNodeInstance** mPlacedNodes;
		MapNodeInstance*** mBuildMap;
		MapNodeInstance* mPlacedNodes[MAX_NODES];
		int mNrOfNodes;

	protected:
	public:
		static MapNodePlacer* instance;
	private:
		//int CanPlace( MapNodeInstance* currentNode, MapNodeInstance* newNode );
		bool CanPlace( int x, int y, MapNodeInstance* newNode );
	protected:
	public:

		static MapNodePlacer*	GetInstance();
		MapNodeInstance**		BuildMap();
		UINT					GetNrOfNodes() const;
		HRESULT					Initialize( Map* map );
		void					Release();
								MapNodePlacer();
		virtual					~MapNodePlacer();
};
#endif