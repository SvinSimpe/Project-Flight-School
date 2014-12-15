#ifndef MAPQUAD_H
#define MAPQUAD_H
#include <Graphics.h>
//#include <Vertex.h>

class MapQuad
{
	private:
		StaticVertex mQuad[4];
		UINT mVertexSpacing;
		UINT mSectionId;

	protected:
	public:

	private:
		void SetUpVertices( UINT dimX, UINT dimY );
	protected:
	public:
		StaticVertex* GetQuad();
		HRESULT Initialize( UINT sectionId, UINT dimX, UINT dimY );
		void Release();
		MapQuad();
		~MapQuad();


};
#endif