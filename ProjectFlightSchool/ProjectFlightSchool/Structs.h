#ifndef STRUCTS_H
#define STRUCTS_H

struct Empty // Used for sends where no actual package needs to be sent
{
};

struct Message
{
	char* msg = "";
};

struct Position
{
	int x = 0;
	int y = 0;
	int z = 0;
};
#endif