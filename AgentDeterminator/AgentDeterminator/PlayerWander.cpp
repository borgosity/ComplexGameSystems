#include "PlayerWander.h"
#include "Agents.h"
#include "imgui_glfw3.h"


PlayerWander::PlayerWander()
{
}

PlayerWander::PlayerWander(float a_distanceMin, float distanceMax, float a_healthMin, float a_healthMax, float a_wanderMin, float a_wanderMax)
{
	traits.name = "Wander";
	traits.action = AN_WANDER;
	// ------------------ distance ------------------------------------------
	std::vector<float> distance = genSteepLTRSet(a_distanceMin, distanceMax);
	// membership function objects
	m_distanceMS = new LeftShoulderTriangularRightShoulder(distance, "Distance");

	// ------------------ health ------------------------------------------
	std::vector<float> health = genSteepLTRSet(a_healthMin, a_healthMax);
	// membership function objects
	m_healthMS = new LeftShoulderTriangularRightShoulder(health, "Health");

	// ------------------ wanderable ------------------------------------------
	std::vector<float> wander = genSteepLTRSet(a_wanderMin, a_wanderMax);
	// membership function objects
	m_wanderMS = new LeftShoulderTriangularRightShoulder(wander, "Desire");

	// fill settings vector
	initVectors();
	// set initial wander weight
	traits.currWeight = 0;
}

PlayerWander::PlayerWander(float a_distanceCloseMin, float distanceCloseMax, float a_distanceMiddleMin, float a_distanceMiddleMid, float distanceMiddleMax, float a_distanceFarMin, float distanceFarMax,
	float a_healthLowMin, float healthLowMax, float a_healthOkayMin, float a_healthOkayMid, float healthOkayMax, float a_healthGoodMin, float healthGoodMax,
	float a_wanderLowMin, float wanderLowMax, float a_wanderMediumMin, float a_wanderMediumMid, float wanderMediumMax, float a_wanderHighMin, float wanderHighMax)
{
	traits.name = "Wander";
	// distance
	m_distanceMS = new LeftShoulderTriangularRightShoulder(a_distanceCloseMin, distanceCloseMax,
		a_distanceMiddleMin, a_distanceMiddleMid, distanceMiddleMax,
		a_distanceFarMin, distanceFarMax, "Distance");
	// health
	m_healthMS = new LeftShoulderTriangularRightShoulder(a_healthLowMin, healthLowMax,
		a_healthOkayMin, a_healthOkayMid, healthOkayMax,
		a_healthGoodMin, healthGoodMax, "Health");
	// wanderable
	m_wanderMS = new LeftShoulderTriangularRightShoulder(a_wanderLowMin, wanderLowMax,
		a_wanderMediumMin, a_wanderMediumMid, wanderMediumMax,
		a_wanderHighMin, wanderHighMax, "Desire");

	// fill settings vector
	initVectors();
}


PlayerWander::~PlayerWander()
{
	destroy();
}

void PlayerWander::update(Agent & a_agent)
{
	float wander = 0;
	// update member sets
	m_distanceMS->update(a_agent, a_agent.vitals.foeDistance);
	m_healthMS->update(a_agent, a_agent.vitals.health);
	m_wanderMS->update(a_agent, traits.currWeight);

	// how far from target
	float targetClose = m_distanceMS->doms.leftShoulder;
	float targetNear = m_distanceMS->doms.triangular;
	float targetFar = m_distanceMS->doms.rightShoulder;
	// how much health
	float healthLow = m_healthMS->doms.leftShoulder;
	float healthOkay = m_healthMS->doms.triangular;
	float healthGood = m_healthMS->doms.rightShoulder;
	
	// how wanderable is the target
	float wanderLow = AND(healthLow, targetClose);
	float wanderMid = OR(AND(healthLow, NOT(targetClose)),
						 OR(AND(healthOkay, targetClose), AND(healthGood, targetClose))
					);
	float wanderHigh = AND(NOT(healthLow),NOT(targetClose));

	// set max values
	float maxWanderLow  = m_wanderMS->maxDom.leftShoulder;
	float maxWanderMid  = m_wanderMS->maxDom.triangular;
	float maxWanderHigh = m_wanderMS->maxDom.rightShoulder;
	// defuzzify
	wander = maxWanderHigh * wanderHigh + maxWanderMid * wanderMid + maxWanderLow * wanderLow;
	wander /= (0.1f + wanderHigh + wanderMid + wanderLow);
	// set weight
	traits.prevWeight = traits.currWeight;
	traits.currWeight = wander;
	saveHistory(wander);
}

//void PlayerWander::drawGUI()
//{
//	std::string windowName = traits.name + " Membership Sets";
//	ImGui::Begin(windowName.c_str());
//	ImGui::Columns(3, "mixed", false);
//	m_healthMS->drawGUI();
//	ImGui::NextColumn();
//	m_distanceMS->drawGUI();
//	ImGui::NextColumn();
//	m_wanderMS->drawGUI();
//	ImGui::End();
//}

std::vector<float> PlayerWander::distance(std::vector<float> a_settings)
{
	// clear settings
	m_distanceSettings.empty();
	// set new settings
	m_distanceSettings = m_distanceMS->settings(a_settings);

	return m_distanceSettings;
}

std::vector<float> PlayerWander::health(std::vector<float> a_settings)
{
	// clear settings
	m_healthSettings.empty();
	// set new settings
	m_healthSettings = m_healthMS->settings(a_settings);

	return m_healthSettings;
}

std::vector<float> PlayerWander::wanderable(std::vector<float> a_settings)
{
	// clear settings
	m_wanderSettings.empty();
	// set new settings
	m_wanderSettings = m_wanderMS->settings(a_settings);

	return m_wanderSettings;
}

void PlayerWander::destroy()
{
	deallocate(m_distanceMS);
	deallocate(m_healthMS);
	deallocate(m_wanderMS);
}

void PlayerWander::initVectors()
{
	// fill list of membersets
	m_memberSets.push_back(m_distanceMS);
	m_memberSets.push_back(m_healthMS);
	m_memberSets.push_back(m_wanderMS);
	// clear settings
	m_distanceSettings.empty();
	// set new settings
	m_distanceSettings = m_distanceMS->settings();
	// clear settings
	m_healthSettings.empty();
	// set new settings
	m_healthSettings = m_healthMS->settings();
	// clear settings
	m_wanderSettings.empty();
	// set new settings
	m_wanderSettings = m_wanderMS->settings();
}
