#include "EnemySeek.h"
#include "Agents.h"



EnemySeek::EnemySeek()
{
}

EnemySeek::EnemySeek(float a_distanceMin, float distanceMax, float a_healthMin, float a_healthMax, float a_seekMin, float a_seekMax)
{
	traits.name = "Seek";
	traits.action = AN_SEEK;
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
		distanceFarMin, distanceFarMax, "Distance");

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
		healthGoodMin, healthGoodMax, "Health");

	// ------------------ seekable ------------------------------------------
	// left variables
	float seekLowMin = a_seekMin;
	float seekLowMax = (a_seekMax - a_seekMin) / 3;
	// triangular variables
	float seekMidMin = (seekLowMax - seekLowMin) / 3;
	float seekMidPeak = a_seekMax  * 0.5f;
	float seekMidMax = seekMidPeak + seekMidMin;
	// right variables
	float seekHighMin = (seekMidMax - seekMidMin) / 3;
	float seekHighMax = a_seekMax;
	// membership function objects
	m_seekMS = new LeftShoulderTriangularRightShoulder(seekLowMin, seekLowMax,
		seekMidMin, seekMidPeak, seekMidMax,
		seekHighMin, seekHighMax, "Desire");

	// fill settings vector
	initVectors();
	// set initial seek weight
	traits.currWeight = 0;
}

EnemySeek::EnemySeek(float a_distanceCloseMin, float distanceCloseMax, float a_distanceMiddleMin, float a_distanceMiddleMid, float distanceMiddleMax, float a_distanceFarMin, float distanceFarMax, 
				float a_healthLowMin, float healthLowMax, float a_healthOkayMin, float a_healthOkayMid, float healthOkayMax, float a_healthGoodMin, float healthGoodMax, 
				float a_seekLowMin, float seekLowMax, float a_seekMediumMin, float a_seekMediumMid, float seekMediumMax, float a_seekHighMin, float seekHighMax)
{
	traits.name = "Seek";
	// distance
	m_distanceMS = new LeftShoulderTriangularRightShoulder(a_distanceCloseMin, distanceCloseMax,
		a_distanceMiddleMin, a_distanceMiddleMid, distanceMiddleMax,
		a_distanceFarMin, distanceFarMax, "Distance");
	// health
	m_healthMS = new LeftShoulderTriangularRightShoulder(a_healthLowMin, healthLowMax,
		a_healthOkayMin, a_healthOkayMid, healthOkayMax,
		a_healthGoodMin, healthGoodMax, "Health");
	// seekable
	m_seekMS = new LeftShoulderTriangularRightShoulder(a_seekLowMin, seekLowMax,
		a_seekMediumMin, a_seekMediumMid, seekMediumMax,
		a_seekHighMin, seekHighMax, "Desire");
	
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
	// update member sets
	m_distanceMS->update(a_agent, a_agent.vitals.foeDistance);
	m_healthMS->update(a_agent, a_agent.vitals.health);
	m_seekMS->update(a_agent, traits.currWeight);
	// how far from target
	float targetClose = m_distanceMS->doms.leftShoulder;
	float targetNear = m_distanceMS->doms.triangular;
	float targetFar = m_distanceMS->doms.rightShoulder;
	// how much health
	float healthLow = m_healthMS->doms.leftShoulder;
	float healthOkay = m_healthMS->doms.triangular;
	float healthGood = m_healthMS->doms.rightShoulder;

	// how seekable is the target
	float seekLow = OR(AND(healthLow, targetNear), AND(healthLow, targetFar));
	float seekMid = OR(AND(healthLow, targetClose), 
						OR(AND(healthOkay, targetNear), 
							AND(healthOkay, targetFar)
						)
					);
	float seekHigh = OR(healthGood, AND(healthGood, targetClose));

	// set max values
	float maxSeekLow = m_seekMS->maxDom.leftShoulder;
	float maxSeekMid = m_seekMS->maxDom.triangular;
	float maxSeekHigh = m_seekMS->maxDom.rightShoulder;
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
	m_distanceSettings = m_distanceMS->settings(a_settings);

	return m_distanceSettings;
}

std::vector<float> EnemySeek::health(std::vector<float> a_settings)
{
	// clear settings
	m_healthSettings.empty();
	// set new settings
	m_healthSettings = m_healthMS->settings(a_settings);

	return m_healthSettings;
}

std::vector<float> EnemySeek::seekable(std::vector<float> a_settings)
{
	// clear settings
	m_seekSettings.empty();
	// set new settings
	m_seekSettings = m_seekMS->settings(a_settings);

	return m_seekSettings;
}

void EnemySeek::destroy()
{
	deallocate(m_distanceMS);
	deallocate(m_healthMS);
	deallocate(m_seekMS);
}

void EnemySeek::initVectors()
{
	// fill list of membersets
	m_memberSets.push_back(m_distanceMS);
	m_memberSets.push_back(m_healthMS);
	m_memberSets.push_back(m_seekMS);


	// clear settings
	m_distanceSettings.empty();
	// set new settings
	m_distanceSettings = m_distanceMS->settings();
	// clear settings
	m_healthSettings.empty();
	// set new settings
	m_healthSettings = m_healthMS->settings();
	// clear settings
	m_seekSettings.empty();
	// set new settings
	m_seekSettings = m_seekMS->settings();
}
