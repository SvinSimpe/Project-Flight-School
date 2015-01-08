#ifndef REMOTEPLAYER_H
#define REMOTEPLAYER_H

#include "Graphics.h"
#include "EventManager.h"
#include "Events.h"

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
		UpperBody		mUpperBody;
		LowerBody		mLowerBody;
		float			mCurrentHp;
		float			mMaxHp;
		bool			mIsAlive;

	public:

	// Member functions
	private:
		void		RemoteUpdate( IEventPtr newEvent );

	protected:
		void		LookAt( float rotation );

	public:
		void			RemoteInit( unsigned int id );
		virtual void	Die();
		int				GetID() const;
		virtual HRESULT	Render( float deltaTime );
		virtual HRESULT	Initialize();
		void			Release();
						RemotePlayer();
		virtual			~RemotePlayer();
};

#endif

