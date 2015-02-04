#include "PacketImpl.h"

const PacketType BinaryPacket::GUID( 0x21b2 );
const PacketType TextPacket::GUID( 0x38c5 );

/////////////////////////////////////////////////////////////////// 
// TestPacket
TextPacket::TextPacket( char const* const text )
	: BinaryPacket( static_cast<u_long>(strlen(text) + 2 ) )
{
	MemCpy( text, strlen(text), 0 );
	MemCpy( "\r\n", 2, 2 );
	*(u_long*)mData = 0;
}

/////////////////////////////////////////////////////////////////
