#ifndef CLIENT_H
#define CLIENT_H

#include "Connection.h"
#include "EventManager.h"
#include "Events.h"

class Client // The class used by clients to connect to the server
{
	// Members
	private:
		unsigned int	mID;
		int				mResult;
		addrinfo*		mAddrResult;
		SOCKET			mServerSocket;
		Connection*		mConn;

	protected:

	public:

		// Template functions
	private:
		template <typename T>
		void HandlePkg( Package<T>* p );

	protected:
	public:
		
		// Functions
	private:
		bool	ReceiveLoop();
		void	PlayerMoved( IEventPtr newEvent );

	protected:

	public:
		bool	Connect();
		bool	Run();
		bool	Initialize( const char* port, const char* ip ); // Sets up and connects to the server
		void	Release();
				Client();
		virtual	~Client();
};

template <typename T>
void Client::HandlePkg( Package<T>* p )
{
	switch ( p->head.eventType )
	{
		case Net_Event::QUIT:
		{
			printf( "Disconnected from server.\n" );
			mConn->DisconnectSocket( mServerSocket );
		}
			break;
		case Net_Event::EV_PLAYER_MOVED:
		{
			//printf("Eventet från servern var Event_Player_Moved och den innehöll positionerna:\n" ); // %f, %f, %f och %f, %f, %f
			EvPlayerMoved msg = (EvPlayerMoved&)p->body.content;
			IEventPtr E1( new Event_Remote_Player_Update( mID, msg.lowerBody, msg.upperBody ) );
			EventManager::GetInstance()->QueueEvent( E1 );
		}
			break;
		case Net_Event::EV_PLAYER_JOINED:
		{
			EvPlayerConnection msg = (EvPlayerConnection&)p->body.content;
			IEventPtr E1( new Event_Remote_Player_Connection( msg.ID ) );
			EventManager::GetInstance()->QueueEvent( E1 );
			printf("Remote player with ID: %d joined.\n", msg.ID);
		}
			break;
		case Net_Event::ACCEPT_ID:
		{
			Message msg = (Message&)p->body.content;
			mID = (unsigned int)msg.msg;
			printf("Your ID is: %d.\n", mID);
		}
			break;
		default:
		{
			printf( "Error handling event from server.\n" );
		}
	}
}
#endif
