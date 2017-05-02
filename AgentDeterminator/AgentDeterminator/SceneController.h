#pragma once
#include <vector>
#include "Agents.h"

class SceneController
{
public:
	SceneController();
	~SceneController();
	
	std::vector<Agent*> m_agentObjects;

	void update(float a_dt);
	void addAgent(Agent * a_newAgent);

private:

};

