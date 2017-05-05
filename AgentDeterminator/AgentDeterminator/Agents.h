#pragma once
#include <vector>
#include <iostream>
#include "GameObject.h"
#include "Action.h"
#include "Decisions.h"

//class Action;
class WanderAction;
class FollowAction;
class AttackAction;
class SeekAction;
class FleeAction;
class EvadeAction;

/******************************************************************************************************************************
* Agent Statistics Struct
*******************************************************************************************************************************/
enum AgentType
{
	PLAYER,
	ENEMY,
	COMPANION,
	OTHER
};
/******************************************************************************************************************************
* Agent Statistics Struct
*******************************************************************************************************************************/
struct AgentStats {
	float health;
	float strength;
	float speed;
	float size;
	float mass;
	float currentDistance;	// distance from target
	float minDistance;		// minimum distance to target
	AgentType type;
	bool dead;
	ActionType action;
};
/******************************************************************************************************************************
* Agent Movement Struct
*******************************************************************************************************************************/
struct MovementInfo {
	glm::vec3 position;
	glm::vec3 sensor;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	glm::vec3 heading;
	float maxSpeed;
	float maxAcceleration;
	float maxForce;
	float livelyness;		// used to determine delay between wander updates
	float prevTime;			//used in wander delay
	float sight;
	float rotation;
	float rotationDampening;
};
/******************************************************************************************************************************
* Base Agent
*******************************************************************************************************************************/
class Agent :
	public GameObject
{
public:
	Agent();
	virtual ~Agent();


	virtual glm::vec3 position() override { return movedata.position; };
	virtual glm::vec3 position(glm::vec3 a_position) override { movedata.position = a_position;  return movedata.position; };

	void virtual drawGUI() = 0;
	std::vector<Agent*> * m_agents;
	AgentStats		vitals;
	MovementInfo	movedata;
	std::vector<Action*> actions;

protected:
	void move(float a_dt);
	void healthCheck();
};
/******************************************************************************************************************************
* Player Agent
*******************************************************************************************************************************/
class PlayerAgent : public Agent {
public:
	PlayerAgent();
	PlayerAgent(std::string a_name, glm::vec3 a_position);
	virtual ~PlayerAgent();

	virtual void update(float a_dt);
	void drawGUI();
	WanderAction * wanderPtr() { return m_wanderAction; };
	void findEnemy();
private:
	// agents
	Agent * m_pEnemyAgent = nullptr;
	// brain
	PlayerBrain * m_brain = nullptr;
	// Actions
	WanderAction * m_wanderAction = nullptr;
	AttackAction * m_attackAction = nullptr;
	EvadeAction * m_evadeAction = nullptr;
};
/******************************************************************************************************************************
* Enemy Agent
*******************************************************************************************************************************/
class EnemyAgent : public Agent {
public:
	EnemyAgent();
	EnemyAgent(std::string a_name, glm::vec3 a_position);
	virtual ~EnemyAgent();

	virtual void update(float a_dt);
	void drawGUI();
	void findTarget();
private:
	// agents
	Agent * m_pBuddyAgent = nullptr;
	Agent * m_pEnemyAgent = nullptr;
	bool	m_allDead; // flag for all enemies dead
	// brain
	EnemyBrain * m_brain = nullptr;
	// Actions
	SeekAction * m_seekAction = nullptr;
	AttackAction * m_attackAction = nullptr;
	FleeAction * m_fleeAction = nullptr;
	WanderAction * m_wanderAction = nullptr;
};
/******************************************************************************************************************************
* Companion Agent
*******************************************************************************************************************************/
class CompanionAgent : public Agent {
public:
	CompanionAgent();
	CompanionAgent(std::string a_name, glm::vec3 a_position);
	virtual ~CompanionAgent();

	virtual void update(float a_dt);
	void drawGUI();
	void findEnemy();

	// set get
	Agent * buddyAgent() { return m_pBuddyAgent; };
	Agent * buddyAgent(Agent * a_buddyAgent) {
		m_pBuddyAgent = a_buddyAgent; return m_pBuddyAgent;
	};
	Agent * enemyAgent() { return m_pEnemyAgent; };
	Agent * enemyAgent(Agent * a_enemyAgent) {
		m_pEnemyAgent = a_enemyAgent; return m_pEnemyAgent;
	};

private:
	// agents
	Agent * m_pBuddyAgent = nullptr;
	Agent * m_pEnemyAgent = nullptr;
	// brain
	CompanionBrain * m_brain = nullptr;
	// Actions
	FollowAction * m_followAction = nullptr;
	AttackAction * m_attackAction = nullptr;
	EvadeAction * m_evadeAction = nullptr;
};

