#ifndef EVENTS_H
#define EVENTS_H
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
	- Events that are to be sent to the server from the client (don't worry, it's a singleton) needs to be sent, never listened to by the client itself.
	- Events that needs to be sent by the client all have the word "Client" in their names.
	- Events that handles anything that's only Server->Client related have the word "Server" in their names.
	- More rules will be implemented later. */

extern EventType counter; // If you want know what this does, just check Events.cpp :D

class Event_Exit_Game : public IEvent
{
	private:
	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Exit_Game()
		{
		}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
		}
		void Deserialize( std::istringstream& in )
		{
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Exit_Game() );
		}
};

// A client-local event for switching to fullscreen
class Event_Toggle_Fullscreen : public IEvent
{
	private:
	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Toggle_Fullscreen()
		{
		}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
		}
		void Deserialize( std::istringstream& in )
		{
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Toggle_Fullscreen() );
		}
};

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

// Listened to by the player who's joining a game in order for it to get it's ID
class Event_Local_Joined : public IEvent
{
	private:
		UINT mID;
		UINT mTeamID;

	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Local_Joined()
		{
			mID		= (UINT)-1;
			mTeamID = (UINT)-1;
		}
		Event_Local_Joined( UINT id, UINT teamID )
		{
			mID		= id;
			mTeamID = teamID;
		}
		~Event_Local_Joined() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			out << mTeamID << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			in >> mTeamID;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Local_Joined( mID, mTeamID ) );
		}
		UINT ID() const
		{
			return mID;
		}
		UINT TeamID() const
		{
			return mTeamID;
		}
};

// Listened to by every client and handles a remote client joining the game
class Event_Remote_Joined : public IEvent
{
	private:
		UINT mID;
		UINT mTeamID;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Remote_Joined()
		{
			mID		= (UINT)-1;
			mTeamID = (UINT)-1;
		}
		Event_Remote_Joined( UINT id, UINT teamID )
		{
			mID		= id;
			mTeamID = teamID;
		}
		~Event_Remote_Joined() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			out << mTeamID << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			in >> mTeamID;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Remote_Joined( mID, mTeamID ) );
		}
		UINT ID() const
		{
			return mID;
		}
		UINT TeamID() const
		{
			return mTeamID;
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

// Listened to by the server and handles the Client client updating
class Event_Client_Update : public IEvent
{
	private:
		UINT		mID;
		XMFLOAT3	mLowerBodyPos;
		XMFLOAT3	mVelocity;
		XMFLOAT3	mUpperBodyDirection;
		std::string mName;
		bool		mIsBuffed;
		bool		mIsAlive;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Client_Update()
		{
			mID						= (UINT)-1;
			mLowerBodyPos			= XMFLOAT3( 0.0f, 0.0f, 0.0f );
			mVelocity				= XMFLOAT3( 0.0f, 0.0f, 0.0f );
			mUpperBodyDirection		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
			mName					= "";
			mIsBuffed				= false;
			mIsAlive				= false;
		}
		Event_Client_Update( UINT id, XMFLOAT3 lowerBodyPos, XMFLOAT3 velocity, XMFLOAT3 upperBodyDir, std::string name, bool isBuffed, bool isAlive )
		{
			mID						= id;
			mLowerBodyPos			= lowerBodyPos;
			mVelocity				= velocity;
			mUpperBodyDirection		= upperBodyDir;
			mName					= name;
			mIsBuffed				= isBuffed;
			mIsAlive				= isAlive;
		}
		~Event_Client_Update() {}
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

			out << mName << " ";
			out << mIsBuffed << " ";
			out << mIsAlive << " ";
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

			in >> mName;
			in >> mIsBuffed;
			in >> mIsAlive;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Client_Update( mID, mLowerBodyPos, mVelocity, mUpperBodyDirection, mName, mIsBuffed, mIsAlive ) );
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
		std::string Name() const
		{
			return mName;
		}
		bool IsBuffed() const
		{
			return mIsBuffed;
		}
		bool IsAlive() const
		{
			return mIsAlive;
		}
};

// Created and broadcasted to each remote client whenever a local update is sent to the server
class Event_Remote_Update : public IEvent
{
	private:
		UINT		mID;
		XMFLOAT3	mLowerBodyPos;
		XMFLOAT3	mVelocity;
		XMFLOAT3	mUpperBodyDirection;
		std::string mName;
		bool		mIsAlive;

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
			mName					= "";
			mIsAlive				= false;
		}
		Event_Remote_Update( UINT id, XMFLOAT3 lowerBodyPos, XMFLOAT3 velocity, XMFLOAT3 upperBodyDir, std::string name, bool isAlive )
		{
			mID						= id;
			mLowerBodyPos			= lowerBodyPos;
			mVelocity				= velocity;
			mUpperBodyDirection		= upperBodyDir;
			mName					= name;
			mIsAlive				= isAlive;
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

			out << mName << " ";

			out << mIsAlive << " ";
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

			in >> mName;

			in >> mIsAlive;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Remote_Update( mID, mLowerBodyPos, mVelocity, mUpperBodyDirection, mName, mIsAlive ) );
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
		std::string Name() const
		{
			return mName;
		}
		bool IsAlive() const
		{
			return mIsAlive;
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
		std::string mPort;
		UINT		mMaxPlayers;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Start_Server()
		{
			mPort = "";
			mMaxPlayers = (UINT)-1;
		}
		Event_Start_Server( std::string port, UINT maxPlayers )
		{
			mPort = port;
			mMaxPlayers = maxPlayers;
		}
		~Event_Start_Server() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mPort << " ";
			out << mMaxPlayers << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mPort;
			in >> mMaxPlayers;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Start_Server( mPort, mMaxPlayers ) );
		}
		std::string Port() const
		{
			return mPort;
		}
		UINT MaxPlayers() const
		{
			return mMaxPlayers;
		}
};

// This is an event used by the client to start a client-object given IP and port
class Event_Start_Client : public IEvent
{
	private:
		std::string mIP;
		std::string	mPort;
	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Start_Client()
		{
			mIP		= "";
			mPort	= "";
		}
		Event_Start_Client( std::string IP, std::string port )
		{
			mIP		= IP;
			mPort	= port;
		}
		~Event_Start_Client() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mIP << " ";
			out << mPort << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mIP;
			in >> mPort;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Start_Client( mIP, mPort ) );
		}
		std::string IP() const
		{
			return mIP;
		}
		std::string Port() const
		{
			return mPort;
		}
};

// An event used to start the game
class Event_Game_Started : public IEvent
{
	private:
	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Game_Started()
		{
		}
		~Event_Game_Started() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
		}
		void Deserialize( std::istringstream& in )
		{
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Game_Started() );
		}
};

// An event used to end the game
class Event_Game_Ended : public IEvent
{
	private:
	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Game_Ended()
		{
		}
		~Event_Game_Ended() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
		}
		void Deserialize( std::istringstream& in )
		{
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Game_Ended() );
		}
};

// An event sent by the local client whenever s/he dies
class Event_Client_Died : public IEvent
{
	private:
		UINT mID;
		UINT mKillerID;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Client_Died()
		{
			mID			= (UINT)-1;
			mKillerID	= (UINT)-1;
		}
		Event_Client_Died( UINT id, UINT killerID )
		{
			mID			= id;
			mKillerID	= killerID;
		}
		~Event_Client_Died() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			out << mKillerID << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			in >> mKillerID;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Client_Died( mID, mKillerID ) );
		}
		UINT ID() const
		{
			return mID;
		}
		UINT KillerID() const
		{
			return mKillerID;
		}
};

// An event sent by the server to the remote clients whenever someone dies
class Event_Remote_Died : public IEvent
{
	private:
		UINT mID;
		UINT mKillerID;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Remote_Died()
		{
			mID			= (UINT)-1;
			mKillerID	= (UINT)-1;
		}
		Event_Remote_Died( UINT id, UINT killerID )
		{
			mID			= id;
			mKillerID	= killerID;
		}
		~Event_Remote_Died() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			out << mKillerID << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			in >> mKillerID;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Remote_Died( mID, mKillerID ) );
		}
		UINT ID() const
		{
			return mID;
		}
		UINT KillerID() const
		{
			return mKillerID;
		}
};

// An event created by the client whenever s/he is shot by a projectile

class Event_Client_Removed_Projectile : public IEvent
{
	private:
		UINT mProjectileID;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Client_Removed_Projectile()
		{
			mProjectileID = (UINT)-1;
		}
		Event_Client_Removed_Projectile( UINT projectileID )
		{
			mProjectileID = projectileID;
		}
		~Event_Client_Removed_Projectile() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mProjectileID << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mProjectileID;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Client_Removed_Projectile( mProjectileID ) );
		}
		UINT ProjectileID() const
		{
			return mProjectileID;
		}
};

class Event_Remote_Removed_Projectile : public IEvent
{
	private:
		UINT mProjectileID;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Remote_Removed_Projectile()
		{
			mProjectileID = (UINT)-1;
		}
		Event_Remote_Removed_Projectile( UINT projectileID )
		{
			mProjectileID = projectileID;
		}
		~Event_Remote_Removed_Projectile() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mProjectileID << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mProjectileID;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Remote_Removed_Projectile( mProjectileID ) );
		}
		UINT ProjectileID() const
		{
			return mProjectileID;
		}
};

class Event_Client_Damaged : public IEvent
{
	private:
		UINT mID;
		UINT mProjecileID;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Client_Damaged()
		{
			mID			= (UINT)-1;
			mProjecileID	= (UINT)-1;
		}
		Event_Client_Damaged( UINT id, UINT projectileID )
		{
			mID				= id;
			mProjecileID	= projectileID;
		}
		~Event_Client_Damaged() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			out << mProjecileID << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			in >> mProjecileID;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Client_Damaged( mID, mProjecileID ) );
		}
		UINT ID() const
		{
			return mID;
		}
		UINT ProjectileID() const
		{
			return mProjecileID;
		}
};

// An event sent from the server whenever a local client has been shot
class Event_Remote_Damaged : public IEvent
{
	private:
		UINT mID;
		UINT mProjecileID;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Remote_Damaged()
		{
			mID			= (UINT)-1;
			mProjecileID	= (UINT)-1;
		}
		Event_Remote_Damaged( UINT id, UINT projectileID )
		{
			mID				= id;
			mProjecileID	= projectileID;
		}
		~Event_Remote_Damaged() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			out << mProjecileID << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			in >> mProjecileID;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Remote_Damaged( mID, mProjecileID ) );
		}
		UINT ID() const
		{
			return mID;
		}
		UINT ProjectileID() const
		{
			return mProjecileID;
		}
};

// An event sent from the client to the server whenever the client respawns
class Event_Client_Spawned : public IEvent
{
	private:
		UINT mID;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Client_Spawned()
		{
			mID = (UINT)-1;
		}
		Event_Client_Spawned( UINT id )
		{
			mID = id;
		}
		~Event_Client_Spawned() {}
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
			return IEventPtr( new Event_Client_Spawned( mID ) );
		}
		UINT ID() const
		{
			return mID;
		}
};

