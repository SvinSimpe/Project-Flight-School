#ifndef DSBUFFER_H
#define DSBUFFER_H

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>

struct WaveHeaderType
{
	char			chunkId[4];
	unsigned long	chunkSize;
	char			format[4];
	char			subChunkId[4];
	unsigned long	subChunkSize;
	unsigned short	audioFormat;
	unsigned short	numChannels;
	unsigned long	sampleRate;
	unsigned long	bytesPerSecond;
	unsigned short	blockAlign;
	unsigned short	bitsPerSample;
	char			dataChunkId[4];
	unsigned long	dataSize;
};

class DSBuffer
{
private:

protected:
	LPDIRECTSOUNDBUFFER8	mBuffer;
	char*					mFileName;
	int						mID;

public:

private:

protected:
	HRESULT CreateBasicBuffer( LPDIRECTSOUND8 lpDirectSound, WaveHeaderType waveFileHeader );

public:
	void		PlayBuffer();
	void		PlayBufferLoop();
	void		StopBuffer();
	DSBuffer	DuplicateBuffer( LPDIRECTSOUND8 lpds, DSBuffer buffer1 );
	bool		FillBufferWithWave( LPDIRECTSOUND8 lpds, char *fileName );
	char*		GetFileName();
	int			GetID();
	bool		Initialize( LPDIRECTSOUND8 lpds, char *fileName, int ID );
	void		Release();
				DSBuffer();
				~DSBuffer();
};

#endif // !DSBuffer
