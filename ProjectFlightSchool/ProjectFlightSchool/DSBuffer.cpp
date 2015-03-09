#include "DSBuffer.h"

//Kalla bara på denna en gång. Sen kan du använda IDirectSound8::DuplicateSoundBuffer
HRESULT DSBuffer::CreateBasicBuffer( LPDIRECTSOUND8 lpDirectSound, WaveHeaderType waveFileHeader )
{
	WAVEFORMATEX		wfx;
	DSBUFFERDESC		dsbdesc;
	HRESULT				hr;

	// Set up WAV format structure. 

	memset( &wfx, 0, sizeof(WAVEFORMATEX) );
	wfx.wFormatTag		= WAVE_FORMAT_PCM;
	wfx.nChannels		= waveFileHeader.numChannels;
	wfx.nSamplesPerSec	= waveFileHeader.sampleRate;
	wfx.nBlockAlign		= waveFileHeader.blockAlign;
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
	wfx.wBitsPerSample	= waveFileHeader.bitsPerSample;

	if ( waveFileHeader.dataSize == 0 )
	{
		waveFileHeader.dataSize = 3 * wfx.nAvgBytesPerSec;
	}

	// Set up DSBUFFERDESC structure. 

	memset( &dsbdesc, 0, sizeof(DSBUFFERDESC) );
	dsbdesc.dwSize			= sizeof(DSBUFFERDESC);
	dsbdesc.dwFlags			=
		DSBCAPS_CTRL3D | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY //DSBCAPS_CTRL3D och DSBCAPS_CTRLPAN kan inte användas samtidigt. PAN är höger/vänster och tror inte att det kommer att behövas. Antingen 3D eller inget låter vettigt.
		| DSBCAPS_GLOBALFOCUS; //Fortsätter spela trots att andra fönster är i fokus
	dsbdesc.dwBufferBytes	= waveFileHeader.dataSize;//3 * wfx.nAvgBytesPerSec; //storlek på 3 sekunders streamingdata
	dsbdesc.lpwfxFormat		= &wfx;

	// Create buffer. 


	for( int i = 0; i < mNrOfBuffers; i++ )
	{
		LPDIRECTSOUNDBUFFER pDsb = NULL;
		hr = lpDirectSound->CreateSoundBuffer( &dsbdesc, &pDsb, NULL );
		if ( FAILED( hr ) )
		{
			printf( "CreateSoundBuffer has failed\n" );
			return FAILED( hr );
		}
		else if ( SUCCEEDED( hr ) )
		{
			hr = pDsb->QueryInterface( IID_IDirectSoundBuffer8, (LPVOID*)&mBuffer[i] );
		}
		pDsb->Release();
	}
		
	return hr;
}

bool DSBuffer::FillBufferWithWave( LPDIRECTSOUND8 lpds, char *fileName, LONG volume )
{
	///////////////////////////Read header
	WaveHeaderType	waveFileHeader;
	FILE*			filePtr = 0;

	// Open the wave file in binary.
	int error = fopen_s( &filePtr, fileName, "rb" );
	if ( error != 0 )
	{
		printf( "fopen_s in ReadWaveHeader has failed\n" );
		return false;
	}

	// Read in the wave file header.
	int count = fread( &waveFileHeader, sizeof(waveFileHeader), 1, filePtr );
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

	///////////////////////////////////Prepare buffer desc
	WAVEFORMATEX		wfx;
	DSBUFFERDESC		dsbdesc;
	HRESULT				hr;

	// Set up WAV format structure. 

	memset (&wfx, 0, sizeof(WAVEFORMATEX) );
	wfx.wFormatTag		= WAVE_FORMAT_PCM;
	wfx.nChannels		= waveFileHeader.numChannels;
	wfx.nSamplesPerSec	= waveFileHeader.sampleRate;
	wfx.nBlockAlign		= waveFileHeader.blockAlign;
	wfx.nAvgBytesPerSec = wfx.nSamplesPerSec * wfx.nBlockAlign;
	wfx.wBitsPerSample	= waveFileHeader.bitsPerSample;

	if ( waveFileHeader.dataSize == 0 )
	{
		waveFileHeader.dataSize = 3 * wfx.nAvgBytesPerSec;
	}

	// Set up DSBUFFERDESC structure. 

	memset( &dsbdesc, 0, sizeof(DSBUFFERDESC) );
	dsbdesc.dwSize			= sizeof(DSBUFFERDESC);
	dsbdesc.dwFlags			=
		DSBCAPS_CTRL3D | DSBCAPS_CTRLVOLUME | DSBCAPS_CTRLFREQUENCY //DSBCAPS_CTRL3D och DSBCAPS_CTRLPAN can't be used at the same time. PAN is left/right.
		| DSBCAPS_GLOBALFOCUS; //Continue playing no matter which window is used.
	dsbdesc.dwBufferBytes	= waveFileHeader.dataSize;//3 * wfx.nAvgBytesPerSec; //3 seconds size
	dsbdesc.lpwfxFormat		= &wfx;

	////////////////////////////////////// Create buffer and initialize it. 
	for( int i = 0; i < mNrOfBuffers; i++ )
	{
		LPDIRECTSOUNDBUFFER pDsb = NULL;
		hr = lpds->CreateSoundBuffer( &dsbdesc, &pDsb, NULL );
		if ( FAILED( hr ) )
		{
			printf( "CreateSoundBuffer has failed\n" );
			return false;
		}
		else if ( SUCCEEDED( hr ) )
		{
		

				hr = pDsb->QueryInterface( IID_IDirectSoundBuffer8, (LPVOID*)&mBuffer[i] );
				if ( FAILED( hr ) )
				{
					printf( "QueryInterface has failed\n" );
					return false;
				}
		
			pDsb->Release();
		}
	}
	hr = CreateBasicBuffer( lpds, waveFileHeader );
	if ( FAILED( hr ) )
	{
		printf( "CreateBasicBuffer in FillBufferWithWave has failed\n" );
		return false;
	}

	/////////////////////////////////////// Fill the buffer with wavedata.

	unsigned char* waveData = new unsigned char[waveFileHeader.dataSize];
	if ( !waveData )
	{
		printf( "waveData array allocation in FillBufferWithWave has failed\n" );
		return false;
	}

	// Read in the wave file data into the newly created buffer.
	count = fread( waveData, 1, waveFileHeader.dataSize, filePtr );
	if ( count != waveFileHeader.dataSize )
	{
		printf( "fread in FillBufferWithWave has not read correctly\n" );
		return false;
	}

	error = fclose( filePtr );
	if ( error != 0 )
	{
		printf( "fclose in FillBufferWithWave has failed\n" );
		return false;
	}

	//////////////////Dags att fylla buffern
	
	for( int i = 0; i < mNrOfBuffers; i++ )
	{		
		unsigned char *bufferPtr;
		unsigned long bufferSize;

		// Lock the secondary buffer to write wave data into it.
		hr = mBuffer[i]->Lock( 0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0 );
		if ( FAILED( hr ) )
		{
			return false;
		}

		// Copy the wave data into the buffer.
		memcpy( bufferPtr, waveData, waveFileHeader.dataSize );

		// Unlock the secondary buffer after the data has been written to it.
		hr = mBuffer[i]->Unlock( (void*)bufferPtr, bufferSize, NULL, 0 );
		if ( FAILED( hr ) )
		{
			return false;
		}

		// Set volume of the buffer to 100%.
		hr = mBuffer[i]->SetVolume( -volume  ); // mellan 0 och -10 000
		if ( FAILED( hr ) )
		{
			printf( "SetVolume in main has failed\n" );
		}
	}
	
	// Release the wave data since it was copied into the secondary buffer.
	delete[] waveData;
	waveData = 0;

	return true;
}