// An event sent from the server to the remote clients whenever a client respawns
class Event_Remote_Spawned : public IEvent
{
	private:
		UINT mID;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Remote_Spawned()
		{
			mID = (UINT)-1;
		}
		Event_Remote_Spawned( UINT id )
		{
			mID = id;
		}
		~Event_Remote_Spawned() {}
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
			return IEventPtr( new Event_Remote_Spawned( mID ) );
		}
		UINT ID() const
		{
			return mID;
		}
};

// An event sent from the client to the server whenever a client fires a projectile
class Event_Client_Fired_Projectile : public IEvent
{
	private:
		UINT		mID;
		XMFLOAT3	mBodyPos;
		XMFLOAT3	mDirection;
		float		mSpeed;
		float		mRange;
		float		mDamage;
		int			mWeapon;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
	Event_Client_Fired_Projectile()
		{
			mID				= (UINT)-1;
			mBodyPos		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
			mDirection		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
			mSpeed			= 0.0f;
			mRange			= 0.0f;
			mDamage			= 0.0f;
			mWeapon			= 4;
		}

		Event_Client_Fired_Projectile( unsigned int id, XMFLOAT3 bodyPos, XMFLOAT3 direction, float speed, float range, float damage, int weapon )
		{
			mID				= id;
			mBodyPos		= bodyPos;
			mDirection		= direction;
			mSpeed			= speed;
			mRange			= range;
			mDamage			= damage;
			mWeapon			= weapon;
		}

		~Event_Client_Fired_Projectile() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			
			out << mBodyPos.x << " ";
			out << mBodyPos.y << " ";
			out << mBodyPos.z << " ";

			out << mDirection.x << " ";
			out << mDirection.y << " ";
			out << mDirection.z << " ";

			out << mSpeed << " ";
			out	<< mRange << " ";
			out << mDamage << " ";
			out	<< mWeapon << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;

			in >> mBodyPos.x;
			in >> mBodyPos.y;
			in >> mBodyPos.z;

			in >> mDirection.x;
			in >> mDirection.y;
			in >> mDirection.z;

			in >> mSpeed;
			in >> mRange;
			in >> mDamage;
			in >> mWeapon;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Client_Fired_Projectile( mID, mBodyPos, mDirection, mSpeed, mRange, mDamage, mWeapon ) );
		}
		UINT ID() const
		{
			return mID;
		}
		XMFLOAT3 BodyPos() const
		{
			return mBodyPos;
		}
		XMFLOAT3 Direction() const
		{
			return mDirection;
		}
		float Speed() const
		{
			return mSpeed;
		}
		float Range() const
		{
			return mRange;
		}
		float Damage() const
		{
			return mDamage;
		}
		int Weapon() const
		{
			return mWeapon;
		}
};

// An event sent from the server to the other clients whenever a client fires a projectile
class Event_Remote_Fired_Projectile : public IEvent
{
	private:
		UINT		mID;
		UINT		mProjectileID;
		XMFLOAT3	mBodyPos;
		XMFLOAT3	mDirection;
		float		mSpeed;
		float		mRange;
		float		mDamage;
		int			mWeapon;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
	Event_Remote_Fired_Projectile()
		{
			mID				= (UINT)-1;
			mProjectileID	= 0;
			mBodyPos		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
			mDirection		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
			mSpeed			= 0.0f;
			mRange			= 0.0f;
			mDamage			= 0.0;
			mWeapon			= 4;
		}

		Event_Remote_Fired_Projectile( UINT id, UINT projectileID, XMFLOAT3 bodyPos, XMFLOAT3 direction, float speed, float range, float damage, int weapon )
		{
			mID				= id;
			mProjectileID	= projectileID;
			mBodyPos		= bodyPos;
			mDirection		= direction;
			mSpeed			= speed;
			mRange			= range;
			mDamage			= damage;
			mWeapon			= weapon;
		}

		~Event_Remote_Fired_Projectile() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			out << mProjectileID << " ";

			out << mBodyPos.x << " ";
			out << mBodyPos.y << " ";
			out << mBodyPos.z << " ";

			out << mDirection.x << " ";
			out << mDirection.y << " ";
			out << mDirection.z << " ";

			out << mSpeed << " ";
			out	<< mRange << " ";
			out << mDamage << " ";
			out << mWeapon << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			in >> mProjectileID;

			in >> mBodyPos.x;
			in >> mBodyPos.y;
			in >> mBodyPos.z;

			in >> mDirection.x;
			in >> mDirection.y;
			in >> mDirection.z;

			in >> mSpeed;
			in >> mRange;
			in >> mDamage;
			in >> mWeapon;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Remote_Fired_Projectile( mID, mProjectileID, mBodyPos, mDirection, mSpeed, mRange, mDamage, mWeapon ) );
		}
		UINT ID() const
		{
			return mID;
		}
		UINT ProjectileID() const
		{
			return mProjectileID;
		}
		XMFLOAT3 BodyPos() const
		{
			return mBodyPos;
		}
		XMFLOAT3 Direction() const
		{
			return mDirection;
		}
		float Speed() const
		{
			return mSpeed;
		}
		float Range() const
		{
			return mRange;
		}
		float Damage() const
		{
			return mDamage;
		}
		int Weapon() const
		{
			return mWeapon;
		}
};

// An event sent from the client to the server whenever HP is lost/gained
class Event_Client_Update_HP : public IEvent
{
	private:
		UINT		mID;
		float		mHP;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Client_Update_HP()
		{
			mID	= (UINT)-1;
			mHP	= -1.0f;
		}
		Event_Client_Update_HP( UINT id, float hp )
		{
			mID	= id;
			mHP = hp;
		}
		~Event_Client_Update_HP() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			out << mHP << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			in >> mHP;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Client_Update_HP( mID, mHP ) );
		}
		UINT ID() const
		{
			return mID;
		}
		float HP() const
		{
			return mHP;
		}
};

// An event used to update the other clients on a certain clients HP-update
class Event_Remote_Update_HP : public IEvent
{
	private:
		UINT		mID;
		float		mHP;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Remote_Update_HP()
		{
			mID	= (UINT)-1;
			mHP	= -1.0f;
		}
		Event_Remote_Update_HP( UINT id, float hp )
		{
			mID	= id;
			mHP = hp;
		}
		~Event_Remote_Update_HP() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			out << mHP << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			in >> mHP;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Remote_Update_HP( mID, mHP ) );
		}
		UINT ID() const
		{
			return mID;
		}
		float HP() const
		{
			return mHP;
		}
};

// An event used by the server to send clients the list of existing enemies
class Event_Server_Create_Enemy : public IEvent
{
	private:
		UINT		mID;
		UINT		mState;
		UINT		mType;
		XMFLOAT3	mPosition;
		XMFLOAT3	mDirection;
		float		mMaxHp; 

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Server_Create_Enemy()
		{
			mID			= (UINT)-1;
			mState		= (UINT)-1;
			mType		= (UINT)-1;
			mPosition	= XMFLOAT3( 0.0f, 0.0f, 0.0f );
			mDirection	= XMFLOAT3( 0.0f, 0.0f, 0.0f );
			mMaxHp		= 0.0f;
		}
		Event_Server_Create_Enemy( UINT id, UINT state, UINT type, XMFLOAT3 position, XMFLOAT3 direction, float maxHp )
		{
			mID			= id;
			mState		= state;
			mType		= type;
			mPosition	= position;
			mDirection	= direction;
			mMaxHp		= maxHp;
		}
		~Event_Server_Create_Enemy() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			out << mState << " ";
			out << mType << " ";

			out << mPosition.x << " ";
			out << mPosition.y << " ";
			out << mPosition.z << " ";

			out << mDirection.x << " ";
			out << mDirection.y << " ";
			out << mDirection.z << " ";

			out << mMaxHp << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			in >> mState;
			in >> mType;

			in >> mPosition.x;
			in >> mPosition.y;
			in >> mPosition.z;

			in >> mDirection.x;
			in >> mDirection.y;
			in >> mDirection.z;

			in >> mMaxHp;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Server_Create_Enemy( mID, mState, mType, mPosition, mDirection, mMaxHp ) );
		}
		UINT ID() const
		{
			return mID;
		}
		UINT State() const
		{
			return mState;
		}
		UINT Type() const
		{
			return mType;
		}
		XMFLOAT3 Position() const
		{
			return mPosition;
		}
		XMFLOAT3 Direction() const
		{
			return mDirection;
		}
		float MaxHP() const
		{
			return mMaxHp;
		}
};

// Sent from the server when all of the enemies has been created
class Event_Server_Enemies_Created : public IEvent
{
	private:
	protected:
	public:
		static const EventType GUID;
	
	private:
	protected:
	public:
		Event_Server_Enemies_Created()
		{
		}
		~Event_Server_Enemies_Created() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
		}
		void Deserialize( std::istringstream& in )
		{
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Server_Enemies_Created() );
		}
};

// This event is sent by a client who hits a remote client
class Event_Client_Melee_Hit : public IEvent
{
	private:
		UINT		mID;
		float		mDamage;
		float		mKnockBack;
		XMFLOAT3	mDirection;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Client_Melee_Hit()
		{
			mID			= (UINT)-1;
			mDamage		= -1.0f;
			mKnockBack	= -1.0f;
			mDirection	= XMFLOAT3( 0.0f, 0.0f, 0.0f );
		}
		Event_Client_Melee_Hit( UINT id, float damage, float knockBack, XMFLOAT3 direction )
		{
			mID			= id;
			mDamage		= damage;
			mKnockBack	= knockBack;
			mDirection	= direction;
		}
		~Event_Client_Melee_Hit() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			out << mDamage << " ";
			out << mKnockBack << " ";

			out << mDirection.x << " ";
			out << mDirection.y << " ";
			out << mDirection.z << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			in >> mDamage;
			in >> mKnockBack;

			in >> mDirection.x;
			in >> mDirection.y;
			in >> mDirection.z;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Client_Melee_Hit( mID, mDamage, mKnockBack, mDirection ) );
		}
		UINT ID() const
		{
			return mID;
		}
		float Damage() const
		{
			return mDamage;
		}
		float KnockBack() const
		{
			return mKnockBack;
		}
		XMFLOAT3 Direction() const
		{
			return mDirection;
		}
};

// This event is sent by the server to the other clients to notify them that someone's been hit
class Event_Remote_Melee_Hit : public IEvent
{
	private:
		UINT		mID;
		float		mDamage;
		float		mKnockBack;
		XMFLOAT3	mDirection;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Remote_Melee_Hit()
		{
			mID			= (UINT)-1;
			mDamage		= -1.0f;
			mKnockBack	= -1.0f;
			mDirection	= XMFLOAT3( 0.0f, 0.0f, 0.0f );
		}
		Event_Remote_Melee_Hit( UINT id, float damage, float knockBack, XMFLOAT3 direction )
		{
			mID			= id;
			mDamage		= damage;
			mKnockBack	= knockBack;
			mDirection	= direction;
		}
		~Event_Remote_Melee_Hit() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			out << mDamage << " ";
			out << mKnockBack << " ";

