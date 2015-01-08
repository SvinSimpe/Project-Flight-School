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
		void	EventListener( IEventPtr newEvent );

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
		case Net_Event::EV_PLAYER_MOVED:
		{
			//printf("Eventet från servern var Event_Player_Moved och den innehöll positionerna:\n" ); // %f, %f, %f och %f, %f, %f
			EvPlayerMoved msg = (EvPlayerMoved&)p->body.content;
			IEventPtr E1( new Event_Remote_Player_Update( msg.id, msg.lowerBody, msg.upperBody, msg.direction ) );
			EventManager::GetInstance()->QueueEvent( E1 );
		}
			break;
		case Net_Event::EV_PLAYER_JOINED:
		{
			EvPlayerID msg = (EvPlayerID&)p->body.content;
			IEventPtr E1( new Event_Remote_Player_Joined( msg.ID ) );
			EventManager::GetInstance()->QueueEvent( E1 );
			printf( "Remote player with ID: %d joined.\n", msg.ID );
		}
			break;
		case Net_Event::EV_PLAYER_LEFT:
		{
			EvPlayerID msg = (EvPlayerID&)p->body.content;
			IEventPtr E1( new Event_Remote_Player_Left( msg.ID ) );
			EventManager::GetInstance()->QueueEvent( E1 );
			printf( "Remote player with ID: %d left.\n", msg.ID );
		}
			break;
		case Net_Event::YOUR_ID:
		{
			EvPlayerID msg	= (EvPlayerID&)p->body.content;
			mID						= msg.ID;
			printf( "Your id is %d.\n", msg.ID );
		}
			break;
		case Net_Event::EV_PLAYER_DIED:
		{
			EvPlayerID deadPlayer = (EvPlayerID&)p->body.content;
			IEventPtr E1( new Event_Remote_Player_Died( deadPlayer.ID ) );
			EventManager::GetInstance()->QueueEvent( E1 );
		}
			break;
		case Net_Event::EV_PLAYER_DAMAGED:
		{
			EvPlayerID damagedPlayer = (EvPlayerID&)p->body.content;
			IEventPtr E1( new Event_Remote_Player_Damaged( damagedPlayer.ID ) );
			EventManager::GetInstance()->QueueEvent( E1 );
		}
			break;
		default:
		{
			printf( "Error handling event from server.\n" );
		}
	}
}
#endif
