#include "WanderAction.h"
#include "Agents.h"
#include <GLFW\glfw3.h>
#include <glm\gtx\norm.hpp>

WanderAction::WanderAction()
{
	m_actionType = AN_WANDER;
	controls.radius = 50.0f;
	controls.jitter = 0.25f;
	controls.distance = (float)(rand() % 75 + 1);
	controls.target = glm::vec3((float)(rand() % 800), (float)(rand() % 600), 0.0f);
	controls.wanderAngle = (float)(rand() % 360 + 1);
}

WanderAction::WanderAction(float radius, float jitter, float distance)
{
	m_actionType = AN_WANDER;
	controls.radius = radius;
	controls.jitter = jitter;
	controls.distance = distance;
	controls.target = glm::vec3((float)(rand() % 800), (float)(rand() % 600), 0.0f);
	controls.wanderAngle = (float)(rand() % 360 + 1);
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

	glm::vec3 acceleration = (glm::normalize(controls.target - a_agent.movedata.position) * a_agent.movedata.maxSpeed) - a_agent.movedata.velocity;

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
	controls.target.x = controls.radius*cos(controls.wanderAngle) + controls.circleCentre.x;
	controls.target.y = controls.radius*sin(controls.wanderAngle) + controls.circleCentre.y;
}

void WanderAction::wander1(float a_dt, MovementInfo & a_movedata)
{
	// reset velocity
	a_movedata.velocity.x = 0;
	a_movedata.velocity.y = 0;

	// specify the location of the circle centre based on the agents current velocity
	// then normalise it and move(scale) it to the specified distance infront of the agent
	controls.circleCentre.x = a_movedata.position.x;
	controls.circleCentre.y = a_movedata.position.y;
	// record current position of agent
	controls.prevLoc.x = a_movedata.position.x;
	controls.prevLoc.y = a_movedata.position.y;
	//controls.circleCentre = glm::normalize(controls.circleCentre);

	// ** NEED TO GET THE DIRECTION THAT THE AGENT IS FACING **
	controls.circleCentre.x = controls.distance*cos(a_movedata.rotation) + a_movedata.position.x;
	controls.circleCentre.y = controls.distance*sin(a_movedata.rotation) + a_movedata.position.y;
	//controls.circleCentre += controls.distance; // moves circle ot in front of agent

	// find random spot on circle
	randomTarget();

	// change wanderAngle for next frame
	controls.wanderAngle += ((float)(rand() % 360 + 1) * controls.jitter) - (controls.jitter * 0.5f);

	// calcutate and return wander force
	glm::vec3 wanderForce(controls.circleCentre + controls.target);

	// calculate the acceleration required to move agent to target (distance minus current velocity)
	//glm::vec2 acceleration(glm::distance(wanderForce, a_movedata.velocity) - a_movedata.velocity);
}

void WanderAction::wander2(float a_dt, MovementInfo & a_movedata)
{
	//// pick a random target on the screen
	glm::vec3 target((float)(rand() % 800 + 0.5), (float)(rand() % 600 + 0.5), 0.0f);

	// vector from agent to target
	glm::vec3 distance = glm::normalize(target - a_movedata.position);
	distance *= controls.radius;
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


