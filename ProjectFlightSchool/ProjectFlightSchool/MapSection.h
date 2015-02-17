#ifndef MAPSECTION_H
#define MAPSECTION_H
#include <Graphics.h>
#include "MapConstants.h"
#include "MapNodeInstance.h"
#include "Player.h"
//#include "MapQuad.h"

static int sections = 0;

class Map;
class MapSection
{
	private:
		BoundingRectangle mBoundingBox;
		MapSection* mParent;
		MapSection*	mChildren[4];
		std::vector<MapNodeInstance*> mInstances;
	protected:
	public:
	private:

	protected:
	public:
		HRESULT Render( float deltaTime );
		HRESULT Initialize( UINT sectionID,  float vertexSpacing, UINT mapDim );
		HRESULT Initialize( Map* map, MapSection* parent, MapNodeInstance** mMapNodes, int childID );
		bool	AddNodeToSection( MapNodeInstance* node );
		bool	GetSectionContainingUnit( MapSection** container, int& sectionCount, BoundingRectangle* unit );
		int		GetSections();

		void Release();
		MapSection();
		~MapSection();
};
#endif
