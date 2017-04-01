#pragma once
// source includes
#include "Server.h"


int main()
{
	auto server = new Server();
	server->run();
	delete server;
	
	return 0;
}