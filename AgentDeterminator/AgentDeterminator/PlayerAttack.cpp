#include "PlayerAttack.h"
#include "Agents.h"



PlayerAttack::PlayerAttack()
{
}

PlayerAttack::PlayerAttack(float a_distanceMin, float distanceMax, float a_healthMin, float a_healthMax, float a_attackMin, float a_attackMax)
{
	// ------------------ distance ------------------------------------------
	// left variables
	float distanceCloseMin = a_distanceMin;
	float distanceCloseMax = (distanceMax - a_distanceMin) / 3;
	// triangular variables
	float distanceMiddleMin = (distanceCloseMax - distanceCloseMin) / 3;
	float distanceMiddlePeak = distanceMax  * 0.5f;
	float distanceMiddleMax = distanceMiddlePeak + distanceMiddleMin;
	// right variables
	float distanceFarMin = (distanceMiddleMax - distanceMiddleMin) / 3;
	float distanceFarMax = distanceMax;
	// membership function objects
	m_distanceMS = new LeftShoulderTriangularRightShoulder(distanceCloseMin, distanceCloseMax,
															distanceMiddleMin, distanceMiddlePeak, distanceMiddleMax,
															distanceFarMin, distanceFarMax);

	// ------------------ health ------------------------------------------
	// left variables
	float healthLowMin = a_healthMin;
	float healthLowMax = (a_healthMax - a_healthMin) / 3;
	// triangular variables
	float healthOkayMin = (healthLowMax - healthLowMin) / 3;
	float healthOkayPeak = a_healthMax  * 0.5f;
	float healthOkayMax = healthOkayPeak + healthOkayMin;
	// right variables
	float healthGoodMin = (healthOkayMax - healthOkayMin) / 3;
	float healthGoodMax = a_healthMax;
	// membership function objects
	m_healthMS = new LeftShoulderTriangularRightShoulder(healthLowMin, healthLowMax,
														 healthOkayMin, healthOkayPeak, healthOkayMax,
														 healthGoodMin, healthGoodMax);

	// ------------------ attackable ------------------------------------------
	// left variables
	float attackLowMin = a_attackMin;
	float attackLowMax = (a_attackMax - a_attackMin) / 3;
	// triangular variables
	float attackMidMin = (attackLowMax - attackLowMin) / 3;
	float attackMidPeak = a_attackMax  * 0.5f;
	float attackMidMax = attackMidPeak + attackMidMin;
	// right variables
	float attackHighMin = (attackMidMax - attackMidMin) / 3;
	float attackHighMax = a_attackMax;
	// membership function objects
	m_attackMS = new LeftShoulderTriangularRightShoulder(attackLowMin, attackLowMax,
		attackMidMin, attackMidPeak, attackMidMax,
		attackHighMin, attackHighMax);

	// fill settings vector
	initVectors();
	// set initial attack weight
	traits.currWeight = 0;
}

PlayerAttack::PlayerAttack(float a_distanceCloseMin, float distanceCloseMax, float a_distanceMiddleMin, float a_distanceMiddleMid, float distanceMiddleMax, float a_distanceFarMin, float distanceFarMax,
	float a_healthLowMin, float healthLowMax, float a_healthOkayMin, float a_healthOkayMid, float healthOkayMax, float a_healthGoodMin, float healthGoodMax,
	float a_attackLowMin, float attackLowMax, float a_attackMediumMin, float a_attackMediumMid, float attackMediumMax, float a_attackHighMin, float attackHighMax)
{
	// distance
	m_distanceMS = new LeftShoulderTriangularRightShoulder(a_distanceCloseMin, distanceCloseMax, 
														   a_distanceMiddleMin, a_distanceMiddleMid, distanceMiddleMax, 
														   a_distanceFarMin, distanceFarMax);
	// health
	m_healthMS = new LeftShoulderTriangularRightShoulder(a_healthLowMin, healthLowMax,
														 a_healthOkayMin, a_healthOkayMid, healthOkayMax,
														 a_healthGoodMin, healthGoodMax);
	// attackable
	m_attackMS = new LeftShoulderTriangularRightShoulder(a_attackLowMin, attackLowMax,
														 a_attackMediumMin, a_attackMediumMid, attackMediumMax,
														 a_attackHighMin, attackHighMax);

	// fill settings vector
	initVectors();
}


PlayerAttack::~PlayerAttack()
{
	destroy();
}

void PlayerAttack::update(Agent & a_agent)
{
	float attack = 0;
	// update member sets
	m_distanceMS->update(a_agent);
	m_healthMS->update(a_agent);
	m_attackMS->update(a_agent);
	// how far from target
	float targetClose = m_distanceMS->doms.leftShoulder;
	float targetNear = m_distanceMS->doms.triangular;
	float targetFar = m_distanceMS->doms.rightShoulder;
	// how much health
	float healthLow = m_healthMS->doms.leftShoulder;
	float healthOkay = m_healthMS->doms.triangular;
	float healthGood = m_healthMS->doms.rightShoulder;

	// how attackable is the target
	float attackLow = OR( healthLow, 
						OR(
							OR (AND(healthOkay,targetNear), AND(healthOkay, targetFar)),
							OR(AND(healthGood, targetNear), AND(healthGood, targetFar))
						)
					);
	float attackMid = 0;
	float attackHigh = OR(AND(healthGood, targetClose), AND(healthOkay, targetClose));

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
}

std::vector<float> PlayerAttack::distance(std::vector<float> a_settings)
{
	// clear settings
	m_distanceSettings.empty();
	// set new settings
	m_distanceSettings = m_distanceMS->settings(a_settings);

	return m_distanceSettings;
}

std::vector<float> PlayerAttack::health(std::vector<float> a_settings)
{
	// clear settings
	m_healthSettings.empty();
	// set new settings
	m_healthSettings = m_healthMS->settings(a_settings);

	return m_healthSettings;
}

std::vector<float> PlayerAttack::attackable(std::vector<float> a_settings)
{
	// clear settings
	m_attackSettings.empty();
	// set new settings
	m_attackSettings = m_attackMS->settings(a_settings);

	return m_attackSettings;
}

void PlayerAttack::destroy()
{
	deallocate(m_distanceMS);
	deallocate(m_healthMS);
	deallocate(m_attackMS);
}

void PlayerAttack::initVectors()
{
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
