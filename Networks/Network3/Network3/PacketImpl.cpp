#include "PacketImpl.h"

const char* BinaryPacket::gType	= "BinaryPacket";
const char* TextPacket::gType	= "TextPacket";

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