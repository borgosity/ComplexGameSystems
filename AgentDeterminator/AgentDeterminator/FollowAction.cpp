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
		glm::vec2 target(m_pTarget->movedata.position.x, m_pTarget->movedata.position.y);
		glm::vec2 position(a_agent.movedata.position.x, a_agent.movedata.position.y);

		glm::vec2 direction = target - position;
		direction = glm::normalize(direction);

		a_agent.movedata.acceleration.x = direction.x * a_agent.movedata.maxSpeed;
		a_agent.movedata.acceleration.y = direction.y * a_agent.movedata.maxSpeed;

		glm::vec2 accel(a_agent.movedata.acceleration.x, a_agent.movedata.acceleration.y);
		if (glm::length(accel) > a_agent.movedata.maxForce)
		{
			accel = glm::normalize(accel) * a_agent.movedata.maxForce;
			a_agent.movedata.acceleration.x = accel.x;
			a_agent.movedata.acceleration.y = accel.y;
		}
	}
	else
	{
		std::cout << "ERROR :: Follow Action - target agent is nullptr" << std::endl;
	}
}




