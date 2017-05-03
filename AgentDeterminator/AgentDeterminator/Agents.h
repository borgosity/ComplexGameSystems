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

	std::vector<Agent*> * m_agents;
	AgentStats		vitals;
	MovementInfo	movedata;
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

	virtual void update(float a_dt);
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

	virtual void update(float a_dt);

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

	virtual void update(float a_dt);

private:
	CompanionFollow	* m_followBehaviour = nullptr;
	CompanionEvade	* m_evadeBehaviour = nullptr;
	CompanionAttack	* m_attackBehaviour = nullptr;
};

