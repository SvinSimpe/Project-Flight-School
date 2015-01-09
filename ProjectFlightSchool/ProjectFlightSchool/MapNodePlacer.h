#ifndef MAPNODEPLACER_H
#define MAPNODEPLACER_H
#include "Map.h"

class MapNodePlacer
{
	private:
		Map* mMap;
		char** mBuildMap;
		int mBuildMapSize;

	protected:
	public:
		static MapNodePlacer* instance;
	private:
		bool canPlace(int left, int top, MapNode node);
	protected:
	public:

		static MapNodePlacer* GetInstance();
		std::vector<MapNodeInstance> PlaceNodes();
		HRESULT			Initialize( Map* map );
		void			Release();
						MapNodePlacer();
		virtual			~MapNodePlacer();
};
#endif