#include "MapSection.h"

UINT MapSection::INDICES[INDEX_COUNT] = {0};

void MapSection::Render( float deltaTime )
{
	Graphics::GetInstance()->RenderStatic3dAssetIndexed( mAssetID, INDEX_COUNT, 0 );
}
void MapSection::SetUpIndices()
{
	UINT index = 0;
	for( int i = 0; i < VERTICES_PER_AXIS - 1; i++ )
	{
		for( int j = 0; j < VERTICES_PER_AXIS - 1; j++)
		{
			MapSection::INDICES[index++] = j + ( i + 1 ) * VERTICES_PER_AXIS;
			MapSection::INDICES[index++] = ( j + 1 ) + ( 1 + i ) * VERTICES_PER_AXIS;
			MapSection::INDICES[index++] = j + i * VERTICES_PER_AXIS;

			MapSection::INDICES[index++] = ( j + 1 ) + ( 1 + i ) * VERTICES_PER_AXIS;
			MapSection::INDICES[index++] = ( j + 1 ) + ( i * VERTICES_PER_AXIS );
			MapSection::INDICES[index++] = j + ( i * VERTICES_PER_AXIS );
		}
	}
}
HRESULT MapSection::Initialize( UINT sectionID, float vertexSpacing, UINT mapDim )
{
	mSectionID = sectionID;

	float posX, posY, posZ;

	posX = 0.0f;
	posY = 0.0f;
	posZ = 0.0f;

	float du=1.0f / ( SECTION_DIM );
	float dv=1.0f / ( SECTION_DIM );

	float halfX = (float)mapDim * 0.5f;
	float halfZ = (float)mapDim * 0.5f;

	int xID = mSectionID % mapDim;
	int zID = mSectionID / mapDim;

	

	float dx = xID - halfX; //  = ( mSectionID - halfX ) * 2;
	float dz = zID - halfZ; // = ( sectionZ - halfZ ) * 2;

	
	for( int z = 0; z < (int)VERTICES_PER_AXIS; z++ )
	{
		posZ = ( ( dz * SECTION_DIM ) * vertexSpacing ) + ( z * vertexSpacing ); 
		for( int x = 0; x < (int)VERTICES_PER_AXIS; x++ )
		{
			posX = ( ( dx * SECTION_DIM ) * vertexSpacing ) + ( x * vertexSpacing ); 
			mVertices[x + z * VERTICES_PER_AXIS].position[1]	= posY;
			mVertices[x + z * VERTICES_PER_AXIS].position[0]	= posX;
			mVertices[x + z * VERTICES_PER_AXIS].position[2]	= posZ;
										   
			mVertices[x + z * VERTICES_PER_AXIS].uv[0]			= x * du;
			mVertices[x + z * VERTICES_PER_AXIS].uv[1]			= z * dv;
										   
			mVertices[x + z * VERTICES_PER_AXIS].normal[0]		= 0.0f;
			mVertices[x + z * VERTICES_PER_AXIS].normal[1]		= 1.0f;
			mVertices[x + z * VERTICES_PER_AXIS].normal[2]		= 0.0f;

		}
	}


	char name[50];
	sprintf_s( name, "mapSection%d", mSectionID );
	Indexed3DAssetInfo info;
	info.assetName		= name;
	info.indexCount		= INDEX_COUNT;
	info.indices		= MapSection::INDICES;
	info.vertexCount	= VERTEX_COUNT;
	info.vertices		= mVertices;

	Graphics::GetInstance()->LoadStatic3dAssetIndexed( name, info, mAssetID );
	
	return S_OK;
}
void MapSection::Release()
{
}
MapSection::MapSection()
{
}
MapSection::~MapSection()
{
}