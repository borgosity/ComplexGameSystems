#include "CompanionFollow.h"
#include "Agents.h"



CompanionFollow::CompanionFollow()
{
}

CompanionFollow::CompanionFollow(float a_distanceMin, float distanceMax, float a_healthMin, float a_healthMax, float a_followMin, float a_followMax)
{
	traits.name = "Follow";
	traits.action = AN_FOLLOW;
	// ------------------ distance ------------------------------------------
	std::vector<float> distance = genSteepLTRSet(a_distanceMin, distanceMax);
	// membership function objects
	m_distanceMS = new LeftShoulderTriangularRightShoulder(distance, "Distance");

	// ------------------ health ------------------------------------------
	std::vector<float> health = genSteepLTRSet(a_healthMin, a_healthMax);
	// membership function objects
	m_healthMS = new LeftShoulderTriangularRightShoulder(health, "Health");

	// ------------------ followable ------------------------------------------
	std::vector<float> follow = genSteepLTRSet(a_followMin, a_followMax);
	// membership function objects
	m_followMS = new LeftShoulderTriangularRightShoulder(follow, "Desire");

	// fill settings vector
	initVectors();
	// set initial follow weight
	traits.currWeight = 0;
}

CompanionFollow::CompanionFollow(float a_distanceCloseMin, float distanceCloseMax, float a_distanceMiddleMin, float a_distanceMiddleMid, float distanceMiddleMax, float a_distanceFarMin, float distanceFarMax, 
				float a_healthLowMin, float healthLowMax, float a_healthOkayMin, float a_healthOkayMid, float healthOkayMax, float a_healthGoodMin, float healthGoodMax, 
				float a_followLowMin, float followLowMax, float a_followMediumMin, float a_followMediumMid, float followMediumMax, float a_followHighMin, float followHighMax)
{
	traits.name = "Follow";
	// distance
	m_distanceMS = new LeftShoulderTriangularRightShoulder(a_distanceCloseMin, distanceCloseMax,
		a_distanceMiddleMin, a_distanceMiddleMid, distanceMiddleMax,
		a_distanceFarMin, distanceFarMax, "Distance");
	// health
	m_healthMS = new LeftShoulderTriangularRightShoulder(a_healthLowMin, healthLowMax,
		a_healthOkayMin, a_healthOkayMid, healthOkayMax,
		a_healthGoodMin, healthGoodMax, "Health");
	// followable
	m_followMS = new LeftShoulderTriangularRightShoulder(a_followLowMin, followLowMax,
		a_followMediumMin, a_followMediumMid, followMediumMax,
		a_followHighMin, followHighMax, "Desire");
	
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
	// update member sets
	m_distanceMS->update(a_agent, a_agent.vitals.foeDistance);
	m_healthMS->update(a_agent, a_agent.vitals.health);
	m_followMS->update(a_agent, traits.currWeight);
	// how far from target
	float targetClose = m_distanceMS->doms.leftShoulder;
	float targetNear = m_distanceMS->doms.triangular;
	float targetFar = m_distanceMS->doms.rightShoulder;
	// how much health
	float healthLow = m_healthMS->doms.leftShoulder;
	float healthOkay = m_healthMS->doms.triangular;
	float healthGood = m_healthMS->doms.rightShoulder;

	// how followable is the target
	float followLow = OR(AND(healthLow, targetNear), AND(healthLow, targetFar));
	float followMid = OR(AND(healthOkay, targetNear), AND(healthOkay, targetFar));
	float followHigh = OR(healthGood, OR(AND(healthLow, targetClose), AND(healthOkay, targetClose)));

	// set max values
	float maxFollowLow = m_followMS->maxDom.leftShoulder;
	float maxFollowMid = m_followMS->maxDom.triangular;
	float maxFollowHigh = m_followMS->maxDom.rightShoulder;
	// defuzzify
	follow = maxFollowHigh * followHigh + maxFollowMid * followMid + maxFollowLow * followLow;
	follow /= (0.1f + followHigh + followMid + followLow);
	// set weight
	traits.prevWeight = traits.currWeight;
	traits.currWeight = follow;
	saveHistory(follow);
}

std::vector<float> CompanionFollow::distance(std::vector<float> a_settings)
{
	// clear settings
	m_distanceSettings.empty();
	// set new settings
	m_distanceSettings = m_distanceMS->settings(a_settings);

	return m_distanceSettings;
}

std::vector<float> CompanionFollow::health(std::vector<float> a_settings)
{
	// clear settings
	m_healthSettings.empty();
	// set new settings
	m_healthSettings = m_healthMS->settings(a_settings);

	return m_healthSettings;
}

std::vector<float> CompanionFollow::followable(std::vector<float> a_settings)
{
	// clear settings
	m_followSettings.empty();
	// set new settings
	m_followSettings = m_followMS->settings(a_settings);

	return m_followSettings;
}

void CompanionFollow::destroy()
{
	deallocate(m_distanceMS);
	deallocate(m_healthMS);
	deallocate(m_followMS);
}

void CompanionFollow::initVectors()
{
	// fill list of membersets
	m_memberSets.push_back(m_distanceMS);
	m_memberSets.push_back(m_healthMS);
	m_memberSets.push_back(m_followMS);


	// clear settings
	m_distanceSettings.empty();
	// set new settings
	m_distanceSettings = m_distanceMS->settings();
	// clear settings
	m_healthSettings.empty();
	// set new settings
	m_healthSettings = m_healthMS->settings();
	// clear settings
	m_followSettings.empty();
	// set new settings
	m_followSettings = m_followMS->settings();
}
