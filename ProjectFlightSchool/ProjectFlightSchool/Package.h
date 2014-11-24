#ifndef PACKAGE_H
#define PACKAGE_H

enum class Operation
{
	INVALID_OPERATION, // Used to initialize operations
	MESSAGE
};

struct Package
{
	struct Head
	{
		Operation op = Operation::INVALID_OPERATION;
		int size = 0;
	};

	struct Body
	{
		char* content = nullptr;
	};

	Head head = Head();
	Body body = Body();
};
#endif