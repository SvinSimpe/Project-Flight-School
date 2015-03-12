#include "DSBufferStream.h"

void CALLBACK TimerProcess( UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2 )
{
	DSBufferStream* ptr = (DSBufferStream*)dwUser;
	ptr->TimerCallBack();
}

void CALLBACK TimerProcessLoop( UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2 )
{
	DSBufferStream* ptr = (DSBufferStream*)dwUser;
	ptr->TimerCallBackLoop();
}

bool DSBufferStream::ReFillBuffer1()
{
	unsigned char* waveData = new unsigned char[mDataSize];
	if ( !waveData )
	{
		printf( "waveData array allocation in FillBufferWithWave has failed\n" );
		return false;
	}

	// Read in the wave file data into the newly created buffer.
	int count = fread( waveData, 1, mDataSize, mFileptr );
	if ( count != mDataSize )
	{
		fseek( mFileptr, sizeof(WaveHeaderType), SEEK_SET );
		timeKillEvent( mTimerID );
	}

	//////////////////Dags att fylla buffern

	for( int i = 0; i < mNrOfBuffers; i++ )
	{
		unsigned char *bufferPtr;
		unsigned long bufferSize;

		// Lock the secondary buffer to write wave data into it.
		HRESULT hr = mBuffer[i]->Lock( 0, mDataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0 );
		if ( FAILED( hr ) )
		{
			return false;
		}

		// Copy the wave data into the buffer.
		memcpy( bufferPtr, waveData, mDataSize );

		// Unlock the secondary buffer after the data has been written to it.
		hr = mBuffer[i]->Unlock( (void*)bufferPtr, bufferSize, NULL, 0 );
		if ( FAILED( hr ) )
		{
			return false;
		}
	}
	delete[] waveData;
	waveData = 0;
	
	return true;
}

bool DSBufferStream::ReFillBuffer2()
{
	unsigned char* waveData = new unsigned char[mDataSize];
	if ( !waveData )
	{
		printf( "waveData array allocation in FillBufferWithWave has failed\n" );
		return false;
	}

	// Read in the wave file data into the newly created buffer.
	int count = fread( waveData, 1, mDataSize, mFileptr );
	if ( count != mDataSize )
	{
		fseek( mFileptr, sizeof(WaveHeaderType), SEEK_SET );
		timeKillEvent( mTimerID );
	}

	//////////////////Dags att fylla buffern

	for( int i = 0; i < mNrOfBuffers; i++ )
	{
		unsigned char *bufferPtr;
		unsigned long bufferSize;

		// Lock the secondary buffer to write wave data into it.
		HRESULT hr = mBuffer[i]->Lock( mDataSize, mDataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0 );
		if ( FAILED( hr ) )
		{
			return false;
		}

		// Copy the wave data into the buffer.
		memcpy( bufferPtr, waveData, mDataSize );

		// Unlock the secondary buffer after the data has been written to it.
		hr = mBuffer[i]->Unlock( (void*)bufferPtr, bufferSize, NULL, 0 );
		if ( FAILED( hr ) )
		{
			return false;
		}
	}
	delete[] waveData;
	waveData = 0;

	return true;
}

bool DSBufferStream::ReFillBuffer1Loop()
{
	unsigned char* waveData = new unsigned char[mDataSize];
	if ( !waveData )
	{
		printf( "waveData array allocation in FillBufferWithWave has failed\n" );

		delete [] waveData;

		return false;
	}

	// Read in the wave file data into the newly created buffer.
	int count = fread( waveData, 1, mDataSize, mFileptr );
	if ( count != mDataSize )
	{
		fseek( mFileptr, sizeof(WaveHeaderType), SEEK_SET );
		unsigned char* waveData2 = new unsigned char[mDataSize - count];
		fread( waveData2, 1, mDataSize - count, mFileptr );
		for( int i = 0; i < mDataSize - count; i++ )
		{
			waveData[count + i] = waveData2[i];
		}

		delete [] waveData2;
	}

	//////////////////Dags att fylla buffern

	for( int i = 0; i < mNrOfBuffers; i++ )
	{
		unsigned char *bufferPtr;
		unsigned long bufferSize;

		// Lock the secondary buffer to write wave data into it.
		HRESULT hr = mBuffer[i]->Lock( 0, mDataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0 );
		if ( FAILED( hr ) )
		{
			delete [] waveData;
			return false;
		}


		// Copy the wave data into the buffer.
		memcpy( bufferPtr, waveData, mDataSize );
		

		// Unlock the secondary buffer after the data has been written to it.
		hr = mBuffer[i]->Unlock( (void*)bufferPtr, bufferSize, NULL, 0 );
		if ( FAILED( hr ) )
		{
			delete [] waveData;

			return false;
		}
	}

	delete [] waveData;

	return true;
}

