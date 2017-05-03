#include "WanderAction.h"
#include "Agents.h"
#include <GLFW\glfw3.h>
#include <glm\gtx\norm.hpp>

WanderAction::WanderAction()
{
	m_actionType = AN_WANDER;
	m_fRadius = 50.0f;
	m_fJitter = 0.25f;
	m_fDistance = (float)(rand() % 75 + 1);
	m_vTarget.x = (float)(rand() % 800);
	m_vTarget.y = (float)(rand() % 600);
	m_fWanderAngle = (float)(rand() % 360 + 1);
}

WanderAction::WanderAction(float radius, float jitter, float distance)
{
	m_actionType = AN_WANDER;
	m_fRadius = radius;
	m_fJitter = jitter;
	m_fDistance = distance;
	m_vTarget.x = (float)(rand() % 800);
	m_vTarget.y = (float)(rand() % 600);
	m_fWanderAngle = (float)(rand() % 360 + 1);
}

WanderAction::~WanderAction()
{
}


void WanderAction::update(float a_dt, Agent & a_agent)
{
	if ((glfwGetTime() - a_agent.movedata.prevTime) > a_agent.movedata.livelyness || (glfwGetTime() - a_agent.movedata.prevTime) < 0)
	{
		a_agent.movedata.prevTime = (float)glfwGetTime();
		wander1(a_dt, a_agent.movedata);
		//wander2(deltaTime, behaviour);
	}

	glm::vec3 acceleration = (glm::normalize(m_vTarget - a_agent.movedata.position) * a_agent.movedata.maxSpeed) - a_agent.movedata.velocity;

	if (glm::length(acceleration) > a_agent.movedata.maxAcceleration)
	{
		acceleration = glm::normalize(acceleration) * a_agent.movedata.maxAcceleration;
		a_agent.movedata.acceleration = acceleration;
	}

	// Apply accleration to agent
	a_agent.movedata.velocity += acceleration * a_dt;
	a_agent.movedata.position += a_agent.movedata.velocity * a_dt;
}

void WanderAction::setAngle(glm::vec2 & a_vec2, float a_wanderAngle)
{
	float length = glm::length2(a_vec2);
	a_vec2.x = cos(a_wanderAngle) * length;
	a_vec2.y = sin(a_wanderAngle) * length;
}

void WanderAction::randomTarget()
{
	m_vTarget.x = m_fRadius*cos(m_fWanderAngle) + m_vCircleCentre.x;
	m_vTarget.y = m_fRadius*sin(m_fWanderAngle) + m_vCircleCentre.y;
}

void WanderAction::wander1(float a_dt, MovementInfo & a_movedata)
{
	// reset velocity
	a_movedata.velocity.x = 0;
	a_movedata.velocity.y = 0;

	// specify the location of the circle centre based on the agents current velocity
	// then normalise it and move(scale) it to the specified distance infront of the agent
	m_vCircleCentre.x = a_movedata.position.x;
	m_vCircleCentre.y = a_movedata.position.y;
	// record current position of agent
	m_vPrevLoc.x = a_movedata.position.x;
	m_vPrevLoc.y = a_movedata.position.y;
	//m_vCircleCentre = glm::normalize(m_vCircleCentre);

	// ** NEED TO GET THE DIRECTION THAT THE AGENT IS FACING **
	m_vCircleCentre.x = m_fDistance*cos(a_movedata.rotation) + a_movedata.position.x;
	m_vCircleCentre.y = m_fDistance*sin(a_movedata.rotation) + a_movedata.position.y;
	//m_vCircleCentre += m_fDistance; // moves circle ot in front of agent

	// find random spot on circle
	randomTarget();

	// change wanderAngle for next frame
	m_fWanderAngle += ((float)(rand() % 360 + 1) * m_fJitter) - (m_fJitter * 0.5f);

	// calcutate and return wander force
	glm::vec2 wanderForce(m_vCircleCentre + m_vTarget);

	// calculate the acceleration required to move agent to target (distance minus current velocity)
	//glm::vec2 acceleration(glm::distance(wanderForce, a_movedata.velocity) - a_movedata.velocity);
}

void WanderAction::wander2(float a_dt, MovementInfo & a_movedata)
{
	//// pick a random target on the screen
	glm::vec3 target((float)(rand() % 800 + 0.5), (float)(rand() % 600 + 0.5), 0.0f);

	// vector from agent to target
	glm::vec3 distance = glm::normalize(target - a_movedata.position);
	distance *= m_fRadius;
	// scale vector by maxSpeed
	distance = distance * a_movedata.maxSpeed;
	// calculate the acceleration required to move agent to target (distance minus current velocity)
	glm::vec3 acceleration = distance - a_movedata.velocity;

	if (glm::length(acceleration) > a_movedata.maxAcceleration)
	{
		acceleration = glm::normalize(acceleration) * a_movedata.maxAcceleration;
		a_movedata.acceleration = acceleration;
	}

	// Apply accleration to agent
	a_movedata.velocity += acceleration * a_dt;
	a_movedata.position += a_movedata.velocity * a_dt;
}


