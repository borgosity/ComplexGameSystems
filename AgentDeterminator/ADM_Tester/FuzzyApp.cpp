#include "FuzzyApp.h"
#include "Font.h"
#include "Input.h"
#include "glm\glm.hpp"
#include "WanderAction.h"

FuzzyApp::FuzzyApp()
{
}


FuzzyApp::~FuzzyApp()
{
}

bool FuzzyApp::startup()
{
	// renderer
	m_renderer = new aie::Renderer2D();
	m_font = new aie::Font("./font/consolas.ttf", 32);
	//scene controller
	m_sceneController = new SceneController();
	// agents
	m_playerAgent = new PlayerAgent("Hero", glm::vec3(400.0f, 400.0f, 0.0f));
	m_buddyAgent = new CompanionAgent("Buddy", glm::vec3(400.0f, 500.0f, 0.0f));
	m_enemyAgent = new EnemyAgent("Enemy", glm::vec3(500.0f, 400.0f, 0.0f));
	// add egents to scene controller
	m_sceneController->addAgent(m_buddyAgent);
	m_sceneController->addAgent(m_enemyAgent);
	m_sceneController->addAgent(m_playerAgent);
	// initialise agent behaviours
	m_buddyAgent->buddyAgent(m_playerAgent);
	m_enemyAgent->findTarget();
	return true;
}

void FuzzyApp::shutdown()
{
	delete m_font;
	delete m_renderer;
	delete m_buddyAgent;
	delete m_enemyAgent;
	delete m_playerAgent;
	delete m_sceneController;
}

void FuzzyApp::update(float a_dt)
{
	// update input
	aie::Input* input = aie::Input::getInstance();

	// update agents
	m_sceneController->update(a_dt);

	// agent off screen?
	offScreen();

	m_playerAgent->vitals.currentDistance = glm::distance(m_playerAgent->position(), m_enemyAgent->position());
	//m_playerAgent->update(deltaTime);
	
	m_buddyAgent->vitals.currentDistance = glm::distance(m_buddyAgent->position(), m_enemyAgent->position());
	//m_buddyAgent->update(deltaTime);

	m_enemyAgent->vitals.currentDistance = glm::distance(m_enemyAgent->position(), m_buddyAgent->position());
	//m_enemyAgent->update(deltaTime);

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void FuzzyApp::draw()
{
	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_renderer->begin();

	// draw your stuff here!
	drawAgents(m_renderer);
	drawWander(*m_playerAgent, *m_playerAgent->wanderPtr());
	drawSeek(*m_enemyAgent);
	drawGUI();

	// output some text
	m_renderer->drawText(m_font, "Press ESC to quit", 0, 0);


	// done drawing sprites
	m_renderer->end();
}

void FuzzyApp::drawAgents(aie::Renderer2D * a_renderer)
{
	glm::vec4 colour = glm::vec4(0.0f);
	glm::vec3 position = glm::vec3(0.0f);
	// draw agents from list
	for (auto agent : m_sceneController->m_agentObjects)
	{
		colour = agent->colour();
		position = agent->position();
		a_renderer->setRenderColour(colour.r, colour.g, colour.b, colour.a);
		a_renderer->drawSprite(nullptr, position.x, position.y, agent->vitals.size, agent->vitals.size);
	}
}

void FuzzyApp::drawWander(Agent & a_agent, WanderAction & a_wander)
{
	// line from original location to centre of circle
	//m_renderer->drawLine(a_wander.controls.prevLoc.x, a_wander.controls.prevLoc.y, a_wander.controls.circleCentre.x, a_wander.controls.circleCentre.y, 1.0f);
	m_renderer->drawLine(a_agent.movedata.position.x, a_agent.movedata.position.y, a_wander.controls.circleCentre.x, a_wander.controls.circleCentre.y, 1.0f);
	// line to edge of circle
	m_renderer->drawLine(a_wander.controls.circleCentre.x, a_wander.controls.circleCentre.y, a_wander.controls.target.x, a_wander.controls.target.y, 2.0f);
	// path agent should be heading along
	m_renderer->drawLine(a_agent.movedata.position.x, a_agent.movedata.position.y, a_wander.controls.target.x, a_wander.controls.target.y, 3.0f);

	// draw a cirle with lines
	double slice = 2 * M_PI / 360;
	glm::vec2 point1(0.0f, 0.0f);


	for (int i = 0; i < 360; i++)
	{
		double angle = slice * i;
		glm::vec2 point2(a_wander.controls.circleCentre.x + a_wander.controls.radius * cos(angle), a_wander.controls.circleCentre.y + a_wander.controls.radius * sin(angle));
		if (point1.x == 0 && point1.y == 0)
		{
			point1 = point2;
		}
		m_renderer->drawLine(point1.x, point1.y, point2.x, point2.y);
		point1 = point2;
	}
}

void FuzzyApp::drawSeek(Agent & a_agent)
{
	// draw a cirle with lines
	double slice = 2 * M_PI / 360;
	glm::vec2 point1(0.0f, 0.0f);
	
	// set coulor
	glm::vec4 colour = a_agent.colour();
	m_renderer->setRenderColour(colour.r, colour.g, colour.b, colour.a);

	// simplify data
	float radius = a_agent.movedata.sight;
	glm::vec3 center = a_agent.movedata.position;

	// draw points
	for (int i = 0; i < 360; i++)
	{
		double angle = slice * i;
		glm::vec2 point2(center.x + radius * cos(angle), center.y + radius * sin(angle));
		if (point1.x == 0 && point1.y == 0)
		{
			point1 = point2;
		}
		m_renderer->drawLine(point1.x, point1.y, point2.x, point2.y);
		point1 = point2;
	}
}

void FuzzyApp::drawGUI()
{
	for (auto agent : m_sceneController->m_agentObjects)
	{
		agent->drawGUI();
	}
}

void FuzzyApp::offScreen()
{
	for (auto agent : m_sceneController->m_agentObjects)
	{
		// gone off east side of screen
		if (agent->movedata.position.x > (SCREEN_W + agent->vitals.size))
		{
			agent->movedata.position.x = (0.0f + agent->vitals.size);
		}
		// gone off west side of screen
		if (agent->movedata.position.x < (0.0f - agent->vitals.size))
		{
			agent->movedata.position.x = (SCREEN_W - agent->vitals.size);
		}
		// gone off north side of screen
		if (agent->movedata.position.y > (SCREEN_H + agent->vitals.size))
		{
			agent->movedata.position.y = (0.0f + agent->vitals.size);
		}
		// gone off south side of screen
		if (agent->movedata.position.y < (0.0f - agent->vitals.size))
		{
			agent->movedata.position.y = (SCREEN_H - agent->vitals.size);
		}
	}
}

