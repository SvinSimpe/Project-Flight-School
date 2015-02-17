#include "DS3DBuffer.h"

bool DS3DBuffer::FillBufferWithWave( LPDIRECTSOUND8 lpds, char *fileName )
{
	DSBuffer::FillBufferWithWave( lpds, fileName );
	HRESULT hr = mBuffer->QueryInterface( IID_IDirectSound3DBuffer8, (LPVOID*)&mBuffer3D );
	if ( FAILED( hr ) )
	{
		MessageBox( NULL, L"Buffer3D failed", L"Error", MB_OK );
		return false;
	}

	return true;
}

void DS3DBuffer::PlayBuffer( XMFLOAT3 pos)
{
	D3DVECTOR posForFunction;
	posForFunction.x = pos.x;
	posForFunction.y = pos.y;
	posForFunction.z = pos.z;
	mBuffer3D->SetPosition( posForFunction.x, posForFunction.y, posForFunction.z, DS3D_IMMEDIATE );
	DSBuffer::PlayBuffer();
}

void DS3DBuffer::PlayBufferLoop( XMFLOAT3 pos )
{
	D3DVECTOR posForFunction;
	posForFunction.x = pos.x;
	posForFunction.y = pos.y;
	posForFunction.z = pos.z;
	mBuffer3D->SetPosition( posForFunction.x, posForFunction.y, posForFunction.z, DS3D_IMMEDIATE );
	DSBuffer::PlayBufferLoop();
}

void DS3DBuffer::StopBuffer()
{
	DSBuffer::StopBuffer();
}

bool DS3DBuffer::Initialize( LPDIRECTSOUND8 lpds, char *fileName, int ID )
{
	mID			= ID;
	mFileName	= fileName;
	return FillBufferWithWave( lpds, mFileName );
}

void DS3DBuffer::Release()
{
	DSBuffer::Release();
	mBuffer3D->Release();
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