bool DSBufferStream::ReFillBuffer2Loop()
{
	unsigned char* waveData = new unsigned char[mDataSize];
	if ( !waveData )
	{
		printf( "waveData array allocation in FillBufferWithWave has failed\n" );
		delete [] waveData;
		return false;
	}

	// Read in the wave file data into the newly created buffer.
	int count = fread( waveData, 1, mDataSize, mFileptr );
	if ( count != mDataSize )
	{
		fseek( mFileptr, sizeof(WaveHeaderType), SEEK_SET );
		unsigned char* waveData2 = new unsigned char[mDataSize - count];
		fread( waveData2, 1, mDataSize - count, mFileptr );
		for( int i = 0; i < mDataSize - count; i++ )
		{
			waveData[count + i] = waveData2[i];
		}

		delete [] waveData2;
	}

	//////////////////Dags att fylla buffern
	
	for( int i = 0; i < mNrOfBuffers; i++ )
	{
		unsigned char *bufferPtr;
		unsigned long bufferSize;

		// Lock the secondary buffer to write wave data into it.
		HRESULT hr = mBuffer[i]->Lock( mDataSize, mDataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0 );
		if ( FAILED( hr ) )
		{
			delete [] waveData;
			return false;
		}

		// Copy the wave data into the buffer.
		memcpy( bufferPtr, waveData, mDataSize );

		// Unlock the secondary buffer after the data has been written to it.
		hr = mBuffer[i]->Unlock( (void*)bufferPtr, bufferSize, NULL, 0 );
		if ( FAILED( hr ) )
		{
			delete [] waveData;

			return false;
		}

	}

	delete [] waveData;

	return true;
}

