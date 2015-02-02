#ifndef SOUNDBUFFERHANDLER_H
#define SOUNDBUFFERHANDLER_H

#include "DSBuffer.h"
#include "DS3DBuffer.h"
#include "EventManager.h"
#include "Events.h"
#include <vector>

class SoundBufferHandler
{
public:
protected:
private:
	std::vector<DSBuffer>		mSoundAssets;
	std::vector<DS3DBuffer>		m3DSoundAssets;
	std::vector<DS3DBuffer>		mActive3DSounds;
	std::vector<DSBuffer>		mActiveSounds;
	LPDIRECTSOUND8				mDSDevice;
	LPDIRECTSOUND3DLISTENER8	mDSListener;

public:
	int							Load3DBuffer(char* fileName);
	int							LoadBuffer(char* fileName);
	void						Play(int SoundID);
	void						Loop(int BufferID);
	void						StopLoop(int BufferID);
	void						Play3D(int SoundID, XMFLOAT3 pos);
	void						Loop3D(int BufferID, XMFLOAT3 pos);
	void						StopLoop3D(int BufferID);
	bool						Initialize();
	void						Release();
	static	SoundBufferHandler*	GetInstance();

protected:
private:
	HRESULT	InitDevice();
	void	EventListener(IEventPtr newEvent);
	bool	GetDSListener();
			SoundBufferHandler();
			~SoundBufferHandler();

};

#endif