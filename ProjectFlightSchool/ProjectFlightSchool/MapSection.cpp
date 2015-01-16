#include "MapSection.h"
#include "Map.h"
//obsolete
//void MapSection::SetUpIndices()
//{
//	UINT index = 0;
//	for( int i = 0; i < VERTICES_PER_AXIS - 1; i++ )
//	{
//		for( int j = 0; j < VERTICES_PER_AXIS - 1; j++)
//		{
//			MapSection::INDICES[index++] = j + ( i + 1 ) * VERTICES_PER_AXIS;
//			MapSection::INDICES[index++] = ( j + 1 ) + ( 1 + i ) * VERTICES_PER_AXIS;
//			MapSection::INDICES[index++] = j + i * VERTICES_PER_AXIS;
//
//			MapSection::INDICES[index++] = ( j + 1 ) + ( 1 + i ) * VERTICES_PER_AXIS;
//			MapSection::INDICES[index++] = ( j + 1 ) + ( i * VERTICES_PER_AXIS );
//			MapSection::INDICES[index++] = j + ( i * VERTICES_PER_AXIS );
//		}
//	}
//}

//obsolete
//HRESULT MapSection::Initialize( UINT sectionID, float vertexSpacing, UINT mapDim )
//{
//	mSectionID = sectionID;
//
//	float posX, posY, posZ;
//
//	posX = 0.0f;
//	posY = 0.0f;
//	posZ = 0.0f;
//
//	float du=1.0f / ( SECTION_DIM );
//	float dv=1.0f / ( SECTION_DIM );
//
//	float halfX = (float)mapDim * 0.5f;
//	float halfZ = (float)mapDim * 0.5f;
//
//	int xID = mSectionID % mapDim;
//	int zID = mSectionID / mapDim;
//
//	
//
//	float dx = halfX - xID; //  = ( mSectionID - halfX ) * 2;
//	float dz = halfZ - zID; // = ( sectionZ - halfZ ) * 2;
//
//	
//	for( int z = 0; z < (int)VERTICES_PER_AXIS; z++ )
//	{
//		posZ = ( ( dz * SECTION_DIM ) * vertexSpacing ) - ( z * vertexSpacing ); 
//		for( int x = 0; x < (int)VERTICES_PER_AXIS; x++ )
//		{
//			posX = ( ( dx * SECTION_DIM ) * vertexSpacing ) - ( x * vertexSpacing ); 
//			mVertices[x + z * VERTICES_PER_AXIS].position[0]	= posX;
//			mVertices[x + z * VERTICES_PER_AXIS].position[1]	= posY;
//			mVertices[x + z * VERTICES_PER_AXIS].position[2]	= posZ;
//										   
//			mVertices[x + z * VERTICES_PER_AXIS].uv[0]			= x * du;
//			mVertices[x + z * VERTICES_PER_AXIS].uv[1]			= z * dv;
//										   
//			mVertices[x + z * VERTICES_PER_AXIS].normal[0]		= 0.0f;
//			mVertices[x + z * VERTICES_PER_AXIS].normal[1]		= 1.0f;
//			mVertices[x + z * VERTICES_PER_AXIS].normal[2]		= 0.0f;
//
//		}
//	}
//	char name[50];
//	sprintf_s( name, "mapSection%d", mSectionID );
//	Indexed3DAssetInfo info;
//	info.assetName		= name;
//	info.indexCount		= INDEX_COUNT;
//	info.indices		= MapSection::INDICES;
//	info.vertexCount	= VERTEX_COUNT;
//	info.vertices		= mVertices;
//
//	Graphics::GetInstance()->LoadStatic3dAssetIndexed( info, mAssetID );
//	
//	return S_OK;
//}
HRESULT MapSection::Render( float deltaTime )
{
	for ( int i = 0; i < (int)mInstances.size(); i++ )
	{
		mInstances[i]->Render( deltaTime );
	}
	for( int i = 0; i < 4; i++ )
	{
		if( mChildren[i] != nullptr )
		{
			mChildren[i]->Render( deltaTime );
		}
	}
	return S_OK;
}
HRESULT MapSection::Initialize( Map* map, MapSection* parent, MapNodeInstance** mapNodes, int childID )
{
	//if null, its the first section
	mParent = parent;
	if( mParent == nullptr )
	{
		mBoundingBox.position = XMFLOAT3( -( (float)map->GetMapHalfWidth() * NODE_DIM ), 0, -( (float)map->GetMapHalfHeight() * NODE_DIM ) );
		mBoundingBox.width		= (float)map->GetMapWidth() * NODE_DIM;
		mBoundingBox.height		= (float)map->GetMapHeight() * NODE_DIM;
	}
	else
	{
		int offSetX = childID / 2;
		int offSetY = childID % 2;

		mBoundingBox.position = XMFLOAT3( ( mParent->mBoundingBox.position.x + ( offSetX * (mParent->mBoundingBox.width * 0.5f ) ) ),
										(  0 ),
										( ( mParent->mBoundingBox.position.z + ( offSetY * ( mParent->mBoundingBox.height * 0.5f ) ) ) ) );
		mBoundingBox.width	= (mParent->mBoundingBox.width * 0.5f );
		mBoundingBox.height = (mParent->mBoundingBox.height * 0.5f );

	}
	int count = 0;
	for( int i = 0; i < (int)map->GetNrOfNodes(); i++ )
	{
		if( mBoundingBox.Intersect( &mapNodes[i]->GetBoundingBox() ) )
		{
			count++;
		}
	}
	if ( 10 < count )
	{
		for(int i = 0; i < 4; i++)
		{
			mChildren[i] = new MapSection();
			mChildren[i]->Initialize( map, this, mapNodes, i );
			sections++;
		}
	}
	return S_OK;
}
bool MapSection::AddNodeToSection( MapNodeInstance* node )
{
	bool result = false;
	if( mBoundingBox.Intersect( &node->GetBoundingBox() ) )
	{
		for (int i = 0; i < 4; i++)
		{
			if (mChildren[i] != nullptr)
			{
				if ( mChildren[i]->AddNodeToSection( node ) )
				{
					return true;
				}
			}
		}
		mInstances.push_back( node );
		return true;
	}
	return result;
}
bool MapSection::GetSectionContainingUnit( MapSection** container, int& sectionCount, BoundingCircle* unit )
{
	if( mBoundingBox.Intersect( unit->center ) )
	{
		for( int i = 0; i < 4; i++ )
		{
			if( mChildren[i] != nullptr )
			{
				if( mChildren[i]->GetSectionContainingUnit( container, sectionCount, unit ) )
				{
					return true;
				}
			}
		}
		container[sectionCount++] = this;
		return true;
	}
	return false;
}
int MapSection::GetSections()
{
	return sections;
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