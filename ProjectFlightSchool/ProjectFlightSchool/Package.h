#ifndef PACKAGE_H
#define PACKAGE_H
#include "Structs.h"

enum class Net_Event
{
	ERROR_EVENT,
	MESSAGE,
	EV_PLAYER_UPDATE,
	EV_PLAYER_JOINED,
	EV_PLAYER_LEFT,
	EV_PLAYER_DIED,
	EV_PLAYER_DAMAGED,
	EV_PLAYER_SPAWNED,
	EV_PLAYER_DOWN,
	EV_PLAYER_UP,
	EV_PLAYER_REVIVE,
	EV_PROJECTILE_FIRED,
	EV_UPDATE_HP,
	EV_ENEMY_CREATED,
	EV_ENEMY_UPDATE_POSITION,
	EV_SYNC_ENEMY,
	EV_SYNC_SPAWN,
	EV_ENEMY_LIST_SYNCED,
	EV_MELEE_HIT,
	EV_PLAYER_ATTACK,
	EV_SET_ENEMY_STATE,
	EV_SET_PLAYER_NAME,
	EV_ENEMY_PROJECTILE_DAMGAE,
	EV_ENEMY_ATTACK_PLAYER,
	EV_ENEMY_MELEE_HIT,
	YOUR_ID
};

template <typename T>
struct Package
{
	struct Head
	{
		int			index		= -1;
		Net_Event	eventType	= Net_Event::ERROR_EVENT;
		int			contentSize	= -1;
	};

	struct Body
	{
		T content;
	};

	Head head = Head();
	Body body = Body();
};
#endif