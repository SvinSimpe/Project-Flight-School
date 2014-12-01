#ifndef PACKAGE_H
#define PACKAGE_H
#include "Structs.h"

enum class ContentType
{
	NEW_EVENT,
	OPERATION,
	MESSAGE,
};

struct Package
{
	struct Head
	{
		int index = 0;
		ContentType contentType = ContentType::OPERATION;
		int contentSize = 0;
	};

	struct Body
	{
		char* content = nullptr;
	};

	Head head = Head();
	Body body = Body();
};
#endif