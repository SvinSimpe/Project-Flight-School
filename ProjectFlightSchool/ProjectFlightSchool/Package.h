#ifndef PACKAGE_H
#define PACKAGE_H
#include "Structs.h"

enum class ContentType
{
	OPERATION,
	MESSAGE,
	POSITION
};

struct PackageType
{
	ContentType type = ContentType::OPERATION;
};

template <typename T>
struct Package
{
	struct Head
	{
		int			index		= 0;
		ContentType contentType	= ContentType::OPERATION;
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