#ifndef REMOTEPLAYER_H
#define REMOTEPLAYER_H

#include "Graphics.h"
#include "EventManager.h"
#include "Events.h"
#include "BoundingGeometry.h"
#include "RenderManager.h"

struct UpperBody
{
	UINT		playerModel;
	XMFLOAT3	direction;
	XMFLOAT3	position;
};

struct LowerBody
{
	UINT		playerModel;
	XMFLOAT3	direction;
	XMFLOAT3	position;
	float		speed;
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
		virtual HRESULT	Render( float deltaTime );
		virtual HRESULT	Initialize();
		void			Release();
						RemotePlayer();
		virtual			~RemotePlayer();
};

#endif

