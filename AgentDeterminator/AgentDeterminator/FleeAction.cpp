#include "FleeAction.h"
#include "Agents.h"
#include <GLFW\glfw3.h>
#include <glm\gtx\norm.hpp>

FleeAction::FleeAction()
{
	m_actionType = AN_FLEE;
	controls.distance = 0.0f;
	controls.target = glm::vec3(0.0f);
	m_pTarget = nullptr;
}

FleeAction::FleeAction(Agent * a_target)
{
	m_pTarget = a_target;
	m_actionType = AN_FLEE;
	controls.target = a_target->movedata.position;
	controls.distance = 0.0f;
}



FleeAction::~FleeAction()
{
}


void FleeAction::update(float a_dt, Agent & a_agent)
{
	if (m_pTarget != nullptr)
	{
		// subtract target position + targets velocity from agent postion
		// scale resultant vector by maxSpeed
		// calculate the acceleration required to move agent away to targets estimated location
		glm::vec3 acceleration = (glm::normalize(a_agent.movedata.position - m_pTarget->movedata.position)
													* a_agent.movedata.maxSpeed) - a_agent.movedata.velocity;

		// limit acceleration
		if (glm::length(acceleration) > a_agent.movedata.maxAcceleration)
		{
			acceleration = glm::normalize(acceleration) * a_agent.movedata.maxAcceleration;
			a_agent.movedata.acceleration = acceleration;
		}

		// Apply accleration to agent
		a_agent.movedata.velocity += acceleration * a_dt;
		a_agent.movedata.position += a_agent.movedata.velocity * a_dt;
	}
	else
	{
		std::cout << "ERROR :: Flee Action - target agent is nullptr" << std::endl;
	}
}