			out << mDirection.x << " ";
			out << mDirection.y << " ";
			out << mDirection.z << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			in >> mDamage;
			in >> mKnockBack;

			in >> mDirection.x;
			in >> mDirection.y;
			in >> mDirection.z;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Remote_Melee_Hit( mID, mDamage, mKnockBack, mDirection ) );
		}
		UINT ID() const
		{
			return mID;
		}
		float Damage() const
		{
			return mDamage;
		}
		float KnockBack() const
		{
			return mKnockBack;
		}
		XMFLOAT3 Direction() const
		{
			return mDirection;
		}
};

// Sent by the client whenever it attacks in order to sync the animations to the remotes
class Event_Client_Attack : public IEvent
{
	private:
		UINT mID;
		UINT mArmID;
		UINT mAnimation;

	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Client_Attack()
		{
			mID			= (UINT)-1;
			mArmID		= (UINT)-1;
			mAnimation	= (UINT)-1;
		}
		Event_Client_Attack( UINT id, UINT armID, UINT animation )
		{
			mID			= id;
			mArmID		= armID;
			mAnimation	= animation;
		}
		~Event_Client_Attack() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			out << mArmID << " ";
			out << mAnimation << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			in >> mArmID;
			in >> mAnimation;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Client_Attack( mID, mArmID, mAnimation ) );
		}
		UINT ID() const
		{
			return mID;
		}
		UINT ArmID() const
		{
			return mArmID;
		}
		UINT Animation() const
		{
			return mAnimation;
		}
};

// Sent by the server to update the remotes on the animation of the client
class Event_Remote_Attack : public IEvent
{
	private:
		UINT mID;
		UINT mArmID;
		UINT mAnimation;

	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Remote_Attack()
		{
			mID			= (UINT)-1;
			mArmID		= (UINT)-1;
			mAnimation	= (UINT)-1;
		}
		Event_Remote_Attack( UINT id, UINT armID, UINT animation )
		{
			mID			= id;
			mArmID		= armID;
			mAnimation	= animation;
		}
		~Event_Remote_Attack() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			out << mArmID << " ";
			out << mAnimation << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			in >> mArmID;
			in >> mAnimation;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Remote_Attack( mID, mArmID, mAnimation ) );
		}
		UINT ID() const
		{
			return mID;
		}
		UINT ArmID() const
		{
			return mArmID;
		}
		UINT Animation() const
		{
			return mAnimation;
		}
};

// Syncs the list of spawners that the server has with the one the client (apparantely?) has
class Event_Server_Sync_Spawn : public IEvent
{
	private:
		UINT		mID;
		XMFLOAT3	mPosition;

	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Server_Sync_Spawn()
		{
			mID			= (UINT)-1;
			mPosition	= XMFLOAT3( 0.0f, 0.0f, 0.0f );
		}
		Event_Server_Sync_Spawn( UINT id, XMFLOAT3 position )
		{
			mID			= id;
			mPosition	= position;
		}
		~Event_Server_Sync_Spawn() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";

			out << mPosition.x << " ";
			out << mPosition.y << " ";
			out << mPosition.z << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			
			in >> mPosition.x;
			in >> mPosition.y;
			in >> mPosition.z;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Server_Sync_Spawn( mID, mPosition ) );
		}
		UINT ID() const
		{
			return mID;
		}
		XMFLOAT3 Position() const
		{
			return mPosition;
		}
};

// A client-local event used for creating point lights
class Event_Add_Point_Light : public IEvent
{
	private:
		void* mLight;

	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Add_Point_Light()
		{
			mLight = nullptr;
		}
		Event_Add_Point_Light( void* light )
		{
			mLight = light;
		}
		~Event_Add_Point_Light() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			// Isn't supposed to be sent over the network
		}
		void Deserialize( std::istringstream& in )
		{
			// Isn't supposed to be sent over the network
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Add_Point_Light( mLight ) );
		}
		void* Light() const
		{
			return mLight;
		}
};

// A client-local event used for removing point lights
class Event_Remove_Point_Light : public IEvent
{
	private:
		void* mLight;

	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Remove_Point_Light()
		{
			mLight = nullptr;
		}
		Event_Remove_Point_Light( void* light )
		{
			mLight = light;
		}
		~Event_Remove_Point_Light() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			// Isn't supposed to be sent over the network
		}
		void Deserialize( std::istringstream& in )
		{
			// Isn't supposed to be sent over the network
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Remove_Point_Light( mLight ) );
		}
		void* Light() const
		{
			return mLight;
		}
};

// Event used by the server to update the enemy
class Event_Server_Update_Enemy : public IEvent
{
	private:
		UINT		mID;
		XMFLOAT3	mPosition;
		XMFLOAT3	mDirection;
		bool		mIsAlive;
		float		mEnemyHP;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Server_Update_Enemy()
		{
			mID			= (UINT)-1;
			mPosition	= XMFLOAT3( 0.0f, 0.0f, 0.0f );
			mDirection	= XMFLOAT3( 0.0f, 0.0f, 0.0f );
			mIsAlive	= false;
			mEnemyHP	= 0.0f;
		}
		Event_Server_Update_Enemy( UINT id, XMFLOAT3 position, XMFLOAT3 direction, bool isAlive, float hp )
		{
			mID			= id;
			mPosition	= position;
			mDirection	= direction;
			mIsAlive	= isAlive;
			mEnemyHP	= hp;
		}
		~Event_Server_Update_Enemy() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";

			out << mPosition.x << " ";
			out << mPosition.y << " ";
			out << mPosition.z << " ";

			out << mDirection.x << " ";
			out << mDirection.y << " ";
			out << mDirection.z << " ";

			out << mIsAlive << " ";

			out << mEnemyHP << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;

			in >> mPosition.x;
			in >> mPosition.y;
			in >> mPosition.z;

			in >> mDirection.x;
			in >> mDirection.y;
			in >> mDirection.z;

			in >> mIsAlive;

			in >> mEnemyHP;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Server_Update_Enemy( mID, mPosition, mDirection, mIsAlive, mEnemyHP ) );
		}
		UINT ID() const
		{
			return mID;
		}
		XMFLOAT3 Position() const
		{
			return mPosition;
		}
		XMFLOAT3 Direction() const
		{
			return mDirection;
		}
		bool IsAlive() const
		{
			return mIsAlive;
		}
		float EnemyHP() const
		{
			return mEnemyHP;
		}
};

// Event used by the server to sync the state of an enemy with the clients
class Event_Server_Sync_Enemy_State : public IEvent
{
	private:
		UINT mID;
		UINT mState;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Server_Sync_Enemy_State()
		{
			mID		= (UINT)-1;
			mState	= (UINT)-1;
		}
		Event_Server_Sync_Enemy_State( UINT id, UINT state )
		{
			mID		= id;
			mState	= state;
		}
		~Event_Server_Sync_Enemy_State() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			out << mState << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			in >> mState;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Server_Sync_Enemy_State( mID, mState ) );
		}
		UINT ID() const
		{
			return mID;
		}
		UINT State() const
		{
			return mState;
		}
};

// Event used locally by the server to change the state of a specific enemy
class Event_Set_Enemy_State : public IEvent
{
	private:
		UINT mID;
		UINT mState;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Set_Enemy_State()
		{
			mID		= (UINT)-1;
			mState	= (UINT)-1;
		}
		Event_Set_Enemy_State( UINT id, UINT state )
		{
			mID		= id;
			mState	= state;
		}
		~Event_Set_Enemy_State() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			out << mState << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			in >> mState;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Set_Enemy_State( mID, mState ) );
		}
		UINT ID() const
		{
			return mID;
		}
		UINT State() const
		{
			return mState;
		}
};

// Not sure what this event does yet, will have to see later.
class Event_Remote_Set_Enemy_State : public IEvent
{
	private:
		UINT mID;
		UINT mState;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Remote_Set_Enemy_State()
		{
			mID		= (UINT)-1;
			mState	= (UINT)-1;
		}
		Event_Remote_Set_Enemy_State( UINT id, UINT killerID )
		{
			mID		= id;
			mState	= killerID;
		}
		~Event_Remote_Set_Enemy_State() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			out << mState << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			in >> mState;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Remote_Set_Enemy_State( mID, mState ) );
		}
		UINT ID() const
		{
			return mID;
		}
		UINT State() const
		{
			return mState;
		}
};

// Sent from the client to the server whenever an enemy is shot by the client
class Event_Client_Projectile_Damage_Enemy : public IEvent
{
	private:
		UINT	mID;
		UINT	mProjectileID;
		UINT	mEnemyID;
		float	mDamage;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Client_Projectile_Damage_Enemy()
		{
			mID				= (UINT)-1;
			mProjectileID	= (UINT)-1;
			mEnemyID		= (UINT)-1;
			mDamage			= -1.0f;
		}
		Event_Client_Projectile_Damage_Enemy( UINT id, UINT projectileID, UINT enemyID, float damage )
		{
			mID				= id;
			mProjectileID	= projectileID;
			mEnemyID		= enemyID;
			mDamage			= damage;
		}
		~Event_Client_Projectile_Damage_Enemy() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			out << mProjectileID << " ";
			out << mEnemyID << " ";
			out << mDamage << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			in >> mProjectileID;
			in >> mEnemyID;
			in >> mDamage;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Client_Projectile_Damage_Enemy( mID, mProjectileID, mEnemyID, mDamage ) );
		}
		UINT ID() const
		{
			return mID;
		}
		UINT ProjectileID() const
		{
			return mProjectileID;
		}
		UINT EnemyID() const
		{
			return mEnemyID;
		}		
		float Damage() const
		{
			return mDamage;
		}
};

// Event sent from the server to the clients whenever an enemy dies
class Event_Server_Enemy_Died : public IEvent
{
	private:
		UINT mID;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Server_Enemy_Died()
		{
			mID = (UINT)-1;
		}
		Event_Server_Enemy_Died( UINT id )
		{
			mID	= id;
		}
		~Event_Server_Enemy_Died() {}
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
			return IEventPtr( new Event_Server_Enemy_Died( mID ) );
		}
		UINT ID() const
		{
			return mID;
		}
};

// Event sent from server whenever an enemy attacks a client
class Event_Server_Enemy_Attack_Player : public IEvent
{
	private:
		UINT	mID;
		UINT	mPlayerID;
		float	mDamage;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Server_Enemy_Attack_Player()
		{
			mID			= (UINT)-1;
			mPlayerID	= (UINT)-1;
			mDamage		= -1.0f;
		}
		Event_Server_Enemy_Attack_Player( UINT id, UINT playerID, float damage )
		{
			mID			= id;
			mPlayerID	= playerID;
			mDamage		= damage;
		}
		~Event_Server_Enemy_Attack_Player() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			out << mPlayerID << " ";
			out << mDamage << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			in >> mPlayerID;
			in >> mDamage;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Server_Enemy_Attack_Player( mID, mPlayerID, mDamage ) );
		}
		UINT ID() const
		{
			return mID;
		}
		UINT PlayerID() const
		{
			return mPlayerID;
		}		
		float Damage() const
		{
			return mDamage;
		}
};

