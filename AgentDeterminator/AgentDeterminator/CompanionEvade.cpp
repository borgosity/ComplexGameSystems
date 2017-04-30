#include "CompanionEvade.h"
#include "Agents.h"



CompanionEvade::CompanionEvade()
{
}

CompanionEvade::CompanionEvade(float a_distanceMin, float distanceMax, float a_healthMin, float a_healthMax, float a_evadeMin, float a_evadeMax)
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
	m_distanceClose = new FMF_LeftShoulder(distanceCloseMin, distanceCloseMax);
	m_distanceMiddle = new FMF_Triangular(distanceMiddleMin, distanceMiddlePeak, distanceMiddleMax);
	m_distanceFar = new FMF_RightShoulder(distanceFarMin, distanceFarMax);

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
	m_healthLow = new FMF_LeftShoulder(healthLowMin, healthLowMax);
	m_healthOkay = new FMF_Triangular(healthOkayMin, healthOkayPeak, healthOkayMax);
	m_healthGood = new FMF_RightShoulder(healthGoodMin, healthGoodMax);


	// ------------------ evadeable ------------------------------------------
	// left variables
	float evadeLowMin = a_evadeMin;
	float evadeLowMax = (a_evadeMax - a_evadeMin) / 3;
	// triangular variables
	float evadeOkayMin = (evadeLowMax - evadeLowMin) / 3;
	float evadeOkayPeak = a_evadeMax  * 0.5f;
	float evadeOkayMax = evadeOkayPeak + evadeOkayMin;
	// right variables
	float evadeGoodMin = (evadeOkayMax - evadeOkayMin) / 3;
	float evadeGoodMax = a_evadeMax;
	// membership function objects
	m_evadeLow = new FMF_LeftShoulder(evadeLowMin, evadeLowMax);
	m_evadeMedium = new FMF_Triangular(evadeOkayMin, evadeOkayPeak, evadeOkayMax);
	m_evadeHigh = new FMF_RightShoulder(evadeGoodMin, evadeGoodMax);

	// fill settings vector
	initVectors();
	// set initial evade weight
	traits.currWeight = 0;
}

CompanionEvade::CompanionEvade(float a_distanceCloseMin, float distanceCloseMax, float a_distanceMiddleMin, float a_distanceMiddleMid, float distanceMiddleMax, float a_distanceFarMin, float distanceFarMax,
	float a_healthLowMin, float healthLowMax, float a_healthOkayMin, float a_healthOkayMid, float healthOkayMax, float a_healthGoodMin, float healthGoodMax,
	float a_evadeLowMin, float evadeLowMax, float a_evadeMediumMin, float a_evadeMediumMid, float evadeMediumMax, float a_evadeHighMin, float evadeHighMax)
{
	// distance
	m_distanceClose = new FMF_LeftShoulder(a_distanceCloseMin, distanceCloseMax);
	m_distanceMiddle = new FMF_Triangular(a_distanceMiddleMin, a_distanceMiddleMid, distanceMiddleMax);
	m_distanceFar = new FMF_RightShoulder(a_distanceFarMin, distanceFarMax);
	// health
	m_healthLow = new FMF_LeftShoulder(a_healthLowMin, healthLowMax);
	m_healthOkay = new FMF_Triangular(a_healthOkayMin, a_healthOkayMid, healthOkayMax);
	m_healthGood = new FMF_RightShoulder(a_healthGoodMin, healthGoodMax);
	// evadeable
	m_evadeLow = new FMF_LeftShoulder(a_evadeLowMin, evadeLowMax);
	m_evadeMedium = new FMF_Triangular(a_evadeMediumMin, a_evadeMediumMid, evadeMediumMax);
	m_evadeHigh = new FMF_RightShoulder(a_evadeHighMin, evadeHighMax);

	// fill settings vector
	initVectors();
}


CompanionEvade::~CompanionEvade()
{
	destroy();
}

void CompanionEvade::update(Agent & a_agent)
{
	float evade = 0;
	// how far from target
	float targetClose = m_distanceClose->membership(a_agent.vitals.currentDistance);
	float targetNear = m_distanceMiddle->membership(a_agent.vitals.currentDistance);
	float targetFar = m_distanceFar->membership(a_agent.vitals.currentDistance);
	// how much health
	float healthLow = m_healthLow->membership(a_agent.vitals.health);
	float healthOkay = m_healthOkay->membership(a_agent.vitals.health);
	float healthGood = m_healthGood->membership(a_agent.vitals.health);
	// how evadeable is the target
	float evadeLow = OR(AND(healthOkay, targetFar),
						OR(AND(healthGood, targetNear), AND(healthGood, targetFar)));
	float evadeMid = OR(AND(healthLow, targetFar), 
						OR(AND(healthOkay, targetNear), AND(healthGood, targetClose)));
	float evadeHigh = OR(OR(AND(healthLow, targetClose), AND(healthLow, targetNear)),
						 AND(healthOkay, targetClose));
	// set max values
	float maxEvadeLow = m_evadeLow->maxMembership();
	float maxEvadeMid = m_evadeMedium->maxMembership();
	float maxEvadeHigh = m_evadeHigh->maxMembership();
	// defuzzify
	evade = maxEvadeHigh * evadeHigh + maxEvadeMid * evadeMid + maxEvadeLow * evadeLow;
	evade /= (0.1f + evadeHigh + evadeMid + evadeLow);
	// set weight
	traits.prevWeight = traits.currWeight;
	traits.currWeight = evade;
}

