#include "MapSection.h"
#include "Map.h"

HRESULT MapSection::Render( float deltaTime )
{
	for ( int i = 0; i < (int)mInstances.size(); i++ )
	{
		mInstances[i]->Render( deltaTime );
	}
	return S_OK;
}
HRESULT MapSection::Initialize( Map* map, MapSection* parent, MapNodeInstance** mapNodes, int childID )
{
	//if null, its the first section
	mParent = parent;
	if( mParent == nullptr )
	{
		mBoundingBox.position	= XMFLOAT3( -( (float)map->GetMapHalfWidth() * NODE_DIM ), 0, -( (float)map->GetMapHalfHeight() * NODE_DIM ) );
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
bool MapSection::GetSectionContainingUnit( MapSection** container, int& sectionCount, BoundingRectangle* unit )
{
	if( mBoundingBox.Intersect( unit ) )
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
	for( int i = 0;i < 4; i++ )
	{
		if( mChildren[i]!= nullptr )
		{
			mChildren[i]->Release();
			delete mChildren[i];
		}
	}
}
MapSection::MapSection()
{
}
MapSection::~MapSection()
{
}