void DSBuffer::PlayBuffer()
{
	LPDWORD lpwStatus = 0; 
	for( int i = 0; i < mNrOfBuffers; i++ )
	{		
		mBuffer[i]->GetStatus( lpwStatus );
		if( !( lpwStatus && DSBSTATUS_LOOPING || lpwStatus && DSBSTATUS_PLAYING  ) )
		{
			mBuffer[i]->SetCurrentPosition( 0 );
			HRESULT hr = mBuffer[i]->Play( 
				0,  // Unused.
				0,  // Priority for voice management.
				0 ); // Flags.
			if ( FAILED( hr ) )
			{
				printf( "Play in main has failed\n" );
			}
		}
	}
}

void DSBuffer::PlayBufferLoop()
{
	LPDWORD lpwStatus = 0; 
	for( int i = 0; i < mNrOfBuffers; i++ )
	{
		mBuffer[i]->GetStatus( lpwStatus );
		if( !( lpwStatus && DSBSTATUS_LOOPING || lpwStatus && DSBSTATUS_PLAYING  ) )
		{
			mBuffer[i]->SetCurrentPosition( 0 );
			HRESULT hr = mBuffer[i]->Play( 
			0,  // Unused.
			0,  // Priority for voice management.
			1 ); // Flags.
			if ( FAILED( hr ) )
			{
				MessageBox( NULL, L"Loopen spelas inte", L"Error", MB_OK );
			}
		}
	}
}

void DSBuffer::StopBuffer()
{
	for( int i = 0; i < mNrOfBuffers; i++ )
	{
		mBuffer[i]->Stop();
	}
}

DSBuffer  DSBuffer::DuplicateBuffer( LPDIRECTSOUND8 lpds, DSBuffer buffer1 )
{
	DSBuffer result;
	/*lpds->DuplicateSoundBuffer( buffer1.buffer, &result.buffer );*/
	return result;
}

char* DSBuffer::GetFileName()
{
	return mFileName;
}

int DSBuffer::GetID()
{
	return mID;
}

void DSBuffer::SoundOn()
{
	for( int i = 0; i < mNrOfBuffers; i++ )
	{
		mBuffer[i]->SetVolume( DSBVOLUME_MAX );
	}
}

void DSBuffer::SoundOff()
{
	for( int i = 0; i < mNrOfBuffers; i++ )
	{
		mBuffer[i]->SetVolume( DSBVOLUME_MIN );
	}
}

bool DSBuffer::Initialize( LPDIRECTSOUND8 lpds, char *fileName, int ID, LONG volume, int nrOfBuffers )
{
	mID				= ID;
	mFileName		= fileName;
	mBuffer			= new LPDIRECTSOUNDBUFFER8[nrOfBuffers];
	mNrOfBuffers	= nrOfBuffers;
	return FillBufferWithWave( lpds, mFileName, volume );
}

void DSBuffer::Release()
{
	for( int i = 0; i < mNrOfBuffers; i++ )
	{
		mBuffer[i]->Release();
	}
	delete [] mBuffer;
}

DSBuffer::DSBuffer()
{
	mBuffer = nullptr;
}

DSBuffer::~DSBuffer()
{

}