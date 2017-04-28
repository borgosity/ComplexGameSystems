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
	return true;
}

void FuzzyApp::shutdown()
{
}

void FuzzyApp::update(float deltaTime)
{
	m_buddyAgent->update(deltaTime);
}

void FuzzyApp::draw()
{
}
