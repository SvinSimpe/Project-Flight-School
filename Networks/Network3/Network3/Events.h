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
/*	This has been slightly modified in order to allow each event-class to serialize it's data. 
	New rules:
	- For every event that you create a REGISTER_EVENT call has to be made for that function. 
	- More rules will be implemented later. */

extern EventType counter; // If you want know what this does, just check Events.cpp :D

// Only used by the server when a new client joins
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
			mID = (UINT)-1;
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

// Only used by the server when a client leaves
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
			mID = (UINT)-1;
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

// Listened to by the player who's joining a game in order for it to get it's ID (no team implemented yet)
class Event_Local_Joined : public IEvent
{
	private:
		UINT mID;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Local_Joined()
		{
			mID	= (UINT)-1;
		}
		Event_Local_Joined( UINT id )
		{
			mID	= id;
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

// Listened to by every client and handles a remote client joining the game
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
			mID	= (UINT)-1;
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

// Listened to by every client and handles a remote client leaving the game
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
			mID = (UINT)-1;
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

// Listened to by the server and handles the local client updating
// The same as Event_Player_Update in the project
class Event_Local_Update : public IEvent
{
	private:
		UINT		mID;
		XMFLOAT3	mLowerBodyPos;
		XMFLOAT3	mVelocity;
		XMFLOAT3	mUpperBodyDirection;
	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Local_Update()
		{
			mID						= (UINT)-1;
			mLowerBodyPos			= XMFLOAT3( 0.0f, 0.0f, 0.0f );
			mVelocity				= XMFLOAT3( 0.0f, 0.0f, 0.0f );
			mUpperBodyDirection		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
		}
		Event_Local_Update( UINT id, XMFLOAT3 lowerBodyPos, XMFLOAT3 velocity, XMFLOAT3 upperBodyDir )
		{
			mID						= id;
			mLowerBodyPos			= lowerBodyPos;
			mVelocity				= velocity;
			mUpperBodyDirection		= upperBodyDir;
		}
		~Event_Local_Update() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			
			out << mLowerBodyPos.x << " ";
			out << mLowerBodyPos.y << " ";
			out << mLowerBodyPos.z << " ";

			out << mVelocity.x << " ";
			out << mVelocity.y << " ";
			out << mVelocity.z << " ";

			out << mUpperBodyDirection.x << " ";
			out << mUpperBodyDirection.y << " ";
			out << mUpperBodyDirection.z << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;

			in >> mLowerBodyPos.x;
			in >> mLowerBodyPos.y;
			in >> mLowerBodyPos.z;

			in >> mVelocity.x;
			in >> mVelocity.y;
			in >> mVelocity.z;

			in >> mUpperBodyDirection.x;
			in >> mUpperBodyDirection.y;
			in >> mUpperBodyDirection.z;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Local_Update( mID, mLowerBodyPos, mVelocity, mUpperBodyDirection ) );
		}
		UINT ID() const
		{
			return mID;
		}
		XMFLOAT3 LowerBodyPos() const
		{
			return mLowerBodyPos;
		}
		XMFLOAT3 Velocity() const
		{
			return mVelocity;
		}
		XMFLOAT3 UpperBodyDirection() const
		{
			return mUpperBodyDirection;
		}
};

// Created and broadcasted to each remote client whenever a local update is sent to the server
// The same as Event_Remote_Player_Update in the project
class Event_Remote_Update : public IEvent
{
	private:
		UINT		mID;
		XMFLOAT3	mLowerBodyPos;
		XMFLOAT3	mVelocity;
		XMFLOAT3	mUpperBodyDirection;
	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Remote_Update()
		{
			mID						= (UINT)-1;
			mLowerBodyPos			= XMFLOAT3( 0.0f, 0.0f, 0.0f );
			mVelocity				= XMFLOAT3( 0.0f, 0.0f, 0.0f );
			mUpperBodyDirection		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
		}
		Event_Remote_Update( UINT id, XMFLOAT3 lowerBodyPos, XMFLOAT3 velocity, XMFLOAT3 upperBodyDir )
		{
			mID						= id;
			mLowerBodyPos			= lowerBodyPos;
			mVelocity				= velocity;
			mUpperBodyDirection		= upperBodyDir;
		}
		~Event_Remote_Update() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			
			out << mLowerBodyPos.x << " ";
			out << mLowerBodyPos.y << " ";
			out << mLowerBodyPos.z << " ";

			out << mVelocity.x << " ";
			out << mVelocity.y << " ";
			out << mVelocity.z << " ";

			out << mUpperBodyDirection.x << " ";
			out << mUpperBodyDirection.y << " ";
			out << mUpperBodyDirection.z << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;

			in >> mLowerBodyPos.x;
			in >> mLowerBodyPos.y;
			in >> mLowerBodyPos.z;

			in >> mVelocity.x;
			in >> mVelocity.y;
			in >> mVelocity.z;

			in >> mUpperBodyDirection.x;
			in >> mUpperBodyDirection.y;
			in >> mUpperBodyDirection.z;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Remote_Update( mID, mLowerBodyPos, mVelocity, mUpperBodyDirection ) );
		}
		UINT ID() const
		{
			return mID;
		}
		XMFLOAT3 LowerBodyPos() const
		{
			return mLowerBodyPos;
		}
		XMFLOAT3 Velocity() const
		{
			return mVelocity;
		}
		XMFLOAT3 UpperBodyDirection() const
		{
			return mUpperBodyDirection;
		}
};

// This is a local event used by the client to switch between game states
class Event_Change_State : public IEvent
{
	private:
		int mEventID;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Change_State()
		{
			mEventID = -1;
		}
		Event_Change_State( int id )
		{
			mEventID = id;
		}
		~Event_Change_State() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mEventID << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mEventID;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Change_State( mEventID ) );
		}
		int EventID() const
		{
			return mEventID;
		}
};

// This is an event used by the client to start the server given a specific port
class Event_Start_Server : public IEvent
{
	private:
		UINT mPort;
	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Start_Server()
		{
			mPort = (UINT)-1;
		}
		Event_Start_Server( UINT port )
		{
			mPort = port;
		}
		~Event_Start_Server() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mPort << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mPort;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Start_Server( mPort ) );
		}
		UINT Port() const
		{
			return mPort;
		}
};