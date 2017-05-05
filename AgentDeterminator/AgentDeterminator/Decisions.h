#pragma once
#include <list>
#include "Action.h"

class Agent;
class Behaviour;
// agents
class PlayerAgent;
class EnemyAgent;
class CompanionAgent;
// behaviours
class PlayerWander;
class PlayerEvade;
class PlayerAttack;
class EnemySeek;
class EnemyFlee;
class EnemyAttack;
class CompanionFollow;
class CompanionEvade;
class CompanionAttack;



/*******************************************************************************
* Decisions Class
********************************************************************************/
class Decisions
{
public:
	Decisions();
	virtual ~Decisions();

	virtual void update(float a_dt) = 0;
	void drawGUI(Agent & a_agent);

	std::list<Behaviour*> behaviours;
	ActionType saysDoThis();
protected:
	Behaviour * m_highestPriority;
	bool compare(Behaviour const & a_first, Behaviour const & a_second);
	void sortBehaviours();
};
/*******************************************************************************
* Player Decisions Class
********************************************************************************/
class PlayerBrain :
	public Decisions
{
public:
	PlayerBrain();
	PlayerBrain(PlayerAgent * a_agent);
	virtual ~PlayerBrain();

	virtual void update(float a_dt);

private:
	// agent
	PlayerAgent * m_pAgent = nullptr;
	// Behaviours
	PlayerWander *	m_wanderBehaviour = nullptr;
	PlayerEvade *	m_evadeBehaviour = nullptr;
	PlayerAttack *	m_attackBehaviour = nullptr;
};
/*******************************************************************************
* Enemy Decisions Class
********************************************************************************/
class EnemyBrain :
	public Decisions
{
public:
	EnemyBrain();
	EnemyBrain(EnemyAgent * a_agent);
	virtual ~EnemyBrain();

	virtual void update(float a_dt);
private:
	// agent
	EnemyAgent * m_pAgent = nullptr;
	// behaviours
	EnemySeek	* m_seekBehaviour = nullptr;
	EnemyFlee	* m_fleeBehaviour = nullptr;
	EnemyAttack	* m_attackBehaviour = nullptr;
};
/*******************************************************************************
* Companion Decisions Class
********************************************************************************/
class CompanionBrain :
	public Decisions
{
public:
	CompanionBrain();
	CompanionBrain(CompanionAgent * a_agent);
	virtual ~CompanionBrain();

	virtual void update(float a_dt);
private:
	// agent
	CompanionAgent * m_pAgent = nullptr;
	// behaviours
	CompanionFollow	* m_followBehaviour = nullptr;
	CompanionEvade	* m_evadeBehaviour = nullptr;
	CompanionAttack	* m_attackBehaviour = nullptr;
};

