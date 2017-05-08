#pragma once
#include <vector>
#include "Behaviour.h"
#include "FuzzyMemberSets.h"

class PlayerEvade :
	public Behaviour
{
public:
	PlayerEvade();
	PlayerEvade(float a_distanceMin, float distanceMax, float a_healthMin, float a_healthMax, float a_evadeMin, float a_evadeMax);
	PlayerEvade(float a_distanceCloseMin, float distanceCloseMax, float a_distanceMiddleMin, float a_distanceMiddleMid, float distanceMiddleMax, float a_distanceFarMin, float distanceFarMax,
		float a_healthLowMin, float healthLowMax, float a_healthOkayMin, float a_healthOkayMid, float healthOkayMax, float a_healthGoodMin, float healthGoodMax,
		float a_evadeLowMin, float evadeLowMax, float a_evadeMediumMin, float a_evadeMediumMid, float evadeMediumMax, float a_evadeHighMin, float evadeHighMax);
	virtual  ~PlayerEvade();

	void update(Agent & a_agent);

	std::vector<float> desireSettings() { return evadeable(); };

	// get fuzzy settings
	std::vector<float> distance() { return m_distanceSettings; };
	std::vector<float> health() { return m_healthSettings; };
	std::vector<float> evadeable() { return m_evadeSettings; };
	// set fuzzy settings
	std::vector<float> distance(std::vector<float> a_settings);
	std::vector<float> health(std::vector<float> a_settings);
	std::vector<float> evadeable(std::vector<float> a_settings);
	// destroy agent
	void destroy();

private:
	// distance
	LeftShoulderTriangularRightShoulder * m_distanceMS = nullptr;
	// health
	LeftShoulderTriangularRightShoulder * m_healthMS = nullptr;
	// evadeable
	LeftShoulderTriangularRightShoulder * m_evadeMS = nullptr;
	// fuzzy settings
	std::vector<float> m_distanceSettings;
	std::vector<float> m_healthSettings;
	std::vector<float> m_evadeSettings;
	// fill vectors function
	void initVectors();
};

