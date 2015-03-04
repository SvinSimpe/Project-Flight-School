#ifndef ENEMYSPAWN_H
#define ENEMYSPAWN_H

#include "Graphics.h"
#include "Events.h"
#include "RenderManager.h"


#define MAX_NR_OF_ENEMY_SPAWNERS	200


class EnemySpawn
{
	private:
		unsigned int		mID;
		AssetID				mModel;
		XMFLOAT3			mPosition;
		XMFLOAT3			mSpawnPosition;

	protected:
	public:
		
	private:
	protected:
	public:
		HRESULT			Update( float deltaTime );
		HRESULT			Render();

		unsigned int	GetID() const;
		XMFLOAT3		GetPosition() const;
		void			SetPosition( XMFLOAT3 position );
		XMFLOAT3		GetSpawnPosition() const;
		void			SetSpawnPosition( XMFLOAT3 position );

		HRESULT			Initialize( unsigned int id );
		void			Release();
						EnemySpawn();
						~EnemySpawn();
};

#endif	