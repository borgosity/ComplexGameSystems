// std lib includes
#include <iostream>
#include <string>
// dependency includes
//#include <RakPeerInterface.h>
//#include <MessageIdentifiers.h>
//#include <BitStream.h>
// source includes
#include "GameMessages.h"

void handleNetworkMessages() {

}

int main()
{
	const unsigned short PORT = 5456;
	int MAXCON = 32;

	RakNet::RakPeerInterface *  pPeerInterface = nullptr;
	RakNet::Packet	* pPacket = nullptr;

	// start up server and listen for clients
	std::cout << " Starting up server..." << std::endl;
	// initialise raknet peer interface
	pPeerInterface = RakNet::RakPeerInterface::GetInstance();
	// create socket descriptor for connection
	RakNet::SocketDescriptor sd(PORT,0);
	// call startup with max of 32 connections on the assigned port
	pPeerInterface->Startup(MAXCON, &sd, 1);
	pPeerInterface->SetMaximumIncomingConnections(MAXCON);

	// processing loop
	while (true) {
		// start thread and ping clients
		std::thread pingThread(sendClientPing, pPeerInterface);
		// loop for receiving messages
		for(pPacket = pPeerInterface->Receive(); 
			pPacket; 
			pPeerInterface->DeallocatePacket(pPacket), pPacket = pPeerInterface->Receive()){
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
			case ID_SERVER_TEXT_MESSAGE:
			{
				// convert message to stream
				RakNet::BitStream bsIn(pPacket->data, pPacket->length, false);
				bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				// read stream and print
				RakNet::RakString str;
				bsIn.Read(str);
				std::cout << str.C_String() << std::endl;
				break;
			}
			default:
				std::cout << "recieved a message with an unknown ID: " << pPacket->data[0];
				break;
			}
		}
		pingThread.join();
	}


	
	return 0;
}