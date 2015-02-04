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
using namespace DirectX;
/*	This has been slightly modified in order to allow each event-class to serialize it's data. */

extern EventType counter; // If you want know what this does, just check Events.cpp :D

class Event_Client_Joined : public IEvent
{
	private:
		UINT mID;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Client_Joined()
		{
			mID = 0;
		}
		Event_Client_Joined( UINT id )
		{
			mID = id;
		}
		~Event_Client_Joined() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Client_Joined( mID ) );
		}
		UINT ID() const
		{
			return mID;
		}
};

class Event_Client_Left : public IEvent
{
	private:
		UINT mID;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Client_Left()
		{
			mID = 0;
		}
		Event_Client_Left( UINT id )
		{
			mID = id;
		}
		~Event_Client_Left() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Client_Left( mID ) );
		}
		UINT ID() const
		{
			return mID;
		}
};

class Event_Local_Joined : public IEvent
{
	private:
		int mID;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Local_Joined()
		{
			mID = 0;
		}
		Event_Local_Joined( int id )
		{
			mID = id;
		}
		~Event_Local_Joined() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Local_Joined( mID ) );
		}
		UINT ID() const
		{
			return mID;
		}
};

class Event_Remote_Joined : public IEvent
{
	private:
		UINT mID;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Remote_Joined()
		{
			mID = 0;
		}
		Event_Remote_Joined( UINT id )
		{
			mID = id;
		}
		~Event_Remote_Joined() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Remote_Joined( mID ) );
		}
		UINT ID() const
		{
			return mID;
		}
};

class Event_Remote_Left : public IEvent
{
	private:
		UINT mID;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Remote_Left()
		{
			mID = 0;
		}
		Event_Remote_Left( UINT id )
		{
			mID = id;
		}
		~Event_Remote_Left() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Remote_Left( mID ) );
		}
		UINT ID() const
		{
			return mID;
		}
};