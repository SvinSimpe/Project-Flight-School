#ifndef REMOTEPLAYER_H
#define REMOTEPLAYER_H

#include "Graphics.h"
#include "EventManager.h"
#include "Events.h"
#include "BoundingGeometry.h"
#include "RenderManager.h"
#include "Font.h"
#include "WeaponInfo.h"

#define	PLAYER_ANIMATION_LEGS_WALK	0
#define PLAYER_ANIMATION_LEGS_IDLE	1

#define PLAYER_ANIMATION_COUNT 2

struct UpperBody
{
	AssetID		playerModel;
	XMFLOAT3	direction;
	XMFLOAT3	position;
};

struct LowerBody
{
	AssetID		playerModel;
	XMFLOAT3	direction;
	XMFLOAT3	position;

	AssetID		currentLowerAnimation;
	float		currentLowerAnimationTime;
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
		AssetID			mRightArm;
		AssetID			mLeftArm;
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
		virtual HRESULT	Render( float deltaTime, int position );
		virtual HRESULT	Initialize();
		void			Release();
						RemotePlayer();
		virtual			~RemotePlayer();







		////TEST
		void TakeDamage( float damage );
};

#endif

