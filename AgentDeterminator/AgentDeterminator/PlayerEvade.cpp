#include "PlayerEvade.h"
#include "Agents.h"



PlayerEvade::PlayerEvade()
{
}

PlayerEvade::PlayerEvade(float a_distanceMin, float distanceMax, float a_healthMin, float a_healthMax, float a_evadeMin, float a_evadeMax)
{
	traits.name = "Evade";
	traits.action = AN_EVADE;
	// ------------------ distance ------------------------------------------
	std::vector<float> distance = genSteepLTRSet(a_distanceMin, distanceMax);
	// membership function objects
	m_distanceMS = new LeftShoulderTriangularRightShoulder(distance, "Distance");

	// ------------------ health ------------------------------------------
	std::vector<float> health = genSteepLTRSet(a_healthMin, a_healthMax);
	// membership function objects
	m_healthMS = new LeftShoulderTriangularRightShoulder(health, "Health");

	// ------------------ evadeable ------------------------------------------
	std::vector<float> evade = genSteepLTRSet(a_evadeMin, a_evadeMax);
	// membership function objects
	m_evadeMS = new LeftShoulderTriangularRightShoulder(evade, "Desire");

	// fill settings vector
	initVectors();

}

PlayerEvade::PlayerEvade(float a_distanceCloseMin, float distanceCloseMax, float a_distanceMiddleMin, float a_distanceMiddleMid, float distanceMiddleMax, float a_distanceFarMin, float distanceFarMax,
	float a_healthLowMin, float healthLowMax, float a_healthOkayMin, float a_healthOkayMid, float healthOkayMax, float a_healthGoodMin, float healthGoodMax,
	float a_evadeLowMin, float evadeLowMax, float a_evadeMediumMin, float a_evadeMediumMid, float evadeMediumMax, float a_evadeHighMin, float evadeHighMax)
{
	traits.name = "Evade";
	// distance
	m_distanceMS = new LeftShoulderTriangularRightShoulder(a_distanceCloseMin, distanceCloseMax,
		a_distanceMiddleMin, a_distanceMiddleMid, distanceMiddleMax,
		a_distanceFarMin, distanceFarMax, "Distance");
	// health
	m_healthMS = new LeftShoulderTriangularRightShoulder(a_healthLowMin, healthLowMax,
		a_healthOkayMin, a_healthOkayMid, healthOkayMax,
		a_healthGoodMin, healthGoodMax, "Health");
	// evadeable
	m_evadeMS = new LeftShoulderTriangularRightShoulder(a_evadeLowMin, evadeLowMax,
		a_evadeMediumMin, a_evadeMediumMid, evadeMediumMax,
		a_evadeHighMin, evadeHighMax, "Desire");

	// fill settings vector
	initVectors();
}


PlayerEvade::~PlayerEvade()
{
	destroy();
}

void PlayerEvade::update(Agent & a_agent)
{
	float evade = 0;
	// update member sets
	m_distanceMS->update(a_agent, a_agent.vitals.foeDistance);
	m_healthMS->update(a_agent, a_agent.vitals.health);
	m_evadeMS->update(a_agent, traits.currWeight);
	// how far from target
	float targetClose = m_distanceMS->doms.leftShoulder;
	float targetNear = m_distanceMS->doms.triangular;
	float targetFar = m_distanceMS->doms.rightShoulder;
	// how much health
	float healthLow = m_healthMS->doms.leftShoulder;
	float healthOkay = m_healthMS->doms.triangular;
	float healthGood = m_healthMS->doms.rightShoulder;

	// how evadeable is the target
	float evadeLow = OR(AND(healthOkay,targetFar),AND(healthGood,NOT(targetClose)));
	float evadeMid = OR(AND(healthLow, targetFar),
						OR(AND(healthOkay, targetNear), AND(healthGood, targetClose))
					);
	float evadeHigh = OR(AND(healthLow, targetClose), 
						OR(AND(healthLow, targetNear), AND(healthOkay, targetClose))
					);

	// set max values
	float maxEvadeLow = m_evadeMS->maxDom.leftShoulder;
	float maxEvadeMid = m_evadeMS->maxDom.triangular;
	float maxEvadeHigh = m_evadeMS->maxDom.rightShoulder;
	// defuzzify
	evade = maxEvadeHigh * evadeHigh + maxEvadeMid * evadeMid + maxEvadeLow * evadeLow;
	evade /= (0.1f + evadeHigh + evadeMid + evadeLow);
	// set weight
	traits.prevWeight = traits.currWeight;
	traits.currWeight = evade;
	saveHistory(evade);
}

std::vector<float> PlayerEvade::distance(std::vector<float> a_settings)
{
	// clear settings
	m_distanceSettings.empty();
	// set new settings
	m_distanceSettings = m_distanceMS->settings(a_settings);

	return m_distanceSettings;
}

std::vector<float> PlayerEvade::health(std::vector<float> a_settings)
{
	// clear settings
	m_healthSettings.empty();
	// set new settings
	m_healthSettings = m_healthMS->settings(a_settings);

	return m_healthSettings;
}

std::vector<float> PlayerEvade::evadeable(std::vector<float> a_settings)
{
	// clear settings
	m_evadeSettings.empty();
	// set new settings
	m_evadeSettings = m_evadeMS->settings(a_settings);

	return m_evadeSettings;
}

void PlayerEvade::destroy()
{
	deallocate(m_distanceMS);
	deallocate(m_healthMS);
	deallocate(m_evadeMS);
}

void PlayerEvade::initVectors()
{
	// fill list of membersets
	m_memberSets.push_back(m_distanceMS);
	m_memberSets.push_back(m_healthMS);
	m_memberSets.push_back(m_evadeMS);


	// clear settings
	m_distanceSettings.empty();
	// set new settings
	m_distanceSettings = m_distanceMS->settings();
	// clear settings
	m_healthSettings.empty();
	// set new settings
	m_healthSettings = m_healthMS->settings();
	// clear settings
	m_evadeSettings.empty();
	// set new settings
	m_evadeSettings = m_evadeMS->settings();
}