// Event sent from client to server whenever it's downed
class Event_Client_Down : public IEvent
{
	private:
		UINT mID;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Client_Down()
		{
			mID = (UINT)-1;
		}
		Event_Client_Down( UINT id )
		{
			mID	= id;
		}
		~Event_Client_Down() {}
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
			return IEventPtr( new Event_Client_Down( mID ) );
		}
		UINT ID() const
		{
			return mID;
		}
};

// Event sent to remotes whenever a client is downed
class Event_Remote_Down : public IEvent
{
	private:
		UINT mID;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Remote_Down()
		{
			mID = (UINT)-1;
		}
		Event_Remote_Down( UINT id )
		{
			mID	= id;
		}
		~Event_Remote_Down() {}
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
			return IEventPtr( new Event_Remote_Down( mID ) );
		}
		UINT ID() const
		{
			return mID;
		}
};

// Event sent from client to server when a client is successfully revived
class Event_Client_Up : public IEvent
{
	private:
		UINT mID;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Client_Up()
		{
			mID = (UINT)-1;
		}
		Event_Client_Up( UINT id )
		{
			mID	= id;
		}
		~Event_Client_Up() {}
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
			return IEventPtr( new Event_Client_Up( mID ) );
		}
		UINT ID() const
		{
			return mID;
		}
};

// Event sent from server to remotes when a client is successfully revived
class Event_Remote_Up : public IEvent
{
	private:
		UINT mID;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Remote_Up()
		{
			mID = (UINT)-1;
		}
		Event_Remote_Up( UINT id )
		{
			mID	= id;
		}
		~Event_Remote_Up() {}
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
			return IEventPtr( new Event_Remote_Up( mID ) );
		}
		UINT ID() const
		{
			return mID;
		}
};

// Event sent from client to server whenever the client attempts a revive
class Event_Client_Attempt_Revive : public IEvent
{
	private:
		UINT mID;			// The ID of the player attempting the revive
		UINT mDownedID;		// The ID of the player getting revived
		float mDeltaTime;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Client_Attempt_Revive()
		{
			mID			= (UINT)-1;
			mDownedID	= (UINT)-1;
			mDeltaTime	= -1.0f;
		}
		Event_Client_Attempt_Revive( UINT id, UINT downedID, float deltaTime )
		{
			mID			= id;
			mDownedID	= downedID;
			mDeltaTime	= deltaTime;
		}
		~Event_Client_Attempt_Revive() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			out << mDownedID << " ";
			out << mDeltaTime << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			in >> mDownedID;
			in >> mDeltaTime;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Client_Attempt_Revive( mID, mDownedID, mDeltaTime ) );
		}
		UINT ID() const
		{
			return mID;
		}
		UINT DownedID() const
		{
			return mDownedID;
		}
		float DeltaTime() const
		{
			return mDeltaTime;
		}
};

// Sent from the server to the remotes whenever a client attempts a revive
class Event_Remote_Attempt_Revive : public IEvent
{
	private:
		UINT mID;			// The ID of the player attempting the revive
		UINT mDownedID;		// The ID of the player getting revived
		float mDeltaTime;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Remote_Attempt_Revive()
		{
			mID			= (UINT)-1;
			mDownedID	= (UINT)-1;
			mDeltaTime	= -1.0f;
		}
		Event_Remote_Attempt_Revive( UINT id, UINT downedID, float deltaTime )
		{
			mID			= id;
			mDownedID	= downedID;
			mDeltaTime	= deltaTime;
		}
		~Event_Remote_Attempt_Revive() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			out << mDownedID << " ";
			out << mDeltaTime << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			in >> mDownedID;
			in >> mDeltaTime;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Remote_Attempt_Revive( mID, mDownedID, mDeltaTime ) );
		}
		UINT ID() const
		{
			return mID;
		}
		UINT DownedID() const
		{
			return mDownedID;
		}
		float DeltaTime() const
		{
			return mDeltaTime;
		}
};

// Not sure what this is for yet
class Event_Load_Level : public IEvent
{
	private:
		std::string mFilePath;
	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Load_Level()
		{
		}
		Event_Load_Level( std::string filePath )
		{
			mFilePath = filePath;
		}
		~Event_Load_Level() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mFilePath << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mFilePath;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Load_Level( mFilePath ) );
		}
		std::string FilePath() const
		{
			return mFilePath;
		}
};

// Creates the player name? I guess..
class Event_Create_Player_Name : public IEvent
{
	private:
		std::string mPlayerName;
	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Create_Player_Name()
		{
		}
		Event_Create_Player_Name( std::string playerName )
		{
			mPlayerName = playerName;
		}
		~Event_Create_Player_Name() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mPlayerName << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mPlayerName;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Create_Player_Name( mPlayerName ) );
		}
		std::string PlayerName() const
		{
			return mPlayerName;
		}
};

// Not sure if the below events are used for when a client attacks an AI or reversed
class Event_Client_Enemy_Attack : public IEvent
{
	private:
		UINT mID;
		UINT mEnemyID;
		float mDamage;
		float mKnockBack;
		XMFLOAT3 mDirection;
		float mStun;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Client_Enemy_Attack()
		{
			mID			= (UINT)-1;
			mEnemyID	= (UINT)-1;
			mDamage		= 0.0f;
			mKnockBack	= 0.0f;
			mDirection  = XMFLOAT3( 0.0f, 0.0f, 0.0f );
			mStun		= 0.0f;
		}
		Event_Client_Enemy_Attack( UINT ID, UINT enemyID, float damage, float knockBack, XMFLOAT3 direction, float stun )
		{
			mID			= ID;
			mEnemyID	= enemyID;
			mDamage		= damage;
			mKnockBack	= knockBack;
			mDirection	= direction;
			mStun		= stun;
		}
		~Event_Client_Enemy_Attack() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			out << mEnemyID << " ";
			out << mDamage << " ";
			out << mKnockBack << " ";
			out << mDirection.x << " ";
			out << mDirection.y << " ";
			out << mDirection.z << " ";
			out << mStun << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			in >> mEnemyID;
			in >> mDamage;
			in >> mKnockBack;
			in >> mDirection.x;
			in >> mDirection.y;
			in >> mDirection.z;
			in >> mStun;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Client_Enemy_Attack( mID, mEnemyID, mDamage, mKnockBack, mDirection, mStun ) );
		}
		UINT ID() const
		{
			return mID;
		}
		UINT EnemyID() const
		{
			return mEnemyID;
		}
		float Damage() const
		{
			return mDamage;
		}
		float KnockBack() const
		{
			return mKnockBack;
		}
		XMFLOAT3 Direction() const
		{
			return mDirection;
		}
		float Stun() const
		{
			return mStun;
		}
};

class Event_Remote_Enemy_Attack : public IEvent
{
	private:
		UINT mID;
		UINT mEnemyID;
		float mDamage;
		float mKnockBack;
		XMFLOAT3 mDirection;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Remote_Enemy_Attack()
		{
			mID			= (UINT)-1;
			mEnemyID	= (UINT)-1;
			mDamage		= 0.0f;
			mKnockBack	= 0.0f;
			mDirection  = XMFLOAT3( 0.0f, 0.0f, 0.0f );
		}
		Event_Remote_Enemy_Attack( UINT ID, UINT enemyID, float damage, float knockBack, XMFLOAT3 direction )
		{
			mID			= ID;
			mEnemyID	= enemyID;
			mDamage		= damage;
			mKnockBack	= knockBack;
			mDirection	= direction;
		}
		~Event_Remote_Enemy_Attack() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			out << mEnemyID << " ";
			out << mDamage << " ";
			out << mKnockBack << " ";
			out << mDirection.x << " ";
			out << mDirection.y << " ";
			out << mDirection.z << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			in >> mEnemyID;
			in >> mDamage;
			in >> mKnockBack;
			in >> mDirection.x;
			in >> mDirection.y;
			in >> mDirection.z;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Remote_Enemy_Attack( mID, mEnemyID, mDamage, mKnockBack, mDirection ) );
		}
		UINT ID() const
		{
			return mID;
		}
		UINT EnemyID() const
		{
			return mEnemyID;
		}
		float Damage() const
		{
			return mDamage;
		}
		float KnockBack() const
		{
			return mKnockBack;
		}
		XMFLOAT3 Direction() const
		{
			return mDirection;
		}
};

// An event used to shut down the server, used when resetting and other stuff
class Event_Shutdown_Server : public IEvent
{
	private:
	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Shutdown_Server()
		{
		}
		~Event_Shutdown_Server() {}
				const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
		}
		void Deserialize( std::istringstream& in )
		{
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Shutdown_Server() );
		}
};

// An event used to shut down the client, used when losing connection to server
class Event_Shutdown_Client : public IEvent
{
	private:
	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Shutdown_Client()
		{
		}
		~Event_Shutdown_Client() {}
				const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
		}
		void Deserialize( std::istringstream& in )
		{
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Shutdown_Client() );
		}
};

// An event used to reset the game, listened to by the Game-class, for further info.. just read the code!
class Event_Reset_Game : public IEvent
{
	private:
	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Reset_Game()
		{
		}
		~Event_Reset_Game() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
		}
		void Deserialize( std::istringstream& in )
		{
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Reset_Game() );
		}
};

// Event used to let stuff know that the server successfully started
class Event_Connect_Server_Success : public IEvent
{
	private:
	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Connect_Server_Success()
		{
		}
		~Event_Connect_Server_Success() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
		}
		void Deserialize( std::istringstream& in )
		{
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Reset_Game() );
		}
};

// Event used to let stuff know that the client successfully started
class Event_Connect_Client_Success : public IEvent
{
	private:
	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Connect_Client_Success()
		{
		}
		~Event_Connect_Client_Success() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
		}
		void Deserialize( std::istringstream& in )
		{
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Connect_Client_Success() );
		}
};

// Event used to let stuff know that the server failed at starting
class Event_Connect_Server_Fail : public IEvent
{
	private:
		std::string mErrorMsg;
	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Connect_Server_Fail()
		{
			mErrorMsg = "";
		}
		Event_Connect_Server_Fail( std::string errorMsg )
		{
			mErrorMsg = errorMsg;
		}
		~Event_Connect_Server_Fail() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mErrorMsg << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mErrorMsg;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Connect_Server_Fail( mErrorMsg ) );
		}
		std::string ErrorMsg() const
		{
			return mErrorMsg;
		}
};

// Event used to let stuff know that the client failed at starting
class Event_Connect_Client_Fail : public IEvent
{
	private:
		std::string mErrorMsg;
	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Connect_Client_Fail()
		{
			mErrorMsg = "";
		}
		Event_Connect_Client_Fail( std::string errorMsg )
		{
			mErrorMsg = errorMsg;
		}
		~Event_Connect_Client_Fail() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mErrorMsg << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mErrorMsg;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Connect_Client_Fail( mErrorMsg ) );
		}
		std::string ErrorMsg() const
		{
			return mErrorMsg;
		}
};

