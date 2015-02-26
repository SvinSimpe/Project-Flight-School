#ifndef PACKETIMPL_H
#define PACKETIMPL_H

#include "IPacket.h"
#include <iostream>

/////////////////////////////////////////////////////////////////
// A virtual packet-class that can be constructed all at once
class BinaryPacket : public IPacket
{
	private:
	protected:
		char* mData;
	public:
		static const PacketType GUID;

	private:
	protected:
	public:
		const PacketType& GetType() const { return GUID; }
		char const* const GetData() const { return mData ; }
		u_long GetSize() const { return ntohl(*(u_long*)mData); }
		inline void MemCpy( char const* const data, size_t size, int destOffset );
		inline BinaryPacket( char const* const data, u_long size );
		inline BinaryPacket( u_long size );
		virtual ~BinaryPacket() { SAFE_DELETE( mData ); }
};

inline void BinaryPacket::MemCpy( char const* const data, size_t size, int destOffset )
{
	PFS_ASSERT( size + destOffset <= GetSize() - sizeof( u_long ) );
	memcpy( mData + destOffset + sizeof( u_long ), data, size );
}

inline BinaryPacket::BinaryPacket( char const* const data, u_long size )
{
	mData = PFS_NEW char[size + sizeof( u_long )];
	PFS_ASSERT( mData );
	*(u_long*)mData = htonl( size + sizeof(u_long) );
	MemCpy( data, size, 0 );
}

inline BinaryPacket::BinaryPacket( u_long size )
{
	mData = PFS_NEW char[size + sizeof(u_long)];
	PFS_ASSERT( mData );
	*(u_long*)mData = htonl( size + sizeof( u_long ) );
}
#endif