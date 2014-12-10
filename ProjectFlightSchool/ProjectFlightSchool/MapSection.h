#ifndef MAPSECTION_H
#define MAPSECTION_H
#include "MapQuad.h"

class MapSection
{
	private:
		unsigned int mDimX = 10;
		unsigned int mDimY = 10;
		MapQuad mSection[10*10];

	protected:
	public:

	private:

	protected:
		StaticVertex& GetVertices();
	public:
		//HRESULT Update( float deltaTime );
		HRESULT Initialize();
		void Release();
		MapSection();
		~MapSection();
};
#endif
