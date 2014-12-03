#ifndef PLAYER_H
#define PLAYER_H

#include "Graphics.h"
#include "EventManager.h"

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
		void HandleInput();

	public:
		HRESULT Update( float deltaTime );
		HRESULT Render( float deltaTime );

		HRESULT Initialize();
		void Release();
		Player();
		~Player();
};

#endif

