#include "EnemySeek.h"
#include "Agents.h"



EnemySeek::EnemySeek()
{
}

EnemySeek::EnemySeek(float a_distanceMin, float distanceMax, float a_healthMin, float a_healthMax, float a_seekMin, float a_seekMax)
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


	// ------------------ seekable ------------------------------------------
	// left variables
	float seekLowMin = a_seekMin;
	float seekLowMax = (a_seekMax - a_seekMin) / 3;
	// triangular variables
	float seekOkayMin = (seekLowMax - seekLowMin) / 3;
	float seekOkayPeak = a_seekMax  * 0.5f;
	float seekOkayMax = seekOkayPeak + seekOkayMin;
	// right variables
	float seekGoodMin = (seekOkayMax - seekOkayMin) / 3;
	float seekGoodMax = a_seekMax;
	// membership function objects
	m_seekLow = new FMF_LeftShoulder(seekLowMin, seekLowMax);
	m_seekMedium = new FMF_Triangular(seekOkayMin, seekOkayPeak, seekOkayMax);
	m_seekHigh = new FMF_RightShoulder(seekGoodMin, seekGoodMax);

	// fill settings vector
	initVectors();
	// set initial seek weight
	traits.currWeight = 0;
}

EnemySeek::EnemySeek(float a_distanceCloseMin, float distanceCloseMax, float a_distanceMiddleMin, float a_distanceMiddleMid, float distanceMiddleMax, float a_distanceFarMin, float distanceFarMax, 
				float a_healthLowMin, float healthLowMax, float a_healthOkayMin, float a_healthOkayMid, float healthOkayMax, float a_healthGoodMin, float healthGoodMax, 
				float a_seekLowMin, float seekLowMax, float a_seekMediumMin, float a_seekMediumMid, float seekMediumMax, float a_seekHighMin, float seekHighMax)
{
	// distance
	m_distanceClose = new FMF_LeftShoulder(a_distanceCloseMin, distanceCloseMax);
	m_distanceMiddle = new FMF_Triangular(a_distanceMiddleMin, a_distanceMiddleMid, distanceMiddleMax);
	m_distanceFar = new FMF_RightShoulder(a_distanceFarMin, distanceFarMax);
	// health
	m_healthLow = new FMF_LeftShoulder(a_healthLowMin, healthLowMax);
	m_healthOkay = new FMF_Triangular(a_healthOkayMin, a_healthOkayMid, healthOkayMax);
	m_healthGood = new FMF_RightShoulder(a_healthGoodMin, healthGoodMax);
	// seekable
	m_seekLow = new FMF_LeftShoulder(a_seekLowMin, seekLowMax);
	m_seekMedium = new FMF_Triangular(a_seekMediumMin, a_seekMediumMid, seekMediumMax);
	m_seekHigh = new FMF_RightShoulder(a_seekHighMin, seekHighMax);
	
	// fill settings vector
	initVectors();
}


EnemySeek::~EnemySeek()
{
	destroy();
}

void EnemySeek::update(Agent & a_agent)
{
	float seek = 0;
	// how far from target
	float targetClose = m_distanceClose->membership(a_agent.vitals.currentDistance);
	float targetNear = m_distanceMiddle->membership(a_agent.vitals.currentDistance);
	float targetFar = m_distanceFar->membership(a_agent.vitals.currentDistance);
	// how much health
	float healthLow = m_healthLow->membership(a_agent.vitals.health);
	float healthOkay = m_healthOkay->membership(a_agent.vitals.health);
	float healthGood = m_healthGood->membership(a_agent.vitals.health);
	// how seekable is the target
	float seekLow = OR(AND(healthLow, targetNear), AND(healthLow, targetFar));
	float seekMid = OR(AND(healthOkay, targetNear), AND(healthOkay, targetFar));
	float seekHigh = OR(healthGood, OR(AND(healthLow, targetClose), AND(healthOkay, targetClose)));
	// set max values
	float maxSeekLow = m_seekLow->maxMembership();
	float maxSeekMid = m_seekMedium->maxMembership();
	float maxSeekHigh = m_seekHigh->maxMembership();
	// defuzzify
	seek = maxSeekHigh * seekHigh + maxSeekMid * seekMid + maxSeekLow * seekLow;
	seek /= (0.1f + seekHigh + seekMid + seekLow);
	// set weight
	traits.prevWeight = traits.currWeight;
	traits.currWeight = seek;
}

std::vector<float> EnemySeek::distance(std::vector<float> a_settings)
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

std::vector<float> EnemySeek::health(std::vector<float> a_settings)
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

std::vector<float> EnemySeek::seekable(std::vector<float> a_settings)
{
	// clear settings
	m_seekSettings.empty();
	// set new settings
	std::vector<float> fl = m_seekLow->settings(a_settings);
	std::vector<float> fm = m_seekMedium->settings(a_settings);
	std::vector<float> fh = m_seekHigh->settings(a_settings);
	// save settings
	m_seekSettings.insert(m_seekSettings.end(), fl.begin(), fl.end());
	m_seekSettings.insert(m_seekSettings.end(), fm.begin(), fm.end());
	m_seekSettings.insert(m_seekSettings.end(), fh.begin(), fh.end());

	return m_seekSettings;
}

void EnemySeek::destroy()
{
	deallocate(m_distanceClose);
	deallocate(m_distanceMiddle);
	deallocate(m_distanceFar);
	deallocate(m_healthLow);
	deallocate(m_healthOkay);
	deallocate(m_healthGood);
	deallocate(m_seekLow);
	deallocate(m_seekMedium);
	deallocate(m_seekHigh);
}

void EnemySeek::initVectors()
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
	m_seekSettings.empty();
	// set new settings
	std::vector<float> fl = m_seekLow->settings();
	std::vector<float> fm = m_seekMedium->settings();
	std::vector<float> fh = m_seekHigh->settings();
	// save settings
	m_seekSettings.insert(m_seekSettings.end(), fl.begin(), fl.end());
	m_seekSettings.insert(m_seekSettings.end(), fm.begin(), fm.end());
	m_seekSettings.insert(m_seekSettings.end(), fh.begin(), fh.end());
}
