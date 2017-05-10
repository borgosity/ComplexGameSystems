#include "EnemyFlee.h"
#include "Agents.h"



EnemyFlee::EnemyFlee()
{
}

EnemyFlee::EnemyFlee(float a_distanceMin, float distanceMax, float a_healthMin, float a_healthMax, float a_fleeMin, float a_fleeMax)
{
	traits.name = "Flee";
	traits.action = AN_FLEE;
	// ------------------ distance ------------------------------------------
	std::vector<float> distance = genSteepLTRSet(a_distanceMin, distanceMax);
	// membership function objects
	m_distanceMS = new LeftShoulderTriangularRightShoulder(distance, "Distance");

	// ------------------ health ------------------------------------------
	std::vector<float> health = genSteepLTRSet(a_healthMin, a_healthMax);
	// membership function objects
	m_healthMS = new LeftShoulderTriangularRightShoulder(health, "Health");

	// ------------------ fleeable ------------------------------------------
	std::vector<float> flee = genSteepLTRSet(a_fleeMin, a_fleeMax);
	// membership function objects
	m_fleeMS = new LeftShoulderTriangularRightShoulder(flee, "Desire");

	// fill settings vector
	initVectors();
	// set initial flee weight
	traits.currWeight = 0;
}

EnemyFlee::EnemyFlee(float a_distanceCloseMin, float distanceCloseMax, float a_distanceMiddleMin, float a_distanceMiddleMid, float distanceMiddleMax, float a_distanceFarMin, float distanceFarMax,
	float a_healthLowMin, float healthLowMax, float a_healthOkayMin, float a_healthOkayMid, float healthOkayMax, float a_healthGoodMin, float healthGoodMax,
	float a_fleeLowMin, float fleeLowMax, float a_fleeMediumMin, float a_fleeMediumMid, float fleeMediumMax, float a_fleeHighMin, float fleeHighMax)
{
	traits.name = "Flee";
	// distance
	m_distanceMS = new LeftShoulderTriangularRightShoulder(a_distanceCloseMin, distanceCloseMax,
		a_distanceMiddleMin, a_distanceMiddleMid, distanceMiddleMax,
		a_distanceFarMin, distanceFarMax, "Distance");
	// health
	m_healthMS = new LeftShoulderTriangularRightShoulder(a_healthLowMin, healthLowMax,
		a_healthOkayMin, a_healthOkayMid, healthOkayMax,
		a_healthGoodMin, healthGoodMax, "Health");
	// fleeable
	m_fleeMS = new LeftShoulderTriangularRightShoulder(a_fleeLowMin, fleeLowMax,
		a_fleeMediumMin, a_fleeMediumMid, fleeMediumMax,
		a_fleeHighMin, fleeHighMax, "Desire");

	// fill settings vector
	initVectors();
}


EnemyFlee::~EnemyFlee()
{
	destroy();
}

void EnemyFlee::update(Agent & a_agent)
{
	float flee = 0;
	// update member sets
	m_distanceMS->update(a_agent, a_agent.vitals.foeDistance);
	m_healthMS->update(a_agent, a_agent.vitals.health);
	m_fleeMS->update(a_agent, traits.currWeight);
	// how far from target
	float targetClose = m_distanceMS->doms.leftShoulder;
	float targetNear = m_distanceMS->doms.triangular;
	float targetFar = m_distanceMS->doms.rightShoulder;
	// how much health
	float healthLow = m_healthMS->doms.leftShoulder;
	float healthOkay = m_healthMS->doms.triangular;
	float healthGood = m_healthMS->doms.rightShoulder;

	// how fleeable is the target
	float fleeLow = OR(healthOkay, healthGood);
	float fleeMid = AND(healthLow, targetNear);
	float fleeHigh = AND(healthLow, targetClose);
	
	// set max values
	float maxFleeLow = m_fleeMS->maxDom.leftShoulder;
	float maxFleeMid = m_fleeMS->maxDom.triangular;
	float maxFleeHigh = m_fleeMS->maxDom.rightShoulder;
	// defuzzify
	flee = maxFleeHigh * fleeHigh + maxFleeMid * fleeMid + maxFleeLow * fleeLow;
	flee /= (0.1f + fleeHigh + fleeMid + fleeLow);
	// set weight
	traits.prevWeight = traits.currWeight;
	traits.currWeight = flee;
	saveHistory(flee);
}

std::vector<float> EnemyFlee::distance(std::vector<float> a_settings)
{
	// clear settings
	m_distanceSettings.empty();
	// set new settings
	m_distanceSettings = m_distanceMS->settings(a_settings);

	return m_distanceSettings;
}

std::vector<float> EnemyFlee::health(std::vector<float> a_settings)
{
	// clear settings
	m_healthSettings.empty();
	// set new settings
	m_healthSettings = m_healthMS->settings(a_settings);

	return m_healthSettings;
}

std::vector<float> EnemyFlee::fleeable(std::vector<float> a_settings)
{
	// clear settings
	m_fleeSettings.empty();
	// set new settings
	m_fleeSettings = m_fleeMS->settings(a_settings);

	return m_fleeSettings;
}

void EnemyFlee::destroy()
{
	deallocate(m_distanceMS);
	deallocate(m_healthMS);
	deallocate(m_fleeMS);
}

void EnemyFlee::initVectors()
{
	// fill list of membersets
	m_memberSets.push_back(m_distanceMS);
	m_memberSets.push_back(m_healthMS);
	m_memberSets.push_back(m_fleeMS);

	// clear settings
	m_distanceSettings.empty();
	// set new settings
	m_distanceSettings = m_distanceMS->settings();
	// clear settings
	m_healthSettings.empty();
	// set new settings
	m_healthSettings = m_healthMS->settings();
	// clear settings
	m_fleeSettings.empty();
	// set new settings
	m_fleeSettings = m_fleeMS->settings();
}
