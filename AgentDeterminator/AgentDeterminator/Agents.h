#pragma once
#include "GameObject.h"
class Agent :
	public GameObject
{
public:
	Agent();
	~Agent();
};

class PlayerAgent : public Agent {
public:
	PlayerAgent();
	~PlayerAgent();
};
class EnemyAgent : public Agent {
public:
	EnemyAgent();
	~EnemyAgent();
};

class CompanionAgent : public Agent {
public:
	CompanionAgent();
	~CompanionAgent();
};

