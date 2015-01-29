#ifndef CONNECTION_H
#define CONNECTION_H

#pragma comment( lib, "Winmm.lib" )

#include "PacketImpl.h"
#include <list>
#include <map>
#include <memory>
#include <mmsystem.h>
#include <stdio.h>

#define MAGIC_NUMBER (0x1f2e3d4c)
#define MAX_PACKET_SIZE (256)
#define RECV_BUFFER_SIZE (MAX_PACKET_SIZE * 512)

// The Netsocket-class, used by socket managers to send and receive data from a socket
class NetSocket
{
	private:
		friend class SocketManager;
		typedef std::list<std::shared_ptr<IPacket>> PacketList;

	protected:
		SOCKET		mSocket;
		int			mID; // A unique ID given by the SocketManager
		int			mDeleteFlag;
		PacketList	mOutList;
		PacketList	mInList;
		char		mRecvBuf[RECV_BUFFER_SIZE];
		UINT		mRecvOfs;
		UINT		mRecvBegin;
		bool		mBinaryProtocol;
		int			mSendOfs;
		UINT		mTimeOut;
		UINT		mIPAddr;
		int			mInternal;
		int			mTimeCreated;

	public:

	private:
	protected:
	public:
		bool			Connect( UINT ip, UINT port, bool forceCoalesce = false );
		void			SetBlocking( bool blocking );
		void			Send( std::shared_ptr<IPacket> pkt, bool clearTimeOut = 1);
		virtual int		HasOutput();
		virtual void	HandleOutput();
		virtual void	HandleInput();
		void			HandleException();
		virtual void	TimeOut();
		void			SetTimeOut( UINT ms = 45 * 1000 );
		UINT			GetIPAddress();
						NetSocket(); // Used by clients to initialize a NetSocket prior to calling Connect()
						NetSocket( SOCKET socket, UINT ip );
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
				NetListenSocket();
				NetListenSocket( int portNum );
};

class ServerListenSocket : public NetListenSocket
{
	private:
	protected:
	public:
		unsigned short gPort;

	private:
	protected:
	public:
		virtual void HandleInput();
					 ServerListenSocket( int portNum );
};

class RemoteEventSocket : public NetSocket
{
	private:
	protected:
	public:
		enum class NetMsg
		{
			Event,
			PlayerLoginOk
		};

	private:
	protected:
		void CreateEvent( std::istringstream &in );

	public:
		virtual void HandleInput();
		RemoteEventSocket( SOCKET newSocket, UINT ip );
		RemoteEventSocket();
};


// Base class for all socket managers
class SocketManager
{
	private:
	protected:
		WSADATA mWsaData;
		typedef std::list<NetSocket*> SocketList;
		typedef std::map<int, NetSocket*> SocketIDMap;

		SocketList mSocketList;
		SocketIDMap mSocketMap;
		int mNextSocketID;
		UINT mInbound;
		UINT mOutbound;
		UINT mMaxOpenSockets;
		UINT mSubnetMask;
		UINT mSubnet;

	public:

	private:
	protected:
		NetSocket* FindSocket( UINT sockID );

	public:
		void PrintError();
		int AddSocket( NetSocket* socket );
		void RemoveSocket( NetSocket* socket );
		UINT GetHostByName( const std::string &hostName );
		const char* GetHostByAddr( UINT ip );
		UINT GetIPAddress( UINT sockID );
		void SetSubnet( UINT subnet, UINT subnetMask );
		bool IsInternal( UINT ipAddr );
		bool Initialize();
		bool Send( UINT sockID, std::shared_ptr<IPacket> packet );
		void AddToOutbound( int rc );
		void AddToInbound( int rc );
		void Release();
		SocketManager();
		virtual ~SocketManager();
};

extern SocketManager* gSocketManager;
#endif