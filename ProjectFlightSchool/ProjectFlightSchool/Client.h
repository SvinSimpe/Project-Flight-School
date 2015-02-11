#ifndef CLIENT_H
#define CLIENT_H

#include "Network.h"

class Client : public Network
{
	private:
		static Client*			mInstance;
		ClientSocketManager*	mSocketManager;
		NetworkEventForwarder*	mNEF;
		std::list<IEventPtr>	mEventList;
		bool					mActive;

	protected:
	public:

	private:
				Client();
		virtual	~Client();

		void	ServerCreateEnemy( IEventPtr eventPtr );
		void	ServerUpdateEnemy( IEventPtr eventPtr );

		void	StartUp( IEventPtr eventPtr );
		void	Shutdown( IEventPtr eventPtr );

	protected:
		bool	Connect( std::string ip, UINT port );
	public:
		static Client* GetInstance();
		void	SendEvent( IEventPtr eventPtr );
		void	FillEventList( std::list<IEventPtr> eventList );
		void	Update( float deltaTime );
		void	DoSelect( int pauseMicroSecs, bool handleInput = true );
		bool	Initialize();
		void	Release();
};
#endif