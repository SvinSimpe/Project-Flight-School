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
#include "IEventManager.h"
#include <DirectXMath.h>
using namespace DirectX;

class Event_Player_Moved : public IEvent
{
	// Member variables
	private:
		XMFLOAT3		mLowerBodyPos;
		XMFLOAT3		mUpperBodyPos;
		XMFLOAT3		mDirection;

	protected:
	public:
		static const EventType GUID;
	
	// Member functions
	private:
	protected:
		const EventType& GetEventType( void ) const
		{
			return GUID;
		}
	public:
		Event_Player_Moved( XMFLOAT3 lowerBodyPos, XMFLOAT3 upperBodyPos, XMFLOAT3 direction )
		{
			mLowerBodyPos	= lowerBodyPos;
			mUpperBodyPos	= upperBodyPos;
			mDirection		= direction;
		}
		XMFLOAT3 LowerBodyPos() const
		{
			return mLowerBodyPos;
		}
		XMFLOAT3 UpperBodyPos() const
		{
			return mUpperBodyPos;
		}
		XMFLOAT3 Direction() const
		{
			return mDirection;
		}
};

class Event_Remote_Player_Update : public IEvent
{
	// Member variables
	private:
		unsigned int	mID;
		XMFLOAT3		mLowerBodyPos;
		XMFLOAT3		mUpperBodyPos;
		XMFLOAT3		mDirection;

	protected:
	public:
		static const EventType GUID;

	// Member functions
	private:
	protected:
		const EventType& GetEventType( void ) const
		{
			return GUID;
		}
	public:
		Event_Remote_Player_Update( unsigned int id, XMFLOAT3 lowerBodyPos, XMFLOAT3 upperBodyPos, XMFLOAT3 direction )
		{
			mID				= id;
			mLowerBodyPos	= lowerBodyPos;
			mUpperBodyPos	= upperBodyPos;
			mDirection		= direction;
		}
		unsigned int ID() const
		{
			return mID;
		}
		XMFLOAT3 LowerBodyPos() const
		{
			return mLowerBodyPos;
		}
		XMFLOAT3 UpperBodyPos() const
		{
			return mUpperBodyPos;
		}
		XMFLOAT3 Direction() const
		{
			return mDirection;
		}
};

class Event_Local_Player_Joined : public IEvent
{
	// Member variables
	private:
		unsigned int mID;

	protected:
	public:
		static const EventType GUID;

	// Member functions
	private:
	protected:
		const EventType& GetEventType( void ) const
		{
			return GUID;
		}
	public:
		Event_Local_Player_Joined( unsigned int id )
		{
			mID = id;
		}
		unsigned int ID() const
		{
			return mID;
		}
};

class Event_Remote_Player_Joined : public IEvent
{
	// Member variables
	private:
		unsigned int mID;

	protected:
	public:
		static const EventType GUID;

	// Member functions
	private:
	protected:
		const EventType& GetEventType( void ) const
		{
			return GUID;
		}
	public:
		Event_Remote_Player_Joined( unsigned int id )
		{
			mID = id;
		}
		unsigned int ID() const
		{
			return mID;
		}
};

class Event_Remote_Player_Left : public IEvent
{
	// Member variables
	private:
		unsigned int mID;

	protected:
	public:
		static const EventType GUID;

	// Member functions
	private:
	protected:
		const EventType& GetEventType( void ) const
		{
			return GUID;
		}
	public:
		Event_Remote_Player_Left( unsigned int id )
		{
			mID = id;
		}
		unsigned int ID() const
		{
			return mID;
		}
};

class Event_Change_State : public IEvent
{
	// Member variables
	private:
		int mEventID; 
	protected:
	public:
		static const EventType GUID;

	// Member functions
	private:
	protected:
		const EventType& GetEventType( void ) const
		{
			return GUID;
		}
	public:
		Event_Change_State( int eventID )
		{
			mEventID = eventID;
		}
		int EventID() const
		{
			return mEventID;
		}
};

class Event_Start_Server : public IEvent
{
	// Member variables;
	private:
	protected:
	public:
		static const EventType GUID;


	// Member functions
	private:
	protected:
		const EventType& GetEventType( void ) const
		{
			return GUID;
		}
	public:
		Event_Start_Server()
		{
		}
};

class Event_Start_Client : public IEvent
{
	// Member variables;
	private:
	protected:
	public:
		static const EventType GUID;


	// Member functions
	private:
	protected:
		const EventType& GetEventType( void ) const
		{
			return GUID;
		}
	public:
		Event_Start_Client()
		{
		}
};

class Event_Player_Died : public IEvent
{
	// Member variables
	private:
		unsigned int mID;
	protected:
	public:
		static const EventType GUID;

	// Member functions
	private:
	protected:
		const EventType& GetEventType( void ) const
		{
			return GUID;
		}
	public:
		Event_Player_Died( unsigned int id )
		{
			mID = id;
		}
		unsigned int ID() const
		{
			return mID;
		}
};

class Event_Remote_Player_Died : public IEvent
{
	// Member variables
	private:
		unsigned int mID;

	protected:
	public:
		static const EventType GUID;

