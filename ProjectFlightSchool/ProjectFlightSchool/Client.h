#ifndef CLIENT_H
#define CLIENT_H

#include "Network.h"
#include <iostream>

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

		// TESTING CODE FOR EVENTS BELOW
		XMFLOAT3	mLowerBodyPos;
		XMFLOAT3	mVelocity;
		XMFLOAT3	mUpperBodyDirection;

	protected:
	public:

	private:
				Client();
		virtual	~Client();

		void	LocalJoined( IEventPtr eventPtr );
		void	RemoteJoined( IEventPtr eventPtr );
		void	RemoteLeft( IEventPtr eventPtr );
		void	RemoteUpdate( IEventPtr eventPtr );
		void	RemoteDied( IEventPtr eventPtr );
		void	RemoteDamaged( IEventPtr eventPtr );
		void	RemoteSpawned( IEventPtr eventPtr );
		void	RemoteFiredProjectile( IEventPtr eventPtr );
		void	RemoteUpdateHP( IEventPtr eventPtr );
		void	RemoteMeleeHit( IEventPtr eventPtr );
		void	RemoteAttack( IEventPtr eventPtr );
		void	RemoteDown( IEventPtr eventPtr );
		void	RemoteUp( IEventPtr eventPtr );
		void	RemoteAttemptRevive( IEventPtr eventPtr );

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