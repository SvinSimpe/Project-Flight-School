#ifndef PACKAGE_H
#define PACKAGE_H
#include "Structs.h"

enum class Net_Event
{
	ERROR_EVENT,
	MESSAGE,
	EV_PLAYER_MOVED,
	EV_PLAYER_JOINED,
	EV_PLAYER_LEFT,
	EV_PLAYER_DIED,
	EV_PLAYER_DAMAGED,
	EV_PLAYER_SPAWNED,
	EV_PROJECTILE_FIRED,
	EV_MELEE_HIT,
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