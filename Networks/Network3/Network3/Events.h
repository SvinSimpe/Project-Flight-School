#pragma once
//========================================================================
// Events.h : defines common game events
//
// Part of the GameCode4 Application
//
// GameCode4 is the sample application that encapsulates much of the source code
// discussed in "Game Coding Complete - 4th Edition" by Mike McShaffry and David
// "Rez" Graham, published by Charles River Media. 
// ISBN-10: 1133776574 | ISBN-13: 978-1133776574
//
// If this source code has found it's way to you, and you think it has helped you
// in any way, do the authors a favor and buy a new copy of the book - there are 
// detailed explanations in it that compliment this code well. Buy a copy at Amazon.com
// by clicking here: 
//    http://www.amazon.com/gp/product/1133776574/ref=olp_product_details?ie=UTF8&me=&seller=
//
// There's a companion web site at http://www.mcshaffry.com/GameCode/
// 
// The source code is managed and maintained through Google Code: 
//    http://code.google.com/p/gamecode4/
//
// (c) Copyright 2012 Michael L. McShaffry and David Graham
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser GPL v3
// as published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See 
// http://www.gnu.org/licenses/lgpl-3.0.txt for more details.
//
// You should have received a copy of the GNU Lesser GPL v3
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
//
//========================================================================
#include "EventManager.h"
#include <DirectXMath.h>
#include <iostream>
using namespace DirectX;

class Event_Client_Amount_Update : public IEvent
{
	private:
		std::list<UINT> mSocketIDs;
	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Client_Amount_Update()
		{
			mSocketIDs = std::list<UINT>();
		}
		Event_Client_Amount_Update( std::list<UINT> socketIDs )
		{
			mSocketIDs	= socketIDs;
		}
		~Event_Client_Amount_Update() {}
		std::list<UINT> SocketID() const
		{
			return mSocketIDs;
		}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::stringstream& out ) const
		{
			std::cout << "Not serializable, stop it." << std::endl;
		}
		void Deserialize( std::stringstream& in )
		{
			std::cout << "Not deserializable either, seriously stop it." << std::endl;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Client_Amount_Update( mSocketIDs ) );
		}
};

class Event_Client_Joined : public IEvent
{
	private:
		int mHostID;
		int mSocketID;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Client_Joined()
		{
			mHostID		= -1;
			mSocketID	= -1;
		}
		Event_Client_Joined( int hostID, int socketID )
		{
			mHostID		= hostID;
			mSocketID	= socketID;
		}
		~Event_Client_Joined() {}
		int HostID() const
		{
			return mHostID;
		}
		int SocketID() const
		{
			return mSocketID;
		}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::stringstream& out ) const
		{
			out << mHostID << " ";
			out << mSocketID << " ";
		}
		void Deserialize( std::stringstream& in )
		{
			in >> mHostID;
			in >> mSocketID;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Client_Joined( mHostID, mSocketID ) );
		}
};

class Event_Text : public IEvent
{
	private:
		unsigned int	mSocket;
		std::string		mText;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Text()
		{
			mSocket = 0;
			mText	= "";
		}
		Event_Text( unsigned int socket, std::string text )
		{
			mSocket = socket;
			mText = text;
		}
		~Event_Text() {}
		unsigned int Socket() const
		{
			return mSocket;
		}
		std::string Text() const
		{
			return mText;
		}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::stringstream& out ) const
		{
			out << mSocket << " ";
			out << mText << " ";
		}
		void Deserialize( std::stringstream& in )
		{
			in >> mSocket;
			in >> mText;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Text( mSocket, mText ) );
		}
};