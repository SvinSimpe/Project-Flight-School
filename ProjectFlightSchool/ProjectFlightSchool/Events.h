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

class Event_Player_Update : public IEvent
{
	// Member variables
	private:
		XMFLOAT3		mLowerBodyPos;
		XMFLOAT3		mVelocity;
		XMFLOAT3		mUpperBodyDirection;

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
		Event_Player_Update( XMFLOAT3 lowerBodyPos, XMFLOAT3 velocity, XMFLOAT3 upperBodyDirection )
		{
			mLowerBodyPos			= lowerBodyPos;
			mVelocity				= velocity;
			mUpperBodyDirection		= upperBodyDirection;
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

class Event_Remote_Player_Update : public IEvent
{
	// Member variables
	private:
		unsigned int	mID;
		XMFLOAT3		mLowerBodyPos;
		XMFLOAT3		mVelocity;
		XMFLOAT3		mUpperBodyDirection;

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
		Event_Remote_Player_Update( unsigned int id, XMFLOAT3 lowerBodyPos, XMFLOAT3 velocity, XMFLOAT3 upperBodyDirection )
		{
			mID						= id;
			mLowerBodyPos			= lowerBodyPos;
			mVelocity				= velocity;
			mUpperBodyDirection		= upperBodyDirection;
		}
		unsigned int ID() const
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

class Event_Local_Player_Joined : public IEvent
{
	// Member variables
	private:
		unsigned int	mID;
		int				mTeam;

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
		Event_Local_Player_Joined( unsigned int id, int team )
		{
			mID		= id;
			mTeam	= team;
		}
		unsigned int ID() const
		{
			return mID;
		}
		int Team() const
		{
			return mTeam;
		}
};

class Event_Remote_Player_Joined : public IEvent
{
	// Member variables
	private:
		unsigned int	mID;
		int				mTeam;

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
		Event_Remote_Player_Joined( unsigned int id, int team )
		{
			mID		= id;
			mTeam	= team;
		}
		unsigned int ID() const
		{
			return mID;
		}
		int Team() const
		{
			return mTeam;
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
		std::string mPort;
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
		Event_Start_Server( std::string port )
		{
			mPort = port;
		}
		std::string Port()
		{
			return mPort;
		}
};

class Event_Game_Started : public IEvent
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
		Event_Game_Started(  )
		{
		}
};

class Event_Game_Ended : public IEvent
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
		Event_Game_Ended(  )
		{
		}
};

class Event_Start_Client : public IEvent
{
	// Member variables;
	private:
		std::string mIP;
		std::string mPort;
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
		Event_Start_Client( std::string ip, std::string port )
		{
			mIP		= ip;
			mPort	= port;
		}
		std::string IP()
		{
			return mIP;
		}
		std::string Port()
		{
			return mPort;
		}
};

class Event_Player_Died : public IEvent
{
	// Member variables
	private:
		unsigned int mID;
		unsigned int mKillerID;

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
		Event_Player_Died( unsigned int id, unsigned int killer )
		{
			mID			= id;
			mKillerID	= killer;
		}
		unsigned int ID() const
		{
			return mID;
		}
		unsigned int KillerID() const
		{
			return mKillerID;
		}
};

class Event_Remote_Player_Died : public IEvent
{
	// Member variables
	private:
		unsigned int mID;
		unsigned int mKillerID;

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
		Event_Remote_Player_Died( unsigned int id, unsigned int killer )
		{
			mID			= id;
			mKillerID	= killer;
		}
		unsigned int ID() const
		{
			return mID;
		}
		unsigned int KillerID() const
		{
			return mKillerID;
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

class Event_Player_Update_HP : public IEvent
{
	// Member variables
	private:
		unsigned int	mID;
		float			mHP;
	protected:
	public:
		static const EventType GUID;

	//Member functions
	private:
	protected:
		const EventType& GetEventType( void ) const
		{
			return GUID;
		}
	public:
		Event_Player_Update_HP( unsigned int id, float hp )
		{
			mID = id;
			mHP	= hp;
		}
		unsigned int ID() const
		{
			return mID;
		} 
		float HP()
		{
			return mHP;
		}
};

class Event_Remote_Player_Update_HP : public IEvent
{
	// Member variables
	private:
		unsigned int	mID;
		float			mHP;
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
		Event_Remote_Player_Update_HP( unsigned int id, float hp )
		{
			mID = id;
			mHP	= hp;
		}
		unsigned int ID() const
		{
			return mID;
		}
		float HP()
		{
			return mHP;
		}
};

class Event_Sync_Enemy : public IEvent
{
	// Member variables
	private:
		unsigned int	mID;
		unsigned int	mState;
		unsigned int	mEnemyType;
		XMFLOAT3		mPosition;
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
	Event_Sync_Enemy( unsigned int id, unsigned int state, unsigned int enemyType, XMFLOAT3 position, XMFLOAT3 direction )
		{
			mID				= id;
			mState			= state;
			mEnemyType		= enemyType;
			mPosition		= position;
			mDirection		= direction;
		}
		unsigned int ID() const
		{
			return mID;
		}
		unsigned int State() const
		{
			return mState;
		}
		unsigned int Type() const
		{
			return mEnemyType;
		}
		XMFLOAT3 Position() const
		{
			return mPosition;
		}
		XMFLOAT3 Direction() const
		{
			return mDirection;
		}
};

class Event_Server_Initialized : public IEvent
{
	// Member variables
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
		Event_Server_Initialized( )
		{
		}
};

class Event_Connection_Failed : public IEvent
{
	// Member variables
	private:
		std::string mFailMessage;
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
		Event_Connection_Failed( std::string failMessage )
		{
			mFailMessage = failMessage;
		}
		std::string FailMessage()
		{
			return mFailMessage;
		}
};

class Event_Enemy_List_Synced : public IEvent
{
	// Member variables
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
		Event_Enemy_List_Synced( )
		{
		}
};
		
class Event_Player_Melee_Hit : public IEvent
{
	// Member variables
	private:
		unsigned int	mID;
		float			mDamage;
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
		Event_Player_Melee_Hit( unsigned int id, float damage, float knockBack, XMFLOAT3 direction )
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

class Event_Remote_Player_Melee_Hit : public IEvent
{
	// Member variables
	private:
		unsigned int	mID;
		float			mDamage;
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
		Event_Remote_Player_Melee_Hit( unsigned int id, float damage, float knockBack, XMFLOAT3 direction )
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

class Event_Player_Attack : public IEvent
{
	// Member variables
	private:
		unsigned int	mArmID;
		unsigned int	mAnimation;

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
		Event_Player_Attack( unsigned int armID, unsigned int animation )
		{
			mArmID			= armID;
			mAnimation		= animation;
		}
		unsigned int ArmID() const
		{
			return mArmID;
		}
		unsigned int Animation() const
		{
			return mAnimation;
		}
};

class Event_Remote_Player_Attack : public IEvent
{
	// Member variables
	private:
		unsigned int	mID;
		unsigned int	mArmID;
		unsigned int	mAnimation;

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
		Event_Remote_Player_Attack( unsigned int id, unsigned int armID, unsigned int animation )
		{
			mID				= id;
			mArmID			= armID;
			mAnimation		= animation;
		}
		unsigned int ID() const
		{
			return mID;
		}
		unsigned int ArmID() const
		{
			return mArmID;
		}
		unsigned int Animation() const
		{
			return mAnimation;
		}
};

class Event_Sync_Spawn : public IEvent
{
	// Member variables
	private:
		unsigned int	mID;
		XMFLOAT3		mPosition;
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
		Event_Sync_Spawn( unsigned int id, XMFLOAT3 position )
		{
			mID		  = id;
			mPosition = position;
		}
		unsigned int ID() const
		{
			return mID;
		}

		XMFLOAT3 Position() const
		{
			return mPosition;
		}

};

class Event_Add_Point_Light : public IEvent
{
	// Member variables
	private:
		void* mLight;

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
		Event_Add_Point_Light( void* light )
		{
			mLight = light;
		}
		void* GetPointLight()
		{
			return mLight;
		}
};

class Event_Update_Enemy_Position : public IEvent
{
	// Member variables
	private:
		unsigned int	mID;
		XMFLOAT3		mPosition;
		XMFLOAT3		mDirection;
		bool			mIsAlive;
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
		Event_Update_Enemy_Position( unsigned int id, XMFLOAT3 position, XMFLOAT3 direction, bool isAlive )
		{
			mID			= id;
			mPosition	= position;
			mDirection	= direction;
			mIsAlive	= isAlive;
		}

		unsigned int ID() const
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
};
		
class Event_Remove_Point_Light : public IEvent
{
	// Member variables
	private:
		void* mLight;

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

		Event_Remove_Point_Light( void* light )
		{
			mLight = light;
		}
		void* GetPointLight()
		{
			return mLight;
		}
};
class Event_Sync_Enemy_Type : public IEvent
{
	// Member variables
	private:
		unsigned int	mID;
		unsigned int	mEnemyType;
	protected:
	public:
		static const EventType GUID;

	private:
	protected:
		const EventType& GetEventType( void ) const
		{
			return GUID;
		}
	public:

	Event_Sync_Enemy_Type( unsigned int id, unsigned int enemyType )
		{
			mID				= id;
			mEnemyType		= enemyType;
		}
		unsigned int ID() const
		{
			return mID;
		}
		unsigned int Type() const
		{
			return mEnemyType;
		}
};

class Event_Load_Level : public IEvent
{
	// Member variables
	private:
		std::string mFilePath;
	protected:
	public:
		static const EventType GUID;

	private:
	protected:
		const EventType& GetEventType( void ) const
		{
			return GUID;
		}
	public:
		Event_Load_Level( std::string filePath )
		{
			mFilePath = filePath;
		}
		std::string GetFileName()
		{
			return mFilePath;
		}
};

class Event_Sync_Enemy_State : public IEvent
{
	// Member variables
	private:
		unsigned int	mID;
		unsigned int	mState;

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
		Event_Sync_Enemy_State( unsigned int id, unsigned int state )
		{
			mID				= id;
			mState			= state;
		}
		unsigned int ID() const
		{
			return mID;
		}
		unsigned int State() const
		{
			return mState;
		}
};

class Event_Set_Enemy_State : public IEvent // Server side
{
	// Member variables
	private:
		unsigned int	mID;
		unsigned int	mState;

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
	Event_Set_Enemy_State( unsigned int id, unsigned int state )
		{
			mID				= id;
			mState			= state;
		}
		unsigned int ID() const
		{
			return mID;
		}
		unsigned int State() const
		{
			return mState;
		}
};

class Event_Set_Remote_Enemy_State : public IEvent // Client side
{
	// Member variables
	private:
		unsigned int	mID;
		unsigned int	mState;

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
	Event_Set_Remote_Enemy_State( unsigned int id, unsigned int state )
		{
			mID				= id;
			mState			= state;
		}
		unsigned int ID() const
		{
			return mID;
		}
		unsigned int State() const
		{
			return mState;
		}
};

class Event_Projectile_Damage_Enemy : public IEvent
{
	// Member variables
	private:
		unsigned int	mShooterID;
		unsigned int	mProjectileID;
		unsigned int	mEnemyID;
		float			mDamage;

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
		Event_Projectile_Damage_Enemy( unsigned int shooterID, unsigned int projectileID, unsigned int enemyID, float damage )
		{
			mShooterID		= shooterID;
			mProjectileID	= projectileID;
			mEnemyID		= enemyID;
			mDamage			= damage;
		}
		unsigned int Shooter() const
		{
			return mShooterID;
		}
		unsigned int Projectile() const
		{
			return mProjectileID;
		}
		unsigned int Enemy() const
		{
			return mEnemyID;
		}
		float Damage() const
		{
			return mDamage;
		}
};

class Event_Enemy_Died : public IEvent
{
	// Member variables
	private:
		unsigned int	mEnemyID;

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
		Event_Enemy_Died( unsigned int enemyID )
		{
			mEnemyID		= enemyID;
		}
		unsigned int Enemy() const
		{
			return mEnemyID;
		}
};

class Event_Enemy_Attack_Player : public IEvent
{
	// Member variables
	private:
		unsigned int	mPlayerID;	
		float			mDamage;

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
		Event_Enemy_Attack_Player( unsigned int playerID, float damage )
		{
			mPlayerID	= playerID;
			mDamage		= damage;
		}
		unsigned int Player() const
		{
			return mPlayerID;
		}

		float Damage() const
		{
			return mDamage;
		}
};

class Event_Remote_Player_Down : public IEvent
{
	// Member variables
	private:
		unsigned int	mPlayerID;

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
		Event_Remote_Player_Down( unsigned int playerID )
		{
			mPlayerID	= playerID;
		}
		unsigned int Player() const
		{
			return mPlayerID;
		}
};

class Event_Player_Down : public IEvent
{
	// Member variables
	private:
		unsigned int	mPlayerID;

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
		Event_Player_Down( unsigned int playerID )
		{
			mPlayerID	= playerID;
		}
		unsigned int Player() const
		{
			return mPlayerID;
		}
};

class Event_Remote_Player_Up : public IEvent
{
	// Member variables
	private:
		unsigned int	mPlayerID;

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
		Event_Remote_Player_Up( unsigned int playerID )
		{
			mPlayerID	= playerID;
		}
		unsigned int Player() const
		{
			return mPlayerID;
		}
};

class Event_Player_Up : public IEvent
{
	// Member variables
	private:
		unsigned int	mPlayerID;

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
		Event_Player_Up( unsigned int playerID )
		{
			mPlayerID	= playerID;
		}
		unsigned int Player() const
		{
			return mPlayerID;
		}
};

class Event_Remote_Player_Revive : public IEvent
{
	// Member variables
	private:
		unsigned int	mPlayerID;
		float			mDeltaTime;

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
		Event_Remote_Player_Revive( unsigned int playerID, float deltaTime )
		{
			mPlayerID	= playerID;
			mDeltaTime	= deltaTime;
		}
		unsigned int Player() const
		{
			return mPlayerID;
		}
		float DeltaTime() const
		{
			return mDeltaTime;
		}
};

class Event_Player_Revive : public IEvent
{
	// Member variables
	private:
		unsigned int	mPlayerID;
		float			mDeltaTime;

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
		Event_Player_Revive( unsigned int playerID, float deltaTime )
		{
			mPlayerID	= playerID;
			mDeltaTime	= deltaTime;
		}
		unsigned int Player() const
		{
			return mPlayerID;
		}
		float DeltaTime() const
		{
			return mDeltaTime;
		}
};