std::vector<float> CompanionEvade::distance(std::vector<float> a_settings)
{
	// clear settings
	m_distanceSettings.empty();
	// set new settings
	std::vector<float> dc = m_distanceClose->settings(a_settings);
	std::vector<float> dm = m_distanceMiddle->settings(a_settings);
	std::vector<float> df = m_distanceFar->settings(a_settings);
	// save settings
	m_distanceSettings.insert(m_distanceSettings.end(), dc.begin(), dc.end());
	m_distanceSettings.insert(m_distanceSettings.end(), dm.begin(), dm.end());
	m_distanceSettings.insert(m_distanceSettings.end(), df.begin(), df.end());

	return m_distanceSettings;
}

std::vector<float> CompanionEvade::health(std::vector<float> a_settings)
{
	// clear settings
	m_healthSettings.empty();
	// set new settings
	std::vector<float> hl = m_healthLow->settings(a_settings);
	std::vector<float> ho = m_healthOkay->settings(a_settings);
	std::vector<float> hg = m_healthGood->settings(a_settings);
	// save settings
	m_healthSettings.insert(m_healthSettings.end(), hl.begin(), hl.end());
	m_healthSettings.insert(m_healthSettings.end(), ho.begin(), ho.end());
	m_healthSettings.insert(m_healthSettings.end(), hg.begin(), hg.end());

	return m_healthSettings;
}

std::vector<float> CompanionEvade::evadeable(std::vector<float> a_settings)
{
	// clear settings
	m_evadeSettings.empty();
	// set new settings
	std::vector<float> fl = m_evadeLow->settings(a_settings);
	std::vector<float> fm = m_evadeMedium->settings(a_settings);
	std::vector<float> fh = m_evadeHigh->settings(a_settings);
	// save settings
	m_evadeSettings.insert(m_evadeSettings.end(), fl.begin(), fl.end());
	m_evadeSettings.insert(m_evadeSettings.end(), fm.begin(), fm.end());
	m_evadeSettings.insert(m_evadeSettings.end(), fh.begin(), fh.end());

	return m_evadeSettings;
}

void CompanionEvade::destroy()
{
	deallocate(m_distanceClose);
	deallocate(m_distanceMiddle);
	deallocate(m_distanceFar);
	deallocate(m_healthLow);
	deallocate(m_healthOkay);
	deallocate(m_healthGood);
	deallocate(m_evadeLow);
	deallocate(m_evadeMedium);
	deallocate(m_evadeHigh);
}

void CompanionEvade::initVectors()
{
	// clear settings
	m_distanceSettings.empty();
	// set new settings
	std::vector<float> dc = m_distanceClose->settings();
	std::vector<float> dm = m_distanceMiddle->settings();
	std::vector<float> df = m_distanceFar->settings();
	// save settings
	m_distanceSettings.insert(m_distanceSettings.end(), dc.begin(), dc.end());
	m_distanceSettings.insert(m_distanceSettings.end(), dm.begin(), dm.end());
	m_distanceSettings.insert(m_distanceSettings.end(), df.begin(), df.end());
	// clear settings
	m_healthSettings.empty();
	// set new settings
	std::vector<float> hl = m_healthLow->settings();
	std::vector<float> ho = m_healthOkay->settings();
	std::vector<float> hg = m_healthGood->settings();
	// save settings
	m_healthSettings.insert(m_healthSettings.end(), hl.begin(), hl.end());
	m_healthSettings.insert(m_healthSettings.end(), ho.begin(), ho.end());
	m_healthSettings.insert(m_healthSettings.end(), hg.begin(), hg.end());
	// clear settings
	m_evadeSettings.empty();
	// set new settings
	std::vector<float> fl = m_evadeLow->settings();
	std::vector<float> fm = m_evadeMedium->settings();
	std::vector<float> fh = m_evadeHigh->settings();
	// save settings
	m_evadeSettings.insert(m_evadeSettings.end(), fl.begin(), fl.end());
	m_evadeSettings.insert(m_evadeSettings.end(), fm.begin(), fm.end());
	m_evadeSettings.insert(m_evadeSettings.end(), fh.begin(), fh.end());
}
