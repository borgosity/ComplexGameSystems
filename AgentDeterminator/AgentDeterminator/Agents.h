#pragma once
#include <vector>
#include <iostream>
#include "GameObject.h"
#include "CompanionFollow.h"
#include "CompanionEvade.h"
#include "CompanionAttack.h"
#include "EnemySeek.h"
#include "EnemyFlee.h"
#include "EnemyAttack.h"
#include "PlayerWander.h"
#include "PlayerEvade.h"
#include "PlayerAttack.h"

#include "Action.h"
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
	float health = 0.0f;
	float strength = 0.0f;
	float speed = 0.0f;
	float size = 0.0f;
	float mass = 0.0f;
	float currentDistance = 0.0f;	// distance from target
	float minDistance = 0.0f;		// minimum distance to target
	AgentType type;
	bool dead = false;
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
	float maxSpeed = 0.0f;
	float maxAcceleration = 0.0f;
	float maxForce = 0.0f;
	float livelyness = 0.0f;		// used to determine delay between wander updates
	float prevTime = 0.0f;			//used in wander delay
	float sight = 0.0f;
	float rotation = 0.0f;
	float rotationDampening = 0.0f;
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
	// draw GUI's
	void drawGUI();
	void drawVitals();
	void drawMoveData();
	virtual void drawBehaviours() = 0 {};

	std::vector<Agent*> * m_agents;
	AgentStats		vitals;
	MovementInfo	movedata;
	std::vector<Action*> actions;


protected:
	// gui controls
	bool m_bShowVitals;
	bool m_bShowMoveData;
	bool m_bShowBehaviour;
	void move(float a_dt);
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

	void drawBehaviours();
	WanderAction * wanderPtr() { return m_wanderAction; };

private:
	// agents
	Agent * m_pEnemyAgent = nullptr;
	bool m_bShowWanderSets;
	bool m_bShowEvadeSets;
	bool m_bShowAttackSets;
	// Behaviours
	PlayerWander * m_wanderBehaviour = nullptr;
	PlayerEvade * m_evadeBehaviour = nullptr;
	PlayerAttack * m_attackBehaviour = nullptr;
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
	void drawBehaviours();

	void findTarget();
private:
	// agents
	Agent * m_pBuddyAgent = nullptr;
	Agent * m_pEnemyAgent = nullptr;
	// behaviours
	EnemySeek	* m_seekBehaviour = nullptr;
	EnemyFlee	* m_fleeBehaviour = nullptr;
	EnemyAttack	* m_attackBehaviour = nullptr;
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
	void drawBehaviours();

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

	// behaviours
	CompanionFollow	* m_followBehaviour = nullptr;
	CompanionEvade	* m_evadeBehaviour = nullptr;
	CompanionAttack	* m_attackBehaviour = nullptr;

	// Actions
	FollowAction * m_followAction = nullptr;
	AttackAction * m_attackAction = nullptr;
	EvadeAction * m_evadeAction = nullptr;
};

