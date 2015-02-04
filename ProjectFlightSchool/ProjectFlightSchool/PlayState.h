#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include <sstream>
#include "Map.h"
#include "BaseState.h"
#include "Player.h"
#include "RemotePlayer.h"
#include "MapNodeManager.h"
#include "Font.h"
#include "Enemy.h"
#include "Ship.h"
#include "Image.h"
#include "EnemySpawn.h"
#include "RemoteEnemy.h"
#include "EnemyAnimationManager.h"
#include "Gui.h"
//Test
#include "ParticleManager.h"
#include "SoundBufferHandler.h"


#define MAX_REMOTE_PLAYERS		14 //There is only 14 colorIDs.
#define COLLISION_CHECK_OFFSET	1	// 0 == Every frame

class PlayState : public BaseState
{
	// Class members
	private:
		AnimationTrack mTestAnimation;

		AssetID mTest2dAsset;
		AssetID mTeams[2];
		AssetID	mColorIDs[MAX_REMOTE_PLAYERS];

		Map*		mWorldMap;
		MapNodeManager* mMapNodeMan;

		//Collision
		unsigned int	mFrameCounter;

		//TEST
		std::vector<RemotePlayer*> mAllPlayers;

		Ship						mShip;

		//Game Data
		Player*						mPlayer;
		std::vector<RemotePlayer*>	mRemotePlayers;
		Projectile**				mProjectiles;				// A collection of the games projectiles
		int							mNrOfProjectilesFired;
		int							mCurrentColor;
		Font						mFont;
		EnemyAnimationManager*		mEnemyAnimationManager;
		RemoteEnemy**				mEnemies;
		XMFLOAT3*					mSpawners;
		AssetID						mSpawnModel;
		bool						mEnemyListSynced;
		bool						mServerInitialized;

		ParticleManager*			mParticleManager;

		RADAR_UPDATE_INFO			mRadarObjects[MAX_RADAR_OBJECTS];
		Gui*						mGui;

		//TestSound
		int							m3DSoundAsset;
		int							mSoundAsset;
	
	protected:
	public:

	// Class functions
	private:
		void			SetEnemyState( unsigned int id, EnemyState state );
		void			SyncEnemy( unsigned int id, EnemyState state, EnemyType type, XMFLOAT3 position, XMFLOAT3 direction );
		void			UpdateEnemyPosition( unsigned int id, XMFLOAT3 position, XMFLOAT3 direction, bool isAlive );
		void			SyncSpawn( unsigned int id, XMFLOAT3 position );
		void			RemoteUpdate( IEventPtr newEvent );
		void			HandleDeveloperCameraInput();
		void			CheckPlayerCollision();
		void			CheckProjectileCollision();
		void			CheckMeeleCollision();
		void			EventListener( IEventPtr newEvent );
		void			BroadcastDamage( unsigned int playerID, unsigned int projectileID );						// Tell server that local  player has taken damage
		void			BroadcastMeleeDamage( unsigned playerID, float damage, float knockBack, XMFLOAT3 direction );
		void			BroadcastEnemyProjectileDamage( unsigned int shooterID, unsigned int projectileID, unsigned int enemyID, float damage );
		void			FireProjectile( unsigned int id, unsigned int projectileID, XMFLOAT3 position, XMFLOAT3 direction );
		void			UpdateProjectiles( float deltaTime );
		void			RenderProjectiles();
		void			HandleRemoteProjectileHit( unsigned int id, unsigned int projectileID );
		void			HandleRemoteMeleeHit( unsigned int id, float damage, float knockBack, XMFLOAT3 direction );

	protected:
	public:
		virtual HRESULT Update( float deltaTime );
		virtual HRESULT Render();
		virtual void	OnEnter();
		virtual void	OnExit();
		virtual void	Reset();

		virtual HRESULT Initialize();
		virtual void	Release();
						PlayState();
		virtual			~PlayState();
};

#endif