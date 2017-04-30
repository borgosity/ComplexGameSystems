#include "Agents.h"


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
}

PlayerAgent::PlayerAgent(std::string a_name, glm::vec3 a_position)
{
	std::string m_name = a_name;
	glm::vec3 m_position = a_position;
	// setup vitals
	vitals.health = 100;
	vitals.size = 15;
	vitals.mass = vitals.size * 0.5f;
	vitals.speed = 100 / vitals.mass;
	vitals.strength = vitals.mass * vitals.size * vitals.speed;
	vitals.minDistance = 10.0f;
	vitals.currentDistance = 0.0f;
	// setup behaviour
	m_wanderBehaviour = new PlayerWander(0.0f, 100.0f, 0.0f, vitals.health, 0.0f, 100.0f);
	m_evadeBehaviour = new PlayerEvade(0.0f, 100.0f, 0.0f, vitals.health, 0.0f, 100.0f);
	m_attackBehaviour = new PlayerAttack(0.0f, 100.0f, 0.0f, vitals.health, 0.0f, 100.0f);
	// set beahvbiour priorities
	m_evadeBehaviour->traits.priority = 1;	// self preservation
	m_attackBehaviour->traits.priority = 2;	// attack if needed
	m_wanderBehaviour->traits.priority = 3;	// wander around if all is cool
}

