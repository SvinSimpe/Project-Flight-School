#ifndef REMOTEENEMY_H
#define REMOTEENEMY_H

#include "Graphics.h"
#include "RenderManager.h"
#include "Enemy.h"

class RemoteEnemy
{
	private:
		unsigned int		mID;
		AssetID				mModel;
		AssetID				mAnimation;
		XMFLOAT3			mPosition;
		XMFLOAT3			mDirection;
		float				mAnimationTime;
		bool				mIsSynced;

	protected:
	public:

	private:
	protected:
	public:
		HRESULT				Update( float deltaTime );
		HRESULT				Render();
		void				SetID( unsigned int id );
		void				SetModelID( AssetID model );
		void				SetAnimation( AssetID animation );
		void				SetPosition( XMFLOAT3 position );
		void				SetDirection( XMFLOAT3 direction );
		void				SetSynced( bool isSynced );
		bool				IsSynced() const;
		virtual HRESULT		Initialize( int id );
		void				Release();
							RemoteEnemy();
		virtual				~RemoteEnemy();
};

#endif