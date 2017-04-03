#pragma once
// std library includes
#include <thread>
#include <string>
// raknet includes
#include <RakPeerInterface.h>
#include <MessageIdentifiers.h>
#include <BitStream.h>

enum GameMessages
{
	ID_SERVER_TEXT_MESSAGE = ID_USER_PACKET_ENUM + 1,
	ID_CLIENT_TEXT_MESSAGE = ID_USER_PACKET_ENUM + 2
};

void sendClientPing(RakNet::RakPeerInterface * a_pPeerInterface)
{
	// construct message
	RakNet::BitStream bs;
	bs.Write((RakNet::MessageID)GameMessages::ID_SERVER_TEXT_MESSAGE);
	bs.Write("Ping!");
	// send message
	a_pPeerInterface->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
	std::this_thread::sleep_for(std::chrono::seconds(2));
}

void broadcastClientMessage(RakNet::RakPeerInterface * a_pPeerInterface, std::string a_message)
{
	// construct message
	RakNet::BitStream bs;
	bs.Write((RakNet::MessageID)GameMessages::ID_CLIENT_TEXT_MESSAGE);
	bs.Write(a_message.c_str());
	std::cout << "-- Broadcasting message to clients: " << a_message << std::endl;
	// send message
	a_pPeerInterface->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void sendServerMessage(RakNet::RakPeerInterface * a_pPeerInterface, RakNet::SystemAddress &a_server, std::string a_message)
{
	// construct message
	RakNet::BitStream bs;
	bs.Write((RakNet::MessageID)GameMessages::ID_CLIENT_TEXT_MESSAGE);
	bs.Write(a_message.c_str());
	std::cout << "> Send message to server: " << a_message << std::endl;
	// send message
	a_pPeerInterface->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, a_server, false);
	std::this_thread::sleep_for(std::chrono::seconds(2));
}