PlayerAgent::~PlayerAgent()
{
}
void PlayerAgent::update(float deltaTime)
{
	// check decision
	// if health or distance has changed reevaluate
	m_wanderBehaviour->update(*this);
	m_evadeBehaviour->update(*this);
	m_attackBehaviour->update(*this);

	if (m_wanderBehaviour->traits.currWeight > 0) {
		std::cout << "--?? Wander behaviour = " << m_wanderBehaviour->traits.currWeight << std::endl;
	}
	else {
		std::cout << "--?? Wander behaviour = 0 " << std::endl;
	}

	if (m_evadeBehaviour->traits.currWeight > 0) {
		std::cout << "--<<! evade behaviour = " << m_evadeBehaviour->traits.currWeight << std::endl;
	}
	else {
		std::cout << "--<<! evade behaviour = 0 " << std::endl;
	}

	if (m_attackBehaviour->traits.currWeight > 0) {
		std::cout << "--!>> Attack behaviour = " << m_attackBehaviour->traits.currWeight << std::endl;
	}
	else {
		std::cout << "--!>> Attack behaviour = 0 " << std::endl;
	}

	// waver the distance
	// reduce agent health over time
	//if (vitals.currentDistance < 50) {
	//	vitals.currentDistance += deltaTime;
	//}
	//else if (vitals.currentDistance > 0) {
	//	vitals.currentDistance -= deltaTime;
	//}

	// reduce agent health over time
	if (vitals.health > 0) {
		vitals.health -= deltaTime;
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
}

EnemyAgent::EnemyAgent(std::string a_name, glm::vec3 a_position) 
{
	std::string m_name = a_name;
	glm::vec3 m_position = a_position;
	// setup vitals
	vitals.health = 100;
	vitals.size = 15;
	vitals.mass = vitals.size * 0.5f;
	vitals.speed = 100 / vitals.mass;
	vitals.strength = vitals.mass * vitals.size * vitals.speed;
	vitals.minDistance = 10.0f;
	vitals.currentDistance = 0.0f;
	// setup behaviour
	m_seekBehaviour = new EnemySeek(0.0f, 100.0f, 0.0f, vitals.health, 0.0f, 100.0f);
	m_fleeBehaviour = new EnemyFlee(0.0f, 100.0f, 0.0f, vitals.health, 0.0f, 100.0f);
	m_attackBehaviour = new EnemyAttack(0.0f, 100.0f, 0.0f, vitals.health, 0.0f, 100.0f);
	// set beahvbiour priorities
	m_fleeBehaviour->traits.priority = 1;	// self preservation
	m_attackBehaviour->traits.priority = 2;	// attack if possible
	m_seekBehaviour->traits.priority = 3;	// find something to attack
}

EnemyAgent::~EnemyAgent()
{
}
void EnemyAgent::update(float deltaTime)
{
	// check decision
	// if health or distance has changed reevaluate
	m_seekBehaviour->update(*this);
	m_fleeBehaviour->update(*this);
	m_attackBehaviour->update(*this);

	if (m_seekBehaviour->traits.currWeight > 0) {
		std::cout << " ## seek behaviour = " << m_seekBehaviour->traits.currWeight << std::endl;
	}
	else {
		std::cout << " ## seek behaviour = 0 " << std::endl;
	}

	if (m_fleeBehaviour->traits.currWeight > 0) {
		std::cout << " <<< flee behaviour = " << m_fleeBehaviour->traits.currWeight << std::endl;
	}
	else {
		std::cout << "<<< flee behaviour = 0 " << std::endl;
	}

	if (m_attackBehaviour->traits.currWeight > 0) {
		std::cout << " >>> Attack behaviour = " << m_attackBehaviour->traits.currWeight << std::endl;
	}
	else {
		std::cout << " >>> Attack behaviour = 0 " << std::endl;
	}

	// waver the distance
	// reduce agent health over time
	//if (vitals.currentDistance < 50) {
	//	vitals.currentDistance += deltaTime;
	//}
	//else if (vitals.currentDistance > 0) {
	//	vitals.currentDistance -= deltaTime;
	//}

	// reduce agent health over time
	if (vitals.health > 0) {
		vitals.health -= deltaTime;
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
}
///
/// Default fuzzy values for agent are set to 0 - 100.
///
CompanionAgent::CompanionAgent(std::string a_name, glm::vec3 a_position)
{
	std::string m_name = a_name;
	glm::vec3 m_position = a_position;
	// setup vitals
	vitals.health = 100;
	vitals.size = 20;
	vitals.mass = vitals.size * 0.5f;
	vitals.speed = 100 / vitals.mass;
	vitals.strength = vitals.mass * vitals.size * vitals.speed;
	vitals.minDistance = 10.0f;
	vitals.currentDistance = 0.0f;
	// setup behaviour
	m_followBehaviour = new CompanionFollow(0.0f, 100.0f, 0.0f, vitals.health, 0.0f, 100.0f);
	m_evadeBehaviour = new CompanionEvade(0.0f, 100.0f, 0.0f, vitals.health, 0.0f, 100.0f);
	m_attackBehaviour = new CompanionAttack(0.0f, 100.0f, 0.0f, vitals.health, 0.0f, 100.0f);
	// set beahvbiour priorities
	m_evadeBehaviour->traits.priority = 1;	// self preservation
	m_followBehaviour->traits.priority = 2;	// stay with friends
	m_attackBehaviour->traits.priority = 3; // attack when nessacary 
}

CompanionAgent::~CompanionAgent()
{
	delete m_followBehaviour;
}

void CompanionAgent::update(float deltaTime)
{
	// check decision
	// if health or distance has changed reevaluate
	m_followBehaviour->update(*this);
	m_evadeBehaviour->update(*this);
	m_attackBehaviour->update(*this);

	if ( m_followBehaviour->traits.currWeight > 0) {
		std::cout << "Follow behaviour = " << m_followBehaviour->traits.currWeight << std::endl;
	}
	else {
		std::cout << "Follow behaviour = 0 " << std::endl;
	}

	if (m_evadeBehaviour->traits.currWeight > 0) {
		std::cout << "Evade behaviour = " << m_evadeBehaviour->traits.currWeight << std::endl;
	}
	else {
		std::cout << "Evade behaviour = 0 " << std::endl;
	}

	if (m_attackBehaviour->traits.currWeight > 0) {
		std::cout << "Attack behaviour = " << m_attackBehaviour->traits.currWeight << std::endl;
	}
	else {
		std::cout << "Attack behaviour = 0 " << std::endl;
	}

	// waver the distance
	// reduce agent health over time
	//if (vitals.currentDistance < 50) {
	//	vitals.currentDistance += deltaTime;
	//}
	//else if (vitals.currentDistance > 0) {
	//	vitals.currentDistance -= deltaTime;
	//}

	// reduce agent health over time
	if (vitals.health > 0) {
		vitals.health -= deltaTime;
	}
	else {
		vitals.health = 0;
	}
}
