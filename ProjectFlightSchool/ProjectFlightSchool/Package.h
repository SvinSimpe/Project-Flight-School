#ifndef PACKAGE_H
#define PACKAGE_H
#include "Structs.h"

enum class Net_Event
{
	ERROR_EVENT,
	MESSAGE,
	QUIT
};

template <typename T>
struct Package
{
	struct Head
	{
		int			index		= 0;
		Net_Event	eventType	= Net_Event::ERROR_EVENT;
		int			contentSize	= 0;
	};

	struct Body
	{
		T content;
	};

	Head head = Head();
	Body body = Body();
};
#endif