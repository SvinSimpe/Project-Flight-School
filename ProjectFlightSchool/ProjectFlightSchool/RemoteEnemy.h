#ifndef REMOTEENEMY_H
#define REMOTEENEMY_H

#include "RenderManager.h"
#include "Enemy.h"

class RemoteEnemy
{
	// Member variables
	private:
		unsigned int		mID;
		float				mCurrentHP;
		float				mMaxHP;
		EnemyType			mEnemyType;
		AnimationTrack		mAnimationTrack;
		XMFLOAT3			mPosition;
		XMFLOAT3			mDirection;
		bool				mIsAlive;
		bool				mIsSynced;
		bool				mLoopAnimation;
		BoundingCircle*		mBoundingCircle;
		AssetID				mHPBar;

	protected:
	public:

	// Member functions
	private:
	protected:
	public:
		XMFLOAT3			GetPosition() const;
		HRESULT				Update( float deltaTime );
		HRESULT				Render();
		void				SetID( unsigned int id );
		unsigned int		GetID()	const;
		void				SetHP(float hp);
		void				SetMaxHP( float hp );
		float				GetMaxHP() const;
		float				GetHP() const;
		void				SetIsAlive( bool isAlive );
		bool				IsAlive() const;
		void				SetLoopAnimation( bool loop );
		void				SetEnemyType( EnemyType type );
		void				SetModelID( AssetID model, AssetID defaultAnimation );
		void				SetAnimation( AssetID animation, bool blendWithCurrent = false );
		void				SetPosition( XMFLOAT3 position );
		void				SetDirection( XMFLOAT3 direction );
		void				SetSynced( bool isSynced );
		bool				IsSynced() const;
		EnemyType			GetEnemyType() const;
		BoundingCircle*		GetBoundingCircle() const;
		float				GetHPBarScaleFactor( float initScale ) const;
		float				GetEnemyHeightOffset() const;
		virtual HRESULT		Initialize( int id, AssetID model, AssetID animation );
		void				Release();
							RemoteEnemy();
		virtual				~RemoteEnemy();
};

#endif