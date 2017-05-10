#include "CompanionAttack.h"
#include "Agents.h"



CompanionAttack::CompanionAttack()
{
}

CompanionAttack::CompanionAttack(float a_distanceMin, float distanceMax, float a_healthMin, float a_healthMax, float a_attackMin, float a_attackMax)
{
	traits.name = "Attack";
	traits.action = AN_ATTACK;
	// ------------------ distance ------------------------------------------
	std::vector<float> distance = genSteepLTRSet(a_distanceMin, distanceMax);
	// membership function objects
	m_distanceMS = new LeftShoulderTriangularRightShoulder(distance, "Distance");

	// ------------------ health ------------------------------------------
	std::vector<float> health = genSteepLTRSet(a_healthMin, a_healthMax);
	// membership function objects
	m_healthMS = new LeftShoulderTriangularRightShoulder(health, "Health");

	// ------------------ attackable ------------------------------------------
	std::vector<float> attack = genSteepLTRSet(a_attackMin, a_attackMax);
	// membership function objects
	m_attackMS = new LeftShoulderTriangularRightShoulder(attack, "Desire");

	// fill settings vector
	initVectors();
}

CompanionAttack::CompanionAttack(float a_distanceCloseMin, float distanceCloseMax, float a_distanceMiddleMin, float a_distanceMiddleMid, float distanceMiddleMax, float a_distanceFarMin, float distanceFarMax,
	float a_healthLowMin, float healthLowMax, float a_healthOkayMin, float a_healthOkayMid, float healthOkayMax, float a_healthGoodMin, float healthGoodMax,
	float a_attackLowMin, float attackLowMax, float a_attackMediumMin, float a_attackMediumMid, float attackMediumMax, float a_attackHighMin, float attackHighMax)
{
	traits.name = "Attack";
	// distance
	m_distanceMS = new LeftShoulderTriangularRightShoulder(a_distanceCloseMin, distanceCloseMax,
		a_distanceMiddleMin, a_distanceMiddleMid, distanceMiddleMax,
		a_distanceFarMin, distanceFarMax, "Distance");
	// health
	m_healthMS = new LeftShoulderTriangularRightShoulder(a_healthLowMin, healthLowMax,
		a_healthOkayMin, a_healthOkayMid, healthOkayMax,
		a_healthGoodMin, healthGoodMax, "Health");
	// attackable
	m_attackMS = new LeftShoulderTriangularRightShoulder(a_attackLowMin, attackLowMax,
		a_attackMediumMin, a_attackMediumMid, attackMediumMax,
		a_attackHighMin, attackHighMax, "Desire");
	// fill settings vector
	initVectors();
}


CompanionAttack::~CompanionAttack()
{
	destroy();
}

void CompanionAttack::update(Agent & a_agent)
{
	float attack = 0;
	// update member sets
	m_distanceMS->update(a_agent, a_agent.vitals.foeDistance);
	m_healthMS->update(a_agent, a_agent.vitals.health);
	m_attackMS->update(a_agent, traits.currWeight);

	// how far from target
	float targetClose = m_distanceMS->doms.leftShoulder;
	float targetNear = m_distanceMS->doms.triangular;
	float targetFar = m_distanceMS->doms.rightShoulder;
	// how much health
	float healthLow = m_healthMS->doms.leftShoulder;
	float healthOkay = m_healthMS->doms.triangular;
	float healthGood = m_healthMS->doms.rightShoulder;

	// how attackable is the target
	float attackLow = healthLow;
	float attackMid = OR(AND(healthOkay, targetNear), AND(healthOkay, targetFar));
	float attackHigh = OR(healthGood, AND(healthOkay, targetClose));

	// set max values
	float maxAttackLow = m_attackMS->maxDom.leftShoulder;
	float maxAttackMid = m_attackMS->maxDom.triangular;
	float maxAttackHigh = m_attackMS->maxDom.rightShoulder;
	// defuzzify
	attack = maxAttackHigh * attackHigh + maxAttackMid * attackMid + maxAttackLow * attackLow;
	attack /= (0.1f + attackHigh + attackMid + attackLow);
	// set weight
	traits.prevWeight = traits.currWeight;
	traits.currWeight = attack;
	saveHistory(attack);
}

std::vector<float> CompanionAttack::distance(std::vector<float> a_settings)
{
	// clear settings
	m_distanceSettings.empty();
	// set new settings
	m_distanceSettings = m_distanceMS->settings(a_settings);

	return m_distanceSettings;
}

std::vector<float> CompanionAttack::health(std::vector<float> a_settings)
{
	// clear settings
	m_healthSettings.empty();
	// set new settings
	m_healthSettings = m_healthMS->settings(a_settings);

	return m_healthSettings;
}

std::vector<float> CompanionAttack::attackable(std::vector<float> a_settings)
{
	// clear settings
	m_attackSettings.empty();
	// set new settings
	m_attackSettings = m_attackMS->settings(a_settings);

	return m_attackSettings;
}

void CompanionAttack::destroy()
{
	deallocate(m_distanceMS);
	deallocate(m_healthMS);
	deallocate(m_attackMS);
}

void CompanionAttack::initVectors()
{
	// fill list of membersets
	m_memberSets.push_back(m_distanceMS);
	m_memberSets.push_back(m_healthMS);
	m_memberSets.push_back(m_attackMS);
	// clear settings
	m_distanceSettings.empty();
	// set new settings
	m_distanceSettings = m_distanceMS->settings();
	// clear settings
	m_healthSettings.empty();
	// set new settings
	m_healthSettings = m_healthMS->settings();
	// clear settings
	m_attackSettings.empty();
	// set new settings
	m_attackSettings = m_attackMS->settings();
}
