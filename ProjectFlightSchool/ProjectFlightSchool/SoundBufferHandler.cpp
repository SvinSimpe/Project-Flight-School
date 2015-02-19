#include "SoundBufferHandler.h"

int SoundBufferHandler::Load3DBuffer( char* fileName )
{
	for ( auto s : m3DSoundAssets )
	{
		if ( s.GetFileName() == fileName )
		{
			return s.GetID();
		}
	}
	DS3DBuffer buffer;
	int id = m3DSoundAssets.size();
	buffer.Initialize( mDSDevice, fileName, id );
	m3DSoundAssets.push_back( buffer );
	return id;
}

int SoundBufferHandler::LoadBuffer( char* fileName )
{
	for ( auto s : mSoundAssets )
	{
		if ( s.GetFileName() == fileName )
		{
			return s.GetID();
		}
	}
	DSBuffer buffer;
	int id = mSoundAssets.size();
	buffer.Initialize( mDSDevice, fileName, id );
	mSoundAssets.push_back( buffer );
	return id;
}

int SoundBufferHandler::LoadStreamBuffer( char* fileName )
{
	for ( auto s : mStreamSoundAssets )
	{
		if ( s.GetFileName() == fileName )
		{
			return s.GetID();
		}
	}
	DSBufferStream buffer;
	int id = mStreamSoundAssets.size();
	buffer.Initialize( mDSDevice, fileName, id );
	mStreamSoundAssets.push_back( buffer );
	return id;
}

void SoundBufferHandler::Play( int SoundID )
{
	mSoundAssets.at(SoundID).PlayBuffer();
}

void SoundBufferHandler::Loop( int SoundID )
{
	mSoundAssets.at(SoundID).PlayBufferLoop();
}

void SoundBufferHandler::StopLoop( int SoundID )
{
	mSoundAssets.at(SoundID).StopBuffer();
}

void SoundBufferHandler::PlayStream( int SoundID )
{
	mStreamSoundAssets.at(SoundID).PlayBuffer();
}

void SoundBufferHandler::LoopStream( int SoundID )
{
	mStreamSoundAssets.at(SoundID).PlayBufferLoop();
}

void SoundBufferHandler::StopLoopStream( int SoundID )
{
	mStreamSoundAssets.at(SoundID).StopBuffer();
}

void SoundBufferHandler::Play3D( int SoundID, XMFLOAT3 pos )
{
	m3DSoundAssets.at(SoundID).PlayBuffer( pos );
}

void SoundBufferHandler::Loop3D( int SoundID, XMFLOAT3 pos )
{
	m3DSoundAssets.at(SoundID).PlayBufferLoop( pos );
}

void SoundBufferHandler::StopLoop3D( int SoundID )
{
	m3DSoundAssets.at(SoundID).StopBuffer();
}

bool SoundBufferHandler::Initialize()
{
	EventManager::GetInstance()->AddListener( &SoundBufferHandler::EventListener, this, Event_Client_Update::GUID );
	bool result = SUCCEEDED( InitDevice() );
	if ( result )
	{
		result = GetDSListener();
	}
	if ( !result )
	{
		MessageBox( NULL, L"GetListener failed", L"Error", MB_OK );
	}
	return result;
}

void SoundBufferHandler::Release()
{
	for ( auto& s : mSoundAssets )
	{
		s.Release();
	}
	mSoundAssets.clear();
	for ( auto& s : mStreamSoundAssets )
	{
		s.Release();
	}
	mStreamSoundAssets.clear();
	for ( auto& s : m3DSoundAssets )
	{
		s.Release();
	}
	m3DSoundAssets.clear();
	for ( auto& s : mActiveSounds )
	{
		s.Release();
	}
	mActiveSounds.clear();
	for ( auto& s : mActiveStreamSounds )
	{
		s.Release();
	}
	mActiveStreamSounds.clear();
	for ( auto& s : mActive3DSounds )
	{
		s.Release();
	}
	mActive3DSounds.clear();
	mDSListener->Release();
	mDSDevice->Release();
}

SoundBufferHandler::SoundBufferHandler()
{
	mDSDevice		= nullptr;
	mSoundAssets	= std::vector<DSBuffer>(0);
	mActiveSounds	= std::vector<DSBuffer>(0);
	mStreamSoundAssets	= std::vector<DSBufferStream>(0);
	mActiveStreamSounds	= std::vector<DSBufferStream>(0);
	m3DSoundAssets	= std::vector<DS3DBuffer>(0);
	mActive3DSounds = std::vector<DS3DBuffer>(0);
	mDSListener		= nullptr;
}

SoundBufferHandler::~SoundBufferHandler()
{
	
}

HRESULT SoundBufferHandler::InitDevice()
{
    /*HRESULT hr = CoCreateInstance( CLSID_DirectSound8,
              NULL, 
              CLSCTX_INPROC_SERVER,
              IID_IDirectSound8,
              (LPVOID*) &mDSDevice );
    if ( FAILED( hr ) )
    {
      MessageBox( NULL, L"InitDevice failed", L"Error", MB_OK );
    }
	hr = mDSDevice->Initialize( NULL );
    if ( FAILED( hr ) )
    {
      MessageBox( NULL, L"InitDevice failed", L"Error", MB_OK );
    }*/

	HRESULT hr = DirectSoundCreate8( NULL, &mDSDevice, NULL );

	HWND hwnd = GetForegroundWindow();
	hr = mDSDevice->SetCooperativeLevel( hwnd, DSSCL_PRIORITY );
	if ( FAILED( hr ) )
	{
		printf( "SetCooperaticeLevel has failed\n" );
		return hr;
	}

	return S_OK;
}

void SoundBufferHandler::EventListener( IEventPtr newEvent )
{
	if (newEvent->GetEventType() == Event_Client_Update::GUID) // Add a remote player to the list when they connect
	{
		std::shared_ptr<Event_Client_Update> data = std::static_pointer_cast<Event_Client_Update>(newEvent);
		D3DVECTOR pos;
		pos.x = data->LowerBodyPos().x;
		pos.y = data->LowerBodyPos().y;
		pos.z = data->LowerBodyPos().z;
		mDSListener->SetPosition(pos.x, pos.y, pos.z, DS3D_IMMEDIATE);
	}
}

bool SoundBufferHandler::GetDSListener()
{
	DSBUFFERDESC             dsbd;
	LPDIRECTSOUNDBUFFER      lpdsbPrimary;  // Cannot be IDirectSoundBuffer8.
	LPDIRECTSOUND3DLISTENER8 lp3DListener = NULL;
	HRESULT hr;

	ZeroMemory(&dsbd, sizeof(DSBUFFERDESC));
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_CTRL3D | DSBCAPS_PRIMARYBUFFER;

	if ( SUCCEEDED( hr = mDSDevice->CreateSoundBuffer( &dsbd, &lpdsbPrimary, NULL ) ) )
	{
		hr = lpdsbPrimary->QueryInterface( IID_IDirectSound3DListener8,
			(LPVOID *)&mDSListener );
		lpdsbPrimary->Release();
	}
	mDSListener->SetPosition( 0, 0, 0, DS3D_IMMEDIATE );
	return SUCCEEDED( hr );
}

SoundBufferHandler* SoundBufferHandler::GetInstance()
{
	static SoundBufferHandler instance;
	return &instance;
}