#ifndef ENEMY_H
#define ENEMY_H

#include "Graphics.h"
#include "EventManager.h"
#include "Events.h"
#include "BoundingGeometry.h"
#include "RenderManager.h"
#include "Font.h"

#include <stdlib.h>
#include <time.h>

#define MAX_NR_OF_ENEMIES		10

#define ENEMY_ANIMATION_IDLE	0
#define ENEMY_ANIMATION_COUNT	1

class Enemy
{
	// Member variables
	private:
		unsigned int		mID;
		AssetID				mModel;
		AssetID				mAnimations[ENEMY_ANIMATION_COUNT];
		float				mAnimationTime;
		float				mCurrentHp;
		float				mMaxHp;
		bool				mIsAlive;
		XMFLOAT3			mPosition;
		XMFLOAT3			mDirection;
		float				mVelocity;

	protected:
	public:

	// Member functions
	private:
		virtual void		RemoteUpdate( IEventPtr newEvent );

	protected:
	public:
		virtual HRESULT		Update( float deltaTime );
		virtual HRESULT		Render();
		virtual void		Spawn( XMFLOAT3 spawnPos );
		virtual void		Die();
		unsigned int		GetID() const;
		void				SetID( unsigned int id );
		float				GetHP() const;
		void				SetHP( float hp );
		bool				IsAlive() const;
		void				SetIsAlive( bool isAlive );
		XMFLOAT3			GetPosition() const;
		void				SetPosition( XMFLOAT3 position );
		XMFLOAT3			GetDirection() const;
		void				SetDirection( XMFLOAT3 direction );
		virtual HRESULT		Initialize( int id );
		void				Release();
							Enemy();
		virtual				~Enemy();
};

#endif