#ifndef MAPSECTION_H
#define MAPSECTION_H
#include <Graphics.h>
//#include "MapQuad.h"

static const UINT SECTION_DIMX = 10;
static const UINT SECTION_DIMZ = 10;

class MapSection
{
	private:

		UINT mIndices[SECTION_DIMX * SECTION_DIMZ * 6];
		UINT mSectionID;

	protected:
	public:

	private:
		void SetUpIndices();
	protected:
	public:
		//HRESULT Update( float deltaTime );
		UINT* GetIndices();
		HRESULT Initialize( UINT sectionID );
		void Release();
		MapSection();
		~MapSection();
};
#endif
