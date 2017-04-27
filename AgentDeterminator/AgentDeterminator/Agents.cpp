#include "Agents.h"

Agent::Agent()
{
}

Agent::~Agent()
{
}

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
	// setup behaviour
	m_followBehaviour = new Follow(0.0f, 100.0f, 0.0f, vitals.health, 0.0f, 100.0f);
}

CompanionAgent::~CompanionAgent()
{
	delete m_followBehaviour;
}
