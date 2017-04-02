#include "Client.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include "GameMessages.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

Client::Client() {

}

Client::~Client() {
}
/***************************************************************************************************/
bool Client::startup() {
	
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// create simple camera transforms
	m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
										  getWindowWidth() / (float)getWindowHeight(),
										  0.1f, 1000.f);
	// connect to server
	handleNetworkConnection();

	return true;
}
/***************************************************************************************************/
void Client::shutdown() {

	Gizmos::destroy();
}
/*******************************************************************************************
                                         UPDATE
********************************************************************************************/
void Client::update(float deltaTime) {

	// query time since application started
	float time = getTime();

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	
	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	// start thread and send message to other clients
	std::thread messageThread(sendServerMessage, m_pPeerInterface,"-!! Hello Clients !!-\n ");

	// update network messages
	handleNetworkMessages();

	// join thread
	messageThread.join();

}
/*******************************************************************************************
                                        DRAW
********************************************************************************************/
void Client::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// update perspective in case window resized
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
										  getWindowWidth() / (float)getWindowHeight(),
										  0.1f, 1000.f);

	Gizmos::draw(m_projectionMatrix * m_viewMatrix);
}
/***************************************************************************************************/
void Client::handleNetworkConnection()
{
	// initialise raknet peer interface
	m_pPeerInterface = RakNet::RakPeerInterface::GetInstance();
	initialiseClientConnection();
}
/***************************************************************************************************/
void Client::initialiseClientConnection()
{
	// create socket descriptor for connection, no data required for server connection
	RakNet::SocketDescriptor sd;

	// call startup with max of 1 connection to the server
	m_pPeerInterface->Startup(1, &sd, 1);
	std::cout << " Connecting to server at: " << IP << std::endl;
	// attempt connection to server
	RakNet::ConnectionAttemptResult conResult = m_pPeerInterface->Connect(IP, PORT, nullptr, 0);
	// check if connection was successful
	if (conResult != RakNet::CONNECTION_ATTEMPT_STARTED) {
		std::cout << "  >> ERROR - Client unable to start connection, Error number : " << conResult << std::endl;
	}
}
/***************************************************************************************************/
void Client::handleNetworkMessages()
{
	RakNet::Packet* pPacket;

	for ( pPacket = m_pPeerInterface->Receive(); 
		 pPacket;
		m_pPeerInterface->DeallocatePacket(pPacket),	pPacket = m_pPeerInterface->Receive()) 
	{
		switch (pPacket->data[0]) {
		case ID_REMOTE_DISCONNECTION_NOTIFICATION:
			std::cout << "Another client has disconnected.\n";
			break;
		case ID_REMOTE_CONNECTION_LOST:
			std::cout << "Another client has lost the connection!!\n";
			break;
		case ID_REMOTE_NEW_INCOMING_CONNECTION:
			std::cout << "Another client has connected.\n";
			break;
		case ID_CONNECTION_REQUEST_ACCEPTED:
			std::cout << "Our connection request has been accepted.\n";
			break;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			std::cout << "The server is full.\n";
			break;
		case ID_DISCONNECTION_NOTIFICATION:
			std::cout << "We have been disconnected.\n";
			break;
		case ID_CONNECTION_LOST:
			std::cout << "Connection lost.\n";
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
			std::cout << "Received a message with a unknown ID: " << pPacket->data[0];
			break;
		}
	}
}

