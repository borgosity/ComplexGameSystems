#include "Client.h"
#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
//#include <iostream>
#include "GameMessages.h"

#include "imgui_glfw3.h"

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
	// set msg input to false until server connection
	m_readOnly = true;
	m_newMsg = false;
	m_sendMessage = false;
	m_maxMsg = 10;
	m_bufferSize = 0;
	m_msgBuffer[0] = '\0';
	m_conversation[0] = '\0';
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
	//std::thread messageThread(sendServerMessage, m_pPeerInterface, *m_serverAddress, "-!! Hello Clients !!-\n ");

	// update network messages
	handleNetworkMessages();

	// join thread
	//messageThread.join();

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
	// draw chat window
	chatGUI();
}
/***************************************************************************************************/
void Client::handleNetworkConnection()
{
	// initialise raknet peer interface
	m_pPeerInterface = RakNet::RakPeerInterface::GetInstance();
	m_serverAddress = new RakNet::SystemAddress(SERVERIP, PORT);
	initialiseClientConnection();
}
/***************************************************************************************************/
void Client::initialiseClientConnection()
{
	// create socket descriptor for connection, no data required for server connection
	RakNet::SocketDescriptor sd;

	// call startup with max of 1 connection to the server
	m_pPeerInterface->Startup(1, &sd, 1);
	addMessage("> Connecting to server at: " + std::string(SERVERIP) );
	std::cout << "> Connecting to server at: " << SERVERIP << std::endl;
	// attempt connection to server
	RakNet::ConnectionAttemptResult conResult = m_pPeerInterface->Connect(SERVERIP, PORT, nullptr, 0);
	// get server ID
	std::cout << "> MYGUID : " << m_pPeerInterface->GetMyGUID().ToString() << std::endl;
	std::cout << "> System Address : " << m_pPeerInterface->GetSystemAddressFromGuid(m_pPeerInterface->GetMyGUID()).ToString() << std::endl;
	
	addMessage("> MYGUID : " + std::string(m_pPeerInterface->GetMyGUID().ToString()));
	addMessage("> System Address : " + std::string(m_pPeerInterface->GetSystemAddressFromGuid(m_pPeerInterface->GetMyGUID()).ToString()));

	// check if connection was successful
	if (conResult != RakNet::CONNECTION_ATTEMPT_STARTED) {
		std::cout << "  >> ERROR - Client unable to start connection, Error number : " << conResult << std::endl;
		addMessage(">> ERROR - Client unable to start connection, Error number : " + std::string((char*)conResult));
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
			addMessage("> Connection request has been accepted.");
			break;
		case ID_NO_FREE_INCOMING_CONNECTIONS:
			std::cout << "The server is full.\n";
			addMessage("> The server is full.");
			break;
		case ID_DISCONNECTION_NOTIFICATION:
			std::cout << "We have been disconnected.\n";
			addMessage("> We have been disconnected.");
			break;
		case ID_CONNECTION_LOST:
			std::cout << "Connection lost.\n";
			addMessage("> Connection has been lost.");
			initialiseClientConnection();
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
			addMessage(std::string(str));
			break;
		}
		case ID_CLIENT_TEXT_MESSAGE:
		{
			// convert message to stream
			RakNet::BitStream bsIn(pPacket->data, pPacket->length, false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			// read stream and print
			RakNet::RakString str;
			bsIn.Read(str);
			std::cout << str.C_String() << std::endl;
			addMessage(std::string(str));
			break;
		}
		default:
			// convert message to stream
			RakNet::BitStream bsIn(pPacket->data, pPacket->length, false);
			bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
			// read stream and print
			RakNet::RakString str;
			bsIn.Read(str);
			std::cout << "Received a message with a unknown ID: " << str.C_String() << pPacket->data[0];
			break;
		}
	}
}

void Client::chatGUI()
{

	// write message to window if there is a new message
	if (m_newMsg) {
		for (auto msgit : m_messages)
		{
			std::string msg = msgit;
			unsigned int size = 1024 * 16 - msg.length();
			if (m_newMsg &&  m_bufferSize < size) {
				strcat_s(m_conversation, msg.c_str());
			}
		}
		m_messages.clear();
		m_newMsg = false;
	}
	// create chat window
	ImGui::Begin("Chat");
	// conversation area
	//ImGui::TextWrapped(text);
	ImGui::InputTextMultiline("##source", m_conversation, 1024 * 16, ImVec2(-1.0f, ImGui::GetTextLineHeight() * 16),
							  ImGuiInputTextFlags_AllowTabInput | 
							  ImGuiInputTextFlags_ReadOnly);
	
	// message input text field
	!m_sendMessage ? m_sendMessage = ImGui::InputText("##msg",m_msgBuffer, 512, ImGuiInputTextFlags_EnterReturnsTrue) : ImGui::InputText("##msg", m_msgBuffer, 512, ImGuiInputTextFlags_EnterReturnsTrue);
	// set focus if nothing is in focus
	if (!ImGui::IsAnyItemActive()) {
		ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget
	}
	ImGui::SameLine(0.0f, -1.0f);
	// send button
	!m_sendMessage ? m_sendMessage = ImGui::Button("Send", ImVec2(40, 20)) : ImGui::Button("Send", ImVec2(40, 20));
	// debug text
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::End();
	// send message if there is one
	if (m_sendMessage) {
		m_currentMessage = m_msgBuffer;
		m_msgBuffer[0] = '\0';
		m_sendMessage = false;
		sendMessage();
	}
}

void Client::addMessage(std::string a_msg)
{
	if (m_messages.size() < m_maxMsg) {
		m_messages.push_back(a_msg + "\n");
	}
	else {
		m_messages.pop_front();
		m_messages.push_back(a_msg + "\n");
	}
	m_bufferSize += a_msg.length();
	m_newMsg = true;
}

void Client::sendMessage()
{
	//std::thread messageThread(sendServerMessage, m_pPeerInterface, *m_serverAddress, m_currentMessage);
	sendServerMessage(m_pPeerInterface, *m_serverAddress, m_currentMessage);
	//messageThread.join();
}

