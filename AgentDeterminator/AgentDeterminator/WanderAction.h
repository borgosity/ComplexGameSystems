#pragma once
#include "Action.h"
#include <glm\glm.hpp>
#include <memory>

class Agent;
typedef std::shared_ptr<Agent> spAgent;

class WanderAction :
	public Action
{
public:
	WanderAction();
	virtual ~WanderAction();
	WanderAction(float radius, float jitter, float distance);
	virtual void update(float a_dt, Agent & a_agent);

private:
	// agent member variables
	spAgent	m_spTarget;

	// wander member variables
	glm::vec3	m_vTarget;			// target vector
	glm::vec3	m_vCircleCentre;	// wander circle
	glm::vec3	m_vPrevLoc;			// previous location
	float		m_fRadius;			// circle radious
	float		m_fDistance;		// circle distance from player	
	float		m_fJitter;			// wander jitter
	float		m_fWanderAngle;		// wander angle

									// private member functions
	void setAngle(glm::vec2 & a_vec2, float a_wanderAngle);
	void randomTarget();  // uses private meber variable to produce the target
	void wander1(float a_dt, MovementInfo & a_movedata);
	void wander2(float a_dt, MovementInfo & a_movedata);
};

