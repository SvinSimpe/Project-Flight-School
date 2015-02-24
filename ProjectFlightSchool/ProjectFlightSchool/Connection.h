#ifndef CONNECTION_H
#define CONNECTION_H

#pragma comment( lib, "Winmm.lib" )
//#pragma warning( disable:C4996 )

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "PacketImpl.h"
#include "Events.h"
#include <list>
#include <map>
#include <memory>
#include <mmsystem.h>
#include <stdio.h>
#include <sstream>
#include <iostream>

#define MAGIC_NUMBER		(0x1f2e3d4c)
#define MAX_PACKET_SIZE		(256)
#define RECV_BUFFER_SIZE	(MAX_PACKET_SIZE * 512)

// The Netsocket-class, used by socket managers to send and receive data from a socket
class NetSocket
{
	private:
		typedef std::list<std::shared_ptr<IPacket>> PacketList;
		friend class SocketManager;

	protected:
		SocketManager*	mSocketManager;
		SOCKET			mSocket;
		int				mID; // A unique ID given by the SocketManager
		int				mDeleteFlag;
		PacketList		mOutList;
		PacketList		mInList;
		char			mRecvBuf[RECV_BUFFER_SIZE];
		UINT			mRecvOfs;
		UINT			mRecvBegin;
		bool			mBinaryProtocol;
		int				mSendOfs;
		UINT			mTimeOut;
		UINT			mIPAddr;
		int				mInternal;
		int				mTimeCreated;

	public:

	private:
	protected:
	public:
		bool			Connect( UINT ip, UINT port, bool forceCoalesce = false );
		void			SetBlocking( bool blocking );
		void			Send( std::shared_ptr<IPacket> pkt, bool clearTimeOut = 1);
		virtual bool	HasOutput();
		virtual void	HandleOutput();
		virtual bool	HandleInput();
		void			HandleException();
		virtual void	TimeOut();
		void			SetTimeOut( UINT ms = 45 * 1000 );
		UINT			GetIPAddress();
						NetSocket( SocketManager* socketManager ); // Used by clients to initialize a NetSocket prior to calling Connect()
						NetSocket( SocketManager* socketManager, SOCKET socket, UINT ip );
		virtual			~NetSocket();
};

class NetListenSocket : public NetSocket
{
	private:
	protected:
	public:
		unsigned short gPort;

	private:
	protected:
	public:
		void	InitScan( int portNum_min, int portNum_max );
		SOCKET	AcceptConnection( UINT* addr );
		void	Initialize( int portNum );
				NetListenSocket( SocketManager* socketManager );
				NetListenSocket( SocketManager* socketManager, int portNum );
};

class ServerListenSocket : public NetListenSocket
{
	private:
	protected:
	public:

	private:
	protected:
	public:
		virtual bool	HandleInput();
						ServerListenSocket( SocketManager* socketManager, int portNum );
};

class RemoteEventSocket : public NetSocket
{
	private:
	protected:
	public:
		// This enum is made just in case we need to send something else than a packet
		enum
		{
			NetMsg_Event,			// If this is sent, an event is created
		};

	private:
	protected:
		void CreateEvent( std::istringstream& in );

	public:
		virtual bool HandleInput();
		RemoteEventSocket( SocketManager* socketManager, SOCKET newSocket, UINT ip );
		RemoteEventSocket( SocketManager* socketManager );
};


// Base class for all socket managers
class SocketManager
{
	private:
	protected:
		typedef std::list<NetSocket*> SocketList;
		typedef std::map<int, NetSocket*> SocketIDMap;

		WSADATA		mWsaData;
		SocketList	mSocketList;
		SocketIDMap mSocketMap;
		int			mNextSocketID;
		UINT		mInbound;
		UINT		mOutbound;
		UINT		mMaxOpenSockets;
		UINT		mSubnetMask;
		UINT		mSubnet;

	public:

	private:
	protected:
		NetSocket*	FindSocket( UINT sockID );

	public:
		void		PrintError();
		int			AddSocket( NetSocket* socket );
		void		RemoveSocket( NetSocket* socket );
		UINT		GetHostByName( const std::string &hostName );
		const char* GetHostByAddr( UINT ip );
		UINT		GetIPAddress( UINT sockID );
		void		SetSubnet( UINT subnet, UINT subnetMask );
		bool		IsInternal( UINT ipAddr );
		bool		Send( UINT sockID, std::shared_ptr<IPacket> packet );
		void		AddToOutbound( int rc );
		void		AddToInbound( int rc );
		void		DoSelect( int pauseMicroSecs, bool handleInput = true );
		bool		Initialize();
		void		Release();
					SocketManager();
		virtual		~SocketManager();
};

// class used by the clients
class ClientSocketManager : public SocketManager
{
	private:
		std::string mHostName;
		UINT		mPort;
	protected:
	public:

	private:
	protected:
	public:
		bool	Connect( const std::string &hostName, UINT port );
				ClientSocketManager();
		virtual ~ClientSocketManager() {}
};

class NetworkEventForwarder
{
	private:
	protected:
		UINT			mSocketID;
		SocketManager*	mSocketManager;

	public:

	private:
	protected:
	public:
		void	ForwardEvent( IEventPtr eventPtr );
		void	Initialize( UINT socketID, SocketManager* sm );
				NetworkEventForwarder();
		virtual	~NetworkEventForwarder();
};
#endif