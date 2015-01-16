#ifndef REMOTEPLAYER_H
#define REMOTEPLAYER_H

#include "Graphics.h"
#include "EventManager.h"
#include "Events.h"
#include "BoundingGeometry.h"
#include "RenderManager.h"

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


	public:

	// Member functions
	private:
		void		RemoteUpdate( IEventPtr newEvent );

	protected:
		void		LookAt( float rotation );

	public:
		void			RemoteInit( unsigned int id, int team, AssetID teamColor, AssetID colorID );
		void			BroadcastDeath();

		virtual void	Die();
		void			HandleSpawn( float deltaTime );
		void			Spawn();
		void			TakeDamage( unsigned int damage );
		void			SetHP( float hp );
		int				GetID() const;
		BoundingBox*	GetBoundingBox() const;
		BoundingCircle*	GetBoundingCircle() const;
		XMFLOAT3		GetPosition() const;
		HRESULT			Render();
		virtual HRESULT	Initialize();
		void			Release();
						RemotePlayer();
		virtual			~RemotePlayer();
};

#endif

