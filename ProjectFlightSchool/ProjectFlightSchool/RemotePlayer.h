#ifndef REMOTEPLAYER_H
#define REMOTEPLAYER_H

#include "Graphics.h"
#include "EventManager.h"
#include "Events.h"
#include "BoundingGeometry.h"
#include "RenderManager.h"
#include "Font.h"
#include "WeaponInfo.h"

#define LEFT_ARM_ID		0
#define RIGHT_ARM_ID	1

#define PLAYER_ANIMATION_LEGS_IDLE			0
#define	PLAYER_ANIMATION_LEGS_WALK			1

#define PLAYER_ANIMATION_CLAYMORE_IDLE		2
#define PLAYER_ANIMATION_CLAYMORE_WALK		3
#define PLAYER_ANIMATION_CLAYMORE_ATTACK	4

#define PLAYER_ANIMATION_SHOTGUN_WALK		5
#define PLAYER_ANIMATION_SHOTGUN_ATTACK		6

#define PLAYER_ANIMATION_COUNT 7

struct ServerPlayer
{
	unsigned int	ID;
	XMFLOAT3		Position;
};

struct UpperBody
{
	AssetID		playerModel;
	XMFLOAT3	direction;
};

struct LowerBody
{
	AnimationTrack	playerModel;
	XMFLOAT3		direction;
	XMFLOAT3		position;
};

struct Arms
{
	AnimationTrack leftArm;
	AnimationTrack rightArm;
};

struct LoadOut
{
	RangedInfo*	rangedWeapon;
	MeleeInfo*	meleeWeapon;

	LoadOut()
	{
		rangedWeapon	= nullptr;
		meleeWeapon		= nullptr;
	}

	void Release()
	{
		SAFE_DELETE( rangedWeapon );
		if( meleeWeapon )
			SAFE_DELETE( meleeWeapon->boundingCircle );
		SAFE_DELETE( meleeWeapon );
	}
};

class RemotePlayer
{
	// Member variables
	private:
	protected:
		unsigned int	mID;
		int				mTeam;
		UpperBody		mUpperBody;
		LowerBody		mLowerBody;
		Arms			mArms;
		bool			mLeftArmAnimationCompleted;
		bool			mRightArmAnimationCompleted;
		AssetID			mAnimations[PLAYER_ANIMATION_COUNT];

		BoundingBox*	mBoundingBox;
		BoundingCircle*	mBoundingCircle;
		float			mCurrentHp;
		float			mMaxHp;
		bool			mIsAlive;
		float			mSpawnTime;
		float			mTimeTillSpawn;
		AssetID			mGreenHPAsset;
		AssetID			mRedHPAsset;
		AssetID			mOrangeHPAsset;
		AssetID			mTeamAsset;
		AssetID			mColorIDAsset;
		int				mNrOfDeaths;
		int				mNrOfKills;
		Font			mFont;

		XMFLOAT3	mVelocity;
		LoadOut*	mLoadOut;

	public:

	// Member functions
	private:
		void			RemoteUpdate( IEventPtr newEvent );

	protected:

	public:
		void			RemoteInit( unsigned int id, int team, AssetID teamColor, AssetID colorID );
		void			BroadcastDeath( unsigned int shooter );

		virtual void	Die();
		void			HandleSpawn( float deltaTime );
		void			Spawn();
		virtual void	TakeDamage( float damage, unsigned int shooter );
		void			SetHP( float hp );
		void			CountUpKills();
		bool			IsAlive() const;
		LoadOut*		GetLoadOut() const;
		float			GetHP() const;
		int				GetID() const;
		int				GetTeam() const;
		BoundingBox*	GetBoundingBox() const;
		BoundingCircle*	GetBoundingCircle() const;
		XMFLOAT3		GetPosition() const;
		XMFLOAT3		GetDirection() const;
		void			SetDirection( XMFLOAT3 direction );
		void			AddImpuls( XMFLOAT3 impuls );
		virtual HRESULT	Update( float deltaTime );
		virtual HRESULT	Render( int position );
		virtual HRESULT	Initialize();
		void			Release();
						RemotePlayer();
		virtual			~RemotePlayer();







		////TEST
		void TakeDamage( float damage );
};

#endif

