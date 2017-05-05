#include "Decisions.h"
// agents
#include "Agents.h"
// behaviour classes
#include "CompanionFollow.h"
#include "CompanionEvade.h"
#include "CompanionAttack.h"
#include "EnemySeek.h"
#include "EnemyFlee.h"
#include "EnemyAttack.h"
#include "PlayerWander.h"
#include "PlayerEvade.h"
#include "PlayerAttack.h"
// gui
#include "imgui_glfw3.h"


/*******************************************************************************
* Decisions Functions
********************************************************************************/
Decisions::Decisions()
{
	m_highestPriority = nullptr;
}

Decisions::~Decisions()
{
}
void Decisions::drawGUI(Agent & a_agent)
{
	// create agent window
	ImGui::Begin(a_agent.name().c_str());

	for (auto behaviour : behaviours)
	{
		float weight = behaviour->traits.currWeight / 100;
		// display behaviour values

		std::vector<float> values = behaviour->desireSettings();

		//ImGui::PlotLines(behaviour->traits.name.c_str(), values.data(), values.size(), 0, NULL, 0.0f, 100.0f, ImVec2(0, 100));

		ImGui::ProgressBar(weight, ImVec2(0.0f, 0.0f));
		ImGui::SameLine(0.0f, -1.0f);
		ImGui::Text(behaviour->traits.name.c_str());
	}
	ImGui::End();
}
ActionType Decisions::saysDoThis()
{
	if (m_highestPriority) {
		return m_highestPriority->traits.action;
	}
	else {
		return AN_NONE;
	}
}
bool Decisions::compare(Behaviour const &  a_first, Behaviour const &  a_second)
{
	bool result = false;
	if (a_first.traits.currWeight > a_second.traits.currWeight) {
		result = true;
	}
	return result;
}

void Decisions::sortBehaviours()
{
	behaviours.sort([](Behaviour *  a_first, Behaviour *  a_second) {
		return a_first->traits.currWeight > a_second->traits.currWeight;
	});
	m_highestPriority = behaviours.front();
}

/*******************************************************************************
* Player Functions
********************************************************************************/
PlayerBrain::PlayerBrain()
{
	m_pAgent = nullptr;
}

PlayerBrain::PlayerBrain(PlayerAgent * a_agent)
{
	m_pAgent = a_agent;
	// setup behaviour
	m_wanderBehaviour = new PlayerWander(0.0f, 100.0f, 0.0f, m_pAgent->vitals.health, 0.0f, 100.0f);
	m_evadeBehaviour = new PlayerEvade(0.0f, 100.0f, 0.0f, m_pAgent->vitals.health, 0.0f, 100.0f);
	m_attackBehaviour = new PlayerAttack(0.0f, 100.0f, 0.0f, m_pAgent->vitals.health, 0.0f, 100.0f);
	// set beahvbiour priorities
	m_evadeBehaviour->traits.priority = 1;	// self preservation
	m_attackBehaviour->traits.priority = 2;	// attack if needed
	m_wanderBehaviour->traits.priority = 3;	// wander around if all is cool
	// fill behaviour list
	behaviours.push_back(m_wanderBehaviour);
	behaviours.push_back(m_evadeBehaviour);
	behaviours.push_back(m_attackBehaviour);
}

PlayerBrain::~PlayerBrain()
{
	m_wanderBehaviour->destroy();
	m_evadeBehaviour->destroy();
	m_attackBehaviour->destroy();
}

void PlayerBrain::update(float a_dt)
{
	// if health or distance has changed reevaluate
	m_wanderBehaviour->update(*m_pAgent);
	m_evadeBehaviour->update(*m_pAgent);
	m_attackBehaviour->update(*m_pAgent);
	// set highest priority
	sortBehaviours();
}

/*******************************************************************************
* Enemy Functions
********************************************************************************/
EnemyBrain::EnemyBrain()
{
	m_pAgent = nullptr;
}

EnemyBrain::EnemyBrain(EnemyAgent * a_agent)
{
	m_pAgent = a_agent;
	// setup behaviour
	m_seekBehaviour = new EnemySeek(0.0f, 100.0f, 0.0f, m_pAgent->vitals.health, 0.0f, 100.0f);
	m_fleeBehaviour = new EnemyFlee(0.0f, 100.0f, 0.0f, m_pAgent->vitals.health, 0.0f, 100.0f);
	m_attackBehaviour = new EnemyAttack(0.0f, 100.0f, 0.0f, m_pAgent->vitals.health, 0.0f, 100.0f);
	// set beahvbiour priorities
	m_fleeBehaviour->traits.priority = 1;	// self preservation
	m_attackBehaviour->traits.priority = 2;	// attack if possible
	m_seekBehaviour->traits.priority = 3;	// find something to attack
	// fill behaviour list
	behaviours.push_back(m_fleeBehaviour);
	behaviours.push_back(m_attackBehaviour);
	behaviours.push_back(m_seekBehaviour);
}

EnemyBrain::~EnemyBrain()
{
	delete m_seekBehaviour;
	delete m_fleeBehaviour;
	delete m_attackBehaviour;
}

void EnemyBrain::update(float a_dt)
{


	// if health or distance has changed reevaluate
	m_seekBehaviour->update(*m_pAgent);
	m_fleeBehaviour->update(*m_pAgent);
	m_attackBehaviour->update(*m_pAgent);
	// set highest priority
	sortBehaviours();
}
/*******************************************************************************
* Companion Functions
********************************************************************************/
CompanionBrain::CompanionBrain()
{
	m_pAgent = nullptr;
}

CompanionBrain::CompanionBrain(CompanionAgent * a_agent)
{
	m_pAgent = a_agent;
	// setup behaviour
	m_followBehaviour = new CompanionFollow(0.0f, 100.0f, 0.0f, m_pAgent->vitals.health, 0.0f, 100.0f);
	m_evadeBehaviour = new CompanionEvade(0.0f, 100.0f, 0.0f, m_pAgent->vitals.health, 0.0f, 100.0f);
	m_attackBehaviour = new CompanionAttack(0.0f, 100.0f, 0.0f, m_pAgent->vitals.health, 0.0f, 100.0f);
	// set beahvbiour priorities
	m_evadeBehaviour->traits.priority = 1;	// self preservation
	m_followBehaviour->traits.priority = 2;	// stay with friends
	m_attackBehaviour->traits.priority = 3; // attack when nessacary 
	// fill behaviour list
	behaviours.push_back(m_evadeBehaviour);
	behaviours.push_back(m_followBehaviour);
	behaviours.push_back(m_attackBehaviour);
}

CompanionBrain::~CompanionBrain()
{
	delete m_evadeBehaviour;
	delete m_followBehaviour;
	delete m_attackBehaviour;
}

void CompanionBrain::update(float a_dt)
{
	// if health or distance has changed reevaluate
	m_followBehaviour->update(*m_pAgent);
	m_evadeBehaviour->update(*m_pAgent);
	m_attackBehaviour->update(*m_pAgent);
	// set highest priority
	sortBehaviours();
}
