#include "MapSection.h"

void MapSection::Render( float deltaTime )
{
	Graphics::GetInstance()->RenderStatic3dAssetIndexed( mAssetID, INDEX_COUNT, 0 );
}
void MapSection::SetUpIndices()
{
	UINT index = 0;
	for( int i = 0; i < VERTICES_PER_X - 1; i++ )
	{
		for( int j = 0; j < VERTICES_PER_Z - 1; j++)
		{
			mIndices[index++] = j + ( i + 1 ) * VERTICES_PER_Z;
			mIndices[index++] = ( j + 1 ) + ( 1 + i ) * VERTICES_PER_Z;
			mIndices[index++] = j + i * VERTICES_PER_Z;

			mIndices[index++] = ( j + 1 ) + ( 1 + i ) * VERTICES_PER_Z;
			mIndices[index++] = ( j + 1 ) + ( i * VERTICES_PER_Z );
			mIndices[index++] = j + ( i * VERTICES_PER_Z );
		}
	}
}
void MapSection::SetUpVertices()
{
	float posX, posY, posZ;

	posY = 0.0f;

	float du=1.0f / ( SECTION_DIMX );
	float dv=1.0f / ( SECTION_DIMZ );
	int sectionZ = ( mSectionID / MAP_DIMZ ) - (float)( MAP_DIMZ * 0.5f );
	int sectionX = ( mSectionID % MAP_DIMX ) - (float)( MAP_DIMX * 0.5f );


	for( int z = 0; z < (int)VERTICES_PER_X; z++ )
	{
		//posZ=z - ( (float)( SECTION_DIMZ * 0.5f ) + ( ( SECTION_DIMZ ) * 0.5f ) * mSectionID );
		posZ = z - ( (float)( SECTION_DIMZ * 0.5f ) ) + ( sectionZ * SECTION_DIMZ );

		for( int x = 0; x < (int)VERTICES_PER_Z; x++ )
		{
			//posX =x - ( ( (float)( ( SECTION_DIMZ ) * 0.5f ) +  + ( ( SECTION_DIMZ ) * 0.5f ) * mSectionID ) );
			posX = x - ( (float)( SECTION_DIMZ * 0.5f ) ) + ( sectionX * SECTION_DIMX );//  * (int)( mSectionID - 5 );

			mVertices[x + z * VERTICES_PER_Z].position[0]	= posX;
			mVertices[x + z * VERTICES_PER_Z].position[1]	= posY;
			mVertices[x + z * VERTICES_PER_Z].position[2]	= posZ;

			mVertices[x + z * VERTICES_PER_Z].uv[0]			= x * du;
			mVertices[x + z * VERTICES_PER_Z].uv[1]			= z * dv;

			mVertices[x + z * VERTICES_PER_Z].normal[0]		= 0.0f;
			mVertices[x + z * VERTICES_PER_Z].normal[1]		= 1.0f;
			mVertices[x + z * VERTICES_PER_Z].normal[2]		= 0.0f;

		}
	}
}
UINT& MapSection::GetIndices() 
{
	return mIndices[0];
}
HRESULT MapSection::Initialize( UINT sectionID )
{
	mSectionID = sectionID;
	
	SetUpIndices();
	SetUpVertices();
	char name[50];
	sprintf_s( name, "mapSection%d", mSectionID );
	Indexed3DAssetInfo info;
	info.assetName		= name;
	info.indexCount		= INDEX_COUNT;
	info.indices		= mIndices;
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