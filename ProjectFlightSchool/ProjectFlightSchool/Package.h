#ifndef PACKAGE_H
#define PACKAGE_H
#include "Body.h"
#include "Head.h"

// This class is used to store information to be sent over the network
class Package
{
	private:
		Head mHead; // Stores the operation ID, socket ID and the size of content
		Body mBody; // Stores the actual content of the package

	protected:
	public:

	private:
	protected:
	public:
		bool	Initialize(Head head, Body body);
		void	Release();
				Package();
		virtual ~Package();
};
#endif