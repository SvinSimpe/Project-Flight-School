#ifndef SOUNDBUFFERHANDLER_H
#define SOUNDBUFFERHANDLER_H

#include "DSBuffer.h"
#include "DSBufferStream.h"
#include "DS3DBuffer.h"
#include "Events.h"
#include <vector>

class SoundBufferHandler
{
private:
	std::vector<DSBuffer>		mSoundAssets;
	std::vector<DSBufferStream>	mStreamSoundAssets;
	std::vector<DS3DBuffer>		m3DSoundAssets;
	std::vector<DS3DBuffer>		mActive3DSounds;
	std::vector<DSBuffer>		mActiveSounds;
	std::vector<DSBufferStream>	mActiveStreamSounds;
	LPDIRECTSOUND8				mDSDevice;
	LPDIRECTSOUND3DLISTENER8	mDSListener;
	bool						mSoundOn;

protected:
public:

private:
	HRESULT	InitDevice();
	void	EventListener( IEventPtr newEvent );
	bool	GetDSListener();
			SoundBufferHandler();
			~SoundBufferHandler();

protected:
public:
	int							Load3DBuffer( char* fileName, LONG volume = 0 ); //highest is 0, lowest is 10 000. Since it's decibels 5 000 is not half the sound.
	int							LoadBuffer( char* fileName, LONG volume = 0 ); //highest is 0, lowest is 10 000. Since it's decibels 5 000 is not half the sound.
	int							LoadStreamBuffer( char* fileName, LONG volume = 0 ); //highest is 0, lowest is 10 000. Since it's decibels 5 000 is not half the sound.
	void						Play( int SoundID );
	void						Loop( int BufferID );
	void						StopLoop( int BufferID );
	void						PlayStream( int SoundID );
	void						LoopStream( int BufferID );
	void						StopLoopStream( int BufferID );
	void						Play3D( int SoundID, XMFLOAT3 pos );
	void						Loop3D( int BufferID, XMFLOAT3 pos );
	void						StopLoop3D( int BufferID );
	void						SoundOn();
	void						SoundOff();
	bool						SoundIsOn() const;
	bool						Initialize();
	void						Release();
	static	SoundBufferHandler*	GetInstance();
};

#endif