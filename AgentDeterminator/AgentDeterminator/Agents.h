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

	virtual void update(float deltaTime) = 0;
	AgentStats vitals;
protected:

};
/******************************************************************************************************************************
* Player Agent
*******************************************************************************************************************************/
class PlayerAgent : public Agent {
public:
	PlayerAgent();
	PlayerAgent(std::string a_name, glm::vec3 a_position);
	virtual ~PlayerAgent();

	virtual void update(float deltaTime);
private:
	PlayerWander * m_wanderBehaviour = nullptr;
	PlayerEvade * m_evadeBehaviour = nullptr;
	PlayerAttack * m_attackBehaviour = nullptr;
};
/******************************************************************************************************************************
* Enemy Agent
*******************************************************************************************************************************/
class EnemyAgent : public Agent {
public:
	EnemyAgent();
	EnemyAgent(std::string a_name, glm::vec3 a_position);
	virtual ~EnemyAgent();

	virtual void update(float deltaTime);

private:
	EnemySeek	* m_seekBehaviour = nullptr;
	EnemyFlee	* m_fleeBehaviour = nullptr;
	EnemyAttack	* m_attackBehaviour = nullptr;
};
/******************************************************************************************************************************
* Companion Agent
*******************************************************************************************************************************/
class CompanionAgent : public Agent {
public:
	CompanionAgent();
	CompanionAgent(std::string a_name, glm::vec3 a_position);
	virtual ~CompanionAgent();

	virtual void update(float deltaTime);

private:
	CompanionFollow	* m_followBehaviour = nullptr;
	CompanionEvade	* m_evadeBehaviour = nullptr;
	CompanionAttack	* m_attackBehaviour = nullptr;
};

