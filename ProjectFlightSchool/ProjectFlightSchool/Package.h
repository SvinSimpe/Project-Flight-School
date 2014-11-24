#ifndef PACKAGE_H
#define PACKAGE_H

enum class Operation
{
	INVALID_OPERATION, // Used to initialize operations
	MESSAGE
};

struct Head
{
	Operation op;
	int size;
};

struct Body
{
	char* content;
};

struct Package
{
	Head head;
	Body body;
};
#endif