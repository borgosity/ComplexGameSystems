#pragma once
// source includes
#include "Application.h"
// opengl includes
#include <glm/mat4x4.hpp>
// raknet includes
#include <RakPeerInterface.h>
#include <MessageIdentifiers.h>
#include <BitStream.h>

class Client : public aie::Application {
public:

	Client();
	virtual ~Client();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	// initialise the connection
	void handleNetworkConnection();
	void initialiseClientConnection();

	// handle incoming packets
	void handleNetworkMessages();

protected:
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;
	// network variables
	RakNet::RakPeerInterface * m_pPeerInterface = nullptr;
	const char * SERVERIP = "127.0.0.1";
	const unsigned short PORT = 5456;
	RakNet::SystemAddress * m_serverAddress = nullptr;
};