bool DSBufferStream::FillBufferWithWave( LPDIRECTSOUND8 lpds, char *fileName, LONG volume )
{
	///////////////////////////Read header
	WaveHeaderType	waveFileHeader;

	// Open the wave file in binary.
	int error = fopen_s( &mFileptr, fileName, "rb" );
	if ( error != 0 )
	{
		printf( "fopen_s in ReadWaveHeader has failed\n" );
		return false;
	}

	// Read in the wave file header.
	int count = fread( &waveFileHeader, sizeof(waveFileHeader), 1, mFileptr );
	if ( count != 1 )
	{
		printf( "fread in ReadWaveHeader has failed\n" );
		return false;
	}
	// Check that the chunk ID is the RIFF format.
	if ( ( waveFileHeader.chunkId[0] != 'R' ) || ( waveFileHeader.chunkId[1] != 'I' ) ||
		( waveFileHeader.chunkId[2] != 'F' ) || ( waveFileHeader.chunkId[3] != 'F' ) )
	{
		return false;
	}

	// Check that the file format is the WAVE format.
	if ( ( waveFileHeader.format[0] != 'W' ) || ( waveFileHeader.format[1] != 'A' ) ||
		( waveFileHeader.format[2] != 'V' ) || ( waveFileHeader.format[3] != 'E' ) )
	{
		return false;
	}

	// Check that the sub chunk ID is the fmt format.
	if ( ( waveFileHeader.subChunkId[0] != 'f' ) || ( waveFileHeader.subChunkId[1] != 'm' ) ||
		( waveFileHeader.subChunkId[2] != 't' ) || ( waveFileHeader.subChunkId[3] != ' ' ) )
	{
		return false;
	}

	// Check that the audio format is WAVE_FORMAT_PCM.
	if ( waveFileHeader.audioFormat != WAVE_FORMAT_PCM )
	{
		return false;
	}

	// Check for the data chunk header.
	if ( ( waveFileHeader.dataChunkId[0] != 'd' ) || ( waveFileHeader.dataChunkId[1] != 'a' ) ||
		( waveFileHeader.dataChunkId[2] != 't' ) || ( waveFileHeader.dataChunkId[3] != 'a' ) )
	{
		return false;
	}
	
	if( waveFileHeader.numChannels == 1 )
	{
		waveFileHeader.numChannels	= 2;
		waveFileHeader.sampleRate	/= 2;
	}

	waveFileHeader.dataSize = 2 * waveFileHeader.sampleRate * ( ( waveFileHeader.numChannels * waveFileHeader.bitsPerSample ) / 8 );

	mDataSize = waveFileHeader.dataSize / 2;

	HRESULT hr = CreateBasicBuffer( lpds, waveFileHeader );
	if ( FAILED( hr ) )
	{
		printf( "CreateBasicBuffer in FillBufferWithWave has failed\n" );
		return false;
	}
	
	for( int i = 0; i < mNrOfBuffers; i++ )
	{
		hr = mBuffer[i]->SetVolume( -volume );
		if ( FAILED( hr ) )
		{
			printf( "SetVolume in main has failed\n" );
		}
	}


	return true;
}

void DSBufferStream::TimerCallBack()
{
	//Switch buffers
	if( mCurrentBuffer == 1 )
	{
		mCurrentBuffer = 2;
		ReFillBuffer1();
	}
	else
	{
		mCurrentBuffer = 1;
		ReFillBuffer2();
	}
}

void DSBufferStream::TimerCallBackLoop()
{
	//Switch buffers
	if( mCurrentBuffer == 1 )
	{
		mCurrentBuffer = 2;
		ReFillBuffer1Loop();
	}
	else
	{
		mCurrentBuffer = 1;
		ReFillBuffer2Loop();
	}
}

void DSBufferStream::PlayBuffer()
{
	mCurrentBuffer	= 1;
	ReFillBuffer1();
	ReFillBuffer2();
	DSBuffer::PlayBuffer();
	mTimerID = timeSetEvent( 1000, 0, TimerProcess, (DWORD)this, TIME_PERIODIC | TIME_CALLBACK_FUNCTION );
}

void DSBufferStream::PlayBufferLoop()
{
	mCurrentBuffer	= 1;
	ReFillBuffer1Loop();
	ReFillBuffer2Loop();
	DSBuffer::PlayBufferLoop();
	mTimerID = timeSetEvent( 1000, 0, TimerProcessLoop, (DWORD)this, TIME_PERIODIC | TIME_CALLBACK_FUNCTION );
}

void DSBufferStream::StopBuffer()
{
	DSBuffer::StopBuffer();
	timeKillEvent( mTimerID );
	fseek( mFileptr, sizeof(WaveHeaderType), SEEK_SET );
}

bool DSBufferStream::Initialize( LPDIRECTSOUND8 lpds, char *fileName, int ID, LONG volume, int nrOfBuffers )
{
	mFileptr		= nullptr;
	mID				= ID;
	mFileName		= fileName;
	mNrOfBuffers	= nrOfBuffers;
	mBuffer			= new LPDIRECTSOUNDBUFFER8[nrOfBuffers];
	return FillBufferWithWave( lpds, mFileName, volume );
}

void DSBufferStream::Release()
{
	DSBuffer::Release();
	fclose( mFileptr );
}

DSBufferStream::DSBufferStream()
{
	DSBuffer::DSBuffer();
	mCurrentBuffer	= 0;
}

DSBufferStream::~DSBufferStream()
{
	DSBuffer::~DSBuffer();
}