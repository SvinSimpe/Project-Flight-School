#ifndef DS3DBUFFER_H
#define DS3DBUFFER_H

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>
#include "DSBuffer.h"
#include <DirectXMath.h>
using namespace DirectX;

//struct WaveHeaderType
//{
//	char chunkId[4];
//	unsigned long chunkSize;
//	char format[4];
//	char subChunkId[4];
//	unsigned long subChunkSize;
//	unsigned short audioFormat;
//	unsigned short numChannels;
//	unsigned long sampleRate;
//	unsigned long bytesPerSecond;
//	unsigned short blockAlign;
//	unsigned short bitsPerSample;
//	char dataChunkId[4];
//	unsigned long dataSize;
//};

class DS3DBuffer
{
public:

protected:

private:
	LPDIRECTSOUNDBUFFER8	mBuffer;
	LPDIRECTSOUND3DBUFFER8	mBuffer3D;
	char*					mFileName;
	int						mID;

public:
	void		PlayBuffer( XMFLOAT3 pos );
	void		PlayBufferLoop( XMFLOAT3 pos );
	void		StopBuffer();
	DS3DBuffer	DuplicateBuffer( LPDIRECTSOUND8 lpds, DS3DBuffer buffer1 );
	bool		FillBufferWithWave( LPDIRECTSOUND8 lpds, char *fileName );
	char*		GetFileName();
	int			GetID();
	bool		Initialize( LPDIRECTSOUND8 lpds, char *fileName, int ID );
	void		Release();
				DS3DBuffer();
				~DS3DBuffer();

protected:

private:
	HRESULT CreateBasicBuffer( LPDIRECTSOUND8 lpDirectSound, WaveHeaderType waveFileHeader );

};

#endif // !DSBuffer
