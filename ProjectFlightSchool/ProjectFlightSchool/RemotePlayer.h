#ifndef REMOTEPLAYER_H
#define REMOTEPLAYER_H

#include "Graphics.h"
#include "EventManager.h"
#include "Events.h"
#include "BoundingGeometry.h"
#include "RenderManager.h"
#include "Font.h"

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

	public:

	// Member functions
	private:
		void		RemoteUpdate( IEventPtr newEvent );

	protected:
		void		LookAt( float rotation );

	public:
		void			RemoteInit( unsigned int id, int team, AssetID teamColor, AssetID colorID );
		void			BroadcastDeath( unsigned int shooter );

		virtual void	Die();
		void			HandleSpawn( float deltaTime );
		void			Spawn();
		void			TakeDamage( unsigned int damage, unsigned int shooter );
		void			SetHP( float hp );
		void			CountUpKills();
		int				GetID() const;
		int				GetTeam() const;
		BoundingBox*	GetBoundingBox() const;
		BoundingCircle*	GetBoundingCircle() const;
		XMFLOAT3		GetPosition() const;
		virtual HRESULT	Render( float deltaTime, int position );
		virtual HRESULT	Initialize();
		void			Release();
						RemotePlayer();
		virtual			~RemotePlayer();
};

#endif

