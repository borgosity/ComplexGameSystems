#include "SceneController.h"



SceneController::SceneController()
{
}


SceneController::~SceneController()
{
}

void SceneController::update(float a_dt)
{
	for (auto agent : m_agentObjects)
	{
		agent->update(a_dt);
	}
}

void SceneController::addAgent(Agent * a_newAgent)
{
	a_newAgent->m_agents = &m_agentObjects;
	m_agentObjects.push_back(a_newAgent);
}