// Event sent from the server to connecting clients to create a ship on the client
class Event_Server_Spawn_Ship : public IEvent
{
	private:
		UINT		mID;
		UINT		mTeamID;
		XMFLOAT3	mPos;
		XMFLOAT4	mRot;
		XMFLOAT3	mScale;
		float		mHP;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Server_Spawn_Ship()
		{
			mID		= (UINT)-1;
			mTeamID = (UINT)-1;
			mPos	= XMFLOAT3( 0.0f, 0.0f, 0.0f );
			mRot	= XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f );
			mScale	= XMFLOAT3( 0.0f, 0.0f, 0.0f );
			mHP		= -1.0f;
		}
		Event_Server_Spawn_Ship( UINT id, UINT teamID, XMFLOAT3 pos, XMFLOAT4 rot, XMFLOAT3 scale, float currentHP )
		{
			mID		= id;
			mTeamID = teamID;
			mPos	= pos;
			mRot	= rot;
			mScale	= scale;
			mHP		= currentHP;
		}
		~Event_Server_Spawn_Ship() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			out << mTeamID << " ";

			out << mPos.x << " ";
			out << mPos.y << " ";
			out << mPos.z << " ";

			out << mRot.x << " ";
			out << mRot.y << " ";
			out << mRot.z << " ";
			out << mRot.w << " ";

			out << mScale.x << " ";
			out << mScale.y << " ";
			out << mScale.z << " ";

			out << mHP << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			in >> mTeamID;

			in >> mPos.x;
			in >> mPos.y;
			in >> mPos.z;

			in >> mRot.x;
			in >> mRot.y;
			in >> mRot.z;
			in >> mRot.w;

			in >> mScale.x;
			in >> mScale.y;
			in >> mScale.z;

			in >> mHP;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Server_Spawn_Ship( mID, mTeamID, mPos, mRot, mScale, mHP ) );
		}
		UINT ID() const
		{
			return mID;
		}
		UINT TeamID() const
		{
			return mTeamID;
		}
		XMFLOAT3 Position() const
		{
			return mPos;
		}
		XMFLOAT4 Rotation() const
		{
			return mRot;
		}
		XMFLOAT3 Scale() const
		{
			return mScale;
		}
		float HP() const
		{
			return mHP;
		}
};

// Event sent from the server to the affected client whenever a player's buff-state changes
class Event_Server_Change_Buff_State : public IEvent
{
	private:
		UINT		mID;
		bool		mIsBuffed;
		float		mBuffMod; // Buffmodifier

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Server_Change_Buff_State()
		{
			mID			= (UINT)-1;
			mIsBuffed	= false;
			mBuffMod	= -1.0f;
		}
		Event_Server_Change_Buff_State( UINT id, bool isBuffed, float buffMod )
		{
			mID			= id;
			mIsBuffed	= isBuffed;
			mBuffMod	= buffMod;
		}
		~Event_Server_Change_Buff_State() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			out << mIsBuffed << " ";
			out << mBuffMod << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			in >> mIsBuffed;
			in >> mBuffMod;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Server_Change_Buff_State( mID, mIsBuffed, mBuffMod ) );
		}
		UINT ID() const
		{
			return mID;
		}
		bool IsBuffed() const
		{
			return mIsBuffed;
		}
		float BuffMod() const
		{
			return mBuffMod;
		}
};

class Event_Server_Sync_Energy_Cell : public IEvent
{
	private:
		UINT				mEnergyCellID;
		UINT				mOwnerID;
		DirectX::XMFLOAT3	mPosition;
		bool				mPickedUp;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Server_Sync_Energy_Cell()
		{
			mEnergyCellID	= (UINT)-1;
			mOwnerID		= (UINT)-1;
			mPosition		= DirectX::XMFLOAT3( 0.0f, 0.0f, 0.0f );
			mPickedUp		= false;
		}

		Event_Server_Sync_Energy_Cell( UINT energyCellID, UINT ownerID, DirectX::XMFLOAT3 position, bool pickedUp )
		{
			mEnergyCellID	= energyCellID;
			mOwnerID		= ownerID;
			mPosition		= position;
			mPickedUp		= pickedUp;
		}

		~Event_Server_Sync_Energy_Cell() {}

		const EventType& GetEventType() const
		{
			return GUID;
		}

		void Serialize( std::ostringstream& out ) const
		{
			out << mEnergyCellID << " ";
			out << mOwnerID << " ";
			out << mPosition.x << " ";
			out << mPosition.y << " ";
			out << mPosition.z << " ";
			out << mPickedUp << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mEnergyCellID;
			in >> mOwnerID;
			in >> mPosition.x;
			in >> mPosition.y;
			in >> mPosition.z;
			in >> mPickedUp;
		}

		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Server_Sync_Energy_Cell( mEnergyCellID, mOwnerID, mPosition, mPickedUp ) );
		}

		UINT EnergyCellID() const
		{
			return mEnergyCellID;
		}

		UINT OwnerID() const
		{
			return mOwnerID;
		}

		DirectX::XMFLOAT3 Position() const
		{
			return mPosition;
		}

		bool PickedUp() const
		{
			return mPickedUp;
		}
};

// Event sent from Enemy Behavior to server to broadcast attack
class Event_Tell_Server_Enemy_Attack_Player : public IEvent
{
	private:
		UINT	mID;
		UINT	mPlayerID;
		float	mDamage;
	
	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Tell_Server_Enemy_Attack_Player()
		{
			mID			= (UINT)-1;
			mPlayerID	= (UINT)-1;
			mDamage		= -1.0f;
		}
		Event_Tell_Server_Enemy_Attack_Player( UINT id, UINT playerID, float damage )
		{
			mID			= id;
			mPlayerID	= playerID;
			mDamage		= damage;
		}
		~Event_Tell_Server_Enemy_Attack_Player() {}	
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			out << mPlayerID << " ";
			out << mDamage << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			in >> mPlayerID;
			in >> mDamage;
		}

		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Tell_Server_Enemy_Attack_Player( mID, mPlayerID, mDamage ) );
		}
		UINT ID() const
		{
			return mID;
		}
		UINT PlayerID() const
		{
			return mPlayerID;
		}		
		float Damage() const
		{
			return mDamage;
		}
};

class Event_Remote_Win : public IEvent
{
	private:
		UINT mTeam;
		
	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Remote_Win()
		{
			mTeam		= (UINT)-1;
		}
		Event_Remote_Win( UINT team )
		{
			mTeam		= team;
		}
		~Event_Remote_Win() {}

		const EventType& GetEventType() const
		{
			return GUID;
		}

		void Serialize( std::ostringstream& out ) const
		{
			out << mTeam << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mTeam;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Remote_Win( mTeam ) );
		}
		UINT Team() const
		{
			return mTeam;
		}
};
	
class Event_Client_Sync_Energy_Cell : public IEvent
{
	private:
		UINT				mEnergyCellID;
		UINT				mOwnerID;
		DirectX::XMFLOAT3	mPosition;
		bool				mPickedUp;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Client_Sync_Energy_Cell()
		{
			mEnergyCellID	= (UINT)-1;
			mOwnerID		= (UINT)-1;
			mPosition		= DirectX::XMFLOAT3( 0.0f, 0.0f, 0.0f );
			mPickedUp		= false;
		}

		Event_Client_Sync_Energy_Cell( UINT energyCellID, UINT ownerID, DirectX::XMFLOAT3 position, bool pickedUp )
		{
			mEnergyCellID	= energyCellID;
			mOwnerID		= ownerID;
			mPosition		= position;
			mPickedUp		= pickedUp;
		}

		~Event_Client_Sync_Energy_Cell() {}

		const EventType& GetEventType() const
		{
			return GUID;
		}

		void Serialize( std::ostringstream& out ) const
		{
			out << mEnergyCellID << " ";
			out << mOwnerID << " ";
			out << mPosition.x << " ";
			out << mPosition.y << " ";
			out << mPosition.z << " ";
			out << mPickedUp << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mEnergyCellID;
			in >> mOwnerID;
			in >> mPosition.x;
			in >> mPosition.y;
			in >> mPosition.z;
			in >> mPickedUp;
		}

		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Client_Sync_Energy_Cell( mEnergyCellID, mOwnerID, mPosition, mPickedUp ) );
		}

		UINT EnergyCellID() const
		{
			return mEnergyCellID;
		}

		UINT OwnerID() const
		{
			return mOwnerID;
		}

		DirectX::XMFLOAT3 Position() const
		{
			return mPosition;
		}

		bool PickedUp() const
		{
			return mPickedUp;
		}
};

class Event_Client_Win : public IEvent
{
	private:
		UINT mTeam;
		
	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Client_Win()
		{
			mTeam		= (UINT)-1;
		}
		Event_Client_Win( UINT team )
		{
			mTeam		= team;
		}
		~Event_Client_Win() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mTeam << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mTeam;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Client_Win( mTeam ) );
		}
		UINT Team() const
		{
			return mTeam;
		}
};

// Event sent from the client whenever a ship needs an update
class Event_Client_Update_Ship : public IEvent
{
	private:
		UINT mID;
		float mDamage;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:

		Event_Client_Update_Ship()
		{
			mID		= (UINT)-1;
			mDamage	= 0.0f;
		}
		Event_Client_Update_Ship( UINT id, float damage )
		{
			mID		= id;
			mDamage	= damage;
		}
		~Event_Client_Update_Ship() {}

		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			out << mDamage << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			in >> mDamage;
		}
		
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Client_Update_Ship( mID, mDamage ) );
		}
		UINT ID() const
		{
			return mID;
		}
		float Damage() const
		{
			return mDamage;
		}
};

// Event sent from server whenever a ship is updated
class Event_Server_Update_Ship : public IEvent
{
	private:
		UINT mID;
		float mMaxShield;
		float mCurrentShield;
		float mCurrentHP;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Server_Update_Ship()
		{
			mID				= (UINT)-1;
			mMaxShield		= -1.0f;
			mCurrentShield	= -1.0f;
			mCurrentHP		= -1.0f;
		}
		Event_Server_Update_Ship( UINT id, float maxShield, float currentShield, float currentHP )
		{
			mID				= id;
			mMaxShield		= maxShield;
			mCurrentShield	= currentShield;
			mCurrentHP		= currentHP;
		}
		~Event_Server_Update_Ship() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			out << mMaxShield << " ";
			out << mCurrentShield << " ";
			out << mCurrentHP << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			in >> mMaxShield;
			in >> mCurrentShield;
			in >> mCurrentHP;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Server_Update_Ship( mID, mMaxShield, mCurrentShield, mCurrentHP ) );
		}
		UINT ID() const
		{
			return mID;
		}
		float MaxShield() const
		{
			return mMaxShield;
		}
		float CurrentShield() const
		{
			return mCurrentShield;
		}
		float CurrentHP() const
		{
			return mCurrentHP;
		}
};

