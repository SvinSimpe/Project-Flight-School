#ifndef CLIENT_H
#define CLIENT_H

#include "Connection.h"

class Client
{
	private:
		NetworkEventForwarder* mNEF;

	protected:
	public:
		static ClientSocketManager*	mSocketManager;

	private:
		void			InitEventListening();
		void			InitForwardingEvents();

	protected:
	public:
		bool			Initialize( std::string ip, unsigned int port );
		void			Release();
						Client();
		virtual			~Client();
};
#endif