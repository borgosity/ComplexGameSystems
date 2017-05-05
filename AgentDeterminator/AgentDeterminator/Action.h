#pragma once

struct MovementInfo;
class Agent;

enum ActionType
{
	AN_ARRIVE,
	AN_AVOID,
	AN_ATTACK,
	AN_EVADE,
	AN_FLEE,
	AN_FOLLOW,
	AN_PURSUE,
	AN_SEEK,
	AN_WAIT,
	AN_WANDER,
	AN_NONE
};

/// Base class for Agent Actions
class Action
{
public:
	virtual ~Action() = 0 {};
	// sub classes must implement
	virtual void update(float a_dt, Agent & a_agent) = 0;
	// accessable parent functions
	virtual ActionType actionType() { return m_actionType; };
	virtual bool doingAction() { return m_inProgress; };
protected:
	ActionType	m_actionType;
	bool		m_inProgress;
};

