#pragma once
#include "Action.h"
#include <glm\glm.hpp>
#include <memory>

class Agent;
typedef std::shared_ptr<Agent> spAgent;

struct WanderValues
{
	glm::vec3	target;			// target vector
	glm::vec3	circleCentre;	// wander circle
	glm::vec3	prevLoc;		// previous location
	float		radius;			// circle radious
	float		distance;		// circle distance from player	
	float		jitter;			// wander jitter
	float		wanderAngle;		// wander angle
	float		screenH;
	float		screenW;
};

class WanderAction :
	public Action
{
public:
	WanderAction();
	WanderAction(float radius, float jitter, float distance);
	virtual ~WanderAction();
	virtual void update(float a_dt, Agent & a_agent);

	WanderValues controls;

private:
	// agent member variables
	spAgent	m_spTarget;
	bool m_arrived;

	// private member functions
	void setAngle(glm::vec2 & a_vec2, float a_wanderAngle);
	void randomTarget();  // uses private meber variable to produce the target
	void wander(float a_dt, MovementInfo & a_movedata);
	void arrive(float a_dt, Agent & a_agent);
};

