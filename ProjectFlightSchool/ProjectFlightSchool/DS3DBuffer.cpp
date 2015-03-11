#include "DS3DBuffer.h"

bool DS3DBuffer::FillBufferWithWave( LPDIRECTSOUND8 lpds, char *fileName, LONG volume )
{
	DSBuffer::FillBufferWithWave( lpds, fileName, volume );
	
	for( int i = 0; i < mNrOfBuffers; i++ )
	{
		HRESULT hr = mBuffer[i]->QueryInterface( IID_IDirectSound3DBuffer8, (LPVOID*)&mBuffer3D[i] );
		if ( FAILED( hr ) )
		{
			MessageBox( NULL, L"Buffer3D failed", L"Error", MB_OK );
			return false;
		}
	}

	return true;
}

void DS3DBuffer::PlayBuffer( XMFLOAT3 pos)
{
	DWORD  lpwStatus	= 0; 
	bool played			= false;
 	for( int i = 0; i < mNrOfBuffers; i++ )
	{		
		mBuffer[i]->GetStatus( &lpwStatus );
		if( !( lpwStatus & DSBSTATUS_LOOPING || lpwStatus & DSBSTATUS_PLAYING  ) )
		{
			D3DVECTOR posForFunction;
			posForFunction.x = pos.x;
			posForFunction.y = pos.y;
			posForFunction.z = pos.z;
			mBuffer3D[i]->SetPosition( posForFunction.x, posForFunction.y, posForFunction.z, DS3D_IMMEDIATE );
			mBuffer[i]->SetCurrentPosition( 0 );
			HRESULT hr = mBuffer[i]->Play( 
				0,  // Unused.
				0,  // Priority for voice management.
				0 ); // Flags.
			if ( FAILED( hr ) )
			{
				printf( "Play in main has failed\n" );
			}
			played = true;
			break;
		}
	}
	if( !played )
	{
		//OutputDebugString( L"Not enough buffers\n" );
	}
}

void DS3DBuffer::PlayBufferLoop( XMFLOAT3 pos )
{
	DWORD lpwStatus = 0; 
	bool played		= false;
	for( int i = 0; i < mNrOfBuffers; i++ )
	{
		mBuffer[i]->GetStatus( &lpwStatus );
		if( !( lpwStatus & DSBSTATUS_LOOPING || lpwStatus & DSBSTATUS_PLAYING  ) )
		{
			D3DVECTOR posForFunction;
			posForFunction.x = pos.x;
			posForFunction.y = pos.y;
			posForFunction.z = pos.z;
			mBuffer3D[i]->SetPosition( posForFunction.x, posForFunction.y, posForFunction.z, DS3D_IMMEDIATE );
			mBuffer[i]->SetCurrentPosition( 0 );
			HRESULT hr = mBuffer[i]->Play( 
			0,  // Unused.
			0,  // Priority for voice management.
			1 ); // Flags.
			if ( FAILED( hr ) )
			{
				MessageBox( NULL, L"Loopen spelas inte", L"Error", MB_OK );
			}
			played = true;
			break;
		}
	}
	if( !played )
	{
		OutputDebugString( L"Not enough buffers\n" );
	}
}

void DS3DBuffer::StopBuffer()
{
	DSBuffer::StopBuffer();
}

bool DS3DBuffer::Initialize( LPDIRECTSOUND8 lpds, char *fileName, int ID, LONG volume, int nrOfBuffers )
{
	mID				= ID;
	mFileName		= fileName;
	mNrOfBuffers	= nrOfBuffers;
	mBuffer			= new LPDIRECTSOUNDBUFFER8[nrOfBuffers];
	mBuffer3D		= new LPDIRECTSOUND3DBUFFER8[nrOfBuffers];
	return FillBufferWithWave( lpds, mFileName, volume );
}

void DS3DBuffer::Release()
{
	DSBuffer::Release();
	for( int i = 0; i < mNrOfBuffers; i++ )
	{	
		mBuffer3D[i]->Release();
	}
	delete [] mBuffer3D;
}

DS3DBuffer::DS3DBuffer()
{
	DSBuffer::DSBuffer();
	mBuffer3D	= nullptr;
}

DS3DBuffer::~DS3DBuffer()
{
	DSBuffer::~DSBuffer();
}