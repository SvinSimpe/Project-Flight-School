#ifndef PACKAGE_H
#define PACKAGE_H
#include "Structs.h"

enum class EventType
{
	ERROR_EVENT,
	MESSAGE,
	POSITION
};

template <typename T>
struct Package
{
	struct Head
	{
		int			index		= 0;
		EventType	eventType	= EventType::ERROR_EVENT;
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