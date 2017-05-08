#pragma once
#include "Application.h"
#include "Renderer2D.h"
#include "SceneController.h"
#include "Agents.h"
#include "GameDef.h"

class FuzzyApp : public aie::Application
{
public:
	FuzzyApp();
	virtual ~FuzzyApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float a_dt);
	virtual void draw();

private:
	SceneController * m_sceneController = nullptr;
	CompanionAgent	* m_buddyAgent = nullptr;
	EnemyAgent		* m_enemyAgent = nullptr;
	PlayerAgent		* m_playerAgent = nullptr;

	// rendering
	aie::Renderer2D * m_renderer;
	aie::Font		* m_font;
	// draws
	void drawAgents(aie::Renderer2D * a_renderer);
	void drawAction(Agent & a_agent);
	void drawWander(Agent & a_agent, WanderAction & a_wander);
	void drawRadius(Agent & a_agent);
	void drawSeek(Agent & a_agent);
	void drawGUI();
	

	void offScreen();
};

