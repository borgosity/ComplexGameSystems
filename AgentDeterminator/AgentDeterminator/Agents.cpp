#include "Agents.h"
#include "WanderAction.h"
#include "FollowAction.h"
#include "FleeAction.h"
#include "EvadeAction.h"
#include "SeekAction.h"
#include "AttackAction.h"


#define M_PI       3.14159265358979323846   // pi

/******************************************************************************************************************************
* Base Agent
*******************************************************************************************************************************/
Agent::Agent()
{
}

Agent::~Agent()
{
}

/// Common move function for all agents, 
/// - relies on action calculations to be completed first
void Agent::move(float a_dt)
{
	// update velocity vector
	movedata.velocity += movedata.acceleration * a_dt;

	glm::vec3 vel = movedata.velocity;

	// check if the magnitude of the velocity is greater than max speed
	if (glm::length(vel) > movedata.maxSpeed)
	{
		// update members old vars 
		vel = glm::normalize(vel) * movedata.maxSpeed;

		// update new vector style member vars
		movedata.velocity = glm::normalize(vel) * movedata.maxSpeed;

	}

	// set rotation
	movedata.rotation = (float)atan2(movedata.velocity.x, movedata.velocity.y) * (float)(M_PI / 2);

	// reset acceleration vector
	movedata.acceleration.x = 0.0f;
	movedata.acceleration.y = 0.0f;

	// increase postition vector by velocity vector
	movedata.position += movedata.velocity;
}
void Agent::healthCheck()
{
	if (vitals.dead) {
		return;
	}
	else {
		if (vitals.health <= 0.0f) {
			vitals.dead = true;
		}
	}
}
/******************************************************************************************************************************
* Player Agent
*******************************************************************************************************************************/
PlayerAgent::PlayerAgent()
{
	m_name = "Player Agent";
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	movedata.position = m_position;
	m_colour = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	vitals.type = PLAYER;
}

PlayerAgent::PlayerAgent(std::string a_name, glm::vec3 a_position)
{
	m_name = a_name;
	m_position = a_position;
	m_colour = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	// setup vitals
	vitals.health = 100;
	vitals.size = 15;
	vitals.mass = vitals.size * 0.5f;
	vitals.speed = 100 / vitals.mass;
	vitals.strength = vitals.mass * vitals.size * vitals.speed;
	vitals.minDistance = 10.0f;
	vitals.currentDistance = 0.0f;
	vitals.type = PLAYER;
	vitals.dead = false;
	// movement
	movedata.position = a_position;
	movedata.acceleration = glm::vec3(0.0f);
	movedata.maxAcceleration = 0.5f;
	movedata.velocity = glm::vec3(0.0f);
	movedata.maxSpeed = 2.0f;
	movedata.maxForce = 0.1f;
	movedata.rotation = 0.0f;
	movedata.rotationDampening = 0.05f;
	movedata.livelyness = 3.0f;
	movedata.sight = 20.0f;
	movedata.sensor += (movedata.position + movedata.sight);
	// brain
	m_brain = new PlayerBrain(this);
	// actions
	m_wanderAction = new WanderAction(20.0f, 0.25f, 150.0f);
	m_attackAction = new AttackAction();
	m_evadeAction = new EvadeAction();
	actions.push_back(m_wanderAction);
	actions.push_back(m_evadeAction);
	actions.push_back(m_attackAction);
}

PlayerAgent::~PlayerAgent()
{
	delete m_brain;
	delete m_wanderAction;
	delete m_evadeAction;
	delete m_attackAction;
}
void PlayerAgent::update(float a_dt)
{
	// check health
	healthCheck();

	// ------------------ decide what to do -------------------------------------------------------------------
	m_brain->update(a_dt);
	// ----------------------------------- do appropriate action ----------------------------------------------
	switch (m_brain->saysDoThis())
	{
	case AN_WANDER:
		m_wanderAction->update(a_dt, *this);
		break;
	case AN_EVADE:
	{
		if (m_evadeAction->targetAgent() != nullptr) {
			m_evadeAction->update(a_dt, *this);
		}
		else
		{
			if (m_pEnemyAgent != nullptr) {
				m_evadeAction->targetAgent(m_pEnemyAgent);
			}
			else {
				findEnemy();
				std::cout << "ERROR :: Player Agent has no enemy to evade (= nullptr)" << std::endl;
			}
		}
		break;
	}
	case AN_ATTACK:
		m_attackAction->update(a_dt, *this);
		break;
	default:
		std::cout << "Player Doing Nothing!" << std::endl;
		break;
	}

	// ----------------------------------- movement after action processed ------------------------------------
	move(a_dt);
}

