#include "Network.h"

BaseSocketManager *gpBaseSocketManager;

int main()
{
	BaseSocketManager *gBaseSocketManager = new BaseSocketManager();


	gBaseSocketManager->Init();
	gBaseSocketManager->DoSelect(0);

	return 0;
}