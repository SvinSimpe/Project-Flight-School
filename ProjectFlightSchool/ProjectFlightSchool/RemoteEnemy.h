#ifndef REMOTEENEMY_H
#define REMOTEENEMY_H

#include "Graphics.h"
#include "RenderManager.h"
#include "Enemy.h"

class RemoteEnemy
{
	// Member variables
	private:
		unsigned int		mID;
		EnemyType			mEnemyType;
		AnimationTrack		mAnimationTrack;
		XMFLOAT3			mPosition;
		XMFLOAT3			mDirection;
		bool				mIsSynced;

	protected:
	public:

	// Member functions
	private:
	protected:
	public:
		HRESULT				Update( float deltaTime );
		HRESULT				Render();
		void				SetID( unsigned int id );
		void				SetEnemyType( EnemyType type );
		void				SetModelID( AssetID model, AssetID defaultAnimation );
		void				SetAnimation( AssetID animation );
		void				SetPosition( XMFLOAT3 position );
		void				SetDirection( XMFLOAT3 direction );
		void				SetSynced( bool isSynced );
		bool				IsSynced() const;
		EnemyType			GetEnemyType() const;
		virtual HRESULT		Initialize( int id, AssetID model, AssetID animation );
		void				Release();
							RemoteEnemy();
		virtual				~RemoteEnemy();
};

#endif