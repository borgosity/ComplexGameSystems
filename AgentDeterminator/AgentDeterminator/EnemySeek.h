#pragma once
#include <vector>
#include "Behaviour.h"
#include "FuzzyMemberSets.h"


class EnemySeek :
	public Behaviour
{
public:
	EnemySeek();
	EnemySeek(float a_distanceMin, float distanceMax, float a_healthMin, float a_healthMax, float a_seekMin, float a_seekMax);
	EnemySeek(float a_distanceCloseMin, float distanceCloseMax, float a_distanceMiddleMin, float a_distanceMiddleMid, float distanceMiddleMax, float a_distanceFarMin, float distanceFarMax,
			float a_healthLowMin, float healthLowMax, float a_healthOkayMin, float a_healthOkayMid, float healthOkayMax, float a_healthGoodMin, float healthGoodMax,
			float a_seekLowMin, float seekLowMax, float a_seekMediumMin, float a_seekMediumMid, float seekMediumMax, float a_seekHighMin, float seekHighMax);
	virtual  ~EnemySeek();

	void update(Agent & a_agent);

	std::vector<float> desireSettings() { return seekable(); };

	// get fuzzy settings
	std::vector<float> distance() { return m_distanceSettings; };
	std::vector<float> health() { return m_healthSettings; };
	std::vector<float> seekable() { return m_seekSettings; };
	// set fuzzy settings
	std::vector<float> distance(std::vector<float> a_settings);
	std::vector<float> health(std::vector<float> a_settings);
	std::vector<float> seekable(std::vector<float> a_settings);
	// destroy agent
	void destroy();

private:
	// distance
	LeftShoulderTriangularRightShoulder * m_distanceMS = nullptr;
	// health
	LeftShoulderTriangularRightShoulder * m_healthMS = nullptr;
	// evadeable
	LeftShoulderTriangularRightShoulder * m_seekMS = nullptr;
	// fuzzy settings
	std::vector<float> m_distanceSettings;
	std::vector<float> m_healthSettings;
	std::vector<float> m_seekSettings;
	// fill vectors function
	void initVectors();
};

