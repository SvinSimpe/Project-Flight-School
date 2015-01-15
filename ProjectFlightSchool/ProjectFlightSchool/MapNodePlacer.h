#ifndef MAPNODEPLACER_H
#define MAPNODEPLACER_H
#include "Map.h"

class MapNodePlacer
{
	private:
		Map* mMap;
		int** mBuildMap;
		int mBuildMapSize;

	protected:
	public:
		static MapNodePlacer* instance;
	private:
		//bool canPlace(int left, int top, MapNode node);
		int CanPlace(MapNodeInstance* currentNode, MapNodeInstance* newNode);
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