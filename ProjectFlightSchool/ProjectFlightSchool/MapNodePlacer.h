#ifndef MAPNODEPLACER_H
#define MAPNODEPLACER_H
#include "Map.h"
class MapNodePlacer
{
	private:
		Map* mMap;
		MapNodeInstance** mPlacedNodes;
		int mNrOfNodes;

	protected:
	public:
		static MapNodePlacer* instance;
	private:
		int CanPlace( MapNodeInstance* currentNode, MapNodeInstance* newNode );
	protected:
	public:

		static MapNodePlacer*	GetInstance();
		MapNodeInstance**		PlaceNodes();
		HRESULT					Initialize( Map* map );
		void					Release();
								MapNodePlacer();
		virtual					~MapNodePlacer();
};
#endif