void PlayerAgent::drawGUI()
{
	m_brain->drawGUI(*this);
}

void PlayerAgent::findEnemy()
{
	float closestDistance = movedata.sight;
	Agent * closestAgent = nullptr;

	for (auto agent : *m_agents)
	{
		// if not a friend
		if (agent->vitals.type == ENEMY && !agent->vitals.dead) {
			// get distance
			float distance = glm::distance(movedata.position, agent->movedata.position);
			// check if it is the closest yet seen
			if (distance <= closestDistance) {
				closestAgent = agent;
			}
		}
	}
	// set closest agent
	m_pEnemyAgent = closestAgent;
}

/******************************************************************************************************************************
* Enemy Agent
*******************************************************************************************************************************/
EnemyAgent::EnemyAgent()
{
	m_name = "Enemy Agent";
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	movedata.position = m_position;
	m_colour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	vitals.type = ENEMY;
}

EnemyAgent::EnemyAgent(std::string a_name, glm::vec3 a_position) 
{
	m_name = a_name;
	m_position = a_position;
	m_colour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	// setup vitals
	vitals.health = 100;
	vitals.size = 15;
	vitals.mass = vitals.size * 0.5f;
	vitals.speed = 100 / vitals.mass;
	vitals.strength = vitals.mass * vitals.size * vitals.speed;
	vitals.minDistance = 10.0f;
	vitals.currentDistance = 0.0f;
	vitals.type = ENEMY;
	vitals.dead = false;
	// movement
	movedata.position = a_position;
	movedata.velocity = glm::vec3(0.0f);
	movedata.acceleration = glm::vec3(0.0f);
	movedata.maxSpeed = 2.0f;
	movedata.maxForce = 0.1f;
	movedata.maxAcceleration = 1.5f;
	movedata.rotation = 0.0f;
	movedata.rotationDampening = 0.05f;
	movedata.sight = 80.0f;
	movedata.sensor += (movedata.position + movedata.sight);
	// brain
	m_brain = new EnemyBrain(this);
	// actions
	m_wanderAction = new WanderAction(50.0f, 0.25f, 50.0f);
	m_seekAction = new SeekAction();
	m_fleeAction = new FleeAction();
	m_attackAction = new AttackAction();
	actions.push_back(m_wanderAction);
	actions.push_back(m_attackAction);
	actions.push_back(m_fleeAction);
	//
	m_allDead = false;
}

EnemyAgent::~EnemyAgent()
{

	delete m_wanderAction;
	delete m_seekAction;
	delete m_fleeAction;
	delete m_attackAction;
}

