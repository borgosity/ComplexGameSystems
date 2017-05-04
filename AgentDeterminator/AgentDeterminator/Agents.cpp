#include "Agents.h"
#include "WanderAction.h"
#include "FollowAction.h"

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
	// setup behaviour
	m_wanderBehaviour = new PlayerWander(0.0f, 100.0f, 0.0f, vitals.health, 0.0f, 100.0f);
	m_evadeBehaviour = new PlayerEvade(0.0f, 100.0f, 0.0f, vitals.health, 0.0f, 100.0f);
	m_attackBehaviour = new PlayerAttack(0.0f, 100.0f, 0.0f, vitals.health, 0.0f, 100.0f);
	// set beahvbiour priorities
	m_evadeBehaviour->traits.priority = 1;	// self preservation
	m_attackBehaviour->traits.priority = 2;	// attack if needed
	m_wanderBehaviour->traits.priority = 3;	// wander around if all is cool
	// actions
	m_wanderAction = new WanderAction(20.0f, 0.25f, 150.0f);
	actions.push_back(m_wanderAction);
}

PlayerAgent::~PlayerAgent()
{
}
void PlayerAgent::update(float a_dt)
{
	// check decision

	// if health or distance has changed reevaluate
	m_wanderBehaviour->update(*this);
	m_evadeBehaviour->update(*this);
	m_attackBehaviour->update(*this);

	// ----------------------------------- do appropriate action
	m_wanderAction->update(a_dt, *this);
	std::cout << "-- Player pos = " << movedata.position.x << ", " << movedata.position.y << std::endl;


	// ----------------------------------- movement after action processed -----------------------------------
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
	movedata.rotation = (float)atan2(movedata.velocity.x, movedata.velocity.y) * M_PI / 2;

	// reset acceleration vector
	movedata.acceleration.x = 0.0f;
	movedata.acceleration.y = 0.0f;

	// increase postition vector by velocity vector
	movedata.position += movedata.velocity;

	// reduce agent health over time
	if (vitals.health > 0) {
		vitals.health -= a_dt;
	}
	else {
		vitals.health = 0;
	}
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
	// movement
	movedata.position = a_position;
	movedata.velocity.x = 0.0f;
	movedata.velocity.y = 0.0f;
	movedata.acceleration.x = 0.0f;
	movedata.acceleration.y = 0.0f;
	movedata.maxSpeed = 50.0f;
	movedata.maxForce = 0.5f;
	movedata.maxAcceleration = 1.5f;
	movedata.rotation = 0.0f;
	movedata.rotationDampening = 0.05f;
	movedata.sight = 20.0f;
	movedata.sensor += (movedata.position + movedata.sight);
	// setup behaviour
	m_seekBehaviour = new EnemySeek(0.0f, 100.0f, 0.0f, vitals.health, 0.0f, 100.0f);
	m_fleeBehaviour = new EnemyFlee(0.0f, 100.0f, 0.0f, vitals.health, 0.0f, 100.0f);
	m_attackBehaviour = new EnemyAttack(0.0f, 100.0f, 0.0f, vitals.health, 0.0f, 100.0f);
	// set beahvbiour priorities
	m_fleeBehaviour->traits.priority = 1;	// self preservation
	m_attackBehaviour->traits.priority = 2;	// attack if possible
	m_seekBehaviour->traits.priority = 3;	// find something to attack
	// actions
	//m_wAction = new WanderAction(50.0f, 0.25f, 50.0f);
	//actions.push_back(m_wanderAction);
}

EnemyAgent::~EnemyAgent()
{
}
void EnemyAgent::update(float a_dt)
{
	// check decision
	// if health or distance has changed reevaluate
	m_seekBehaviour->update(*this);
	m_fleeBehaviour->update(*this);
	m_attackBehaviour->update(*this);



	// reduce agent health over time
	if (vitals.health > 0) {
		vitals.health -= a_dt;
	}
	else {
		vitals.health = 0;
	}
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
	// setup behaviour
	m_followBehaviour = new CompanionFollow(0.0f, 100.0f, 0.0f, vitals.health, 0.0f, 100.0f);
	m_evadeBehaviour = new CompanionEvade(0.0f, 100.0f, 0.0f, vitals.health, 0.0f, 100.0f);
	m_attackBehaviour = new CompanionAttack(0.0f, 100.0f, 0.0f, vitals.health, 0.0f, 100.0f);
	// set beahvbiour priorities
	m_evadeBehaviour->traits.priority = 1;	// self preservation
	m_followBehaviour->traits.priority = 2;	// stay with friends
	m_attackBehaviour->traits.priority = 3; // attack when nessacary 
	// actions
	m_followAction = new FollowAction();
	actions.push_back(m_followAction);
}

CompanionAgent::~CompanionAgent()
{
	delete m_followBehaviour;
}

void CompanionAgent::update(float a_dt)
{
	// check decision
	// if health or distance has changed reevaluate
	m_followBehaviour->update(*this);
	m_evadeBehaviour->update(*this);
	m_attackBehaviour->update(*this);

	// ----------------------------------- do appropriate action
	if (m_followAction->targetAgent() != nullptr) {
		m_followAction->update(a_dt, *this);
	}
	else
	{
		if (m_pBuddyAgent != nullptr){
			m_followAction->targetAgent(m_pBuddyAgent);
		}	
		else {
			std::cout << "ERROR :: Companion Agent has no buddy to follow (= nullptr)" << std::endl;
		}
	}
	std::cout << "-- Player pos = " << movedata.position.x << ", " << movedata.position.y << std::endl;


	// ----------------------------------- movement after action processed -----------------------------------
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
	movedata.rotation = (float)atan2(movedata.velocity.x, movedata.velocity.y) * M_PI / 2;

	// reset acceleration vector
	movedata.acceleration.x = 0.0f;
	movedata.acceleration.y = 0.0f;

	// increase postition vector by velocity vector
	movedata.position += movedata.velocity;

	// reduce agent health over time
	if (vitals.health > 0) {
		vitals.health -= a_dt;
	}
	else {
		vitals.health = 0;
	}

	// reduce agent health over time
	if (vitals.health > 0) {
		vitals.health -= a_dt;
	}
	else {
		vitals.health = 0;
	}
}
