#include "MapSection.h"

void MapSection::SetUpIndices()
{
	UINT index = 0;
	for( int i = 0; i < SECTION_DIMX - 1  ; i++ )
	{
		for( int j = 0; j < SECTION_DIMZ - 1 ; j++)
		{
			mIndices[index++] = j + ( i * SECTION_DIMZ ) + ( SECTION_DIMZ * mSectionID );
			mIndices[index++] = ( j + 1 ) + ( i * SECTION_DIMZ )  + ( SECTION_DIMZ * mSectionID );
			mIndices[index++] = ( j + 1 ) + ( 1 + i ) * SECTION_DIMZ  + ( SECTION_DIMZ * mSectionID );

			mIndices[index++] = j + i * SECTION_DIMZ  + ( SECTION_DIMZ * mSectionID );
			mIndices[index++] = ( j + 1 ) + ( 1 + i ) * SECTION_DIMZ  + ( SECTION_DIMZ * mSectionID );
			mIndices[index++] = j + ( i + 1 ) * SECTION_DIMZ  + ( SECTION_DIMZ * mSectionID );
		}
	}
}
UINT* MapSection::GetIndices() 
{
	return mIndices;
}
HRESULT MapSection::Initialize( UINT sectionID )
{
	mSectionID = sectionID;

	SetUpIndices();

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