// Event sent from a client whenever the ship needs an update in any way
class Event_Client_Change_Ship_Levels : public IEvent
{
	private:
		UINT mID;
		int mTurretLevelChange;
		int mShieldLevelChange;
		int mBuffLevelChange;
		int mEngineLevelChange;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Client_Change_Ship_Levels()
		{
			mID					= (UINT)-1;
			mTurretLevelChange	= 0;
			mShieldLevelChange	= 0;
			mBuffLevelChange	= 0;
			mEngineLevelChange	= 0;
		}
		Event_Client_Change_Ship_Levels( UINT id, int turretLevelChange, int shieldLevelChange, int buffLevelChange, int engineLevelChange )
		{
			mID					= id;
			mTurretLevelChange	= turretLevelChange;
			mShieldLevelChange	= shieldLevelChange;
			mBuffLevelChange	= buffLevelChange;
			mEngineLevelChange	= engineLevelChange;
		}
		~Event_Client_Change_Ship_Levels() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			out << mTurretLevelChange << " ";
			out << mShieldLevelChange << " ";
			out << mBuffLevelChange << " ";
			out << mEngineLevelChange << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			in >> mTurretLevelChange;
			in >> mShieldLevelChange;
			in >> mBuffLevelChange;
			in >> mEngineLevelChange;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Client_Change_Ship_Levels( mID, mTurretLevelChange, mShieldLevelChange, mBuffLevelChange, mEngineLevelChange ) );
		}
		UINT ID() const
		{
			return mID;
		}
		int TurretLevelChange() const
		{
			return mTurretLevelChange;
		}
		int ShieldLevelChange() const
		{
			return mShieldLevelChange;
		}
		int BuffLevelChange() const
		{
			return mBuffLevelChange;
		}
		int EngineLevelChange() const
		{
			return mEngineLevelChange;
		}
};

class Event_Server_Change_Ship_Levels : public IEvent
{
	private:
		UINT mID;
		int mTurretLevel;
		int mShieldLevel;
		int mBuffLevel;
		int mEngineLevel;
		int mNrOfEnergyCells;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Server_Change_Ship_Levels()
		{
			mID					= (UINT)-1;
			mTurretLevel		= 0;
			mShieldLevel		= 0;
			mBuffLevel			= 0;
			mEngineLevel		= 0;
			mNrOfEnergyCells	= 0;
		}
		Event_Server_Change_Ship_Levels( UINT id, int turretLevel, int shieldLevel, int buffLevel, int engineLevel, int NrOfEnergyCells )
		{
			mID					= id;
			mTurretLevel		= turretLevel;
			mShieldLevel		= shieldLevel;
			mBuffLevel			= buffLevel;
			mEngineLevel		= engineLevel;
			mNrOfEnergyCells	= NrOfEnergyCells;
		}
		~Event_Server_Change_Ship_Levels() {}

		const EventType& GetEventType() const
		{
			return GUID;
		}

		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			out << mTurretLevel << " ";
			out << mShieldLevel << " ";
			out << mBuffLevel << " ";
			out << mEngineLevel << " ";
			out << mNrOfEnergyCells << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			in >> mTurretLevel;
			in >> mShieldLevel;
			in >> mBuffLevel;
			in >> mEngineLevel;
			in >> mNrOfEnergyCells;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Server_Change_Ship_Levels( mID, mTurretLevel, mShieldLevel, mBuffLevel, mEngineLevel, mNrOfEnergyCells ) );
		}
		UINT ID() const
		{
			return mID;
		}
		int TurretLevelChange() const
		{
			return mTurretLevel;
		}
		int ShieldLevelChange() const
		{
			return mShieldLevel;
		}
		int BuffLevelChange() const
		{
			return mBuffLevel;
		}
		int EngineLevelChange() const
		{
			return mEngineLevel;
		}
		int NrOfEnergyCells() const
		{
			return mNrOfEnergyCells;
		}
};

class Event_Upgrade_Player : public IEvent
{
	private:
		int mSpeed;
		int mHealth;
		int mMelee;
		int mRange;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Upgrade_Player()
		{
			mSpeed	= 0;
			mHealth	= 0;
			mMelee	= 0;
			mRange	= 0;
		}
		Event_Upgrade_Player( int speed, int health, int melee, int range )
		{
			mSpeed	= speed;
			mHealth	= health;
			mMelee	= melee;
			mRange	= range;
		}
		~Event_Upgrade_Player() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mSpeed << " ";
			out << mHealth << " ";
			out << mMelee << " ";
			out << mRange << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mSpeed;
			in >> mHealth;
			in >> mMelee;
			in >> mRange;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Upgrade_Player( mSpeed, mHealth, mMelee, mRange ) );
		}
		int Speed() const
		{
			return mSpeed;
		}
		int Health() const
		{
			return mHealth;
		}
		int Melee() const
		{
			return mMelee;
		}
		int Range() const
		{
			return mRange;
		}
};

class Event_Request_Player_Spawn_Position : public IEvent
{
	private:
		UINT mPlayerID;
		UINT mTeamID;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Request_Player_Spawn_Position()
		{
			mPlayerID		= (UINT)-1;
			mTeamID			= (UINT)-1;
		}
		Event_Request_Player_Spawn_Position( UINT playerID, UINT teamID )
		{
			mPlayerID	= playerID;
			mTeamID		= teamID;
		}
		~Event_Request_Player_Spawn_Position() {}

		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mPlayerID	 << " ";
			out << mTeamID		 << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mPlayerID;
			in >> mTeamID;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Request_Player_Spawn_Position( mPlayerID, mTeamID ) );
		}
		UINT PlayerID() const
		{
			return mPlayerID;
		}
		int TeamID() const
		{
			return mTeamID;
		}
};

class Event_New_Player_Spawn_Position : public IEvent
{
	private:
		UINT		mPlayerID;
		XMFLOAT2	mSpawnPosition;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_New_Player_Spawn_Position()
		{
			mPlayerID		= (UINT)-1;
			mSpawnPosition	= XMFLOAT2( 0.0f ,0.0f );
		}
		Event_New_Player_Spawn_Position( UINT playerID, XMFLOAT2 spawnPosition )
		{
			mPlayerID		= playerID;
			mSpawnPosition	= spawnPosition;
		}
		~Event_New_Player_Spawn_Position() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mPlayerID	 << " ";
			out << mSpawnPosition.x	 << " ";
			out << mSpawnPosition.y	 << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mPlayerID;
			in >> mSpawnPosition.x;
			in >> mSpawnPosition.y;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_New_Player_Spawn_Position( mPlayerID, mSpawnPosition ) );
		}
		UINT PlayerID() const
		{
			return mPlayerID;
		}
		XMFLOAT2 SpawnPosition() const
		{
			return mSpawnPosition;
		}
};

class Event_Server_XP : public IEvent
{
	private:
		UINT		mPlayerID;
		int			mXP;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Server_XP()
		{
			mPlayerID	= (UINT)-1;
			mXP			= 0;
		}
		Event_Server_XP( UINT playerID, int XP )
		{
			mPlayerID	= playerID;
			mXP			= XP;
		}
		~Event_Server_XP() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mPlayerID	 << " ";
			out << mXP			 << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mPlayerID;
			in >> mXP;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Server_XP( mPlayerID, mXP ) );
		}
		UINT PlayerID() const
		{
			return mPlayerID;
		}
		int XP() const
		{
			return mXP;
		}

};

class Event_XP : public IEvent
{
	private:
		UINT		mPlayerID;
		int			mXP;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_XP()
		{
			mPlayerID	= (UINT)-1;
			mXP			= 0;
		}
		Event_XP( UINT playerID, int XP )
		{
			mPlayerID	= playerID;
			mXP			= XP;
		}
		~Event_XP() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mPlayerID	 << " ";
			out << mXP			 << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mPlayerID;
			in >> mXP;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_XP( mPlayerID, mXP ) );
		}
		UINT PlayerID() const
		{
			return mPlayerID;
		}
		int XP() const
		{
			return mXP;
		}

};

// Sent from the client when the turret wants to shoot something
class Event_Client_Turret_Fire : public IEvent
{
	private:
		UINT mID;
		UINT mTeamID; 
		// No need for more variables since the client will know where the turret is and where it's facing

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Client_Turret_Fire()
		{
			mID		= (UINT)-1;
			mTeamID	= (UINT)-1;
		}
		Event_Client_Turret_Fire( UINT playerID, UINT teamID )
		{
			mID		= playerID;
			mTeamID	= teamID;
		}
		~Event_Client_Turret_Fire() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			out << mTeamID << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			in >> mTeamID;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Client_Turret_Fire( mID, mTeamID ) );
		}
		UINT ID() const
		{
			return mID;
		}
		UINT TeamID() const
		{
			return mID;
		}
};

class Event_Remote_Turret_Fire : public IEvent
{
	private:
		UINT		mID;
		UINT		mTeamID; 

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Remote_Turret_Fire()
		{
			mID		= (UINT)-1;
			mTeamID	= (UINT)-1;
		}
		Event_Remote_Turret_Fire( UINT playerID, UINT teamID )
		{
			mID		= playerID;
			mTeamID	= teamID;
		}
		~Event_Remote_Turret_Fire() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			out << mTeamID << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			in >> mTeamID;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Remote_Turret_Fire( mID, mTeamID ) );
		}
		UINT ID() const
		{
			return mID;
		}
		UINT TeamID() const
		{
			return mID;
		}
};

// Event sent from the server whenever a turret levels up/down
class Event_Server_Turret_Level_Update : public IEvent
{
	private:
		UINT mID;
		UINT mNewLevel; 
		// No need for more variables since the client will know where the turret is and where it's facing

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Server_Turret_Level_Update()
		{
			mID			= (UINT)-1;
			mNewLevel	= (UINT)-1;
		}
		Event_Server_Turret_Level_Update( UINT playerID, UINT newLevel )
		{
			mID			= playerID;
			mNewLevel	= newLevel;
		}
		~Event_Server_Turret_Level_Update() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			out << mNewLevel << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			in >> mNewLevel;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Client_Turret_Fire( mID, mNewLevel ) );
		}
		UINT ID() const
		{
			return mID;
		}
		UINT NewLevel() const
		{
			return mID;
		}
};

class Event_Client_Initialize_LobbyPlayer : public IEvent
{
	private:
		UINT		mID;
		UINT		mTeamID;
		std::string	mName;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Client_Initialize_LobbyPlayer()
		{
			mID		= (UINT)-1;
			mTeamID = (UINT)-1;
			mName	= "";
		}
		Event_Client_Initialize_LobbyPlayer( UINT id, UINT teamID, std::string name )
		{
			mID		= id;
			mTeamID = teamID;
			mName	= name;
		}
		~Event_Client_Initialize_LobbyPlayer() {}

		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			out << mTeamID << " ";
			out << mName << "";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			in >> mTeamID;
			in >> mName;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Client_Initialize_LobbyPlayer( mID, mTeamID, mName ) );
		}
		UINT ID() const
		{
			return mID;
		}
		UINT TeamID() const
		{
			return mTeamID;
		}
		std::string Name() const
		{
			return mName;
		}
};

