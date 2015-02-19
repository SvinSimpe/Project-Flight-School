#ifndef DSBUFFERSTREAM_H
#define DSBUFFERSTREAM_H

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>
#include "DSBuffer.h"

class DSBufferStream : public DSBuffer
{
private:
	int						mCurrentBuffer;
	FILE*					mFileptr;
	int						mDataSize;
	MMRESULT				mTimerID;

protected:

public:

private:
	bool	ReFillBuffer1();
	bool	ReFillBuffer2();
	bool	ReFillBuffer1Loop();
	bool	ReFillBuffer2Loop();

protected:

public:
	void		TimerCallBack();
	void		TimerCallBackLoop();
	void		PlayBuffer();
	void		PlayBufferLoop();
	void		StopBuffer();
	bool		FillBufferWithWave( LPDIRECTSOUND8 lpds, char *fileName );
	bool		Initialize( LPDIRECTSOUND8 lpds, char *fileName, int ID );
	void		Release();
				DSBufferStream();
				~DSBufferStream();
};

#endif // !DSBufferStream
