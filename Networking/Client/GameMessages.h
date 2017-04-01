#pragma once
#include <thread>

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
	while (true)
	{
		// construct message
		RakNet::BitStream bs;
		bs.Write((RakNet::MessageID)GameMessages::ID_SERVER_TEXT_MESSAGE);
		bs.Write("Ping!");
		// send message
		a_pPeerInterface->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

void broadcastClientMessage(RakNet::RakPeerInterface * a_pPeerInterface)
{
	// construct message
	RakNet::BitStream bs;
	bs.Write((RakNet::MessageID)GameMessages::ID_CLIENT_TEXT_MESSAGE);
	bs.Write("Ping!");
	// send message
	a_pPeerInterface->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}

void sendServerMessage(RakNet::RakPeerInterface * a_pPeerInterface)
{
	// construct message
	RakNet::BitStream bs;
	bs.Write((RakNet::MessageID)GameMessages::ID_CLIENT_TEXT_MESSAGE);
	bs.Write("Here is a message sent from a client");
	// send message
	a_pPeerInterface->Send(&bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true);
}