#include "Follow.h"



Follow::Follow()
{
}

Follow::Follow(float a_distanceMin, float distanceMax, float a_healthMin, float a_healthMax, float a_followMin, float a_followMax)
{
	// ------------------ distance ------------------------------------------
	// left variables
	float distanceCloseMin = a_distanceMin;
	float distanceCloseMax = (distanceMax - a_distanceMin) / 3;
	// triangular variables
	float distanceMiddleMin = (distanceCloseMax - distanceCloseMin) / 3;
	float distanceMiddlePeak = distanceMax  * 0.5;
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
	float healthOkayPeak = a_healthMax  * 0.5;
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
	float followOkayPeak = a_followMax  * 0.5;
	float followOkayMax = followOkayPeak + followOkayMin;
	// right variables
	float followGoodMin = (followOkayMax - followOkayMin) / 3;
	float followGoodMax = a_followMax;
	// membership function objects
	m_followLow = new FMF_LeftShoulder(followLowMin, followLowMax);
	m_followMedium = new FMF_Triangular(followOkayMin, followOkayPeak, followOkayMax);
	m_followHigh = new FMF_RightShoulder(followGoodMin, followGoodMax);
}

Follow::Follow(float a_distanceCloseMin, float distanceCloseMax, float a_distanceMiddleMin, float a_distanceMiddleMid, float distanceMiddleMax, float a_distanceFarMin, float distanceFarMax, 
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
}


Follow::~Follow()
{
	destroy();
}

void Follow::update()
{
}

std::vector<float> Follow::distance(std::vector<float> a_settings)
{
	return m_distanceSettings;
}

std::vector<float> Follow::health(std::vector<float> a_settings)
{
	return m_healthSettings;
}

std::vector<float> Follow::followable(std::vector<float> a_settings)
{
	return m_followSettings;
}

void Follow::destroy()
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
