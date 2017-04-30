#include "EnemyFlee.h"
#include "Agents.h"



EnemyFlee::EnemyFlee()
{
}

EnemyFlee::EnemyFlee(float a_distanceMin, float distanceMax, float a_healthMin, float a_healthMax, float a_fleeMin, float a_fleeMax)
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


	// ------------------ fleeable ------------------------------------------
	// left variables
	float fleeLowMin = a_fleeMin;
	float fleeLowMax = (a_fleeMax - a_fleeMin) / 3;
	// triangular variables
	float fleeOkayMin = (fleeLowMax - fleeLowMin) / 3;
	float fleeOkayPeak = a_fleeMax  * 0.5f;
	float fleeOkayMax = fleeOkayPeak + fleeOkayMin;
	// right variables
	float fleeGoodMin = (fleeOkayMax - fleeOkayMin) / 3;
	float fleeGoodMax = a_fleeMax;
	// membership function objects
	m_fleeLow = new FMF_LeftShoulder(fleeLowMin, fleeLowMax);
	m_fleeMedium = new FMF_Triangular(fleeOkayMin, fleeOkayPeak, fleeOkayMax);
	m_fleeHigh = new FMF_RightShoulder(fleeGoodMin, fleeGoodMax);

	// fill settings vector
	initVectors();
	// set initial flee weight
	traits.currWeight = 0;
}

EnemyFlee::EnemyFlee(float a_distanceCloseMin, float distanceCloseMax, float a_distanceMiddleMin, float a_distanceMiddleMid, float distanceMiddleMax, float a_distanceFarMin, float distanceFarMax,
	float a_healthLowMin, float healthLowMax, float a_healthOkayMin, float a_healthOkayMid, float healthOkayMax, float a_healthGoodMin, float healthGoodMax,
	float a_fleeLowMin, float fleeLowMax, float a_fleeMediumMin, float a_fleeMediumMid, float fleeMediumMax, float a_fleeHighMin, float fleeHighMax)
{
	// distance
	m_distanceClose = new FMF_LeftShoulder(a_distanceCloseMin, distanceCloseMax);
	m_distanceMiddle = new FMF_Triangular(a_distanceMiddleMin, a_distanceMiddleMid, distanceMiddleMax);
	m_distanceFar = new FMF_RightShoulder(a_distanceFarMin, distanceFarMax);
	// health
	m_healthLow = new FMF_LeftShoulder(a_healthLowMin, healthLowMax);
	m_healthOkay = new FMF_Triangular(a_healthOkayMin, a_healthOkayMid, healthOkayMax);
	m_healthGood = new FMF_RightShoulder(a_healthGoodMin, healthGoodMax);
	// fleeable
	m_fleeLow = new FMF_LeftShoulder(a_fleeLowMin, fleeLowMax);
	m_fleeMedium = new FMF_Triangular(a_fleeMediumMin, a_fleeMediumMid, fleeMediumMax);
	m_fleeHigh = new FMF_RightShoulder(a_fleeHighMin, fleeHighMax);

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
	// how far from target
	float targetClose = m_distanceClose->membership(a_agent.vitals.currentDistance);
	float targetNear = m_distanceMiddle->membership(a_agent.vitals.currentDistance);
	float targetFar = m_distanceFar->membership(a_agent.vitals.currentDistance);
	// how much health
	float healthLow = m_healthLow->membership(a_agent.vitals.health);
	float healthOkay = m_healthOkay->membership(a_agent.vitals.health);
	float healthGood = m_healthGood->membership(a_agent.vitals.health);
	// how fleeable is the target
	float fleeLow = OR(AND(healthOkay, targetFar),
						OR(AND(healthGood, targetNear), AND(healthGood, targetFar)));
	float fleeMid = OR(AND(healthLow, targetFar), 
						OR(AND(healthOkay, targetNear), AND(healthGood, targetClose)));
	float fleeHigh = OR(OR(AND(healthLow, targetClose), AND(healthLow, targetNear)),
						 AND(healthOkay, targetClose));
	// set max values
	float maxFleeLow = m_fleeLow->maxMembership();
	float maxFleeMid = m_fleeMedium->maxMembership();
	float maxFleeHigh = m_fleeHigh->maxMembership();
	// defuzzify
	flee = maxFleeHigh * fleeHigh + maxFleeMid * fleeMid + maxFleeLow * fleeLow;
	flee /= (0.1f + fleeHigh + fleeMid + fleeLow);
	// set weight
	traits.prevWeight = traits.currWeight;
	traits.currWeight = flee;
}

std::vector<float> EnemyFlee::distance(std::vector<float> a_settings)
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

std::vector<float> EnemyFlee::health(std::vector<float> a_settings)
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

std::vector<float> EnemyFlee::fleeable(std::vector<float> a_settings)
{
	// clear settings
	m_fleeSettings.empty();
	// set new settings
	std::vector<float> fl = m_fleeLow->settings(a_settings);
	std::vector<float> fm = m_fleeMedium->settings(a_settings);
	std::vector<float> fh = m_fleeHigh->settings(a_settings);
	// save settings
	m_fleeSettings.insert(m_fleeSettings.end(), fl.begin(), fl.end());
	m_fleeSettings.insert(m_fleeSettings.end(), fm.begin(), fm.end());
	m_fleeSettings.insert(m_fleeSettings.end(), fh.begin(), fh.end());

	return m_fleeSettings;
}

void EnemyFlee::destroy()
{
	deallocate(m_distanceClose);
	deallocate(m_distanceMiddle);
	deallocate(m_distanceFar);
	deallocate(m_healthLow);
	deallocate(m_healthOkay);
	deallocate(m_healthGood);
	deallocate(m_fleeLow);
	deallocate(m_fleeMedium);
	deallocate(m_fleeHigh);
}

void EnemyFlee::initVectors()
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
	m_fleeSettings.empty();
	// set new settings
	std::vector<float> fl = m_fleeLow->settings();
	std::vector<float> fm = m_fleeMedium->settings();
	std::vector<float> fh = m_fleeHigh->settings();
	// save settings
	m_fleeSettings.insert(m_fleeSettings.end(), fl.begin(), fl.end());
	m_fleeSettings.insert(m_fleeSettings.end(), fm.begin(), fm.end());
	m_fleeSettings.insert(m_fleeSettings.end(), fh.begin(), fh.end());
}
