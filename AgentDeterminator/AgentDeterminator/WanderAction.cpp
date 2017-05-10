#include "WanderAction.h"
#include "Agents.h"
#include <GLFW\glfw3.h>
#include <glm\gtx\norm.hpp>
#include <algorithm>


WanderAction::WanderAction()
{
	m_actionType = AN_WANDER;
	controls.radius = 50.0f;
	controls.jitter = 0.25f;
	controls.distance = (float)(rand() % 75 + 1);
	controls.target = glm::vec3((float)(rand() % 800), (float)(rand() % 600), 0.0f);
	controls.wanderAngle = (float)(rand() % 360 + 1);
	m_arrived = false;
}

WanderAction::WanderAction(float radius, float jitter, float distance)
{
	m_actionType = AN_WANDER;
	controls.radius = radius;
	controls.jitter = jitter;
	controls.distance = distance;
	controls.target = glm::vec3((float)(rand() % 800), (float)(rand() % 600), 0.0f);
	controls.wanderAngle = (float)(rand() % 360 + 1);
	m_arrived = false;
}

WanderAction::~WanderAction()
{
}


void WanderAction::update(float a_dt, Agent & a_agent)
{
	// check if it is time to change to a new position
	if ((glfwGetTime() - a_agent.movedata.prevTime) > a_agent.movedata.livelyness || (glfwGetTime() - a_agent.movedata.prevTime) < 0)
	{
		a_agent.movedata.prevTime = (float)glfwGetTime();
		wander(a_dt, a_agent.movedata);
	}
	arrive(a_dt, a_agent);

	//// subtract agent postion from target position + targets velocity 
	//a_agent.movedata.heading = glm::normalize(controls.target - a_agent.movedata.position);
	//// scale vector by maxSpeed
	//// calculate the acceleration required to move agent to target (distance minus current velocity)
	//glm::vec3 acceleration = (a_agent.movedata.heading * a_agent.movedata.maxSpeed) - a_agent.movedata.velocity;

	//if (glm::length(acceleration) > a_agent.movedata.maxAcceleration)
	//{
	//	acceleration = glm::normalize(acceleration) * a_agent.movedata.maxAcceleration;
	//}
	//// set acceleration
	//a_agent.movedata.acceleration = acceleration;
	//// adjust velocity based on agent mass
	//if (a_agent.vitals.mass > 0)
	//{
	//	// Apply accleration to agent
	//	a_agent.movedata.velocity += (acceleration * a_agent.vitals.mass) * a_dt;
	//}
	//else
	//{
	//	// Apply accleration to agent
	//	a_agent.movedata.velocity += acceleration * a_dt;
	//}

	//// adjust agent position accordingly
	//a_agent.movedata.position += a_agent.movedata.velocity * a_dt;
}

void WanderAction::setAngle(glm::vec2 & a_vec2, float a_wanderAngle)
{
	float length = glm::length2(a_vec2);
	a_vec2.x = cos(a_wanderAngle) * length;
	a_vec2.y = sin(a_wanderAngle) * length;
}

void WanderAction::randomTarget()
{
	controls.target.x = controls.radius*cos(controls.wanderAngle) + controls.circleCentre.x;
	controls.target.y = controls.radius*sin(controls.wanderAngle) + controls.circleCentre.y;
}

void WanderAction::wander(float a_dt, MovementInfo & a_movedata)
{
	// reset velocity
	//a_movedata.velocity = glm::vec3(0.0f);

	// specify the location of the circle centre based on the agents current velocity
	controls.circleCentre = a_movedata.position;
	// record current position of agent
	controls.prevLoc = a_movedata.position;
	// then normalise centre location 
	controls.circleCentre = glm::normalize(controls.circleCentre);
	// move(scale) it to the specified distance infront of the agent
	controls.circleCentre.x = controls.distance*cos(a_movedata.rotation) + a_movedata.position.x;
	controls.circleCentre.y = controls.distance*sin(a_movedata.rotation) + a_movedata.position.y;

	// find random spot on circle
	randomTarget();

	// change wanderAngle for next frame
	controls.wanderAngle += ((float)(rand() % 360 + 1) * controls.jitter) - (controls.jitter * 0.5f);
}

void WanderAction::arrive(float a_dt, Agent & a_agent)
{

	// subtract agent postion from target position + targets velocity 
	a_agent.movedata.heading = glm::normalize(controls.target - a_agent.movedata.position);
	// scale resultant vector by maxSpeed
	// calculate the acceleration required to move agent away to targets estimated location
	glm::vec3 acceleration = glm::normalize(a_agent.movedata.heading) * a_agent.movedata.maxSpeed;

	// get distance to target
	float distance = glm::distance(a_agent.movedata.position, controls.target);
	// distance scalar
	float scalar = std::min(distance / a_agent.movedata.sight, 1.0f);

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



