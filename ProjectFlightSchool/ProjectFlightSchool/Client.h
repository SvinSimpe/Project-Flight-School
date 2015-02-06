#ifndef CLIENT_H
#define CLIENT_H

#include "Network.h"

class Client : public Network
{
	private:
		static Client*			mInstance;
		UINT					mID;
		UINT					mTeamID;
		ClientSocketManager*	mSocketManager;
		NetworkEventForwarder*	mNEF;
		std::list<UINT>			mRemoteIDs;
		std::map<UINT, NetEnemy>mEnemies;
		bool					mActive;

	protected:
	public:

	private:
				Client();
		virtual	~Client();

		void	ServerCreateEnemy( IEventPtr eventPtr );
		void	ServerUpdateEnemy( IEventPtr eventPtr );

		void	StartUp( IEventPtr eventPtr );

	protected:
		bool	Connect( std::string ip, UINT port );
	public:
		static Client* GetInstance();
		void	SendEvent( IEventPtr eventPtr );
		void	Update( float deltaTime );
		void	DoSelect( int pauseMicroSecs, bool handleInput = true );
		bool	Initialize();
		void	Release();
};
#endif