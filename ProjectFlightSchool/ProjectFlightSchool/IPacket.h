#ifndef IPACKET_H
#define IPACKET_H

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Winsock2.h>
#include <ws2tcpip.h>
#include <IPHlpApi.h>
#include <Windows.h>

#pragma comment( lib, "Ws2_32.lib" )

#define MAGIC_NUMBER (0x1f2e3d4c)

#if !defined(SAFE_DELETE)
#define SAFE_DELETE( x ) if( x ){ delete x; x = nullptr; }
#endif

#if !defined(SAFE_DELETE_ARRAY)
#define SAFE_DELETE_ARRAY( x ) if( x ){ delete [] x; x = nullptr; }
#endif

#if !defined(SAFE_RELEASE)
#define SAFE_RELEASE( x ) if( x ){ x->Release(); x = nullptr; }
#endif

#if defined(_DEBUG)
#	define PFS_NEW new(_NORMAL_BLOCK,__FILE__, __LINE__)
#else
#	define PFS_NEW new
#endif

#define PFS_ASSERT( x ) ( (void)( 0 && ( x ) ) )

typedef unsigned long PacketType;

// This is the interface of the Packet-classes existing in the game
class IPacket
{
	public:
		virtual const PacketType& 	GetType() const = 0;
		virtual char const* const	GetData() const = 0;
		virtual u_long				GetSize() const = 0;
		virtual ~IPacket() {}
};
#endif