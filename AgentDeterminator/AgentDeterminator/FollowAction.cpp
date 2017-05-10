#include "FollowAction.h"
#include "Agents.h"
#include <GLFW\glfw3.h>
#include <glm\gtx\norm.hpp>

FollowAction::FollowAction()
{
	m_actionType = AN_FOLLOW;
	controls.distance = 0.0f;
	controls.target = glm::vec3(0.0f);
	m_pTarget = nullptr;
}

FollowAction::FollowAction(Agent * a_target)
{
	m_pTarget = a_target;
	m_actionType = AN_FOLLOW;
	controls.target = a_target->movedata.position;
	controls.distance = 0.0f;
}



FollowAction::~FollowAction()
{
}


void FollowAction::update(float a_dt, Agent & a_agent)
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
	}
	else
	{
		std::cout << "ERROR :: Follow Action - target agent is nullptr" << std::endl;
	}
}




