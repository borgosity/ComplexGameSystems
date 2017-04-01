#pragma once
// std library includes
#include <iostream>
#include <string>

// raknet includes
#include <RakPeerInterface.h>


class Server
{
public:
	Server();
	~Server();

	void run();
	void update();
	
	void handleNetworkConnection();
	void handleNetworkMessages();

private:
	RakNet::RakPeerInterface * m_pPeerInterface = nullptr;

	const unsigned short PORT = 5456;
	const int MAXCON = 32;
};

