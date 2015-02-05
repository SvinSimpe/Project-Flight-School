#ifndef GUI_H
#define GUI_H

#include "Radar.h"
#include "Text.h"

#define MAX_REMOTE_PLAYERS 14

class Gui
{

	private:
		Radar*	mRadar;
		Text	mPlayerNames[MAX_REMOTE_PLAYERS - 1]; //Don't need to store the local player's name

	protected:

	public:

	private:

	protected:

	public:
		HRESULT	Update( DirectX::XMFLOAT3 playerPos, RADAR_UPDATE_INFO radarObjects[], UINT nrOfObjects, DirectX::XMFLOAT3 remotePlayerPos, std::string remotePlayerName, int remotePlayerID, bool updateRemotePlayerName );
		HRESULT	Render();
		HRESULT	Initialize();
		void	Release();
				Gui();
				~Gui();

};


#endif