#include "SeekAction.h"
#include "Agents.h"
#include <GLFW\glfw3.h>
#include <glm\gtx\norm.hpp>

SeekAction::SeekAction()
{
	m_actionType = AN_SEEK;
	controls.distance = 0.0f;
	controls.target = glm::vec3(0.0f);
	m_pTarget = nullptr;
}

SeekAction::SeekAction(Agent * a_target)
{
	m_pTarget = a_target;
	m_actionType = AN_SEEK;
	controls.target = a_target->movedata.position;
	controls.distance = 0.0f;
}



SeekAction::~SeekAction()
{
}


void SeekAction::update(float a_dt, Agent & a_agent)
{
	if (m_pTarget != nullptr)
	{
		// vector from agent to target
		a_agent.movedata.heading = glm::normalize(m_pTarget->movedata.position - a_agent.movedata.position);
		// scale vector by maxSpeed
		// calculate the acceleration required to move agent to target (distance minus current velocity)
		glm::vec3 acceleration = (a_agent.movedata.heading * a_agent.movedata.maxSpeed) - a_agent.movedata.velocity;

		if (glm::length(acceleration) > a_agent.movedata.maxAcceleration)
		{
			acceleration = glm::normalize(acceleration) * a_agent.movedata.maxAcceleration;
		}
		// set acceleration
		a_agent.movedata.acceleration = acceleration;
		// adjust velocity based on agent mass
		if (a_agent.vitals.mass > 0)
		{
			// Apply accleration to agent
			a_agent.movedata.velocity += (acceleration * a_agent.vitals.mass) * a_dt;
		}
		else
		{
			// Apply accleration to agent
			a_agent.movedata.velocity += acceleration * a_dt;
		}

		// Apply accleration to agent
		//a_agent.movedata.velocity += acceleration * a_dt;
		// adjust agent position accordingly
		a_agent.movedata.position += a_agent.movedata.velocity * a_dt;
	}
	else
	{
		std::cout << "ERROR :: Seek Action - target agent is nullptr" << std::endl;
	}
}




