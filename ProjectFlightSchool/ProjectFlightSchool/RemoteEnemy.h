#ifndef REMOTEENEMY_H
#define REMOTEENEMY_H

#include "Graphics.h"
#include "RenderManager.h"
#include "Enemy.h"

class RemoteEnemy
{
	private:
		unsigned int		mID;
		AnimationTrack		mAnimationTrack;
		AssetID				mAnimations[ENEMY_ANIMATION_COUNT];
		XMFLOAT3			mPosition;
		XMFLOAT3			mDirection;

	protected:
	public:

	private:
	protected:
	public:
		XMFLOAT3			GetPosition() const;
		HRESULT				Update( float deltaTime );
		HRESULT				Render();
		void				SetID( unsigned int id );
		void				SetModelID( AssetID model );
		void				SetAnimation( AssetID animation );
		void				SetPosition( XMFLOAT3 position );
		void				SetDirection( XMFLOAT3 direction );
		virtual HRESULT		Initialize( int id );
		void				Release();
							RemoteEnemy();
		virtual				~RemoteEnemy();
};

#endif