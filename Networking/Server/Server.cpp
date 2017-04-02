#include "Server.h"
// source includes
#include "GameMessages.h"


Server::Server()
{
}


Server::~Server()
{
}

void Server::run()
{
	// start up server and listen for clients
	std::cout << " Starting up server..." << std::endl;
	// initialise raknet peer interface
	m_pPeerInterface = RakNet::RakPeerInterface::GetInstance();
	// create socket descriptor for connection
	RakNet::SocketDescriptor sd(PORT, 0);
	// call startup with max of 32 connections on the assigned port
	m_pPeerInterface->Startup(MAXCON, &sd, 1);
	m_pPeerInterface->SetMaximumIncomingConnections(MAXCON);
	// server startup successful
	std::cout << "...server running:" << std::endl;

	// server run loop
	while (true)
	{
		update();
	}

}

void Server::update()
{
	// check any network messages
	handleNetworkMessages();
}

void Server::handleNetworkConnection()
{
}

void Server::handleNetworkMessages()
{
	RakNet::Packet* pPacket;

	// start thread and ping clients
	std::thread pingThread(sendClientPing, m_pPeerInterface);

	// loop for receiving messages
	for (pPacket = m_pPeerInterface->Receive();
		pPacket;
		m_pPeerInterface->DeallocatePacket(pPacket), pPacket = m_pPeerInterface->Receive()) 
	{
		switch (pPacket->data[0]) {
		case ID_NEW_INCOMING_CONNECTION:
			std::cout << "A connection is incoming...\n";
			break;
		case ID_DISCONNECTION_NOTIFICATION:
			std::cout << "A client has disconnected.\n";
			break;
		case ID_CONNECTION_LOST:
			std::cout << "A client lost the connection!!\n";
			break;
		//case ID_SERVER_TEXT_MESSAGE:
		//{
		//	// convert message to stream
		//	RakNet::BitStream bsIn(pPacket->data, pPacket->length, false);
		//	bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
		//	// read stream and print
		//	RakNet::RakString str;
		//	bsIn.Read(str);
		//	std::cout << str.C_String() << std::endl;
		//	break;
		//}
		case ID_CLIENT_TEXT_MESSAGE:
		{
			// convert message to stream
			RakNet::BitStream bsIn(pPacket->data, pPacket->length, false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			// read stream and print
			RakNet::RakString str;
			bsIn.Read(str);
			std::cout << "++ Message Recieved from Client..." << std::endl;
			std::cout << str.C_String() << std::endl;
			std::cout << "-- Broadcasting message to clients..." << std::endl;
			broadcastClientMessage(m_pPeerInterface, str.C_String());
			break;
		}
		default:
			std::cout << "recieved a message with an unknown ID: " << pPacket->data[0];
			break;
		}
	}
	// join ping thread
	pingThread.join();
}
