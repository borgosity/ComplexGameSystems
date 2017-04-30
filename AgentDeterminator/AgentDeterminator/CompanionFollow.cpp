#include "CompanionFollow.h"
#include "Agents.h"



CompanionFollow::CompanionFollow()
{
}

CompanionFollow::CompanionFollow(float a_distanceMin, float distanceMax, float a_healthMin, float a_healthMax, float a_followMin, float a_followMax)
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


	// ------------------ followable ------------------------------------------
	// left variables
	float followLowMin = a_followMin;
	float followLowMax = (a_followMax - a_followMin) / 3;
	// triangular variables
	float followOkayMin = (followLowMax - followLowMin) / 3;
	float followOkayPeak = a_followMax  * 0.5f;
	float followOkayMax = followOkayPeak + followOkayMin;
	// right variables
	float followGoodMin = (followOkayMax - followOkayMin) / 3;
	float followGoodMax = a_followMax;
	// membership function objects
	m_followLow = new FMF_LeftShoulder(followLowMin, followLowMax);
	m_followMedium = new FMF_Triangular(followOkayMin, followOkayPeak, followOkayMax);
	m_followHigh = new FMF_RightShoulder(followGoodMin, followGoodMax);

	// fill settings vector
	initVectors();
	// set initial follow weight
	traits.currWeight = 0;
}

CompanionFollow::CompanionFollow(float a_distanceCloseMin, float distanceCloseMax, float a_distanceMiddleMin, float a_distanceMiddleMid, float distanceMiddleMax, float a_distanceFarMin, float distanceFarMax, 
				float a_healthLowMin, float healthLowMax, float a_healthOkayMin, float a_healthOkayMid, float healthOkayMax, float a_healthGoodMin, float healthGoodMax, 
				float a_followLowMin, float followLowMax, float a_followMediumMin, float a_followMediumMid, float followMediumMax, float a_followHighMin, float followHighMax)
{
	// distance
	m_distanceClose = new FMF_LeftShoulder(a_distanceCloseMin, distanceCloseMax);
	m_distanceMiddle = new FMF_Triangular(a_distanceMiddleMin, a_distanceMiddleMid, distanceMiddleMax);
	m_distanceFar = new FMF_RightShoulder(a_distanceFarMin, distanceFarMax);
	// health
	m_healthLow = new FMF_LeftShoulder(a_healthLowMin, healthLowMax);
	m_healthOkay = new FMF_Triangular(a_healthOkayMin, a_healthOkayMid, healthOkayMax);
	m_healthGood = new FMF_RightShoulder(a_healthGoodMin, healthGoodMax);
	// followable
	m_followLow = new FMF_LeftShoulder(a_followLowMin, followLowMax);
	m_followMedium = new FMF_Triangular(a_followMediumMin, a_followMediumMid, followMediumMax);
	m_followHigh = new FMF_RightShoulder(a_followHighMin, followHighMax);
	
	// fill settings vector
	initVectors();
}


CompanionFollow::~CompanionFollow()
{
	destroy();
}

void CompanionFollow::update(Agent & a_agent)
{
	float follow = 0;
	// how far from target
	float targetClose = m_distanceClose->membership(a_agent.vitals.currentDistance);
	float targetNear = m_distanceMiddle->membership(a_agent.vitals.currentDistance);
	float targetFar = m_distanceFar->membership(a_agent.vitals.currentDistance);
	// how much health
	float healthLow = m_healthLow->membership(a_agent.vitals.health);
	float healthOkay = m_healthOkay->membership(a_agent.vitals.health);
	float healthGood = m_healthGood->membership(a_agent.vitals.health);
	// how followable is the target
	float followLow = OR(AND(healthLow, targetNear), AND(healthLow, targetFar));
	float followMid = OR(AND(healthOkay, targetNear), AND(healthOkay, targetFar));
	float followHigh = OR(healthGood, OR(AND(healthLow, targetClose), AND(healthOkay, targetClose)));
	// set max values
	float maxFollowLow = m_followLow->maxMembership();
	float maxFollowMid = m_followMedium->maxMembership();
	float maxFollowHigh = m_followHigh->maxMembership();
	// defuzzify
	follow = maxFollowHigh * followHigh + maxFollowMid * followMid + maxFollowLow * followLow;
	follow /= (0.1f + followHigh + followMid + followLow);
	// set weight
	traits.prevWeight = traits.currWeight;
	traits.currWeight = follow;
}

std::vector<float> CompanionFollow::distance(std::vector<float> a_settings)
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

std::vector<float> CompanionFollow::health(std::vector<float> a_settings)
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

std::vector<float> CompanionFollow::followable(std::vector<float> a_settings)
{
	// clear settings
	m_followSettings.empty();
	// set new settings
	std::vector<float> fl = m_followLow->settings(a_settings);
	std::vector<float> fm = m_followMedium->settings(a_settings);
	std::vector<float> fh = m_followHigh->settings(a_settings);
	// save settings
	m_followSettings.insert(m_followSettings.end(), fl.begin(), fl.end());
	m_followSettings.insert(m_followSettings.end(), fm.begin(), fm.end());
	m_followSettings.insert(m_followSettings.end(), fh.begin(), fh.end());

	return m_followSettings;
}

void CompanionFollow::destroy()
{
	deallocate(m_distanceClose);
	deallocate(m_distanceMiddle);
	deallocate(m_distanceFar);
	deallocate(m_healthLow);
	deallocate(m_healthOkay);
	deallocate(m_healthGood);
	deallocate(m_followLow);
	deallocate(m_followMedium);
	deallocate(m_followHigh);
}

void CompanionFollow::initVectors()
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
	m_followSettings.empty();
	// set new settings
	std::vector<float> fl = m_followLow->settings();
	std::vector<float> fm = m_followMedium->settings();
	std::vector<float> fh = m_followHigh->settings();
	// save settings
	m_followSettings.insert(m_followSettings.end(), fl.begin(), fl.end());
	m_followSettings.insert(m_followSettings.end(), fm.begin(), fm.end());
	m_followSettings.insert(m_followSettings.end(), fh.begin(), fh.end());
}
