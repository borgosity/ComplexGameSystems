#include "Agents.h"
#include "WanderAction.h"
#include "FollowAction.h"
#include "FleeAction.h"
#include "EvadeAction.h"
#include "SeekAction.h"
#include "AttackAction.h"

#include "imgui_glfw3.h"
#include <glm/gtc/type_ptr.hpp>

#define M_PI       3.14159265358979323846   // pi

/******************************************************************************************************************************
* Base Agent
*******************************************************************************************************************************/
Agent::Agent()
{
	m_bShowVitals = false;
	m_bShowMoveData = false;
	m_bShowBehaviour = false;
}

Agent::~Agent()
{
}
void Agent::drawGUI()
{
	// Draw agent GUI window
	std::string windowName = m_name + " Details";
	ImGui::Begin(windowName.c_str());
	// button 1
	ImGui::PushID(1);
	ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(1 / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(1 / 7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(1 / 7.0f, 0.8f, 0.8f));
	// draw button and detect click
	if (ImGui::Button("Vitals")) m_bShowVitals = !m_bShowVitals;
	ImGui::PopStyleColor(3);
	ImGui::PopID();
	// button 2
	ImGui::PushID(2);
	ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(2 / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(2 / 7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(2 / 7.0f, 0.8f, 0.8f));
	ImGui::SameLine();
	// draw button and detect click
	if (ImGui::Button("Move Data")) m_bShowMoveData = !m_bShowMoveData;
	ImGui::PopStyleColor(3);
	ImGui::PopID();
	// button 3
	ImGui::PushID(3);
	ImGui::PushStyleColor(ImGuiCol_Button, ImColor::HSV(3 / 7.0f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImColor::HSV(3 / 7.0f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImColor::HSV(3 / 7.0f, 0.8f, 0.8f));
	ImGui::SameLine();
	// draw button and detect click
	if (ImGui::Button("Behaviours")) m_bShowBehaviour = !m_bShowBehaviour;
	ImGui::PopStyleColor(3);
	ImGui::PopID();
	ImGui::End();

	// draw other GUI windows as required
	if (m_bShowVitals) {
		drawVitals();
	}
	if (m_bShowMoveData) {
		drawMoveData();
	}
	if (m_bShowBehaviour) {
		drawBehaviours();
	}
}
void Agent::drawVitals()
{
	std::string windowName = m_name + " Stats";
	ImGui::Begin(windowName.c_str());
	ImGui::InputFloat("health", &vitals.health, 1.0f, 1.0f, 2);
	ImGui::InputFloat("strength", &vitals.strength, 1.0f, 1.0f, 2);
	ImGui::InputFloat("speed", &vitals.speed, 1.0f, 1.0f, 2);
	ImGui::InputFloat("size", &vitals.size, 1.0f, 1.0f, 2);
	ImGui::InputFloat("mass", &vitals.mass, 1.0f, 1.0f, 2);
	ImGui::InputFloat("minDist", &vitals.minDistance, 1.0f, 1.0f, 2);
	ImGui::End();
}
void Agent::drawMoveData()
{
	std::string windowName = m_name + " Move Data";
	ImGui::Begin(windowName.c_str());
	ImGui::InputFloat3("position", glm::value_ptr(movedata.position), 2);
	ImGui::InputFloat3("velocity", glm::value_ptr(movedata.velocity), 2);
	ImGui::InputFloat3("acceleration", glm::value_ptr(movedata.acceleration), 2);
	ImGui::InputFloat3("heading", glm::value_ptr(movedata.heading), 2);
	ImGui::InputFloat3("sensor", glm::value_ptr(movedata.sensor), 2);

	ImGui::InputFloat("maxSpeed", &movedata.maxSpeed, 1.0f, 1.0f, 2);
	ImGui::InputFloat("maxAccel", &movedata.maxAcceleration, 1.0f, 1.0f, 2);
	ImGui::InputFloat("maxForce", &movedata.maxForce, 1.0f, 1.0f, 2);
	ImGui::InputFloat("livelyness", &movedata.livelyness, 1.0f, 1.0f, 2);
	ImGui::InputFloat("sight", &movedata.sight, 1.0f, 1.0f, 2);
	ImGui::InputFloat("rotation", &movedata.rotation, 1.0f, 1.0f, 2);
	ImGui::InputFloat("rotationDamp", &movedata.rotationDampening, 1.0f, 1.0f, 2);
	ImGui::End();
}

void Agent::move(float a_dt)
{
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
	vitals.dead = false;
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
	m_attackAction = new AttackAction();
	m_evadeAction = new EvadeAction();
	actions.push_back(m_wanderAction);
	actions.push_back(m_evadeAction);
	actions.push_back(m_attackAction);
}

PlayerAgent::~PlayerAgent()
{
}
void PlayerAgent::update(float a_dt)
{
	// check decision
	if (vitals.dead) {
		return;
	}
	// if health or distance has changed reevaluate
	m_wanderBehaviour->update(*this);
	m_evadeBehaviour->update(*this);
	m_attackBehaviour->update(*this);

	// ----------------------------------- do appropriate action
	m_wanderAction->update(a_dt, *this);

	// ----------------------------------- movement after action processed -----------------------------------
	move(a_dt);
}

void PlayerAgent::drawBehaviours()
{
	// create agent window
	std::string windowName = m_name + " Behaviours";
	ImGui::Begin(windowName.c_str());
	// set columns
	ImGui::Columns(3, "mixed");
	ImGui::Separator();
	// first column
	ImGui::Text("Behaviour One");
	ImGui::ProgressBar(m_wanderBehaviour->traits.currWeight / 100, ImVec2(0.0f, 0.0f));
	std::vector<float> health = m_wanderBehaviour->health();

	int count = 0;
	std::string label = "LS";
	for (auto msVal : health)
	{
		if (count > 1 && count < 5) label = "TRI";
		if (count > 4) label = "RS";
		count++;
		ImGui::InputFloat(label.c_str(), &msVal, 1.0f, 1.0f, 2);
	}
	// second column
	ImGui::NextColumn();
	ImGui::Text("Behaviour Two");
	ImGui::ProgressBar(m_evadeBehaviour->traits.currWeight / 100, ImVec2(0.0f, 0.0f));
	health = m_evadeBehaviour->health();

	count = 0;
	label = "LS";
	for (auto msVal : health)
	{
		if (count > 1 && count < 5) label = "TRI";
		if (count > 4) label = "RS";
		count++;
		ImGui::InputFloat(label.c_str(), &msVal, 1.0f, 1.0f, 2);
	}
	// third column
	ImGui::NextColumn();
	ImGui::Text("Behaviour Three");
	ImGui::ProgressBar(m_attackBehaviour->traits.currWeight / 100, ImVec2(0.0f, 0.0f));
	health = m_attackBehaviour->health();

	count = 0;
	label = "LS";
	for (auto msVal : health)
	{
		if (count > 1 && count < 5) label = "TRI";
		if (count > 4) label = "RS";
		count++;
		ImGui::InputFloat(label.c_str(), &msVal, 1.0f, 1.0f, 2);
	}
	ImGui::End();
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
	vitals.dead = false;
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
	m_seekAction = new SeekAction();
	m_fleeAction = new FleeAction();
	m_attackAction = new AttackAction();
	actions.push_back(m_seekAction);
	actions.push_back(m_attackAction);
	actions.push_back(m_fleeAction);
}

EnemyAgent::~EnemyAgent()
{
}
void EnemyAgent::update(float a_dt)
{
	// check decision
	if (vitals.dead) {
		return;
	}
	// if health or distance has changed reevaluate
	m_seekBehaviour->update(*this);
	m_fleeBehaviour->update(*this);
	m_attackBehaviour->update(*this);

	// ----------------------------------- do appropriate action
	if (m_seekAction->targetAgent() != nullptr) {
		m_seekAction->update(a_dt, *this);
	}
	else
	{
		if (m_pEnemyAgent != nullptr && !m_pEnemyAgent->vitals.dead) {
			m_seekAction->targetAgent(m_pEnemyAgent);
		}
		else {
			findTarget();
			std::cout << "ERROR :: Enemy Agent has no enemy to follow (= nullptr)" << std::endl;
		}
	}

	// ----------------------------------- movement after action processed -----------------------------------
	move(a_dt);
}
void EnemyAgent::drawBehaviours()
{
}
void EnemyAgent::findTarget()
{
	float closestDistance = 0.0f;
	Agent * closestAgent = nullptr;

	for (auto agent : *m_agents)
	{
		// if not a friend
		if (agent->vitals.type != ENEMY && !agent->vitals.dead) {
			// get distance
			float distance = glm::distance(movedata.position, agent->movedata.position);
			// check if it is the closest yet seen
			if (distance <= closestDistance){
				closestAgent = agent;
			}
		}
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
	vitals.dead = false;
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
	m_evadeAction = new EvadeAction();
	m_attackAction = new AttackAction();
	actions.push_back(m_followAction);
	actions.push_back(m_evadeAction);
	actions.push_back(m_attackAction);
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

	// ----------------------------------- movement after action processed -----------------------------------
	move(a_dt);

}

void CompanionAgent::drawBehaviours()
{
}