	// Member functions
	private:
		protected:
		const EventType& GetEventType( void ) const
		{
			return GUID;
		}
	public:
		Event_Remote_Player_Died( unsigned int id )
		{
			mID = id;
		}
		unsigned int ID() const
		{
			return mID;
		}
};

class Event_Player_Damaged : public IEvent
{
	// Member variables
	private:
		unsigned int mID;
		unsigned int mProjectileID;
	protected:
	public:
		static const EventType GUID;

	// Member functions
	private:
	protected:
		const EventType& GetEventType( void ) const
		{
			return GUID;
		}
	public:
		Event_Player_Damaged( unsigned int id, unsigned int projectileID )
		{
			mID				= id;
			mProjectileID	= projectileID;
		}
		unsigned int ID() const
		{
			return mID;
		}
		unsigned int ProjectileID() const
		{
			return mProjectileID;
		}
};

class Event_Remote_Player_Damaged : public IEvent
{
	// Member variables
	private:
		unsigned int mID;
		unsigned int mProjectileID;

	protected:
	public:
		static const EventType GUID;

	// Member functions
	private:
		protected:
		const EventType& GetEventType( void ) const
		{
			return GUID;
		}
	public:
		Event_Remote_Player_Damaged( unsigned int id, unsigned int projectileID )
		{
			mID = id;
			mProjectileID	= projectileID;
		}
		unsigned int ID() const
		{
			return mID;
		}
		unsigned int ProjectileID() const
		{
			return mProjectileID;
		}
};

class Event_Player_Spawned : public IEvent
{
	// Member variables
	private:
		unsigned int mID;
	protected:
	public:
		static const EventType GUID;

	// Member functions
	private:
	protected:
		const EventType& GetEventType( void ) const
		{
			return GUID;
		}
	public:
		Event_Player_Spawned( unsigned int id )
		{
			mID = id;
		}
		unsigned int ID() const
		{
			return mID;
		}
};

class Event_Remote_Player_Spawned : public IEvent
{
	// Member variables
	private:
		unsigned int mID;

	protected:
	public:
		static const EventType GUID;

	// Member functions
	private:
		protected:
		const EventType& GetEventType( void ) const
		{
			return GUID;
		}
	public:
		Event_Remote_Player_Spawned( unsigned int id )
		{
			mID = id;
		}
		unsigned int ID() const
		{
			return mID;
		}
};

class Event_Projectile_Fired : public IEvent
{
	// Member variables
	private:
		unsigned int	mID;
		XMFLOAT3		mBodyPos;
		XMFLOAT3		mDirection;

	protected:
	public:
		static const EventType GUID;
	
	// Member functions
	private:
	protected:
		const EventType& GetEventType( void ) const
		{
			return GUID;
		}
	public:
		Event_Projectile_Fired( unsigned int id, XMFLOAT3 bodyPos, XMFLOAT3 direction )
		{
			mID				= id;
			mBodyPos		= bodyPos;
			mDirection		= direction;
		}
		unsigned int ID() const
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
};

class Event_Remote_Projectile_Fired : public IEvent
{
	// Member variables
	private:
		unsigned int	mID;
		unsigned int	mProjectileID;
		XMFLOAT3		mBodyPos;
		XMFLOAT3		mDirection;

	protected:
	public:
		static const EventType GUID;
	
	// Member functions
	private:
	protected:
		const EventType& GetEventType( void ) const
		{
			return GUID;
		}
	public:
		Event_Remote_Projectile_Fired( unsigned int id, unsigned int projectileID, XMFLOAT3 bodyPos, XMFLOAT3 direction )
		{
			mID				= id;
			mProjectileID	= projectileID;
			mBodyPos		= bodyPos;
			mDirection		= direction;
		}
		unsigned int ID() const
		{
			return mID;
		}
		unsigned int ProjectileID() const
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
};

class Event_Player_Melee_Hit : public IEvent
{
	// Member variables
	private:
		unsigned int	mID;
		unsigned int	mDamage;
		float			mKnockBack;
		XMFLOAT3		mDirection;

	protected:
	public:
		static const EventType GUID;
	
	// Member functions
	private:
	protected:
		const EventType& GetEventType( void ) const
		{
			return GUID;
		}
	public:
		Event_Player_Melee_Hit( unsigned int id, unsigned int damage, float knockBack, XMFLOAT3 direction )
		{
			mID				= id;
			mDamage			= damage;
			mKnockBack		= knockBack;
			mDirection		= direction;
		}
		unsigned int ID() const
		{
			return mID;
		} 
		unsigned int Damage() const
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

class Event_Remote_Player_Melee_Hit : public IEvent
{
	// Member variables
	private:
		unsigned int	mID;
		unsigned int	mDamage;
		float			mKnockBack;
		XMFLOAT3		mDirection;

	protected:
	public:
		static const EventType GUID;
	
	// Member functions
	private:
	protected:
		const EventType& GetEventType( void ) const
		{
			return GUID;
		}
	public:
		Event_Remote_Player_Melee_Hit( unsigned int id, unsigned int damage, float knockBack, XMFLOAT3 direction )
		{
			mID				= id;
			mDamage			= damage;
			mKnockBack		= knockBack;
			mDirection		= direction;
		}
		unsigned int ID() const
		{
			return mID;
		}
		unsigned int Damage() const
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