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
}

PlayerAgent::PlayerAgent(std::string a_name = "Player Agent", glm::vec3 a_position = glm::vec3(0.0f, 0.0f, 0.0f))
{
	std::string m_name = a_name;
	glm::vec3 m_position = a_position;
}

PlayerAgent::~PlayerAgent()
{
}
void PlayerAgent::update(float deltaTime)
{
}
/******************************************************************************************************************************
* Enemy Agent
*******************************************************************************************************************************/
EnemyAgent::EnemyAgent()
{
}

EnemyAgent::EnemyAgent(std::string a_name = "Enemy Agent", glm::vec3 a_position = glm::vec3(0.0f, 0.0f, 0.0f))
{
	std::string m_name = a_name;
	glm::vec3 m_position = a_position;
}

EnemyAgent::~EnemyAgent()
{
}
void EnemyAgent::update(float deltaTime)
{
}
/******************************************************************************************************************************
* Companion Agent
*******************************************************************************************************************************/
CompanionAgent::CompanionAgent()
{
}
///
/// Default fuzzy values for agent are set to 0 - 100.
///
CompanionAgent::CompanionAgent(std::string a_name = "Companion Agent", glm::vec3 a_position = glm::vec3(0.0f, 0.0f, 0.0f))
{
	std::string m_name = a_name;
	glm::vec3 m_position = a_position;
	// setup vitals
	vitals.health = 100;
	vitals.size = 20;
	vitals.mass = 20 * 0.5f;
	vitals.speed = 100 / vitals.mass;
	vitals.strength = vitals.mass * vitals.size * vitals.speed;
	vitals.minDistance = 10.0f;
	vitals.currentDistance = 0.0f;
	// setup behaviour
	m_followBehaviour = new Follow(0.0f, 100.0f, 0.0f, vitals.health, 0.0f, 100.0f);
}

CompanionAgent::~CompanionAgent()
{
	delete m_followBehaviour;
}

void CompanionAgent::update(float deltaTime)
{
	// check decision
	m_followBehaviour->update(*this);


	if ( m_followBehaviour->traits.currWeight > 0) {
		std::cout << "Follow behaviour = " << m_followBehaviour->traits.currWeight << std::endl;
	}
	else {
		std::cout << "Follow behaviour = 0 " << std::endl;
	}

	// waver the distance
	// reduce agent health over time
	if (vitals.currentDistance < 50) {
		vitals.currentDistance += deltaTime;
	}
	else if (vitals.currentDistance > 0) {
		vitals.currentDistance -= deltaTime;
	}

	// reduce agent health over time
	//if (vitals.health > 0) {
	//	vitals.health -= deltaTime;
	//}
	//else {
	//	vitals.health = 0;
	//}
}
