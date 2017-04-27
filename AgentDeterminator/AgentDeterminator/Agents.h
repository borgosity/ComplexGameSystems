#pragma once
#include <vector>
#include "GameObject.h"
#include "Follow.h"

struct AgentStats {
	float health;
	float strength;
	float speed;
	float size;
	float mass;
};

class Agent :
	public GameObject
{
public:
	Agent();
	virtual ~Agent();
};

class PlayerAgent : public Agent {
public:
	PlayerAgent();
	PlayerAgent(std::string a_name, glm::vec3 a_position);
	virtual ~PlayerAgent();
};
class EnemyAgent : public Agent {
public:
	EnemyAgent();
	EnemyAgent(std::string a_name, glm::vec3 a_position);
	virtual ~EnemyAgent();
};

class CompanionAgent : public Agent {
public:
	CompanionAgent();
	CompanionAgent(std::string a_name, glm::vec3 a_position);
	virtual ~CompanionAgent();

	AgentStats vitals;

private:
	Follow * m_followBehaviour = nullptr;
};