void EnemyAgent::update(float a_dt)
{
	// check health
	healthCheck();

	// ------------------ decide what to do -------------------------------------------------------------------
	m_brain->update(a_dt);

	// ----------------------------------- do appropriate action ----------------------------------------------
	switch (m_brain->saysDoThis())
	{
	case AN_WANDER:
		m_wanderAction->update(a_dt, *this);
		break;
	case AN_SEEK:
	{
		if (m_seekAction->targetAgent() != nullptr) {
			m_seekAction->update(a_dt, *this);
		}
		else
		{
			if (m_pEnemyAgent != nullptr && !m_pEnemyAgent->vitals.dead) {
				m_seekAction->targetAgent(m_pEnemyAgent);
			}
			else {
				if (!m_allDead)
				{
					findTarget();
				}
			}
		}
		break;
	}
	case AN_ATTACK:
	{
		if (m_attackAction->targetAgent() != nullptr) {
			m_attackAction->update(a_dt, *this);
		}
		else
		{
			if (m_pEnemyAgent != nullptr && !m_pEnemyAgent->vitals.dead) {
				m_attackAction->targetAgent(m_pEnemyAgent);
			}
			else {
				if (!m_allDead)
				{
					findTarget();
				}
			}
		}
		break;
	}
	default:
		std::cout << "Enemy Doing Nothing!" << std::endl;
		break;
	}


	// ----------------------------------- movement after action processed -----------------------------------
	move(a_dt);
}
void EnemyAgent::drawGUI()
{
	m_brain->drawGUI(*this);
}
void EnemyAgent::findTarget()
{
	float closestDistance = movedata.sight;
	Agent * closestAgent = nullptr;
	bool allDead = true;

	for (auto agent : *m_agents)
	{
		// if not a friend
		if (agent->vitals.type != ENEMY && !agent->vitals.dead) {
			// some agents are alive
			allDead = false;
			// get distance
			float distance = glm::distance(movedata.position, agent->movedata.position);
			// check if it is the closest yet seen
			if (distance <= closestDistance){
				closestAgent = agent;
			}
		}
	}
	m_allDead = allDead;

	// set closest agent
	m_pEnemyAgent = closestAgent;
}
/******************************************************************************************************************************
* Companion Agent
*******************************************************************************************************************************/
CompanionAgent::CompanionAgent()
{
	m_name = "Companion Agent";
	m_position = glm::vec3(10.0f, 10.0f, 0.0f);
	movedata.position = m_position;
	m_colour = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	vitals.type = COMPANION;
}
///
/// Default fuzzy values for agent are set to 0 - 100.
///
CompanionAgent::CompanionAgent(std::string a_name, glm::vec3 a_position)
{
	m_name = a_name;
	m_position = a_position;
	movedata.position = a_position;
	m_colour = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
	// setup vitals
	vitals.health = 100;
	vitals.size = 20;
	vitals.mass = vitals.size * 0.5f;
	vitals.speed = 100 / vitals.mass;
	vitals.strength = vitals.mass * vitals.size * vitals.speed;
	vitals.minDistance = 10.0f;
	vitals.currentDistance = 0.0f;
	vitals.type = COMPANION;
	vitals.dead = false;
	// movement
	movedata.position = a_position;
	movedata.velocity.x = 0.0f;
	movedata.velocity.y = 0.0f;
	movedata.acceleration.x = 0.0f;
	movedata.acceleration.y = 0.0f;
	movedata.maxSpeed = 2.0f;
	movedata.maxForce = 0.1f;
	movedata.maxAcceleration = 0.5f;
	movedata.rotation = 0.0f;
	movedata.rotationDampening = 0.05f;
	movedata.sight = 20.0f;
	movedata.sensor += (movedata.position + movedata.sight);
	// brain
	m_brain = new CompanionBrain(this);
	// actions
	m_followAction = new FollowAction();
	m_evadeAction = new EvadeAction();
	m_attackAction = new AttackAction();
	actions.push_back(m_followAction);
	actions.push_back(m_evadeAction);
	actions.push_back(m_attackAction);
}

CompanionAgent::~CompanionAgent()
{

	delete m_followAction;
	delete m_evadeAction;
	delete m_attackAction;
}

void CompanionAgent::update(float a_dt)
{
	// check health
	healthCheck();

	// ------------------ decide what to do -------------------------------------------------------------------
	m_brain->update(a_dt);

	// ----------------------------------- do appropriate action ----------------------------------------------
	switch (m_brain->saysDoThis())
	{
	case AN_FOLLOW:
	{
		if (m_followAction->targetAgent() != nullptr) {
			m_followAction->update(a_dt, *this);
		}
		else
		{
			if (m_pBuddyAgent != nullptr) {
				m_followAction->targetAgent(m_pBuddyAgent);
			}
			else {
				std::cout << "ERROR :: Companion Agent has no buddy to follow (= nullptr)" << std::endl;
			}
		}
		break;
	}
	case AN_EVADE:
	{
		if (m_evadeAction->targetAgent() != nullptr) {
			m_evadeAction->update(a_dt, *this);
		}
		else
		{
			if (m_pEnemyAgent != nullptr) {
				m_evadeAction->targetAgent(m_pEnemyAgent);
			}
			else {
				findEnemy();
				std::cout << "ERROR :: Player Agent has no enemy to evade (= nullptr)" << std::endl;
			}
		}
		break;
	}
	case AN_ATTACK:
		m_attackAction->update(a_dt, *this);
		break;
	default:
		std::cout << "Companion Doing Nothing!" << std::endl;
		break;
	}

	// ----------------------------------- movement after action processed -----------------------------------
	move(a_dt);

}

void CompanionAgent::drawGUI()
{
	m_brain->drawGUI(*this);
}

void CompanionAgent::findEnemy()
{
	float closestDistance = movedata.sight;
	Agent * closestAgent = nullptr;

	for (auto agent : *m_agents)
	{
		// if not a friend
		if (agent->vitals.type == ENEMY && !agent->vitals.dead) {
			// get distance
			float distance = glm::distance(movedata.position, agent->movedata.position);
			// check if it is the closest yet seen
			if (distance <= closestDistance) {
				closestAgent = agent;
			}
		}
	}
	// set closest agent
	m_pEnemyAgent = closestAgent;
}
