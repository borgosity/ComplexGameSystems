#include "PlayerEvade.h"
#include "Agents.h"



PlayerEvade::PlayerEvade()
{
}

PlayerEvade::PlayerEvade(float a_distanceMin, float distanceMax, float a_healthMin, float a_healthMax, float a_evadeMin, float a_evadeMax)
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

	// ------------------ evadeable ------------------------------------------
	// left variables
	float evadeLowMin = a_evadeMin;
	float evadeLowMax = (a_evadeMax - a_evadeMin) / 3;
	// triangular variables
	float evadeMidMin = (evadeLowMax - evadeLowMin) / 3;
	float evadeMidPeak = a_evadeMax  * 0.5f;
	float evadeMidMax = evadeMidPeak + evadeMidMin;
	// right variables
	float evadeHighMin = (evadeMidMax - evadeMidMin) / 3;
	float evadeHighMax = a_evadeMax;
	// membership function objects
	m_evadeMS = new LeftShoulderTriangularRightShoulder(evadeLowMin, evadeLowMax,
		evadeMidMin, evadeMidPeak, evadeMidMax,
		evadeHighMin, evadeHighMax);

	// fill settings vector
	initVectors();
	// set initial evade weight
	traits.currWeight = 0;
}

PlayerEvade::PlayerEvade(float a_distanceCloseMin, float distanceCloseMax, float a_distanceMiddleMin, float a_distanceMiddleMid, float distanceMiddleMax, float a_distanceFarMin, float distanceFarMax,
	float a_healthLowMin, float healthLowMax, float a_healthOkayMin, float a_healthOkayMid, float healthOkayMax, float a_healthGoodMin, float healthGoodMax,
	float a_evadeLowMin, float evadeLowMax, float a_evadeMediumMin, float a_evadeMediumMid, float evadeMediumMax, float a_evadeHighMin, float evadeHighMax)
{
	// distance
	m_distanceMS = new LeftShoulderTriangularRightShoulder(a_distanceCloseMin, distanceCloseMax,
		a_distanceMiddleMin, a_distanceMiddleMid, distanceMiddleMax,
		a_distanceFarMin, distanceFarMax);
	// health
	m_healthMS = new LeftShoulderTriangularRightShoulder(a_healthLowMin, healthLowMax,
		a_healthOkayMin, a_healthOkayMid, healthOkayMax,
		a_healthGoodMin, healthGoodMax);
	// evadeable
	m_evadeMS = new LeftShoulderTriangularRightShoulder(a_evadeLowMin, evadeLowMax,
		a_evadeMediumMin, a_evadeMediumMid, evadeMediumMax,
		a_evadeHighMin, evadeHighMax);

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
	// how far from target
	m_distanceMS->update(a_agent);
	float targetClose = m_distanceMS->doms.leftShoulder;
	float targetNear = m_distanceMS->doms.triangular;
	float targetFar = m_distanceMS->doms.rightShoulder;
	// how much health
	m_healthMS->update(a_agent);
	float healthLow = m_healthMS->doms.leftShoulder;
	float healthOkay = m_healthMS->doms.triangular;
	float healthGood = m_healthMS->doms.rightShoulder;
	// how evadeable is the target
	float evadeLow = healthLow;
	float evadeMid = OR(AND(healthOkay, targetNear), AND(healthOkay, targetFar));
	float evadeHigh = OR(healthGood, AND(healthOkay, targetClose));
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
