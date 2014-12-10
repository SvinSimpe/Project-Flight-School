#include "MapQuad.h"
void MapQuad::SetUpVertices( UINT dimX, UINT dimY )
{
	for ( int i = 0; i < 4; i++ )
	{
		float x, y, z;
		mQuad[i].position[0] = x + mSectionId - (dimX * 0.5f);
		mQuad[i].position[1] = y;
		mQuad[i].position[2] = z;

		mQuad[i].normal[0]= x;
		mQuad[i].normal[1] = y;
		mQuad[i].normal[2] = z;

		mQuad[i].tangent[0] = x;
		mQuad[i].tangent[1] = y;
		mQuad[i].tangent[2] = z;

		mQuad[i].uv[0] = x;
		mQuad[i].uv[1] = y;
	}
}
StaticVertex* MapQuad::GetQuad()
{
	return mQuad;
}
HRESULT MapQuad::Initialize( UINT sectionId, UINT dimX, UINT dimY )
{
	mVertexSpacing	= 1;
	mSectionId		= -1;
	return S_OK;
}
void MapQuad::Release()
{
}
MapQuad::MapQuad()
{
	mVertexSpacing	= 0;
	mSectionId		= 0;
}
MapQuad::~MapQuad()
{
}