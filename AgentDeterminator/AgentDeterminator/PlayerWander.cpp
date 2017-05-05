#include "PlayerWander.h"
#include "Agents.h"



PlayerWander::PlayerWander()
{
}

PlayerWander::PlayerWander(float a_distanceMin, float distanceMax, float a_healthMin, float a_healthMax, float a_wanderMin, float a_wanderMax)
{
	traits.name = "Wander";
	traits.action = AN_WANDER;
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

	// ------------------ wanderable ------------------------------------------
	// left variables
	float wanderLowMin = a_wanderMin;
	float wanderLowMax = (a_wanderMax - a_wanderMin) / 3;
	// triangular variables
	float wanderMidMin = (wanderLowMax - wanderLowMin) / 3;
	float wanderMidPeak = a_wanderMax  * 0.5f;
	float wanderMidMax = wanderMidPeak + wanderMidMin;
	// right variables
	float wanderHighMin = (wanderMidMax - wanderMidMin) / 3;
	float wanderHighMax = a_wanderMax;
	// membership function objects
	m_wanderMS = new LeftShoulderTriangularRightShoulder(wanderLowMin, wanderLowMax,
		wanderMidMin, wanderMidPeak, wanderMidMax,
		wanderHighMin, wanderHighMax);

	// fill settings vector
	initVectors();
	// set initial wander weight
	traits.currWeight = 0;
}

PlayerWander::PlayerWander(float a_distanceCloseMin, float distanceCloseMax, float a_distanceMiddleMin, float a_distanceMiddleMid, float distanceMiddleMax, float a_distanceFarMin, float distanceFarMax,
	float a_healthLowMin, float healthLowMax, float a_healthOkayMin, float a_healthOkayMid, float healthOkayMax, float a_healthGoodMin, float healthGoodMax,
	float a_wanderLowMin, float wanderLowMax, float a_wanderMediumMin, float a_wanderMediumMid, float wanderMediumMax, float a_wanderHighMin, float wanderHighMax)
{
	// distance
	m_distanceMS = new LeftShoulderTriangularRightShoulder(a_distanceCloseMin, distanceCloseMax,
		a_distanceMiddleMin, a_distanceMiddleMid, distanceMiddleMax,
		a_distanceFarMin, distanceFarMax);
	// health
	m_healthMS = new LeftShoulderTriangularRightShoulder(a_healthLowMin, healthLowMax,
		a_healthOkayMin, a_healthOkayMid, healthOkayMax,
		a_healthGoodMin, healthGoodMax);
	// wanderable
	m_wanderMS = new LeftShoulderTriangularRightShoulder(a_wanderLowMin, wanderLowMax,
		a_wanderMediumMin, a_wanderMediumMid, wanderMediumMax,
		a_wanderHighMin, wanderHighMax);

	// fill settings vector
	initVectors();
}


PlayerWander::~PlayerWander()
{
	destroy();
}

void PlayerWander::update(Agent & a_agent)
{
	float wander = 0;
	// update member sets
	m_distanceMS->update(a_agent);
	m_healthMS->update(a_agent);
	m_wanderMS->update(a_agent);
	// how far from target
	float targetClose = m_distanceMS->doms.leftShoulder;
	float targetNear = m_distanceMS->doms.triangular;
	float targetFar = m_distanceMS->doms.rightShoulder;
	// how much health
	float healthLow = m_healthMS->doms.leftShoulder;
	float healthOkay = m_healthMS->doms.triangular;
	float healthGood = m_healthMS->doms.rightShoulder;
	
	// how wanderable is the target
	float wanderLow = AND(healthLow, targetClose);
	float wanderMid = OR(AND(healthLow, NOT(targetClose)),
						 OR(AND(healthOkay, targetClose), AND(healthGood, targetClose))
					);
	float wanderHigh = AND(NOT(healthLow),NOT(targetClose));

	// set max values
	float maxWanderLow = m_wanderMS->maxDom.leftShoulder;
	float maxWanderMid = m_wanderMS->maxDom.triangular;
	float maxWanderHigh = m_wanderMS->maxDom.rightShoulder;
	// defuzzify
	wander = maxWanderHigh * wanderHigh + maxWanderMid * wanderMid + maxWanderLow * wanderLow;
	wander /= (0.1f + wanderHigh + wanderMid + wanderLow);
	// set weight
	traits.prevWeight = traits.currWeight;
	traits.currWeight = wander;
}

std::vector<float> PlayerWander::distance(std::vector<float> a_settings)
{
	// clear settings
	m_distanceSettings.empty();
	// set new settings
	m_distanceSettings = m_distanceMS->settings(a_settings);

	return m_distanceSettings;
}

std::vector<float> PlayerWander::health(std::vector<float> a_settings)
{
	// clear settings
	m_healthSettings.empty();
	// set new settings
	m_healthSettings = m_healthMS->settings(a_settings);

	return m_healthSettings;
}

std::vector<float> PlayerWander::wanderable(std::vector<float> a_settings)
{
	// clear settings
	m_wanderSettings.empty();
	// set new settings
	m_wanderSettings = m_wanderMS->settings(a_settings);

	return m_wanderSettings;
}

void PlayerWander::destroy()
{
	deallocate(m_distanceMS);
	deallocate(m_healthMS);
	deallocate(m_wanderMS);
}

void PlayerWander::initVectors()
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
	m_wanderSettings.empty();
	// set new settings
	m_wanderSettings = m_wanderMS->settings();
}
