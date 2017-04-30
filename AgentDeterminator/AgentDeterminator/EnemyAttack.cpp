#include "EnemyAttack.h"
#include "Agents.h"



EnemyAttack::EnemyAttack()
{
}

EnemyAttack::EnemyAttack(float a_distanceMin, float distanceMax, float a_healthMin, float a_healthMax, float a_attackMin, float a_attackMax)
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


	// ------------------ attackable ------------------------------------------
	// left variables
	float attackLowMin = a_attackMin;
	float attackLowMax = (a_attackMax - a_attackMin) / 3;
	// triangular variables
	float attackOkayMin = (attackLowMax - attackLowMin) / 3;
	float attackOkayPeak = a_attackMax  * 0.5f;
	float attackOkayMax = attackOkayPeak + attackOkayMin;
	// right variables
	float attackGoodMin = (attackOkayMax - attackOkayMin) / 3;
	float attackGoodMax = a_attackMax;
	// membership function objects
	m_attackLow = new FMF_LeftShoulder(attackLowMin, attackLowMax);
	m_attackMedium = new FMF_Triangular(attackOkayMin, attackOkayPeak, attackOkayMax);
	m_attackHigh = new FMF_RightShoulder(attackGoodMin, attackGoodMax);

	// fill settings vector
	initVectors();
	// set initial attack weight
	traits.currWeight = 0;
}

EnemyAttack::EnemyAttack(float a_distanceCloseMin, float distanceCloseMax, float a_distanceMiddleMin, float a_distanceMiddleMid, float distanceMiddleMax, float a_distanceFarMin, float distanceFarMax,
	float a_healthLowMin, float healthLowMax, float a_healthOkayMin, float a_healthOkayMid, float healthOkayMax, float a_healthGoodMin, float healthGoodMax,
	float a_attackLowMin, float attackLowMax, float a_attackMediumMin, float a_attackMediumMid, float attackMediumMax, float a_attackHighMin, float attackHighMax)
{
	// distance
	m_distanceClose = new FMF_LeftShoulder(a_distanceCloseMin, distanceCloseMax);
	m_distanceMiddle = new FMF_Triangular(a_distanceMiddleMin, a_distanceMiddleMid, distanceMiddleMax);
	m_distanceFar = new FMF_RightShoulder(a_distanceFarMin, distanceFarMax);
	// health
	m_healthLow = new FMF_LeftShoulder(a_healthLowMin, healthLowMax);
	m_healthOkay = new FMF_Triangular(a_healthOkayMin, a_healthOkayMid, healthOkayMax);
	m_healthGood = new FMF_RightShoulder(a_healthGoodMin, healthGoodMax);
	// attackable
	m_attackLow = new FMF_LeftShoulder(a_attackLowMin, attackLowMax);
	m_attackMedium = new FMF_Triangular(a_attackMediumMin, a_attackMediumMid, attackMediumMax);
	m_attackHigh = new FMF_RightShoulder(a_attackHighMin, attackHighMax);

	// fill settings vector
	initVectors();
}


EnemyAttack::~EnemyAttack()
{
	destroy();
}

void EnemyAttack::update(Agent & a_agent)
{
	float attack = 0;
	// how far from target
	float targetClose = m_distanceClose->membership(a_agent.vitals.currentDistance);
	float targetNear = m_distanceMiddle->membership(a_agent.vitals.currentDistance);
	float targetFar = m_distanceFar->membership(a_agent.vitals.currentDistance);
	// how much health
	float healthLow = m_healthLow->membership(a_agent.vitals.health);
	float healthOkay = m_healthOkay->membership(a_agent.vitals.health);
	float healthGood = m_healthGood->membership(a_agent.vitals.health);
	// how attackable is the target
	float attackLow = healthLow;
	float attackMid = OR(AND(healthOkay, targetNear), AND(healthOkay, targetFar));
	float attackHigh = OR(healthGood, AND(healthOkay, targetClose));
	// set max values
	float maxAttackLow = m_attackLow->maxMembership();
	float maxAttackMid = m_attackMedium->maxMembership();
	float maxAttackHigh = m_attackHigh->maxMembership();
	// defuzzify
	attack = maxAttackHigh * attackHigh + maxAttackMid * attackMid + maxAttackLow * attackLow;
	attack /= (0.1f + attackHigh + attackMid + attackLow);
	// set weight
	traits.prevWeight = traits.currWeight;
	traits.currWeight = attack;
}

std::vector<float> EnemyAttack::distance(std::vector<float> a_settings)
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

std::vector<float> EnemyAttack::health(std::vector<float> a_settings)
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

std::vector<float> EnemyAttack::attackable(std::vector<float> a_settings)
{
	// clear settings
	m_attackSettings.empty();
	// set new settings
	std::vector<float> fl = m_attackLow->settings(a_settings);
	std::vector<float> fm = m_attackMedium->settings(a_settings);
	std::vector<float> fh = m_attackHigh->settings(a_settings);
	// save settings
	m_attackSettings.insert(m_attackSettings.end(), fl.begin(), fl.end());
	m_attackSettings.insert(m_attackSettings.end(), fm.begin(), fm.end());
	m_attackSettings.insert(m_attackSettings.end(), fh.begin(), fh.end());

	return m_attackSettings;
}

void EnemyAttack::destroy()
{
	deallocate(m_distanceClose);
	deallocate(m_distanceMiddle);
	deallocate(m_distanceFar);
	deallocate(m_healthLow);
	deallocate(m_healthOkay);
	deallocate(m_healthGood);
	deallocate(m_attackLow);
	deallocate(m_attackMedium);
	deallocate(m_attackHigh);
}

void EnemyAttack::initVectors()
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
	m_attackSettings.empty();
	// set new settings
	std::vector<float> fl = m_attackLow->settings();
	std::vector<float> fm = m_attackMedium->settings();
	std::vector<float> fh = m_attackHigh->settings();
	// save settings
	m_attackSettings.insert(m_attackSettings.end(), fl.begin(), fl.end());
	m_attackSettings.insert(m_attackSettings.end(), fm.begin(), fm.end());
	m_attackSettings.insert(m_attackSettings.end(), fh.begin(), fh.end());
}
