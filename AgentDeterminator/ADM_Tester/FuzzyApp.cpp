#include "FuzzyApp.h"
#include "glm\glm.hpp"


FuzzyApp::FuzzyApp()
{
}


FuzzyApp::~FuzzyApp()
{
}

bool FuzzyApp::startup()
{
	m_buddyAgent = new CompanionAgent("Buddy", glm::vec3(0.0f));
	m_enemyAgent = new EnemyAgent("Enemy", glm::vec3(10.0f, 10.0f, 10.0f));
	return true;
}

void FuzzyApp::shutdown()
{
}

void FuzzyApp::update(float deltaTime)
{
	m_buddyAgent->vitals.currentDistance = glm::distance(m_buddyAgent->position(), m_enemyAgent->position());
	m_buddyAgent->update(deltaTime);

	m_enemyAgent->vitals.currentDistance = glm::distance(m_enemyAgent->position(), m_buddyAgent->position());
	m_enemyAgent->update(deltaTime);
}

void FuzzyApp::draw()
{
}