// Event sent from the server whenever the turret is updated
class Event_Server_Update_Turret : public IEvent
{
	private:
		UINT mID;
		XMFLOAT4 mRotation;
	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Server_Update_Turret()
		{
			mID			= (UINT)-1;
			mRotation	= XMFLOAT4( 0.0f, 0.0f, 0.0f, 0.0f );
		}
		Event_Server_Update_Turret( UINT playerID, XMFLOAT4 rotation )
		{
			mID			= playerID;
			mRotation	= rotation;
		}
		~Event_Server_Update_Turret() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			out << mRotation.x << " ";
			out << mRotation.y << " ";
			out << mRotation.z << " ";
			out << mRotation.w << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			in >> mRotation.x;
			in >> mRotation.y;
			in >> mRotation.z;
			in >> mRotation.w;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Server_Update_Turret( mID, mRotation ) );
		}
		UINT ID() const
		{
			return mID;
		}
		XMFLOAT4 Rotation() const
		{
			return mRotation;
		}
};

// Event sent locally on the server whenever a ServerTurret fires a projectile
class Event_Turret_Fired_Projectile : public IEvent
{
	private:
		UINT		mID;
		XMFLOAT3	mPosition;
		XMFLOAT3	mDirection;
		float		mSpeed;
		float		mRange;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Turret_Fired_Projectile()
		{
			mID				= (UINT)-1;
			mPosition		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
			mDirection		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
			mSpeed			= 0.0f;
			mRange			= 0.0f;
		}
		Event_Turret_Fired_Projectile( UINT id, XMFLOAT3 position, XMFLOAT3 direction, float speed, float range )
		{
			mID				= id;
			mPosition		= position;
			mDirection		= direction;
			mSpeed			= speed;
			mRange			= range;
		}
		~Event_Turret_Fired_Projectile() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";

			out << mPosition.x << " ";
			out << mPosition.y << " ";
			out << mPosition.z << " ";

			out << mDirection.x << " ";
			out << mDirection.y << " ";
			out << mDirection.z << " ";

			out <<	mSpeed	<< " ";
			out	<<	mRange	<< " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			
			in >> mPosition.x;
			in >> mPosition.y;
			in >> mPosition.z;

			in >> mDirection.x;
			in >> mDirection.y;
			in >> mDirection.z;

			in >> mSpeed;
			in >> mRange;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Turret_Fired_Projectile( mID, mPosition, mDirection, mSpeed, mRange ) );
		}
		UINT ID() const
		{
			return mID;
		}
		XMFLOAT3 Position() const
		{
			return mPosition;
		}
		XMFLOAT3 Direction() const
		{
			return mDirection;
		}
		float Speed() const
		{
			return mSpeed;
		}
		float Range() const
		{
			return mRange;
		}
};

class Event_Server_Initialize_LobbyPlayer : public IEvent
{
	private:
		UINT		mID;
		UINT		mTeamID;
		std::string	mName;
	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Server_Initialize_LobbyPlayer()
		{
			mID		= (UINT)-1;
			mTeamID = (UINT)-1;
			mName	= "";
		}
		Event_Server_Initialize_LobbyPlayer( UINT id, UINT teamID, std::string name )
		{
			mID		= id;
			mTeamID = teamID;
			mName	= name;
		}
		~Event_Server_Initialize_LobbyPlayer() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			out << mTeamID << " ";
			out << mName << "";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			in >> mTeamID;
			in >> mName;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Server_Initialize_LobbyPlayer( mID, mTeamID, mName ) );
		}
		UINT ID() const
		{
			return mID;
		}
		UINT TeamID() const
		{
			return mTeamID;
		}
		std::string Name() const
		{
			return mName;
		}
};

// Event sent to the clients whenever a turret fires a projectile
class Event_Server_Turret_Fired_Projectile : public IEvent
{
	private:
		UINT		mID;
		UINT		mTeamID;
		UINT		mProjectileID;
		XMFLOAT3	mPosition;
		XMFLOAT3	mDirection;
		float		mSpeed;
		float		mRange;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Server_Turret_Fired_Projectile()
		{
			mID				= (UINT)-1;
			mTeamID			= (UINT)-1;
			mProjectileID	= (UINT)-1;
			mPosition		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
			mDirection		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
			mSpeed			= 0.0f;
			mRange			= 0.0f;
		}
		Event_Server_Turret_Fired_Projectile( UINT id, UINT teamID, UINT projectileID, XMFLOAT3 position, XMFLOAT3 rotation, float speed, float range )
		{
			mID				= id;
			mTeamID			= teamID;
			mProjectileID	= projectileID;
			mPosition		= position;
			mDirection		= rotation;
			mSpeed			= speed;
			mRange			= range;
		}
		~Event_Server_Turret_Fired_Projectile() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			out << mTeamID << " ";
			out << mProjectileID << " ";

			out << mPosition.x << " ";
			out << mPosition.y << " ";
			out << mPosition.z << " ";

			out << mDirection.x << " ";
			out << mDirection.y << " ";
			out << mDirection.z << " ";

			out <<	mSpeed	<< " ";
			out	<<	mRange	<< " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			in >> mTeamID;
			in >> mProjectileID;

			in >> mPosition.x;
			in >> mPosition.y;
			in >> mPosition.z;

			in >> mDirection.x;
			in >> mDirection.y;
			in >> mDirection.z;

			in >> mSpeed;
			in >> mRange;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Server_Turret_Fired_Projectile( mID, mTeamID, mProjectileID, mPosition, mDirection, mSpeed, mRange ) );
		}
		UINT ID() const
		{
			return mID;
		}
		UINT TeamID() const
		{
			return mTeamID;
		}
		UINT ProjectileID() const
		{
			return mProjectileID;
		}
		XMFLOAT3 Position() const
		{
			return mPosition;
		}
		XMFLOAT3 Direction() const
		{
			return mDirection;
		}
		float Speed() const
		{
			return mSpeed;
		}
		float Range() const
		{
			return mRange;
		}
};

class Event_Client_Lobby_Finished : public IEvent
{
	private:
	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Client_Lobby_Finished()
		{

		}
		~Event_Client_Lobby_Finished() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{

		}
		void Deserialize( std::istringstream& in )
		{

		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Client_Lobby_Finished() );
		}
};

class Event_Server_Lobby_Finished : public IEvent
{
	private:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Server_Lobby_Finished()
		{

		}
		~Event_Server_Lobby_Finished() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{

		}
		void Deserialize( std::istringstream& in )
		{

		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Server_Lobby_Finished() );
		}
};

class Event_Client_Switch_Team : public IEvent
{
	private:
		UINT		mID;
		UINT		mTeamID;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Client_Switch_Team()
		{
			mID		= (UINT)-1;
			mTeamID = (UINT)-1;
		}
		Event_Client_Switch_Team( UINT id, UINT teamID )
		{
			mID		= id;
			mTeamID = teamID;
		}
		~Event_Client_Switch_Team() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			out << mTeamID << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			in >> mTeamID;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Client_Switch_Team( mID, mTeamID ) );
		}
		UINT ID() const
		{
			return mID;
		}
		UINT TeamID() const
		{
			return mTeamID;
		}
};

class Event_Server_Switch_Team : public IEvent
{
	private:
		UINT		mID;
		UINT		mTeamID;
	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Server_Switch_Team()
		{
			mID		= (UINT)-1;
			mTeamID = (UINT)-1;
		}
		Event_Server_Switch_Team( UINT id, UINT teamID )
		{
			mID		= id;
			mTeamID = teamID;
		}
		~Event_Server_Switch_Team() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			out << mTeamID << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			in >> mTeamID;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Server_Switch_Team( mID, mTeamID ) );
		}
		UINT ID() const
		{
			return mID;
		}
		UINT TeamID() const
		{
			return mTeamID;
		}
};

// A local event triggered by the Player and listened to by the PlayState in order to transfer data faster
class Event_Trigger_Client_Fired_Projectile : public IEvent
{
	private:
		UINT		mID;
		XMFLOAT3	mBodyPos;
		XMFLOAT3	mDirection;
		float		mSpeed;
		float		mRange;
		float		mDamage;
		int			mWeapon;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Trigger_Client_Fired_Projectile()
		{
			mID				= (UINT)-1;
			mBodyPos		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
			mDirection		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
			mSpeed			= 0.0f;
			mRange			= 0.0f;
			mDamage			= 0.0f;
			mWeapon			= 0;
		}

		Event_Trigger_Client_Fired_Projectile( unsigned int id, XMFLOAT3 bodyPos, XMFLOAT3 direction, float speed, float range, float damage, int weapon )
		{
			mID				= id;
			mBodyPos		= bodyPos;
			mDirection		= direction;
			mSpeed			= speed;
			mRange			= range;
			mDamage			= damage;
			mWeapon			= weapon;
		}

		~Event_Trigger_Client_Fired_Projectile() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			
			out << mBodyPos.x << " ";
			out << mBodyPos.y << " ";
			out << mBodyPos.z << " ";

			out << mDirection.x << " ";
			out << mDirection.y << " ";
			out << mDirection.z << " ";

			out <<	mSpeed	<< " ";
			out	<<	mRange	<< " ";
			out << mDamage << " ";
			out << mWeapon;
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;

			in >> mBodyPos.x;
			in >> mBodyPos.y;
			in >> mBodyPos.z;

			in >> mDirection.x;
			in >> mDirection.y;
			in >> mDirection.z;

			in >> mSpeed;
			in >> mRange;
			in >> mDamage;
			in >> mWeapon;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Trigger_Client_Fired_Projectile( mID, mBodyPos, mDirection, mSpeed, mRange, mDamage, mWeapon ) );
		}
		UINT ID() const
		{
			return mID;
		}
		XMFLOAT3 BodyPos() const
		{
			return mBodyPos;
		}
		XMFLOAT3 Direction() const
		{
			return mDirection;
		}
		float Speed() const
		{
			return mSpeed;
		}
		float Range() const
		{
			return mRange;
		}
		float Damage() const
		{
			return mDamage;
		}
		int Weapon() const
		{
			return mWeapon;
		}
};

// A local event triggered by the Player and listened to by the PlayState in order to transfer data faster
class Event_Trigger_Client_Update : public IEvent
{
	private:
		UINT		mID;
		XMFLOAT3	mLowerBodyPos;
		XMFLOAT3	mVelocity;
		XMFLOAT3	mUpperBodyDirection;
		std::string mName;
		bool		mIsBuffed;
		bool		mIsAlive;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Trigger_Client_Update()
		{
			mID						= (UINT)-1;
			mLowerBodyPos			= XMFLOAT3( 0.0f, 0.0f, 0.0f );
			mVelocity				= XMFLOAT3( 0.0f, 0.0f, 0.0f );
			mUpperBodyDirection		= XMFLOAT3( 0.0f, 0.0f, 0.0f );
			mName					= "";
			mIsBuffed				= false;
			mIsAlive				= false;
		}
		Event_Trigger_Client_Update( UINT id, XMFLOAT3 lowerBodyPos, XMFLOAT3 velocity, XMFLOAT3 upperBodyDir, std::string name, bool isBuffed, bool isAlive )
		{
			mID						= id;
			mLowerBodyPos			= lowerBodyPos;
			mVelocity				= velocity;
			mUpperBodyDirection		= upperBodyDir;
			mName					= name;
			mIsBuffed				= isBuffed;
			mIsAlive				= isAlive;
		}
		~Event_Trigger_Client_Update() {}
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

