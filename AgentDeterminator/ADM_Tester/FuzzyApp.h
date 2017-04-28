#pragma once
#include "Application.h"
#include "Agents.h"


class FuzzyApp : public aie::Application
{
public:
	FuzzyApp();
	virtual ~FuzzyApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

private:
	CompanionAgent * m_buddyAgent = nullptr;

};

