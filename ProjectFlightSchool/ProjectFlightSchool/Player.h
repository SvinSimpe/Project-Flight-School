#ifndef PLAYER_H
#define PLAYER_H

#include "Graphics.h"
#include "EventManager.h"
#include "Input.h"

struct UpperBody
{
	UINT		playerModel;
	XMFLOAT2	direction;
	XMFLOAT3	position;
};

struct LowerBody
{
	UINT		playerModel;
	XMFLOAT3	position;
	float		speed;
};

class Player
{
	private:
		UpperBody	mUpperBody;
		LowerBody	mLowerBody;

	private:
		void HandleInput( float deltaTime );
		void Move( float speed, XMFLOAT2 direction );
		void LookAt( float rotation );

	public:
		HRESULT Update( float deltaTime );
		HRESULT Render( float deltaTime );

		HRESULT Initialize();
		void Release();
		Player();
		~Player();
};

#endif

