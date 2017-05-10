#include "AttackAction.h"
#include "Agents.h"
#include <GLFW\glfw3.h>
#include <glm\gtx\norm.hpp>
#include <algorithm>

AttackAction::AttackAction()
{
	m_actionType = AN_ATTACK;
	controls.target = glm::vec3(0.0f);
	controls.distance = 0.0f;
	m_pTarget = nullptr;
}

AttackAction::AttackAction(Agent * a_target)
{
	m_pTarget = a_target;
	m_actionType = AN_ATTACK;
	controls.target = a_target->movedata.position;
	controls.distance = a_target->vitals.mass * 0.5f;
	m_arrived = false;
}



AttackAction::~AttackAction()
{
}


void AttackAction::update(float a_dt, Agent & a_agent)
{
	if (m_pTarget != nullptr)
	{
		//if (!m_arrived)
		//{
		//	arrive(a_dt, a_agent);
		//}
		//else
		//{
			// do some attack stuff
			float attackStrength = a_agent.vitals.strength + a_agent.vitals.health;
			float oppenentStrength = m_pTarget->vitals.strength + m_pTarget->vitals.health;
			bool inRange = (a_agent.vitals.foeDistance <= a_agent.movedata.sight);

			// ## if opponent not attacking agent gets first strike
			// ## else - random chance at first strike?


			if (!m_pTarget->vitals.dead && inRange) {
				// if as tough of tough do full damage, else half
				if (attackStrength >= oppenentStrength){
					m_pTarget->vitals.health -= a_agent.vitals.mass / 100;
				}
				else {
					m_pTarget->vitals.health -= (a_agent.vitals.mass * 0.5f) / 100;
				}
			}
		//}
	}
	else
	{
		std::cout << "ERROR :: Attack Action - target agent is nullptr" << std::endl;
	}
}

void AttackAction::arrive(float a_dt, Agent & a_agent)
{
	controls.target = m_pTarget->movedata.position;
	float velocity = glm::distance(a_agent.movedata.velocity, glm::vec3(0.0f));
	if (velocity > 0.05)
	{
		// subtract agent postion from target position + targets velocity 
		a_agent.movedata.heading = glm::normalize(controls.target - a_agent.movedata.position);
		// scale resultant vector by maxSpeed
		// calculate the acceleration required to move agent away to targets estimated location
		glm::vec3 acceleration = glm::normalize(a_agent.movedata.heading) * a_agent.movedata.maxSpeed;

		// get distance to target
		float distance = glm::distance(a_agent.movedata.position, controls.target);
		// distance scalar
		float scalar = std::min(distance / controls.distance, 1.0f);

		// if within radius
		if (scalar < 1)
		{
			acceleration *= scalar;

			float d = glm::dot(a_agent.movedata.heading, glm::normalize(a_agent.movedata.velocity));

			glm::vec3 resistance = -(glm::normalize(a_agent.movedata.velocity)) * glm::length(a_agent.movedata.velocity) * d * 2.0f;

			acceleration += resistance;

		}
		acceleration -= a_agent.movedata.velocity;
		// set acceleration
		a_agent.movedata.acceleration = acceleration;

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
		// adjust agents position
		a_agent.movedata.position += a_agent.movedata.velocity * a_dt;
	}
	else
	{
		m_arrived = true;
	}
}




