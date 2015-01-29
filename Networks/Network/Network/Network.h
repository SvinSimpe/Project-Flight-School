
#include <sys/types.h>
#include <WinSock2.h>
#include <assert.h>
#include "EventManager.h"

#define MAX_PACKET_SIZE (256)
#define RECV_BUFFER_SIZE (MAX_PACKET_SIZE * 512)
#define MAX_QUEUE_PER_PLAYER (10000)

#define MAGIC_NUMBER (0x1f2e3d4c)
#define IPMANGLE(a,b,c,d) (((a)<<24)|((b)<<16)|((c)<<8)|((d)))
#define INVALID_SOCKET_ID (-1)


#pragma warning(disable : 4996)

class NetSocket;

class IPacket
{
public:
	virtual char const* const VGetType() const=0;
	virtual char const* const VGetData() const=0;
	virtual u_long VGetSize() const =0;
	virtual ~IPacket() {};
};

class BinaryPacket : public IPacket
{
protected:
	char *mData;
public:
	//Code expansion, Inline replaces function call in instructions, and just compiles the code in place
	inline BinaryPacket( char const * const data, u_long size );
	inline BinaryPacket( u_long size );
	virtual char const* const VGetType() const { return gType; }
	virtual char const* const VGetData() const { return mData; }
	virtual u_long VGetSize() const { return ntohl( *(u_long *)mData ); }
	inline void MemCpy( char const* const data, size_t size, int destOffset );

	static const char* gType;
};

inline BinaryPacket::BinaryPacket( char const* const data, u_long size )
{
	mData = new char[size + sizeof(u_long)];
	
	*(u_long *)mData = htonl( size+sizeof(u_long) );
	memcpy( mData + sizeof(u_long), data, size );
}
inline BinaryPacket::BinaryPacket( u_long size )
{
	mData = new char[size + sizeof(u_long)];
	*(u_long *)mData = htonl( size+sizeof(u_long) );
}
inline void BinaryPacket::MemCpy( char const* const data, size_t size, int destOffset )
{
	//assert( ( size + destOffset ) <= ( VGetSize() - sizeOf(u_long) ) );
	memcpy( mData + destOffset + sizeof(u_long), data, size );
}

class TextPacket : public BinaryPacket
{
public:
	TextPacket( char const* const text );
	virtual char const* const VGetType() const { return gType; }

	static const char* gType;
};

class NetSocket
{
	friend class BaseSocketManager;
	typedef std::list< std::shared_ptr <IPacket> > PacketList;
public:
	NetSocket();
	NetSocket( SOCKET new_sock, unsigned int hostIP );
	virtual ~NetSocket();
	bool Connect( unsigned int ip, unsigned int port, bool forceCoalesce = 0 );
	void SetBlocking( bool blocking );
	void Send( std::shared_ptr<IPacket> pkt, bool clearTimeOut = 1 );
	virtual int VHasOutput() { return !mOutList.empty(); }
	virtual void VHandleOutput();
	virtual void VHandleInput();
	virtual void VTimeOut() { mTimeOut = 0; }
	void HandleException() { mDeleteFlag |= 1; }
	void SetTimeOut( unsigned int ms = 45 * 1000 ) { mTimeOut = timeGetTime() + ms; }
	int GetIpAddress() { return mIPaddr; }

protected:
	SOCKET mSock;
	int mID;
	int mDeleteFlag;
	
	PacketList mOutList;
	PacketList mInList;

	char mRecvBuf[RECV_BUFFER_SIZE];
	unsigned int mRecvOfs, mRecvBegin;
	bool mBinaryProtocol;

	unsigned int mSendOfs;
	unsigned int mTimeOut;
	unsigned int mIPaddr;

	int mInternal;
	int mTimeCreated;
};

class BaseSocketManager
{
protected:
	WSADATA mWsaData;

	typedef std::list<NetSocket*> SocketList;
	typedef std::map<int, NetSocket *> SocketIdMap;

	SocketList mSockList;
	SocketIdMap mSockMap;

	int mNextSocketId;
	unsigned int mInbound;
	unsigned int mOutbound;
	unsigned int mMaxOpenSockets;
	unsigned int mSubnetMask;
	unsigned int mSubnet;

	NetSocket *FindSocket( int sockId );
public:
	BaseSocketManager();
	virtual ~BaseSocketManager() { Shutdown(); }
	
	void DoSelect( int pauseMicroSecs, bool handleInput = true );
	
	bool Init();
	void Shutdown();
	void PrintError();

	int AddSocket( NetSocket *socket );
	void RemoveSocket( NetSocket *socket );

	unsigned int GetHostByName( const std::string & hostName );
	const char *GetHostByAddr( unsigned int ip );

	int GetIpAddress( int sockId );

	void SetSubnet( unsigned int subnet, unsigned int subnetMask )
	{
		mSubnet		= subnet;
		mSubnetMask	= subnetMask;
	}
	bool IsInternal( unsigned int ipaddr );

	bool Send( int sockId, std::shared_ptr<IPacket> packet );

	void AddToOutbound( int rc ) { mOutbound += rc; }
	void AddToInbound( int rc ) { mInbound += rc; }
};

extern BaseSocketManager *gpSocketManager;


class ClientSocketManager : public BaseSocketManager
{
	std::string mHostName;
	unsigned int mPort;
public:
	ClientSocketManager( const std::string &hostName, unsigned int port )
	{
		mHostName	= hostName;
		mPort		= port;
	}

	bool Connect();
};

class NetListenSocket : public NetSocket
{
public:
	NetListenSocket() { };
	NetListenSocket( int portnum );

	void Init( int portnum );
	void InitScan( int portnum_min, int portnum_max );
	SOCKET AcceptConnection( unsigned int *pAddr );

	unsigned short port;

};

class GameServerListenSocket: public NetListenSocket
{
public:
	GameServerListenSocket( int portnum ) { Init( portnum ); }

	virtual void VHandleInput();

};


class RemoteEventSocket : public NetSocket
{
public:
	enum
	{
		NetMsg_Event,
		NetMsg_PlayerLoginOk,
	};

	RemoteEventSocket( SOCKET newSock, unsigned int hostIP )
		: NetSocket( newSock, hostIP )
	{
	}

	RemoteEventSocket() {};

	virtual void VHandleInput();

protected:
	void CreateEventW( std::istrstream &in );
};

class NetworkEventForwarder
{
public:
	NetworkEventForwarder( int sockID ) { mSockID = sockID; }
	void ForwardEvent( IEventDataPtr pEventData );
protected:
	int mSockID;
};