			out << mName << " ";
			out << mIsBuffed << " ";
			out << mIsAlive << " ";
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

			in >> mName;
			in >> mIsBuffed;
			in >> mIsAlive;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Trigger_Client_Update( mID, mLowerBodyPos, mVelocity, mUpperBodyDirection, mName, mIsBuffed, mIsAlive ) );
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
		std::string Name() const
		{
			return mName;
		}
		bool IsBuffed() const
		{
			return mIsBuffed;
		}
		bool IsAlive() const
		{
			return mIsAlive;
		}
};

class Event_Unlock_Player : public IEvent
{
	private:

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Unlock_Player()
		{
		}
		~Event_Unlock_Player() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
		}
		void Deserialize( std::istringstream& in )
		{
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Unlock_Player() );
		}
};

//Empty Event to use to see if you can reach the server
class Event_Client_Reach_Server : public IEvent
{
	private:

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Client_Reach_Server()
		{
		}
		~Event_Client_Reach_Server() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
		}
		void Deserialize( std::istringstream& in )
		{
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Client_Reach_Server() );
		}
};

//Empty Event to use to see if server can reach the client
class Event_Server_Reach_Client : public IEvent
{
	private:

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Server_Reach_Client()
		{
		}
		~Event_Server_Reach_Client() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
		}
		void Deserialize( std::istringstream& in )
		{
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Server_Reach_Client() );
		}
};

class Event_Change_Weapon : public IEvent
{
	private:
		int		mWeapon;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Change_Weapon()
		{
			mWeapon = -1;
		}
		Event_Change_Weapon( int weapon )
		{
			mWeapon	= weapon;
		}
		~Event_Change_Weapon() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mWeapon << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mWeapon;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Change_Weapon( mWeapon ) );
		}
		int Weapon() const
		{
			return mWeapon;
		}
};

class Event_Client_Change_Weapon : public IEvent
{
	private:
		int		mWeapon;
		UINT	mID;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Client_Change_Weapon()
		{
			mWeapon = -1;
			mID		= -1;
		}
		Event_Client_Change_Weapon( int weapon, UINT id )
		{
			mWeapon	= weapon;
			mID		= id;
		}
		~Event_Client_Change_Weapon() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mWeapon << " ";
			out << mID << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mWeapon;
			in >> mID;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Client_Change_Weapon( mWeapon, mID ) );
		}
		int Weapon() const
		{
			return mWeapon;
		}
		UINT ID() const
		{
			return mID;
		}
};

class Event_Server_Change_Weapon : public IEvent
{
	private:
		int		mWeapon;
		UINT	mID;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Server_Change_Weapon()
		{
			mWeapon = -1;
			mID		= -1;
		}
		Event_Server_Change_Weapon( int weapon, UINT id )
		{
			mWeapon	= weapon;
			mID		= id;
		}
		~Event_Server_Change_Weapon() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mWeapon << " ";
			out << mID << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mWeapon;
			in >> mID;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Server_Change_Weapon( mWeapon, mID ) );
		}
		int Weapon() const
		{
			return mWeapon;
		}
		UINT ID() const
		{
			return mID;
		}
};

class Event_Reset_Turret_Targets : public IEvent
{
	private:
	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Reset_Turret_Targets()
		{
		}
		~Event_Reset_Turret_Targets() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
		}
		void Deserialize( std::istringstream& in )
		{
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Reset_Turret_Targets() );
		}
};

class Event_Client_Change_Ready_State : public IEvent
{
	private:
		UINT mID;
		bool mIsReady;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Client_Change_Ready_State()
		{
			mID			= (UINT)-1;
			mIsReady	= false;
		}
		Event_Client_Change_Ready_State( UINT ID, bool isReady )
		{
			mID			= ID;
			mIsReady	= isReady;
		}
		~Event_Client_Change_Ready_State() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			out << mIsReady << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			in >> mIsReady;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Client_Change_Ready_State( mID, mIsReady ) );
		}
		UINT ID() const
		{
			return mID;
		}
		bool IsReady() const
		{
			return mIsReady;
		}
};

class Event_Server_Change_Ready_State : public IEvent
{
	private:
		UINT mID;
		bool mIsReady;

	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Server_Change_Ready_State()
		{
			mID			= (UINT)-1;
			mIsReady	= false;
		}
		Event_Server_Change_Ready_State( UINT ID, bool isReady )
		{
			mID			= ID;
			mIsReady	= isReady;
		}
		~Event_Server_Change_Ready_State() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
			out << mID << " ";
			out << mIsReady << " ";
		}
		void Deserialize( std::istringstream& in )
		{
			in >> mID;
			in >> mIsReady;
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Server_Change_Ready_State( mID, mIsReady ) );
		}
		UINT ID() const
		{
			return mID;
		}
		bool IsReady() const
		{
			return mIsReady;
		}
};

class Event_Host_Start_Game_Countdown : public IEvent
{
	private:
	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Host_Start_Game_Countdown()
		{
		}
		~Event_Host_Start_Game_Countdown() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
		}
		void Deserialize( std::istringstream& in )
		{
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Host_Start_Game_Countdown() );
		}
};

class Event_Server_Start_Game_Countdown : public IEvent
{
	private:
	protected:
	public:
		static const EventType GUID;

	private:
	protected:
	public:
		Event_Server_Start_Game_Countdown()
		{
		}
		~Event_Server_Start_Game_Countdown() {}
		const EventType& GetEventType() const
		{
			return GUID;
		}
		void Serialize( std::ostringstream& out ) const
		{
		}
		void Deserialize( std::istringstream& in )
		{
		}
		IEventPtr Copy() const
		{
			return IEventPtr( new Event_Server_Start_Game_Countdown() );
		}
};

class Event_Enemy_Fired_Projectile : public IEvent
{
private:
	UINT		mEnemy;
	XMFLOAT3	mPosition;
	XMFLOAT3	mDirection;
	float		mSpeed;
	float		mRange;

protected:
public:
	static const EventType GUID;

private:
protected:
public:
	Event_Enemy_Fired_Projectile()
	{
		mEnemy			= (UINT)-1;
		mPosition		= XMFLOAT3(0.0f, 0.0f, 0.0f);
		mDirection		= XMFLOAT3(0.0f, 0.0f, 0.0f);
		mSpeed			= 0.0f;
		mRange			= 0.0f;
	}
	Event_Enemy_Fired_Projectile(UINT enemyID, XMFLOAT3 position, XMFLOAT3 direction, float speed, float range)
	{
		mEnemy			= enemyID;
		mPosition		= position;
		mDirection		= direction;
		mSpeed			= speed;
		mRange			= range;
	}
	~Event_Enemy_Fired_Projectile() {}
	const EventType& GetEventType() const
	{
		return GUID;
	}
	void Serialize(std::ostringstream& out) const
	{
		out << mEnemy << " ";

		out << mPosition.x << " ";
		out << mPosition.y << " ";
		out << mPosition.z << " ";

		out << mDirection.x << " ";
		out << mDirection.y << " ";
		out << mDirection.z << " ";

		out << mSpeed << " ";
		out << mRange << " ";
	}
	void Deserialize(std::istringstream& in)
	{
		in >> mEnemy;

		in >> mPosition.x;
		in >> mPosition.y;
		in >> mPosition.z;

		in >> mDirection.x;
		in >> mDirection.y;
		in >> mDirection.z;

		in >> mSpeed;
		in >> mRange;
	}
	IEventPtr Copy() const
	{
		return IEventPtr(new Event_Enemy_Fired_Projectile(mEnemy, mPosition, mDirection, mSpeed, mRange));
	}
	UINT EnemyID() const
	{
		return mEnemy;
	}
	XMFLOAT3 Position() const
	{
		return mPosition;
	}
	XMFLOAT3 Direction() const
	{
		return mDirection;
	}
	float Speed() const
	{
		return mSpeed;
	}
	float Range() const
	{
		return mRange;
	}
};

class Event_Server_Enemy_Fired_Projectile : public IEvent
{
private:
	UINT		mEnemy;
	UINT		mProjectileID;
	XMFLOAT3	mPosition;
	XMFLOAT3	mDirection;
	float		mSpeed;
	float		mRange;

protected:
public:
	static const EventType GUID;

private:
protected:
public:
	Event_Server_Enemy_Fired_Projectile()
	{
		mEnemy = (UINT)-1;
		mProjectileID = (UINT)-1;
		mPosition = XMFLOAT3(0.0f, 0.0f, 0.0f);
		mDirection = XMFLOAT3(0.0f, 0.0f, 0.0f);
		mSpeed = 0.0f;
		mRange = 0.0f;
	}
	Event_Server_Enemy_Fired_Projectile(UINT enemyID, UINT projectileID, XMFLOAT3 position, XMFLOAT3 direction, float speed, float range)
	{
		mEnemy = enemyID;
		mProjectileID = projectileID;
		mPosition = position;
		mDirection = direction;
		mSpeed = speed;
		mRange = range;
	}
	~Event_Server_Enemy_Fired_Projectile() {}
	const EventType& GetEventType() const
	{
		return GUID;
	}
	void Serialize(std::ostringstream& out) const
	{
		out << mEnemy << " ";

		out << mProjectileID << " ";

		out << mPosition.x << " ";
		out << mPosition.y << " ";
		out << mPosition.z << " ";

		out << mDirection.x << " ";
		out << mDirection.y << " ";
		out << mDirection.z << " ";

		out << mSpeed << " ";
		out << mRange << " ";
	}
	void Deserialize(std::istringstream& in)
	{
		in >> mEnemy;

		in >> mProjectileID;

		in >> mPosition.x;
		in >> mPosition.y;
		in >> mPosition.z;

		in >> mDirection.x;
		in >> mDirection.y;
		in >> mDirection.z;

		in >> mSpeed;
		in >> mRange;
	}
	IEventPtr Copy() const
	{
		return IEventPtr(new Event_Server_Enemy_Fired_Projectile(mEnemy, mProjectileID, mPosition, mDirection, mSpeed, mRange));
	}
	UINT EnemyID() const
	{
		return mEnemy;
	}
	UINT ProjectileID() const
	{
		return mProjectileID;
	}
	XMFLOAT3 Position() const
	{
		return mPosition;
	}
	XMFLOAT3 Direction() const
	{
		return mDirection;
	}
	float Speed() const
	{
		return mSpeed;
	}
	float Range() const
	{
		return mRange;
	}
};

#endif