#include "Map.h"


void Map::InitializeVertices()
{
	float posX, posY, posZ;

	posY = 0.0f;

	float du=1.0f / ( MAP_WIDTH -1 );
	float dv=1.0f / ( MAP_HEIGHT - 1 );

	for( int z = 0; z < (int)MAP_HEIGHT; z++ )
	{
		posZ=z - (float)( MAP_HEIGHT * 0.5f );
		for( int x = 0; x < (int)MAP_WIDTH; x++ )
		{
			posX = x - (float)( ( MAP_WIDTH - 1) * 0.5f);
			mVertices[x + z * MAP_WIDTH].position[0]	= posX;
			mVertices[x + z * MAP_WIDTH].position[1]	= posY;
			mVertices[x + z * MAP_WIDTH].position[2]	= posZ;

			mVertices[x + z * MAP_WIDTH].uv[0]			= x * du;
			mVertices[x + z * MAP_WIDTH].uv[0]			= z * dv;

		}
	}
}
HRESULT Map::Initialize( UINT vertexSpacing )
{
	mVertexSpacing = vertexSpacing;
	InitializeVertices();

	mMapId = 0;

	return S_OK;
}
void Map::Release()
{
}
Map::Map()
{
}
Map::~Map()
{
}