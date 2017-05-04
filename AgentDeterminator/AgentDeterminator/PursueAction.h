#pragma once
#include "Action.h"
#include <glm\glm.hpp>
#include <memory>

class Agent;

struct PursueValues
{
	glm::vec3	target;			// target vector
	glm::vec3	prevLoc;		// previous location
	float		distance;		// target distance from player	
};


class PursueAction :
	public Action
{
public:
	PursueAction();
	PursueAction(Agent * a_target);
	virtual ~PursueAction();
	virtual void update(float a_dt, Agent & a_agent);

	Agent * targetAgent() { return m_pTarget; };
	Agent * targetAgent(Agent * a_targetAgent) {
		m_pTarget = a_targetAgent; return m_pTarget;
	};
	PursueValues controls;

private:
	// agent member variables
	Agent * m_pTarget;
};

